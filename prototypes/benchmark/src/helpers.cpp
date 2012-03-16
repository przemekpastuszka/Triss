#include "helpers.h"

Field::Field(std::string name, std::string type) : name(name), type(type) {}

char GENERATE_SCRIPT_PATH[] =
    "./prototypes/benchmark/test_data_generator/generate.py";
#define NAME 0
#define TYPE 1
#define MS_PER_SEC 1000000

struct timeval *diff_timeval(struct timeval *start, struct timeval *end) {
    struct timeval *res = (struct timeval *) malloc(sizeof(struct timeval));
    res->tv_sec = end->tv_sec - start->tv_sec;
    if (end->tv_usec < start->tv_usec) {
        res->tv_usec = MS_PER_SEC - start->tv_usec + end->tv_usec;
    } else {
        res->tv_usec = end->tv_usec - start->tv_usec;
    }
    return res;
}

void sample(int start, int end, int *dest, int dlen) {
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

std::string exec(const char *cmd) {
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

bool is_list_type(Field f) {
    return (f.schemaType == Schema::STRING_LIST) or
           (f.schemaType == Schema::NUMERICAL_LIST);
}

std::vector< Field > get_field_info(void) {
    // run script to list field names and types
    std::stringstream cmd;
    cmd << GENERATE_SCRIPT_PATH << " -l";
    std::vector<std::string> *tmp = split(exec(cmd.str().c_str()), '\n');
    std::vector<Field> fields;
    fields.resize(tmp -> size(), Field("",""));
    for (int i = 0; i < tmp -> size(); ++i) {
        std::vector<std::string> *f_info = split((*tmp)[i], '-');
        fields[i] = Field((*f_info)[NAME], (*f_info)[TYPE]);
        delete f_info;
    }
    delete tmp;
    return fields;
}

bool generate_test_data(const char *test_data_file, int seed, int ndocs,
                        int *cols, int ncols, std::vector< Field > *fields) {
    std::stringstream cmd;
    cmd << GENERATE_SCRIPT_PATH << " -s " << seed <<
           " -o " << test_data_file << " -n " << ndocs;
    cmd << " -p ";
    for (int i = 0; i < ncols; ++i) {
        Field *f = &((*fields)[cols[i]]);
        cmd << f->name;
        if (rand() % 2) {
            if (f->type.compare("str") == 0) {
                f->schemaType = Schema::STRING;
            } else {
                f->schemaType = Schema::NUMERICAL;
            }
            cmd << ":list:";
            cmd << rand() % 10 + 1 << " "; // no empty lists
        } else {
            if (f->type.compare("str") == 0) {
                f->schemaType = Schema::STRING_LIST;
            } else {
                f->schemaType = Schema::NUMERICAL_LIST;
            }
            cmd << ":: ";
        }
    }
    return system(cmd.str().c_str()) == 0;
}
// function splitting string by delimiter
std::vector<std::string> *split(const std::string &s, char delim) {
    std::vector<std::string> *elems = new std::vector<std::string>;
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems -> push_back(item);
    }
    return elems;
}

std::list<double> to_num_list(const std::string &s) {
    std::vector<std::string> *vals = split(s,',');
    std::list<double> res;
    for (int i = 0; i < vals->size(); ++i) {
        res.push_back(atof((*vals)[i].c_str()));
    }
    delete vals;
    return res;
}
