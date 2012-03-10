/*
* Copyright 2012 Przemysław Pastuszka
*/

#include <vector>
#include <string>
#include <list>
#include "Row.h"

void Row::init(const std::vector<Schema::DataType>& schema) {
    this -> schema = schema;
    values = new void*[schema.size()];
    for(int i = 0; i < schema.size(); ++i) {
        values[i] = NULL;
    }
}

void Row::deleteFieldAt(int index) {
    if(values[index] != NULL) {
        switch(schema[index]) {
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
}
