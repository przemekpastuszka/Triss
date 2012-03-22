#include "helpers.h"

Benchmark::Column::Column(std::string name, Schema::DataType type) : name(name), type(type) {}

//Benchmark::Column::~Column() { free(this->vals); }

std::string Benchmark::Column::random_val(void) {
    return vals[ rand() % vals.size() ];
}

Benchmark::FieldInfo::FieldInfo(std::string name, std::string type) : name(name), type(type) {}

#define NAME 0
#define TYPE 1
#define MS_PER_SEC 1000000

std::vector< Benchmark::Column > Benchmark::choose_random_columns(
        std::vector< Benchmark::FieldInfo > &field_infos, int ncolumns) {
    // helper array storing (randomly) chosen column numbers
    int fnums[ncolumns];
    Benchmark::sample(0,field_infos.size()-1,fnums, ncolumns);
    // create result vector out of drew col numbers
    std::vector < Benchmark::Column > res;
    res.resize(ncolumns, Benchmark::Column("",Schema::STRING));
    for (int i = 0; i < ncolumns; ++i) {
        res[i].name = field_infos[fnums[i]].name;
        if (field_infos[fnums[i]].type.compare("str") == 0) {
                res[i].type =
                    (rand() % 2) ? Schema::STRING : Schema::STRING_LIST;
        } else {
                res[i].type =
                    (rand() % 2) ? Schema::NUMERICAL : Schema::NUMERICAL_LIST;
        }
    }
    return res;
}

struct timeval *Benchmark::diff_timeval(struct timeval *start, struct timeval *end) {
    struct timeval *res = (struct timeval *) malloc(sizeof(struct timeval));
    res->tv_sec = end->tv_sec - start->tv_sec;
    if (end->tv_usec < start->tv_usec) {
        res->tv_usec = MS_PER_SEC - start->tv_usec + end->tv_usec;
    } else {
        res->tv_usec = end->tv_usec - start->tv_usec;
    }
    return res;
}

void Benchmark::sample(int start, int end, int *dest, int dlen) {
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

std::string Benchmark::exec(const char *cmd) {
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

std::list<int> Benchmark::select_columns(int total_cols) {
    int nselect_cols = (rand() % total_cols) + 1;
    // choose column numbers
    int select_cols[nselect_cols];
    Benchmark::sample(0, total_cols-1, select_cols, nselect_cols);
    std::list<int> columns;
    for (int i = 0; i < nselect_cols; ++i) {
        columns.push_back(select_cols[i]);
    }
    return columns;
}

void Benchmark::set_possible_vals(std::vector< Column > &columns) {
    for (int i = 0; i < columns.size(); ++i) {
        std::stringstream cmd;
        cmd << GENERATE_SCRIPT_PATH << " -v " << columns[i].name;
        std::vector< std::string > *tmp = split(exec(cmd.str().c_str()), '\n');

        for (int j = 0; j < tmp->size(); ++j) {
            columns[i].vals.push_back((*tmp)[j]);
        }
    }
}

void Benchmark::save_quantities(const char *file_path, int *quantities,
                                int len) {
    std::ofstream myFile;
    myFile.open(file_path);
    for (int i = 0; i < len; ++i) {
        myFile << quantities[i] << std::endl;
    }
    myFile.close();
}

bool Benchmark::is_list_type(Benchmark::Column f) {
    return (f.type == Schema::STRING_LIST) or
           (f.type == Schema::NUMERICAL_LIST);
}

bool Benchmark::is_string_type(Benchmark::Column f) {
    return (f.type == Schema::STRING) or
           (f.type == Schema::STRING_LIST);
}

std::vector< Benchmark::FieldInfo > Benchmark::get_field_infos(void) {
    // run script to list field names and types
    std::stringstream cmd;
    cmd << GENERATE_SCRIPT_PATH << " -l";
    std::vector<std::string> *tmp = split(exec(cmd.str().c_str()), '\n');
    std::vector<Benchmark::FieldInfo> fields;
    fields.resize(tmp -> size(), Benchmark::FieldInfo("",""));
    for (int i = 0; i < tmp -> size(); ++i) {
        std::vector<std::string> *f_info = split((*tmp)[i], '-');
        fields[i] = Benchmark::FieldInfo((*f_info)[NAME], (*f_info)[TYPE]);
        delete f_info;
    }
    delete tmp;
    return fields;
}

bool Benchmark::generate_test_data(const char *test_data_file, int seed,
        int ndocs, std::vector< Benchmark::Column > &columns) {
    std::stringstream cmd;
    cmd << GENERATE_SCRIPT_PATH << " -s " << seed <<
           " -o " << test_data_file << " -n " << ndocs;
    cmd << " -p ";
    for (int i = 0; i < columns.size(); ++i) {
        Benchmark::Column &c = columns[i];
        cmd << c.name;
        if (is_list_type(c)) {
            cmd << ":list:";
            cmd << rand() % 10 + 1 << " "; // no empty lists
        } else {
            cmd << ":: ";
        }
    }
    return system(cmd.str().c_str()) == 0;
}

// function splitting string by delimiter
std::vector<std::string> *Benchmark::split(const std::string &s, char delim) {
    std::vector<std::string> *elems = new std::vector<std::string>;
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems -> push_back(item);
    }
    return elems;
}

std::list<double> Benchmark::to_num_list(const std::string &s) {
    std::vector<std::string> *vals = split(s,',');
    std::list<double> res;
    for (int i = 0; i < vals->size(); ++i) {
        res.push_back(atof((*vals)[i].c_str()));
    }
    delete vals;
    return res;
}
