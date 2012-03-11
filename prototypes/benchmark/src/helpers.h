#ifndef __BENCHMARK_HELPERS
#define __BENCHMARK_HELPERS

#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>


void sample(int start, int end, int *dest, int dlen);
bool generate_test_data(const char *test_data_file, int seed, int ndocs,
                        int *cols, int *types, int ncols);
std::vector<std::string> *split(const std::string &s, char delim);
std::list<double> to_num_list(const std::string &s);

#endif //__BENCHMARK_HELPERS
