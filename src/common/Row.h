/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef TRISS_SRC_COMMON_ROW_H_
#define TRISS_SRC_COMMON_ROW_H_

#include <cstdlib>
#include <vector>
#include <string>
#include <list>
#include "ColumnDesc.h"

class Row {
    private:
    void** values;
    std::vector<ColumnDesc> schema;

    void deleteFieldAt(int index);

    Row(const std::vector<ColumnDesc>& schema);

    public:
    ~Row();

    template <class T>
    void set(int i, const T& value) {
        deleteFieldAt(i);
        values[i] = new T(value);
    }

    template <class T>
    T& get(int i) const {
        return *static_cast<T*>(values[i]);
    }

    void setNull(int i) {
        deleteFieldAt(i);
    }

    bool isNull(int i) const {
        return values[i] == NULL;
    }

    friend class Table;
};


#endif /* TRISS_SRC_COMMON_ROW_H_ */
