/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_BOB_SRC_COLUMNS_SCALARCOLUMN_H_
#define PROTOTYPES_BOB_SRC_COLUMNS_SCALARCOLUMN_H_

#include <algorithm>
#include <vector>
#include "Column.h"

template <class T>
class ScalarColumn : public TypedColumn<T> {
    private:
    std::vector<Field<T> > fields;

    public:
    Field<T>* getField(unsigned int i) {
        return &fields[i];
    }

    public:
    unsigned int getSize() const { return fields.size(); }
    void sort() {
        std::sort(fields.begin(), fields.end());
    }
    void add(void* value, int nextFieldId) {
        Field<T> field;
        field.value = *static_cast<T*>(value);
        field.nextFieldId = nextFieldId;
        fields.push_back(field);
    }

    int lowerBound(const T& value) {
        typename std::vector<Field<T> >::iterator it =
                std::lower_bound(fields.begin(), fields.end(), value);
        return int(it - fields.begin());
    }
    int upperBound(const T& value) {
        typename std::vector<Field<T> >::iterator it =
                std::upper_bound(fields.begin(), fields.end(), value);
        return int(it - fields.begin()) - 1;
    }
    int fillRowWithValueAndGetNextFieldId(int valueIndex, int columnIndex, Row* row) const {
        row -> set<T>(columnIndex, fields[valueIndex].value);
        return fields[valueIndex].nextFieldId;
    }
};

#endif /* PROTOTYPES_BOB_SRC_COLUMNS_SCALARCOLUMN_H_ */
