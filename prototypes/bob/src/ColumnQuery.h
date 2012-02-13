#ifndef PROTOTYPES_BOB_SRC_COLUMNQUERY_H_
#define PROTOTYPES_BOB_SRC_COLUMNQUERY_H_

#include <cstdlib>
#include <string>
#include "NumericColumnField.h"
#include "StringColumnField.h"
#include "ColumnField.h"

struct ColumnQuery {
    private:
    enum QueryType { EQ, RANGE };
    QueryType type;
    ColumnField *left, *right;

    ColumnQuery(QueryType type, ColumnField* left, ColumnField* right) {
        this -> type = type;
        this -> left = left;
        this -> right = right;
    }

    public:
    ~ColumnQuery() {
        delete left;
        delete right;
    }

    static ColumnQuery* eqQuery(double value) {
        return new ColumnQuery(EQ, new NumericColumnField(value, NULL), new NumericColumnField(value, NULL));
    }

    static ColumnQuery* eqQuery(const std :: string& value) {
        return new ColumnQuery(EQ, new StringColumnField(value, NULL), new StringColumnField(value, NULL));
    }

    static ColumnQuery* rangeQuery(double left, double right) {
        return new ColumnQuery(RANGE, new NumericColumnField(left, NULL), new NumericColumnField(right, NULL));
    }

    static ColumnQuery* rangeQuery(const std :: string& left, const std :: string& right) {
        return new ColumnQuery(RANGE, new StringColumnField(left, NULL), new StringColumnField(right, NULL));
    }
};

#endif  // PROTOTYPES_BOB_SRC_COLUMNQUERY_H_
