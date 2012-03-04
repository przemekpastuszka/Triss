/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_COMMON_SRC_ROW_H_
#define ROW_H_

#include "Schema.h"

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


#endif /* PROTOTYPES_COMMON_SRC_ROW_H_ */
