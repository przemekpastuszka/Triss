/*
* Copyright 2012 Przemysław Pastuszka
*/

#include <vector>
#include <string>
#include <list>
#include <iostream>
#include "Row.h"

Row::Row(int size) {
    values = new void*[size];
    for(int i = 0; i < size; ++i) {
        values[i] = NULL;
    }
}

Row::~Row() {
    if(values != NULL) {
        delete [] values;
    }
}

void Row::dispose(const std::vector<ColumnDesc>& schema) {
    for(int i = 0; i < schema.size(); ++i) {
        deleteFieldAt(i, schema);
    }
    delete [] values;
    values = NULL;
}

void Row::deleteFieldAt(int index, const std::vector<ColumnDesc>& schema) {
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
