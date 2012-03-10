/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_COMMON_SRC_ROW_H_
#define PROTOTYPES_COMMON_SRC_ROW_H_

#include <cstdlib>
#include <vector>
#include "Schema.h"

class Row {
    private:
    void** values;
    int size;

    void init(const std::vector<Schema::DataType>& schema) {
        size = schema.size();
        values = new void*[size];
        for(int i = 0; i < size; ++i) {
            values[i] = NULL;
        }
    }

    public:
    Row(const Schema& schema) { init(schema.schema); }
    Row(const std::vector<Schema::DataType>& schema) { init(schema); }

    ~Row() {
        for(int i = 0; i < size; ++i) {
            if(values[i] != NULL) {
                delete values[i];
            }
        }
        delete [] values;
    }

    template <class T>
    void set(int i, const T& value) {
        if(values[i] != NULL) {
            delete values[i];
        }
        values[i] = new T(value);
    }

    template <class T>
    T& get(int i) const {
        return *static_cast<T*>(values[i]);
    }

    bool isNull(int i) const {
        return values[i] == NULL;
    }
};


#endif /* PROTOTYPES_COMMON_SRC_ROW_H_ */
