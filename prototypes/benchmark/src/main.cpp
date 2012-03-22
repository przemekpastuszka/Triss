#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <math.h>
#include "../../common/src/Schema.h"
//#include "../../bob/src/BobTable.h"
#include "../../alice/src/AliceTable.h"
#include "helpers.h"


int NTYPES = 4;
int NLIST_CONSTR = 2;
int DIVIDENT = 2;
int NNON_LIST_CONSTR = 2;
int MAXRESULTS = 10;
char TEST_DATA_FILE[] = "test_data";

namespace po = boost::program_options;

int main(int argc, char** argv) {
    int seed, ndocs, nqueries, ncolumns, nthreads;
    po::options_description opt_descr("Optional arguments");
    opt_descr.add_options()
        ("help,h", "view this help message")
        ("seed,s", po::value<int>(&seed)->default_value(0),
         "seed for random number generator")
        ("ndocs,d", po::value<int>(&ndocs)->default_value(10000),
         "number of documents to generate and load to database")
        ("nqueries,n", po::value<int>(&nqueries)->default_value(10000),
         "number of queries to commit")
        ("ncolumns,c", po::value<int>(&ncolumns)->default_value(1),
         "number of columns in table")
        ("nthreads,t", po::value<int>(&nthreads)->default_value(1),
         "specify number of threads in which work will be splitted to")
        ("quiet,q", "turns quiet mode on")
        ("verbose,v", "turns verbose mode on (overrides quiet option)")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, opt_descr), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << opt_descr << std::endl;
        return 1;
    }
    // quiet and verbose options are NOT complementary
    bool quiet = false, verbose = false;
    if (vm.count("quiet")) {
        quiet = true;
    }
    if (vm.count("verbose")) {
        verbose = true;
        quiet = false;
    }

    srand(seed);
    if (verbose) { std::cout << "[++] Seed for random number generator set to "
                             << seed << "\n"; }
    if (verbose) { std::cout << "[++] Retrieving info on available fields ... "
                             << std::flush; }
    std::vector< Benchmark::FieldInfo > field_infos = Benchmark::get_field_infos();
    if (verbose) { std::cout << "done\n"; }
    // choose ncolumns random columns
    std::vector< ::Benchmark::Column > columns =
        Benchmark::choose_random_columns(field_infos, ncolumns);
    set_possible_vals(columns);
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
    //BobTable table(schema);
    AliceTable table(schema);
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
        qs[i].limit((rand() % MAXRESULTS)+1);
        // select random contraints on previously selected columns
        for (int j = 0; j < q_cols.size(); ++j) {
            // constraint only some % of columns - 1/DIVIDENT-th of columns
            if (!(rand() % DIVIDENT)) {
                std::string val = columns[j].random_val();
                if (Benchmark::is_string_type(columns[j])) {
                    Benchmark::add_random_constraint< std::string>(qs[i], j, val, columns[j]);
                } else {
                    Benchmark::add_random_constraint<double>(qs[i], j, atoi(val.c_str()), columns[j]);
                }
            }
        }
    }
    if (!quiet) { std::cout << "done\n"; }
    if (!quiet) { std::cout << "[+] Starting benchmark (using "
                            << nthreads << " threads) ... \n"; }
    // measure elapsing time
    struct timeval start, end;
    gettimeofday(&start, NULL);
    Benchmark::benchmark(&table, &qs, nthreads);
    gettimeofday(&end, NULL);
    struct timeval *diff = Benchmark::diff_timeval(&start, &end);
    std::cout << "Elapsed: " << diff->tv_sec << " second";
    if (diff->tv_sec != 1) { std::cout << "s"; }
    std::cout << " and " << diff->tv_usec << " microseconds\n";
    free(diff);

    if (verbose) { std::cout << "[++] Removing test data file... "
                             << std::flush; }
    std::remove(TEST_DATA_FILE);
    if (verbose) { std::cout << "done\n"; }
    return 0;
}
