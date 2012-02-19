/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_BOB_SRC_COLUMN_H_
#define PROTOTYPES_BOB_SRC_COLUMN_H_

#include <algorithm>
#include "Fields.h"

class Column {
    public:
    virtual int getSize() const = 0;

    virtual void sort() = 0;
    virtual int* getMappingFromCurrentToSortedPositions() = 0;
    virtual void updateNextFieldIdsUsingMapping(int* currentColumnMapping, int *nextColumnMapping) = 0;

    virtual void add(void* value, int nextFieldId) = 0;
};

template <class T>
class TypedColumn : public Column {
    private:
    struct Position {
        int position;
        T element;

        bool operator<(const Position& t) const {
            return element < t.element;
        }
    };

    public:
    virtual Field<T>* getField(unsigned int i) = 0;

    public:
    int* getMappingFromCurrentToSortedPositions();
    void updateNextFieldIdsUsingMapping(int* currentColumnMapping, int *nextColumnMapping);
};

template <class T>
int* TypedColumn<T>::getMappingFromCurrentToSortedPositions() {
    Position* positions = new Position[getSize()];
    for(unsigned int i = 0; i < getSize(); ++i) {
        positions[i].element = this -> getField(i) -> value;
        positions[i].position = i;
    }
    std::sort(positions, positions + getSize());

    int* mapping = new int[getSize()];
    for(unsigned int i = 0; i < getSize(); ++i) {
        mapping[positions[i].position] = i;
    }

    delete [] positions;

    return mapping;
}

template <class T>
void TypedColumn<T>::updateNextFieldIdsUsingMapping(int* currentColumnMapping, int *nextColumnMapping) {
    for(unsigned int i = 0; i < getSize(); ++i) {
        getField(i) -> updateNextFieldUsingMapping(currentColumnMapping, nextColumnMapping);
    }
}

#endif  // PROTOTYPES_BOB_SRC_COLUMN_H_
