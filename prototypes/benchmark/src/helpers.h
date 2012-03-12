#ifndef __BENCHMARK_HELPERS
#define __BENCHMARK_HELPERS

#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include "../../common/src/Schema.h"


class Field {
    public:
    std::string type;
    std::string name;
    Schema::DataType schemaType;
    Field(std::string name, std::string type);
};

struct timeval *diff_timeval(struct timeval *start, struct timeval *end);
void sample(int start, int end, int *dest, int dlen);
bool generate_test_data(const char *test_data_file, int seed, int ndocs,
                        int *cols, int ncols, std::vector<Field> *fields);
bool is_list_type(Field f);
std::vector< Field > get_field_info(void);
std::vector<std::string> *split(const std::string &s, char delim);
std::list<double> to_num_list(const std::string &s);

#endif //__BENCHMARK_HELPERS
