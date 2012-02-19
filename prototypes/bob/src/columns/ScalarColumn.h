/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef SCALARCOLUMN_H_
#define SCALARCOLUMN_H_

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
    int getSize() const { return fields.size(); }
    void sort() {
        std::sort(fields.begin(), fields.end());
    }
    void add(void* value, int nextFieldId) {
        Field<T> field;
        field.value = *static_cast<T*>(value);
        field.nextFieldId = nextFieldId;
        fields.push_back(field);
    }
};

#endif /* SCALARCOLUMN_H_ */
