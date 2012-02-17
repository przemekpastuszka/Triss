#ifndef PROTOTYPES_BOB_SRC_TABLE_H_
#define PROTOTYPES_BOB_SRC_TABLE_H_

#include <string>
#include <list>
#include <algorithm>
#include "columns/Column.h"


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

class Row {
    private:
    void** values;
    int size;

    public:
    Row(const Schema& schema) {
        size = schema.schema.size();
        values = new void*[size];
    }
    ~Row() {
        for(int i = 0; i < size; ++i) {
            delete values[i];
        }
        delete [] values;
    }

    template <class T>
    void set(int i, const T& value) {
        values[i] = new T(value);
    }

    void* get(int i) {
        return values[i];
    }
};

class Table {
    private:
    std::vector<Schema::DataType> schema;
    std::vector<Column*> columns;

    void prepareCrossColumnPointers();

    public:
    Table(const Schema& schema);
    ~Table();

    void addRow(Row& row);
    void prepareStructure();
    int getNrOfColumns() const {
        return schema.size();
    }

#ifdef TRISS_TEST
    template <class T>
    TypedColumn<T>* getColumn(int i) {
        return static_cast<TypedColumn<T>*>(columns[i]);
    }
#endif
};

#endif  // PROTOTYPES_BOB_SRC_TABLE_H_
