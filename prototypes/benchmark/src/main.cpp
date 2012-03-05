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

int main(void) {
    Schema::DataType s[] = { Schema::NUMERICAL,  //age
                             Schema::STRING,     //country
                             Schema::STRING_LIST,//language
                             Schema::STRING,     //os
                             Schema::STRING,     //name
                             Schema::STRING_LIST //content
                           };
    Schema schema(s, 6);
    BobTable table(schema);
    // fill the table with documents from data file
    std::ifstream ifs("data");
    std::string line;
    int c = 0;
    while(std::getline(ifs, line)) {
        Row row(schema);
        std::vector<std::string> vals = split(line, ';');
        for (int i=0; i< vals.size(); i++) {
            std::cout << i<< " " <<vals[i] << std::endl;
        }
        std::cout << "--------------------------" << std::endl;
        for (int i=0; i < vals.size(); i++) {
            switch(s[i]) {
                case Schema::NUMERICAL:
                    std::cout <<"numerical " << i << ": "<<  std::atoi(vals[i].c_str()) << std::endl;
                    row.set<double>(i, std::atoi(vals[i].c_str()));
                    break;
                case Schema::STRING:
                    std::cout << "string " <<i<<": "<< vals[i] << std::endl;
                    row.set<std::string>(i, vals[i]);
                    break;
                case Schema::NUMERICAL_LIST:
                    row.set< std::list<double> >(i, to_num_list(vals[i]));
                    break;
                case Schema::STRING_LIST:
                    std::cout << "stringlist "<<i<<": " << vals[i] << std::endl;
                    std::vector<std::string> tmp = split(vals[i], ',');
                    for (int j=0;j<tmp.size();j++) {
                        std::cout << tmp[j] << std::endl;
                    }
                    std::cout << "+++++++++++++++++++++++++" << std::endl;
                    std::list<std::string> str_list(tmp.begin(), tmp.end());
                    row.set< std::list<std::string> >(i, str_list);
                    break;
            }
        }
        std::cout << c++ << std::endl;
        std::cout << "pre here\n";
        Row &row_p = row;
        table.addRow(row_p);
        std::cout << "post here\n";

    }
    table.prepareStructure();

    return 0;
}
