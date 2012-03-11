#include <iostream>
#include <fstream>
#include <boost/thread.hpp>
#include <boost/program_options.hpp>
#include "../../common/src/Schema.h"
#include "../../bob/src/BobTable.h"
#include "helpers.h"

int NTYPES = 4;
char TEST_DATA_FILE[] = "test_data";

namespace po = boost::program_options;

int main(int argc, char** argv) {
    int seed, ndocs, nqueries, ncolumns;
    po::options_description opt_descr("Optional arguments");
    opt_descr.add_options()
        ("help,h", "view this help message")
        ("seed,s", po::value<int>(&seed) -> default_value(0),
         "seed for random number generator")
        ("ndocs,d", po::value<int>(&ndocs) -> default_value(10000),
         "number of documents to generate and load to database")
        ("nqueries,n", po::value<int>(&nqueries) -> default_value(10000),
         "number of queries to commit")
        ("ncolumns,c", po::value<int>(&ncolumns) -> default_value(1),
         "number of columns in table")
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
    if (!quiet) { std::cout << "[+] Generating test data (" << ndocs <<
                               " documents) ... " << std::flush; }
    // choose distinct random columns
    int cols[ncolumns];
    sample(1,14,cols,ncolumns);
    int types[ncolumns];
    for (int i = 0; i < ncolumns; ++i) {
        types[i] = rand() % 2;
    }
    if (!generate_test_data(TEST_DATA_FILE, seed, ndocs,
                            cols, types, ncolumns)) {
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
        if (types[i]) { // list type
            if (cols[i]) { // non-numerical type
                s[i] = Schema::STRING_LIST;
            } else { // numerical type
                s[i] = Schema::NUMERICAL_LIST;
            }
        } else { // non-list type
            if (cols[i]) {
                s[i] = Schema::STRING;
            } else {
                s[i] = Schema::NUMERICAL;
            }
        }
    }
    Schema schema(s, ncolumns);
    BobTable table(schema);
    // fill the table with documents from data file
    if (verbose) { std::cout << "[++] filling table with documents\n"; }
    std::ifstream ifs(TEST_DATA_FILE);
    std::string line;
    Row row(schema);
    while(std::getline(ifs, line)) {
        std::vector<std::string> *vals = split(line, ';');
        for (int i = 0; i < vals -> size(); ++i) {
            switch(s[i]) {
                case Schema::NUMERICAL:
                    row.set<double>(i, std::atoi((*vals)[i].c_str()));
                    break;
                case Schema::STRING:
                    row.set<std::string>(i, (*vals)[i]);
                    break;
                case Schema::NUMERICAL_LIST:
                    row.set< std::list<double> >(i, to_num_list((*vals)[i]));
                    break;
                case Schema::STRING_LIST:
                    std::vector<std::string> *tmp = split((*vals)[i], ',');
                    std::list<std::string> str_list(tmp->begin(), tmp->end());
                    row.set< std::list<std::string> >(i, str_list);
                    break;
            }
        }
        delete vals;
        table.addRow(row);
    }
    table.prepareStructure();
    if (verbose) { std::cout << "[++] Creating tables" << std::flush; }
    std::cout << " done\n";
    /*
    std::cout << "[+] Creating queries..." << std::endl;
    // commit nqueries to the database
    srand(time(NULL));
    int nquery = atoi(argv[2]);
    // build random queries
    std::vector<Query> queries;
    queries.resize(nquery, Query());
    for (int qn = 0; qn < nquery; ++qn) {
        // choose num of columns to inclue in query
        int ncol = (rand() % NCOLS) + 1;
        // choose column numbers
        int shuffle_arr[NCOLS];
        for (int i = 0; i < NCOLS; ++i) {
            shuffle_arr[i] = i;
        }
        for (int i = NCOLS-1; i > 0; --i) {
            int rand_pos = rand() % (i+1);
            int tmp = shuffle_arr[i];
            shuffle_arr[i] = shuffle_arr[rand_pos];
            shuffle_arr[rand_pos] = tmp;
        }
        std::list<int> columns;
        for (int i = 0; i < ncol; ++i) {
            columns.push_back(shuffle_arr[i]);
        }
        queries[qn].selectColumns(columns);
        // add constraints
        // later
    }
    std::cout << "   [+] Done creating queries" << std::endl;
    std::cout << "[+] Starting benchmark..." << std::endl;
    for (int qn; qn < nquery; ++qn) {
        Result *result = table.select(queries[qn]);
        std::list<Row *> rows = result -> fetchAll();
        delete [] rows;
    }
    std::cout << "   [+] Benchmark done" << std::endl;

    */
    if (verbose) { std::cout << "[++] Removing test data file... "
                             << std::flush; }
    std::remove(TEST_DATA_FILE);
    if (verbose) { std::cout << "done\n"; }

    return 0;
}
