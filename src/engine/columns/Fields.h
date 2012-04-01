/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef TRISS_ENGINE_SRC_COLUMNS_FIELDS_H_
#define TRISS_ENGINE_SRC_COLUMNS_FIELDS_H_

#include <vector>

template <class T>
class Field {
    public:
    virtual ~Field() {};

    T value;
    int nextFieldId;

    bool operator<(const Field<T>& other) const {
        return value < other.value;
    }
    bool operator<(const T& other) const {
        return value < other;
    }
};

template <class T>
bool operator<(const T& left, const Field<T>& other) {
    return left < other.value;
}

#endif /* TRISS_ENGINE_SRC_COLUMNS_FIELDS_H_ */
