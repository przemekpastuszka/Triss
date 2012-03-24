#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <math.h>
#include <stdlib.h>
#include "../../common/src/Schema.h"
#include "../../bob/src/BobTable.h"
//#include "../../alice/src/AliceTable.h"
#include "helpers.h"

namespace po = boost::program_options;

int main(int argc, char** argv) {
    int seed, ndocs, nqueries, ncolumns, nthreads, limit, nrounds;
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
        ("limit,l", po::value<int>(&limit)->default_value(10),
         "set limit of each query results (zero turns it off)")
        ("nthreads,t", po::value<int>(&nthreads)->default_value(1),
         "specify number of threads in which work will be splitted to")
        ("random,r", "makes all optional parameters (except from seed) random")
        ("rounds,ro", po::value<int>(&nrounds)->default_value(1),
         "how many times test one table (to count average running time)")
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
    if (vm.count("random")) {
        ndocs = (rand() % 5000) + 5000;
        nqueries = (rand() % 5000) + 5000;
        ncolumns = (rand() % 12) + 1;
        nthreads = (rand() % 5) + 5;
        limit = (rand() % 10) + 10;
    }
    struct timeval **run_time = (struct timeval **) malloc(
            nrounds*sizeof(struct timeval *));
    if (verbose) { std::cout << "[++] Total number of rounds: " << nrounds
                             << std::endl; }
    for (int i = 0; i < nrounds; ++i) {
        if (verbose) { std::cout << "[++] Running round " << i << std::endl; }
        run_time[i] = (struct timeval *) malloc(sizeof (struct timeval));
        Benchmark::run(seed, ndocs, nqueries, ncolumns, nthreads, limit,
                       quiet, verbose, run_time+i);
    }
    for (int i = 0; i < nrounds; ++i) {
        free(run_time[i]);
    }
    free(run_time);
    return 0;
}
