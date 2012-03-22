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

namespace Benchmark {
    class Column {
        public:
        std::string name;
        Schema::DataType type;
        Column(std::string name, Schema::DataType type);
    };

    template<class T>
    void commit(T *table, std::vector< Query > *qs, int start, int end) {
        for (int qn = start; qn <= end; ++qn) {
            Result *result = table->select((*qs)[qn]);
            std::list<Row *> *rows = result -> fetchAll();
            delete result;
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
}

namespace Helpers {
    class FieldInfo {
        public:
        std::string type;
        std::string name;
        FieldInfo(std::string name, std::string type);
    };

    std::vector< Benchmark::Column >
    choose_random_columns(
        std::vector< FieldInfo > &field_infos,
        int ncolumns
    );

    struct timeval *diff_timeval(struct timeval *start, struct timeval *end);
    std::list<int> select_columns(int total_cols);
    void sample(int start, int end, int *dest, int dlen);
    std::string exec(const char *cmd);
    bool generate_test_data(const char *test_data_file, int seed, int ndocs,
                            std::vector<Benchmark::Column> &fields);
    bool is_list_type(Benchmark::Column f);
    std::vector< FieldInfo > get_field_infos(void);
    std::vector<std::string> *split(const std::string &s, char delim);
    std::list<double> to_num_list(const std::string &s);
}

#endif //__BENCHMARK_HELPERS
