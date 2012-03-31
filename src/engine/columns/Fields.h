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

    virtual void updateNextFieldUsingMapping(std::vector<int>& current, std::vector<int>& next) {
        nextFieldId = next[nextFieldId];
    }

    virtual bool isLastListElement() {
        return true;
    }
};

template <class T>
bool operator<(const T& left, const Field<T>& other) {
    return left < other.value;
}

template <class T>
class ListField : public Field<T> {
    public:
    bool isLastElement;

    void updateNextFieldUsingMapping(std::vector<int>& current, std::vector<int>& next) {
        if(isLastElement) {
            this -> nextFieldId = next[this -> nextFieldId];
        }
        else {
            this -> nextFieldId = current[this -> nextFieldId];
        }
    }

    bool isLastListElement() {
        return isLastElement;
    }
};

#endif /* TRISS_ENGINE_SRC_COLUMNS_FIELDS_H_ */
