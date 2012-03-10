#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include "../../common/src/Schema.h"
#include "../../bob/src/BobTable.h"

// function splitting string by delimiter
std::vector<std::string> &split(const std::string &s, char delim,
                                std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

std::list<double> to_num_list(const std::string &s) {
    std::list<double> res;
    return res;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <data_file> <nqueries>\n";
        return 1;
    }
    std::cout << "[+] Creating tables..." << std::endl;
    int NCOLS = 5;
    Schema::DataType s[] = { Schema::NUMERICAL,  //age
                             Schema::STRING_LIST,//content
                             Schema::STRING,     //country
                             Schema::STRING_LIST,//hobby
                             Schema::STRING_LIST,//language
                           };
    Schema schema(s, NCOLS);
    BobTable table(schema);
    // fill the table with documents from data file
    std::ifstream ifs(argv[1]);
    std::string line;
    Row row(schema);
    while(std::getline(ifs, line)) {
        std::vector<std::string> vals = split(line, ';');
        for (int i=0; i < vals.size(); i++) {
            switch(s[i]) {
                case Schema::NUMERICAL:
                    row.set<double>(i, std::atoi(vals[i].c_str()));
                    break;
                case Schema::STRING:
                    row.set<std::string>(i, vals[i]);
                    break;
                case Schema::NUMERICAL_LIST:
                    row.set< std::list<double> >(i, to_num_list(vals[i]));
                    break;
                case Schema::STRING_LIST:
                    std::vector<std::string> tmp = split(vals[i], ',');
                    std::list<std::string> str_list(tmp.begin(), tmp.end());
                    row.set< std::list<std::string> >(i, str_list);
                    break;
            }
        }
        Row &row_p = row;
        table.addRow(row_p);
    }
    table.prepareStructure();
    std::cout << "   [+] Done creating tables" << std::endl;
    std::cout << "[+] Creating queries..." << std::endl;
    // commit nqueries to the database
    srand(time(NULL));
    int nquery = atoi(argv[2]);
    // build random queries
    std::vector<Query> queries;
    queries.resize(nquery, Query());
    for (int qn = 0; qn < nquery; ++qn) {
        // choose num of columns to inclue in query
        int ncol = (rand() % NCOLS) + 1;
        // choose column numbers
        int shuffle_arr[NCOLS];
        for (int i = 0; i < NCOLS; ++i) {
            shuffle_arr[i] = i;
        }
        for (int i = NCOLS-1; i > 0; --i) {
            int rand_pos = rand() % (i+1);
            int tmp = shuffle_arr[i];
            shuffle_arr[i] = shuffle_arr[rand_pos];
            shuffle_arr[rand_pos] = tmp;
        }
        std::list<int> columns;
        for (int i = 0; i < ncol; ++i) {
            columns.push_back(shuffle_arr[i]);
        }
        queries[qn].selectColumns(columns);
        // add constraints
        // later
    }
    std::cout << "   [+] Done creating queries" << std::endl;
    std::cout << "[+] Starting benchmark..." << std::endl;
    for (int qn; qn < nquery; ++qn) {
        Result *result = table.select(queries[qn]);
        std::list<Row *> rows = result -> fetchAll();
    }
    std::cout << "   [+] Benchmark done" << std::endl;

    return 0;
}
