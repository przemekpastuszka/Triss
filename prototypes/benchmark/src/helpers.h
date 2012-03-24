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
    class FieldInfo {
        public:
        std::string type;
        std::string name;
        FieldInfo(std::string name, std::string type);
    };

    std::vector< FieldInfo > get_field_infos(void);
    void save_quantities(const char *file_path, int *quantities, int len);
    struct timeval *diff_timeval(struct timeval *start, struct timeval *end);
    struct timeval *average_time(struct timeval **run_time, int nrounds);
    void print_timeval(struct timeval *t);
    std::list<int> select_columns(int total_cols);
    void sample(int start, int end, int *dest, int dlen);
    std::string exec(const char *cmd);

    std::vector<std::string> *split(const std::string &s, char delim);
    std::list<double> to_num_list(const std::string &s);
}

#endif //_HELPERS
