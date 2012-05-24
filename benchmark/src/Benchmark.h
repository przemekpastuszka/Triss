#ifndef _BENCHMARK
#define _BENCHMARK

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "../../triss/src/common/Schema.h"
#include "../../triss/src/engine/Table.h"
#include "../../triss/src/common/Query.h"
#include "../../triss/src/common/Helpers.h"
#include "../../triss/src/db/DataBase.h"

#define DIVIDENT 2
#define TEST_DATA_FILE "test_data"
#define TEST_TABLE_NAME "test_table"
#define GENERATE_SCRIPT_PATH "./prototypes/benchmark/test_data_generator/generate.py"

extern bool quiet, verbose;

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
    std::vector< ColumnDesc > *create_schema(std::vector< Column > &columns, int ncolumns);

    std::vector< Query > *create_random_queries(
        int nqueries, int ncolumns, int limit,
        std::vector< Column > &columns);

    std::vector< Column >
    choose_random_columns(
        std::vector< Helpers::FieldInfo > &field_infos,
        int ncolumns
    );

    void commit(DataBase *db, std::vector< Query > *qs, int start,
                       int end, int *quantities);
    void submit_queries(DataBase *db, std::vector< Query > *qs,
                        int nthreads, int *quantities);
    void run(std::vector< Helpers::FieldInfo > &field_infos,
             int seed, int ndocs,
             std::vector< Benchmark::Column> &columns,
             int ncolumns, struct timeval **run_time,
             std::vector< Query > *qs, int nthreads, int nrounds,
             const char *result_file);

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
