#ifndef _HELPERS
#define _HELPERS

#include <list>
#include <vector>
#include <string>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sys/time.h>

#ifndef _GENERATE_SCRIPT_PATH
#define GENERATE_SCRIPT_PATH "./prototypes/benchmark/test_data_generator/generate.py"
#endif

extern bool quiet, verbose;

namespace Helpers {
    void sample(int start, int end, int *dest, int dlen);
    std::string exec(const char *cmd);

    std::vector<std::string> *split(const std::string &s, char delim);
    std::list<double> to_num_list(const std::string &s);
}

#endif //_HELPERS
