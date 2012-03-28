#ifndef _BENCHMARK
#define _BENCHMARK

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "../../common/src/Schema.h"
#include "../../bob/src/BobTable.h"
#include "../../alice/src/AliceTable.h"
#include "helpers.h"

#define DIVIDENT 2
#define TEST_DATA_FILE "test_data"
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
    Schema *create_schema(std::vector< Column > &columns, int ncolumns);

    std::vector< Query > *create_random_queries(
        int nqueries, int ncolumns, int limit,
        std::vector< Column > &columns);

    std::vector< Column >
    choose_random_columns(
        std::vector< Helpers::FieldInfo > &field_infos,
        int ncolumns
    );


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

    template <class T>
    void fill_table_with_docs(T *table, Schema *schema,
                            std::vector< Benchmark::Column> &columns) {
        if (verbose) { std::cout << "[++] filling table with documents\n"; }
        std::ifstream ifs(TEST_DATA_FILE);
        std::string line;
        // probably bug
        Row row(*schema);
        while(std::getline(ifs, line)) {
            std::vector<std::string> *vals = Helpers::split(line, ';');
            for (int i = 0; i < vals -> size(); ++i) {
                switch(columns[i].type) {
                    case Schema::NUMERICAL:
                        row.set<double>(i, std::atoi((*vals)[i].c_str()));
                        break;
                    case Schema::STRING:
                        row.set<std::string>(i, (*vals)[i]);
                        break;
                    case Schema::NUMERICAL_LIST:
                        row.set< std::list<double> >(
                            i, Helpers::to_num_list((*vals)[i]));
                        break;
                    case Schema::STRING_LIST:
                        std::vector<std::string> *tmp =
                            Helpers::split((*vals)[i], ',');
                        std::list<std::string> str_list(tmp->begin(), tmp->end());
                        row.set< std::list<std::string> >(i, str_list);
                        break;
                }
            }
            delete vals;
            table->addRow(row);
        }
        table->prepareStructure();
        if (verbose) { std::cout << "[++] Creating tables " << std::flush; }
        if (!quiet) { std::cout << "done\n"; }
    }

    template <class T>
    T *prepare_table(std::vector< Helpers::FieldInfo > &field_infos,
                    int seed, int ndocs, std::vector< Benchmark::Column > &columns,
                    int ncolumns) {
        Schema *schema = create_schema(columns, ncolumns);
        T *table = new T(*schema);

        fill_table_with_docs(table, schema, columns);

        delete schema;
        return table;
    }


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
            threads.create_thread(boost::bind(&commit<T>, table, qs,
                                              start, end, quantities));
        }
        threads.join_all();
    }

    template <class T>
    int *submit_and_count_time(T *table, std::vector< Query > *qs, int nthreads,
                               struct timeval **run_time, int *quantities) {
        if (!quiet) { std::cout << "[+] Starting benchmark (using "
                                << nthreads << " threads) ... \n"; }
        // measure elapsing time and gather result quantities
        struct timeval start, end;
        gettimeofday(&start, NULL);
        Benchmark::submitQueries<T>(table, qs, nthreads, quantities);
        gettimeofday(&end, NULL);
        struct timeval *diff = Helpers::diff_timeval(&start, &end);
        if (!quiet) { std::cout << "Elapsed: "; Helpers::print_timeval(diff); }
        *run_time = diff;
        return quantities;
    }

    template <class T>
    void run(std::vector< Helpers::FieldInfo > &field_infos,
            int seed, int ndocs,
            std::vector< Benchmark::Column > &columns,
            int ncolumns, struct timeval **run_time,
            std::vector< Query > *qs, int nthreads, int nrounds,
            const char *result_file) {

        T *table = Benchmark::prepare_table<T>(
            field_infos, seed, ndocs, columns, ncolumns);

        int *quantities = (int *) calloc(qs->size(), sizeof (int));
        for (int i = 0; i < nrounds; ++i) {
            if (verbose) { std::cout << "[++] Running round " << i+1 << std::endl; }
            Benchmark::submit_and_count_time<T>(table, qs, nthreads,
                                                run_time+i, quantities);
        }
        // write quantities to a file
        Helpers::save_quantities(result_file, quantities, qs->size());
        struct timeval *av = Helpers::average_time(run_time, nrounds);
        std::cout << "Average time of execution: "; Helpers::print_timeval(av);
        free(av);
        free(quantities);
        delete table;
    }
}

#endif // _BENCHMARK
