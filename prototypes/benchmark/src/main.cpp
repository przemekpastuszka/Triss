#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
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
        std::cout << "Usage: "<<argv[0]<<" <data_file>\n";
        return 1;
    }
    //age:: content:list:4 country:: hobby:list:2 language:list:2
    Schema::DataType s[] = { Schema::NUMERICAL,  //age
                             Schema::STRING_LIST,//content
                             Schema::STRING,     //country
                             Schema::STRING_LIST,//hobby
                             Schema::STRING_LIST,//language
                           };
    Schema schema(s, 5);
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

    return 0;
}
