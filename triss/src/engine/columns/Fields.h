/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef TRISS_ENGINE_SRC_COLUMNS_FIELDS_H_
#define TRISS_ENGINE_SRC_COLUMNS_FIELDS_H_

#include <vector>

template <class T>
class Field {
    public:
    Field() : isLastElement(true), isNull(false), isEmptyList(false) {};
    Field(const T& v) : value(v), isLastElement(true), isNull(false), isEmptyList(false) {};
    virtual ~Field() {};

    T value;
    int nextFieldId;
    bool isLastElement;
    bool isNull;
    bool isEmptyList;

    bool operator<(const Field<T>& other) const {
        if(isNull) {
            return false;
        }
        if(isEmptyList) {
            return other.isNull;
        }
        if(other.isNull || other.isEmptyList) {
            return true;
        }
        return value < other.value;
    }
};

#endif /* TRISS_ENGINE_SRC_COLUMNS_FIELDS_H_ */
