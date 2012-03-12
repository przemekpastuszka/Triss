/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_BOB_SRC_COLUMNS_SCALARCOLUMN_H_
#define PROTOTYPES_BOB_SRC_COLUMNS_SCALARCOLUMN_H_

#include <algorithm>
#include <vector>
#include "TypedColumn.h"

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

    void add(const Row& row, int nextFieldId) {
        Field<T> field;
        field.value = row.get<T>(this -> columnId);
        field.nextFieldId = nextFieldId;
        fields.push_back(field);
    }
    void sort() {
        std::sort(fields.begin(), fields.end());
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

    int fillRowWithValueAndGetNextFieldId(int valueIndex, Row* row) {
        if(this -> constraintRange.isInRange(valueIndex) == false) {
            return -1;
        }

        row -> set<T>(this -> columnId, fields[valueIndex].value);
        return fields[valueIndex].nextFieldId;
    }
};

#endif /* PROTOTYPES_BOB_SRC_COLUMNS_SCALARCOLUMN_H_ */
