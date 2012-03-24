#include "helpers.h"

Benchmark::Column::Column(std::string name, Schema::DataType type) : name(name), type(type) {}

std::string Benchmark::Column::random_val(void) {
    return vals[ rand() % vals.size() ];
}

Benchmark::FieldInfo::FieldInfo(std::string name, std::string type) : name(name), type(type) {}

#define NAME 0
#define TYPE 1
#define MS_PER_SEC 1000000

std::vector< Benchmark::Column > Benchmark::choose_random_columns(
        std::vector< Benchmark::FieldInfo > &field_infos, int ncolumns) {
    // helper array storing (randomly) chosen column numbers
    int fnums[ncolumns];
    Benchmark::sample(0,field_infos.size()-1,fnums, ncolumns);
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

struct timeval *Benchmark::diff_timeval(struct timeval *start, struct timeval *end) {
    struct timeval *res = (struct timeval *) malloc(sizeof(struct timeval));
    res->tv_sec = end->tv_sec - start->tv_sec;
    if (end->tv_usec < start->tv_usec) {
        res->tv_usec = MS_PER_SEC - start->tv_usec + end->tv_usec;
    } else {
        res->tv_usec = end->tv_usec - start->tv_usec;
    }
    return res;
}

void Benchmark::sample(int start, int end, int *dest, int dlen) {
    int ilen = end-start+1;
    int shuffle_arr[ilen];

    for (int i = 0; i < ilen; ++i) {
        shuffle_arr[i] = start+i;
    }
    for (int i = ilen-1; i > 0; --i) {
        int rand_pos = rand() % (i+1);
        int tmp = shuffle_arr[i];
        shuffle_arr[i] = shuffle_arr[rand_pos];
        shuffle_arr[rand_pos] = tmp;
    }
    for (int i = 0; i < dlen; ++i) {
        dest[i] = shuffle_arr[i];
    }
}

std::string Benchmark::exec(const char *cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) { return "ERROR"; }
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL) {
            result += buffer;
        }
    }
    pclose(pipe);
    return result;
}

std::list<int> Benchmark::select_columns(int total_cols) {
    int nselect_cols = (rand() % total_cols) + 1;
    // choose column numbers
    int select_cols[nselect_cols];
    Benchmark::sample(0, total_cols-1, select_cols, nselect_cols);
    std::list<int> columns;
    for (int i = 0; i < nselect_cols; ++i) {
        columns.push_back(select_cols[i]);
    }
    return columns;
}

void Benchmark::set_possible_vals(std::vector< Column > &columns) {
    for (int i = 0; i < columns.size(); ++i) {
        std::stringstream cmd;
        cmd << GENERATE_SCRIPT_PATH << " -v " << columns[i].name;
        std::vector< std::string > *tmp = split(exec(cmd.str().c_str()), '\n');

        for (int j = 0; j < tmp->size(); ++j) {
            columns[i].vals.push_back((*tmp)[j]);
        }
    }
}

void Benchmark::save_quantities(const char *file_path, int *quantities,
                                int len) {
    std::ofstream myFile;
    myFile.open(file_path);
    for (int i = 0; i < len; ++i) {
        myFile << quantities[i] << std::endl;
    }
    myFile.close();
}

bool Benchmark::is_list_type(Benchmark::Column f) {
    return (f.type == Schema::STRING_LIST) or
           (f.type == Schema::NUMERICAL_LIST);
}

bool Benchmark::is_string_type(Benchmark::Column f) {
    return (f.type == Schema::STRING) or
           (f.type == Schema::STRING_LIST);
}

std::vector< Benchmark::FieldInfo > Benchmark::get_field_infos(void) {
    // run script to list field names and types
    std::stringstream cmd;
    cmd << GENERATE_SCRIPT_PATH << " -l";
    std::vector<std::string> *tmp = split(exec(cmd.str().c_str()), '\n');
    std::vector<Benchmark::FieldInfo> fields;
    fields.resize(tmp -> size(), Benchmark::FieldInfo("",""));
    for (int i = 0; i < tmp -> size(); ++i) {
        std::vector<std::string> *f_info = split((*tmp)[i], '-');
        fields[i] = Benchmark::FieldInfo((*f_info)[NAME], (*f_info)[TYPE]);
        delete f_info;
    }
    delete tmp;
    return fields;
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
            cmd << rand() % 10 + 1 << " "; // no empty lists
        } else {
            cmd << ":: ";
        }
    }
    return system(cmd.str().c_str()) == 0;
}

// function splitting string by delimiter
std::vector<std::string> *Benchmark::split(const std::string &s, char delim) {
    std::vector<std::string> *elems = new std::vector<std::string>;
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems -> push_back(item);
    }
    return elems;
}

std::list<double> Benchmark::to_num_list(const std::string &s) {
    std::vector<std::string> *vals = split(s,',');
    std::list<double> res;
    for (int i = 0; i < vals->size(); ++i) {
        res.push_back(atof((*vals)[i].c_str()));
    }
    delete vals;
    return res;
}

int Benchmark::run(int seed, int ndocs, int nqueries, int ncolumns,
                   int nthreads, int limit, bool quiet, bool verbose,
                   struct timeval **run_time) {
    if (verbose) { std::cout << "[++] Seed for random number generator set to "
                             << seed << "\n"; }
    if (verbose) { std::cout << "[++] Retrieving info on available fields ... "
                             << std::flush; }
    std::vector< Benchmark::FieldInfo > field_infos =
                                                Benchmark::get_field_infos();
    if (verbose) { std::cout << "done\n"; }
    // choose ncolumns random columns
    std::vector< ::Benchmark::Column > columns =
        Benchmark::choose_random_columns(field_infos, ncolumns);
    Benchmark::set_possible_vals(columns);
    if (verbose) {
        std::cout << "[++] Selected fields: ";
        for (int i = 0; i < ncolumns; ++i) {
            std::cout << columns[i].name << " ";
        }
        std::cout << std::endl;
    }
    if (!quiet) { std::cout << "[+] Generating test data (" << ndocs <<
                               " documents) ... " << std::flush; }
    if (!Benchmark::generate_test_data(TEST_DATA_FILE, seed, ndocs, columns)) {
        std::cerr << "\n   [-] Test data generation failed\n";
        return 1;
    }
    if (!quiet) { std::cout << "done\n"; }

    if (!quiet) { std::cout << "[+] Creating tables (" << ncolumns
                            << " columns) ... " << std::flush; }
    // build random schema
    if (verbose) { std::cout << "\n[++] Creating random schema\n"; }
    Schema::DataType s[ncolumns];
    for (int i = 0; i < ncolumns; ++i) {
        s[i] = columns[i].type;
    }
    Schema schema(s, ncolumns);
    BobTable table(schema);
    //AliceTable table(schema);
    // fill the table with documents from data file
    if (verbose) { std::cout << "[++] filling table with documents\n"; }
    std::ifstream ifs(TEST_DATA_FILE);
    std::string line;
    Row row(schema);
    while(std::getline(ifs, line)) {
        std::vector<std::string> *vals = Benchmark::split(line, ';');
        for (int i = 0; i < vals -> size(); ++i) {
            switch(s[i]) {
                case Schema::NUMERICAL:
                    row.set<double>(i, std::atoi((*vals)[i].c_str()));
                    break;
                case Schema::STRING:
                    row.set<std::string>(i, (*vals)[i]);
                    break;
                case Schema::NUMERICAL_LIST:
                    row.set< std::list<double> >(
                        i, Benchmark::to_num_list((*vals)[i]));
                    break;
                case Schema::STRING_LIST:
                    std::vector<std::string> *tmp =
                        Benchmark::split((*vals)[i], ',');
                    std::list<std::string> str_list(tmp->begin(), tmp->end());
                    row.set< std::list<std::string> >(i, str_list);
                    break;
            }
        }
        delete vals;
        table.addRow(row);
    }
    table.prepareStructure();
    if (verbose) { std::cout << "[++] Creating tables " << std::flush; }
    if (!quiet) { std::cout << "done\n"; }

    if (!quiet) { std::cout << "[+] Creating queries (" << nqueries
                            << ") ... " << std::flush; }
    // build random queries
    std::vector<Query> qs;
    qs.resize(nqueries, Query());
    for (int i = 0; i < nqueries; ++i) {
        std::list<int> q_cols = Benchmark::select_columns(ncolumns);
        qs[i].selectColumns(q_cols);
        if (limit) { qs[i].limit(limit); }
        // select random contraints on previously selected columns
        for (int j = 0; j < q_cols.size(); ++j) {
            // constraint only some % of columns - 1/DIVIDENT-th of columns
            if (!(rand() % DIVIDENT)) {
                std::string val = columns[j].random_val();
                if (Benchmark::is_string_type(columns[j])) {
                    Benchmark::add_random_constraint<std::string>(qs[i], j, val, columns[j]);
                } else {
                    Benchmark::add_random_constraint<double>(qs[i], j, atoi(val.c_str()), columns[j]);
                }
            }
        }
    }
    if (!quiet) { std::cout << "done\n"; }
    if (!quiet) { std::cout << "[+] Starting benchmark (using "
                            << nthreads << " threads) ... \n"; }
    // measure elapsing time and gather result quantities
    int quantities[nqueries];
    struct timeval start, end;
    gettimeofday(&start, NULL);
    Benchmark::submitQueries<BobTable>(&table, &qs, nthreads, quantities);
    gettimeofday(&end, NULL);
    struct timeval *diff = Benchmark::diff_timeval(&start, &end);
    std::cout << "Elapsed: " << diff->tv_sec << " second";
    if (diff->tv_sec != 1) { std::cout << "s"; }
    std::cout << " and " << diff->tv_usec << " microseconds\n";
    *run_time = diff;
    // write quantities to a file
    Benchmark::save_quantities("build/bob", quantities, nqueries);

    if (verbose) { std::cout << "[++] Removing test data file... "
                             << std::flush; }
    std::remove(TEST_DATA_FILE);
    if (verbose) { std::cout << "done\n"; }
}
