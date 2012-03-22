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
//#include "../../bob/src/BobTable.h"
#include "../../alice/src/AliceTable.h"

#ifndef GENERATE_SCRIPT_PATH
#define GENERATE_SCRIPT_PATH "./prototypes/benchmark/test_data_generator/generate.py"
#endif

namespace Benchmark {
    class Column {
        public:
        std::string name;
        Schema::DataType type;
        Column(std::string name, Schema::DataType type);
        std::string random_val(void);
        //~Column();
        std::vector< std::string > vals;
    };

    std::string exec(const char *cmd);
    bool is_list_type(Column f);
    bool is_string_type(Column f);
    std::string get_random_val(std::string param_name);
    void set_possible_vals(std::vector< Column > &columns);

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
        //commit<BobTable>(table, qs, 0, qs->size()-1);
        commit<AliceTable>(table, qs, 0, qs->size()-1);
        /*
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
        */
    }

    template<class T>
    void add_random_constraint(Query &q, int col_num, T val, Column &c) {
        // distinguish list and non-list types
        if (is_list_type(c)) {
            //if (rand() % 2) {
                q.addConstraint(TypedConstraint<T>::contains(col_num, val));
            //} else {
            //    q.addConstraint(TypedConstraint<T>::notContains(col_num, (T) val));
            //}
        } else {
            switch (rand() % 4) {
                case 0:
                    q.addConstraint(TypedConstraint<T>::equals(col_num, val));
                    break;
                case 1:
                    //q.addConstraint(TypedConstraint<T>::notEquals(col_num, (T) val));
                    break;
                case 2:
                    q.addConstraint(TypedConstraint<T>::greaterOrEqual(col_num, val));
                    break;
                case 3:
                    q.addConstraint(TypedConstraint<T>::lessOrEqual(col_num, val));
                    break;
            }
        }
    }

    class FieldInfo {
        public:
        std::string type;
        std::string name;
        FieldInfo(std::string name, std::string type);
    };

    std::vector< Column >
    choose_random_columns(
        std::vector< FieldInfo > &field_infos,
        int ncolumns
    );

    struct timeval *diff_timeval(struct timeval *start, struct timeval *end);
    std::list<int> select_columns(int total_cols);
    void sample(int start, int end, int *dest, int dlen);
    bool generate_test_data(const char *test_data_file, int seed, int ndocs,
                            std::vector< Column > &fields);
    std::vector< FieldInfo > get_field_infos(void);
    std::vector<std::string> *split(const std::string &s, char delim);
    std::list<double> to_num_list(const std::string &s);
}

#endif //__BENCHMARK_HELPERS
