/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_COMMON_SRC_SCHEMA_H_
#define PROTOTYPES_COMMON_SRC_SCHEMA_H_

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
    Schema(const std::vector<DataType> schema) {
        this -> schema = schema;
    }
};


#endif /* PROTOTYPES_COMMON_SRC_SCHEMA_H_ */
