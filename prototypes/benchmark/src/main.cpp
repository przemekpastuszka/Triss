#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <math.h>
#include <stdlib.h>
#include "../../common/src/Schema.h"
#include "../../bob/src/BobTable.h"
//#include "../../alice/src/AliceTable.h"
#include "benchmark.h"

bool quiet = false, verbose = false;

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
         "the more rounds the more plausible running-queries-time")
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
    if (vm.count("quiet")) {
        quiet = true;
    }
    if (vm.count("verbose")) {
        verbose = true;
        quiet = false;
    }
    if (verbose) { std::cout << "[++] Seed for random number generator set to "
                             << seed << "\n"; }
    srand(seed);
    if (vm.count("random")) {
        ndocs = (rand() % 5000) + 5000;
        nqueries = (rand() % 5000) + 5000;
        ncolumns = (rand() % 12) + 1;
        nthreads = (rand() % 5) + 5;
        limit = (rand() % 10) + 10;
    }
    if (verbose) { std::cout << "[++] Retrieving info on available fields ... "
                             << std::flush; }
    std::vector< Helpers::FieldInfo > field_infos =
                                                Helpers::get_field_infos();
    if (verbose) { std::cout << "done\n"; }

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

    BobTable *table = Benchmark::prepare_table<BobTable>(
        field_infos, seed, ndocs, columns, ncolumns);
    struct timeval **run_time = (struct timeval **) malloc(
            nrounds*sizeof(struct timeval *));
    if (verbose) { std::cout << "[++] Total number of rounds: " << nrounds
                             << std::endl; }
    std::vector< Query > *qs =
        create_random_queries(nqueries, ncolumns, limit, columns);
    int *qtts;
    for (int i = 0; i < nrounds; ++i) {
        if (verbose) { std::cout << "[++] Running round " << i+1 << std::endl; }
        run_time[i] = (struct timeval *) malloc(sizeof (struct timeval));
        qtts = Benchmark::run<BobTable>(table, qs, nthreads, run_time+i);
    }
    // write quantities to a file
    Helpers::save_quantities("build/bob", qtts, qs->size());
    struct timeval *av = Helpers::average_time(run_time, nrounds);
    std::cout << "Average time of execution: "; Helpers::print_timeval(av);
    free(av);
    for (int i = 0; i < nrounds; ++i) {
        free(run_time[i]);
    }
    free(run_time);
    free(qtts);
    delete qs;
    delete table;
    return 0;
}
