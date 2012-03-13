#ifndef __BENCHMARK_HELPERS
#define __BENCHMARK_HELPERS

#include <list>
#include <vector>
#include <string>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <sys/time.h>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "../../common/src/Schema.h"
#include "../../bob/src/BobTable.h"


class Field {
    public:
    std::string type;
    std::string name;
    Schema::DataType schemaType;
    Field(std::string name, std::string type);
};

struct timeval *diff_timeval(struct timeval *start, struct timeval *end);
void sample(int start, int end, int *dest, int dlen);
bool generate_test_data(const char *test_data_file, int seed, int ndocs,
                        int *cols, int ncols, std::vector<Field> *fields);
bool is_list_type(Field f);
std::vector< Field > get_field_info(void);
std::vector<std::string> *split(const std::string &s, char delim);
std::list<double> to_num_list(const std::string &s);

template<class T>
void commit(T *table, std::vector< Query > *qs, int start, int end) {
    for (int qn = start; qn <= end; ++qn) {
        Result *result = table->select((*qs)[qn]);
        std::list<Row *> *rows = result -> fetchAll();
        for (std::list<Row *>::iterator it = rows->begin();
             it != rows->end(); ++it) {
            delete *it;
        }
    }
}

template<class T>
void benchmark(T *table, std::vector< Query > *qs, int nthreads) {
    int qpt = qs->size() / nthreads; // queries per thread
    int mod = qs->size() % nthreads;

    int start, end = -1;
    boost::thread_group threads;
    for (int i = 0; i < nthreads; ++i) {
        start = end + 1;
        end += qpt;
        if (i < mod) { end += 1; }
        threads.create_thread(boost::bind(&commit<BobTable>, table, qs,
                                          start, end));
    }
    threads.join_all();
}

#endif //__BENCHMARK_HELPERS
