#ifndef _BENCHMARK
#define _BENCHMARK

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "../../common/src/Schema.h"
#include "../../bob/src/BobTable.h"
//#include "../../alice/src/AliceTable.h"
#include "helpers.h"

#define DIVIDENT 2
#define TEST_DATA_FILE "test_data"
#define GENERATE_SCRIPT_PATH "./prototypes/benchmark/test_data_generator/generate.py"
namespace Benchmark {
    class Column {
        public:
        std::string name;
        Schema::DataType type;
        std::vector< std::string > vals;

        Column(std::string name, Schema::DataType type);
        std::string random_val(void);
    };


    bool is_list_type(Column f);
    bool is_string_type(Column f);
    void set_possible_vals(std::vector< Column > &columns);
    bool generate_test_data(const char *test_data_file, int seed, int ndocs,
                            std::vector< Column > &fields);

    int run(int seed, int ndocs, int nqueries, int ncolumns, int nthreads,
            int limit, bool quiet, bool verbose, struct timeval **run_time);

    std::vector< Column >
    choose_random_columns(
        std::vector< Helpers::FieldInfo > &field_infos,
        int ncolumns
    );


    template<class T>
    void commit(T *table, std::vector< Query > *qs, int start, int end, int *quantities) {
        for (int qn = start; qn <= end; ++qn) {
            Result *result = table->select((*qs)[qn]);
            std::list<Row *> *rows = result -> fetchAll();
            quantities[qn] = rows->size();
            delete result;
        }
    }

    template<class T>
    void submitQueries(T *table, std::vector< Query > *qs, int nthreads, int *quantities) {
        int qpt = qs->size() / nthreads; // queries per thread
        int mod = qs->size() % nthreads;
        int start, end = -1;
        boost::thread_group threads;
        for (int i = 0; i < nthreads; ++i) {
            start = end + 1;
            end += qpt;
            if (i < mod) { end += 1; }
            threads.create_thread(boost::bind(&commit<BobTable>, table, qs,
                                              start, end, quantities));
        }
        threads.join_all();
    }

    template<class T>
    void add_random_constraint(Query &q, int col_num, T val, Column &c) {
        // distinguish list and non-list types
        if (is_list_type(c)) {
            q.addConstraint(TypedConstraint<T>::contains(col_num, val));
        } else {
            switch (rand() % 3) {
                case 0:
                    q.addConstraint(TypedConstraint<T>::equals(col_num, val));
                    break;
                case 1:
                    q.addConstraint(TypedConstraint<T>::greaterOrEqual(col_num, val));
                    break;
                case 2:
                    q.addConstraint(TypedConstraint<T>::lessOrEqual(col_num, val));
                    break;
            }
        }
    }

}

#endif // _BENCHMARK
