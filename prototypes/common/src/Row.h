/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_COMMON_SRC_ROW_H_
#define PROTOTYPES_COMMON_SRC_ROW_H_

#include <cstdlib>
#include "Schema.h"

class Row {
    private:
    void** values;
    int size;

    public:
    Row(const Schema& schema) {
        size = schema.schema.size();
        values = new void*[size];
        for(int i = 0; i < size; ++i) {
            values[i] = NULL;
        }
    }
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
    T& get(int i) {
        return *static_cast<T*>(values[i]);
    }

    void* getPointer(int i) {
        return values[i];
    }
};


#endif /* PROTOTYPES_COMMON_SRC_ROW_H_ */
