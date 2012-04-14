/*
* Copyright 2012 Przemysław Pastuszka
*/

#include <vector>
#include <string>
#include <list>
#include <iostream>
#include "Row.h"

Row::Row(const std::vector<ColumnDesc>& schema) {
    this -> schema = schema;
    values = new void*[schema.size()];
    for(int i = 0; i < schema.size(); ++i) {
        values[i] = NULL;
    }
}

Row::~Row() {
    for(int i = 0; i < schema.size(); ++i) {
        deleteFieldAt(i);
    }
    delete [] values;
}

void Row::deleteFieldAt(int index) {
    if(values[index] != NULL) {
        switch(schema[index].type) {
            case Schema::STRING:
                delete static_cast<std::string*>(values[index]);
                break;
            case Schema::NUMERICAL:
                delete static_cast<double*>(values[index]);
                break;
            case Schema::NUMERICAL_LIST:
                delete static_cast<std::list<double>*>(values[index]);
                break;
            case Schema::STRING_LIST:
                delete static_cast<std::list<std::string>*>(values[index]);
                break;
        }
    }
    values[index] = NULL;
}
