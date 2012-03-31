/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef TRISS_SRC_COMMON_SCHEMA_H_
#define TRISS_SRC_COMMON_SCHEMA_H_

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


#endif /* TRISS_SRC_COMMON_SCHEMA_H_ */
