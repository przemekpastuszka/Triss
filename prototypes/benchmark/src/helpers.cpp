#include "helpers.h"

char GENERATE_SCRIPT_PATH[] =
    "./prototypes/benchmark/test_data_generator/generate.py";

void sample(int start, int end, int *dest, int dlen) {
    int ilen = end-start+1;
    int shuffle_arr[ilen];

    for (int i = 0; i < ilen; ++i) {
        shuffle_arr[i] = i;
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

std::vector<std::string> *get_param_names(void) {
    // run script to list parameter names
    std::stringstream cmd;
    cmd << GENERATE_SCRIPT_PATH << " -l";
    std::string pnam = exec(cmd.str().c_str());
    std::vector<std::string> *param_names = split(pnam, '\n');
    for (int i = 0; i < param_names -> size()-1; ++i) {
        (*param_names)[i] = (*param_names)[i+1];
    }
    param_names -> pop_back();
    return param_names;
}

bool generate_test_data(const char *test_data_file, int seed, int ndocs,
                        int *cols, int *types, int ncols) {
    std::vector<std::string> *pnames = get_param_names();
    std::stringstream cmd;
    cmd << GENERATE_SCRIPT_PATH << " -s " << seed <<
           " -o " << test_data_file << " -n " << ndocs;
    cmd << " -p ";
    for (int i = 0; i < ncols; ++i) {
        cmd << (*pnames)[cols[i]-1];
        if (types[i]) { // list type
            cmd << ":list:";
            cmd << rand() % 10 + 1 << " "; // no empty lists
        } else {
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
    return res;
}
