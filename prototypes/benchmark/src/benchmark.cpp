#include "benchmark.h"

Benchmark::Column::Column(std::string name, Schema::DataType type) :
                          name(name), type(type) {}

std::string Benchmark::Column::random_val(void) {
    return vals[ rand() % vals.size() ];
}

std::vector< Benchmark::Column > Benchmark::choose_random_columns(
        std::vector< Helpers::FieldInfo > &field_infos, int ncolumns) {
    // helper array storing (randomly) chosen column numbers
    int fnums[ncolumns];
    Helpers::sample(0,field_infos.size()-1,fnums, ncolumns);
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
            cmd << rand() % 10 + 1 << " "; // no empty lists
        } else {
            cmd << ":: ";
        }
    }
    return system(cmd.str().c_str()) == 0;
}

Schema *Benchmark::create_schema(std::vector< Benchmark::Column > &columns, int ncolumns) {
    if (!quiet) { std::cout << "[+] Creating schema (" << ncolumns
                            << " columns) ... " << std::flush; }
    // build random schema
    if (verbose) { std::cout << "\n[++] Creating random schema\n"; }
    Schema::DataType s[ncolumns];
    for (int i = 0; i < ncolumns; ++i) {
        s[i] = columns[i].type;
    }
    Schema *schema = new Schema(s, ncolumns);
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
        std::list<int> q_cols = Helpers::select_columns(ncolumns);
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
