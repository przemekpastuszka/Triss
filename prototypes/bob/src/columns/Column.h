/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_BOB_SRC_COLUMNS_COLUMN_H_
#define PROTOTYPES_BOB_SRC_COLUMNS_COLUMN_H_

#include <algorithm>
#include "Fields.h"

class Column {
    public:
    struct IndexRange {
        int left, right;
    };

    virtual unsigned int getSize() const = 0;

    virtual void sort() = 0;
    virtual int* getMappingFromCurrentToSortedPositions() = 0;
    virtual void updateNextFieldIdsUsingMapping(int* currentColumnMapping, int *nextColumnMapping) = 0;

    virtual void add(void* value, int nextFieldId) = 0;
    virtual IndexRange findRange(void* left, void* right) = 0;
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

    int binarySearch(const T& element, bool isLowerBound);

    public:
    virtual Field<T>* getField(unsigned int i) = 0;

    public:
    int* getMappingFromCurrentToSortedPositions();
    void updateNextFieldIdsUsingMapping(int* currentColumnMapping, int *nextColumnMapping);

    IndexRange findRange(void* left, void* right);
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

template<class T> int TypedColumn<T>::binarySearch(const T& element, bool isLowerBound) {
    int p = 0, q = getSize() - 1;
    while(p < q) {
        int d = (p + q) / 2;
        bool goLeft = isLowerBound ? element <= getField(d) -> value : element < getField(d) -> value;
        if(goLeft) {
            q = d - 1;
        }
        else {
            p = d + 1;
        }
    }
    if(isLowerBound) {
        return element <= getField(p) -> value ? p : p + 1;
    }
    else {
        return element >= getField(p) -> value ? p : p - 1;
    }
}

template<class T> Column::IndexRange TypedColumn<T>::findRange(void* left, void* right) {
    IndexRange range;
    range.left = binarySearch(*static_cast<T*>(left), true);
    range.right = binarySearch(*static_cast<T*>(right), false);
    if(range.left >= getSize() || range.right < 0) {
        range.left = range.right = -1;
    }
    return range;
}



#endif  // PROTOTYPES_BOB_SRC_COLUMNS_COLUMN_H_
