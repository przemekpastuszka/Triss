/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef SCHEMA_H_
#define SCHEMA_H_

#include <vector>

class Schema {
    public:
    enum DataType {
        NUMERICAL, STRING, NUMERICAL_LIST, STRING_LIST
    };
    std::vector<DataType> schema;

    Schema(DataType* schema, int n) {
        this -> schema.assign(schema, schema + n);
    }
};


#endif /* SCHEMA_H_ */
