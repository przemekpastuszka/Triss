#include "helpers.h"

#define NAME 0
#define TYPE 1
#define MS_PER_SEC 1000000

Helpers::FieldInfo::FieldInfo(std::string name, std::string type) :
                                name(name), type(type) {}

struct timeval *Helpers::diff_timeval(struct timeval *start, struct timeval *end) {
    struct timeval *res = (struct timeval *) malloc(sizeof(struct timeval));
    res->tv_sec = end->tv_sec - start->tv_sec;
    if (end->tv_usec < start->tv_usec) {
        res->tv_usec = MS_PER_SEC - start->tv_usec + end->tv_usec;
    } else {
        res->tv_usec = end->tv_usec - start->tv_usec;
    }
    return res;
}

struct timeval *Helpers::average_time(struct timeval **run_time, int nrounds) {
    struct timeval *res = (struct timeval *) malloc(sizeof(struct timeval));
    long long u_sec = 0;
    for (int i = 0; i < nrounds; ++i) {
        u_sec += run_time[i]->tv_sec * MS_PER_SEC;
        u_sec += run_time[i]->tv_usec;
    }
    u_sec /= nrounds;
    res->tv_sec = u_sec / MS_PER_SEC;
    res->tv_usec = u_sec % MS_PER_SEC;
    return res;
}

void Helpers::print_timeval(struct timeval *t) {
    std::cout << t->tv_sec << " second";
    if (t->tv_sec != 1) { std::cout << "s"; }
    std::cout << " and " << t->tv_usec << " microseconds" << std::endl;
}

void Helpers::sample(int start, int end, int *dest, int dlen) {
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

std::string Helpers::exec(const char *cmd) {
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

std::list<int> Helpers::select_columns(int total_cols) {
    int nselect_cols = (rand() % total_cols) + 1;
    // choose column numbers
    int select_cols[nselect_cols];
    Helpers::sample(0, total_cols-1, select_cols, nselect_cols);
    std::list<int> columns;
    for (int i = 0; i < nselect_cols; ++i) {
        columns.push_back(select_cols[i]);
    }
    return columns;
}

void Helpers::save_quantities(const char *file_path, int *quantities,
                                int len) {
    if (verbose) { std::cout << "[++] Saving result quantities to file "
                             << file_path << " ... "; }
    std::ofstream myFile;
    myFile.open(file_path);
    for (int i = 0; i < len; ++i) {
        myFile << quantities[i] << std::endl;
    }
    myFile.close();
    if (verbose) { std::cout << "done\n"; }
}

std::vector< Helpers::FieldInfo > Helpers::get_field_infos(void) {
    // run script to list field names and types
    std::stringstream cmd;
    cmd << GENERATE_SCRIPT_PATH << " -l";
    std::vector<std::string> *tmp = split(exec(cmd.str().c_str()), '\n');
    std::vector<Helpers::FieldInfo> fields;
    fields.resize(tmp -> size(), Helpers::FieldInfo("",""));
    for (int i = 0; i < tmp -> size(); ++i) {
        std::vector<std::string> *f_info = split((*tmp)[i], '-');
        fields[i] = Helpers::FieldInfo((*f_info)[NAME], (*f_info)[TYPE]);
        delete f_info;
    }
    delete tmp;
    return fields;
}

std::vector<std::string> *Helpers::split(const std::string &s, char delim) {
    std::vector<std::string> *elems = new std::vector<std::string>;
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems -> push_back(item);
    }
    return elems;
}

std::list<double> Helpers::to_num_list(const std::string &s) {
    std::vector<std::string> *vals = split(s,',');
    std::list<double> res;
    for (int i = 0; i < vals->size(); ++i) {
        res.push_back(atof((*vals)[i].c_str()));
    }
    delete vals;
    return res;
}
