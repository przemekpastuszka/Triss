#include "Benchmark.h"

Benchmark::Column::Column(std::string name, Schema::DataType type) :
                          name(name), type(type) {}

std::string Benchmark::Column::random_val(void) {
    return vals[ rand() % vals.size() ];
}

std::vector< Benchmark::Column > Benchmark::choose_random_columns(
        std::vector< Helpers::FieldInfo > &field_infos, int ncolumns) {
    // helper array storing (randomly) chosen column numbers
    unsigned int fnums[ncolumns];
    Helpers::sample(0,field_infos.size()-1, fnums, ncolumns);
    // create result vector out of drew col numbers
    std::vector < Benchmark::Column > res;
    res.resize(ncolumns, Benchmark::Column("",Schema::STRING));
    for (int i = 0; i < ncolumns; ++i) {
        res[i].name = field_infos[fnums[i]].name;
        if (field_infos[fnums[i]].type.compare("str") == 0) {
                res[i].type =
                    (rand() % 2) ? Schema::STRING : Schema::STRING_LIST;
        } else {
                res[i].type =
                    (rand() % 2) ? Schema::NUMERICAL : Schema::NUMERICAL_LIST;
        }
    }
    return res;
}

void Benchmark::set_possible_vals(std::vector< Column > &columns) {
    for (int i = 0; i < columns.size(); ++i) {
        std::stringstream cmd;
        cmd << GENERATE_SCRIPT_PATH << " -v " << columns[i].name;
        std::vector< std::string > *tmp =
            Helpers::split(Helpers::exec(cmd.str().c_str()), '\n');

        for (int j = 0; j < tmp->size(); ++j) {
            columns[i].vals.push_back((*tmp)[j]);
        }
    }
}

bool Benchmark::is_list_type(Benchmark::Column f) {
    return (f.type == Schema::STRING_LIST) or
           (f.type == Schema::NUMERICAL_LIST);
}

bool Benchmark::is_string_type(Benchmark::Column f) {
    return (f.type == Schema::STRING) or
           (f.type == Schema::STRING_LIST);
}

bool Benchmark::generate_test_data(const char *test_data_file, int seed,
        int ndocs, std::vector< Benchmark::Column > &columns) {
    std::stringstream cmd;
    cmd << GENERATE_SCRIPT_PATH << " -s " << seed <<
           " -o " << test_data_file << " -n " << ndocs;
    cmd << " -p ";
    for (int i = 0; i < columns.size(); ++i) {
        Benchmark::Column &c = columns[i];
        cmd << c.name;
        if (is_list_type(c)) {
            cmd << ":list:";
            cmd << rand() % 10 << " "; // allow empty lists
        } else {
            cmd << ":: ";
        }
    }
    return system(cmd.str().c_str()) == 0;
}

std::vector<ColumnDesc> *Benchmark::create_schema(std::vector< Benchmark::Column > &columns, int ncolumns) {
    if (!quiet) { std::cout << "[+] Creating schema (" << ncolumns
                            << " columns) ... " << std::flush; }
    // build random schema
    if (verbose) { std::cout << "\n[++] Creating random schema\n"; }
    std::vector< ColumnDesc> *schema = new std::vector< ColumnDesc >;
    schema->clear();
    for (int i = 0; i < ncolumns; ++i) {
        schema->push_back(ColumnDesc(columns[i].name, columns[i].type));
    }

    return schema;
}

std::vector< Query > *Benchmark::create_random_queries(
        int nqueries, int ncolumns, int limit,
        std::vector< Benchmark::Column > &columns) {
    if (!quiet) { std::cout << "[+] Creating queries (" << nqueries
                            << ") ... " << std::flush; }
    // build random queries
    std::vector< Query > *qs = new std::vector< Query >;
    qs->resize(nqueries, Query());
    for (int i = 0; i < nqueries; ++i) {
        std::list<unsigned int> q_cols = Helpers::select_columns(ncolumns);
        (*qs)[i].selectColumns(q_cols);
        if (limit) { (*qs)[i].limit(limit); }
        // select random contraints on previously selected columns
        for (int j = 0; j < q_cols.size(); ++j) {
            // constraint only some % of columns - 1/DIVIDENT-th of columns
            if (!(rand() % DIVIDENT)) {
                std::string val = columns[j].random_val();
                if (Benchmark::is_string_type(columns[j])) {
                    Benchmark::add_random_constraint<std::string>((*qs)[i], j, val, columns[j]);
                } else {
                    Benchmark::add_random_constraint<double>((*qs)[i], j, atoi(val.c_str()), columns[j]);
                }
            }
        }
    }
    if (!quiet) { std::cout << "done\n"; }
    return qs;
}

void Benchmark::commit(DataBase *db, std::vector< Query > *qs, int start,
                       int end, int *quantities) {
    for (int qn = start; qn <= end; ++qn) {
        Result *result = db->select(TEST_TABLE_NAME, (*qs)[qn]);
        std::list<Row *> *rows = result -> fetchAll();
        quantities[qn] = rows->size();
        delete result;
    }
}

void Benchmark::submit_queries(DataBase *db, std::vector< Query > *qs,
                              int nthreads, int *quantities) {
    int qpt = qs->size() / nthreads; // queries per thread
    int mod = qs->size() % nthreads;
    int start, end = -1;
    boost::thread_group threads;
    for (int i = 0; i < nthreads; ++i) {
        start = end + 1;
        end += qpt;
        if (i < mod) { end += 1; }
        threads.create_thread(boost::bind(&commit, db, qs,
                                            start, end, quantities));
    }
    threads.join_all();
}

std::vector< ColumnDesc > convert(std::vector< Benchmark::Column > &columns) {
    std::vector< ColumnDesc > res;
    res.resize(columns.size(), ColumnDesc());
    for (int i = 0; i < columns.size(); ++i) {
        res[i] = ColumnDesc(columns[i].name, columns[i].type);
    }
    return res;
}

void Benchmark::run(std::vector< Helpers::FieldInfo > &field_infos,
                    int seed, int ndocs,
                    std::vector< Benchmark::Column > &columns,
                    int ncolumns, struct timeval **run_time,
                    std::vector< Query > *qs, int nthreads, int nrounds,
                    const char *result_file) {
    // Create new data base
    DataBase db;
    // Convert from Benchmark::Column to ColumnDesc
    std::vector<ColumnDesc> cols = convert(columns);
    // Create new table
    db.createTable(TEST_TABLE_NAME, cols);
    db.loadTable(TEST_TABLE_NAME, TEST_DATA_FILE);

    int *quantities = (int *) calloc(qs->size(), sizeof (int));
    for (int i = 0; i < nrounds; ++i) {
        if (verbose) { std::cout << "[++] Running round " << i+1 << std::endl; }

        if (!quiet) { std::cout << "[+] Starting benchmark (using "
                                << nthreads << " threads) ... \n"; }
        // measure elapsing time and gather result quantities
        struct timeval start, end;
        gettimeofday(&start, NULL);

        Benchmark::submit_queries(&db, qs, nthreads, quantities);

        gettimeofday(&end, NULL);
        struct timeval *diff = Helpers::diff_timeval(&start, &end);
        if (!quiet) { std::cout << "Elapsed: "; Helpers::print_timeval(diff); }
        run_time[i] = diff;
    }
    // write quantities to a file
    Helpers::save_quantities(result_file, quantities, qs->size());
    struct timeval *av = Helpers::average_time(run_time, nrounds);
    std::cout << "Average time of execution: "; Helpers::print_timeval(av);
    free(av);
    free(quantities);
    db.dropTable(TEST_TABLE_NAME);
}

