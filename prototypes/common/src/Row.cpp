/*
* Copyright 2012 Przemysław Pastuszka
*/

#include <vector>
#include <string>
#include <list>
#include <iostream>
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

Row* Row::getRowCopy() {
    Row* result = new Row(schema);
    for (int i = 0; i < schema.size(); ++i) {
      switch (this->schema[i]) {
        case Schema::NUMERICAL:
          result->set<double>(i, get<double>(i));
          break;
        case Schema::STRING:
          result->set<std::string>(i, get<std::string>(i));
          break;
        case Schema::NUMERICAL_LIST:
          result->set< std::list<double> >(i, get< std::list<double> >(i));
          break;
        case Schema::STRING_LIST:
          result->set< std::list<std::string> >(i, get< std::list<std::string> >(i));
          break;          
      } 
    }
    return result;
}
