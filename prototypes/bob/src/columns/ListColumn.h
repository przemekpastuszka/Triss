/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_BOB_SRC_COLUMNS_LISTCOLUMN_H_
#define PROTOTYPES_BOB_SRC_COLUMNS_LISTCOLUMN_H_

#include <algorithm>
#include <list>
#include <vector>
#include "TypedColumn.h"

template <class T>
class ListColumn : public TypedColumn<T> {
    private:
    std::vector<ListField<T> > fields;

    std::vector<bool> visited;
    bool isMainColumn;

    public:
    Field<T>* getField(unsigned int i) {
        return &fields[i];
    }

    protected:
    void addField(const T& value, int nextFieldId, bool isLastElement) {
        ListField<T> field;
        field.value = value;
        field.nextFieldId = nextFieldId;
        field.isLastElement = isLastElement;
        fields.push_back(field);
    }

    bool shouldBeVisited(int valueIndex) {
        return isMainColumn && this -> constraintRange.isInRange(valueIndex);
    }

    bool isVisited(int valueIndex) {
        return shouldBeVisited(valueIndex) && visited[valueIndex - this -> constraintRange.left];
    }

    void addValueToResult(int valueIndex, std::list<T>& result) {
        if(shouldBeVisited(valueIndex)) {
            visited[valueIndex - this -> constraintRange.left] = true;
        }
        result.push_back(fields[valueIndex].value);
    }

    public:
    unsigned int getSize() const { return fields.size(); }

    void add(const Row& row, int nextFieldId) {
        std::list<T>& ls = row.get<std::list<T> >(this -> columnId);

        typename std::list<T>::iterator left = ls.begin(), right = ls.begin();
        right++;
        for(;right != ls.end(); left++, right++) {
            addField(*left, fields.size() + 1, false);
        }
        addField(*left, nextFieldId, true);
    }

    void sort() {
        std::sort(fields.begin(), fields.end());
    }

    void prepareColumnForQuery() {
        TypedColumn<T>::prepareColumnForQuery();
        isMainColumn = false;
        visited.clear();
    }

    int lowerBound(const T& value) {
        typename std::vector<ListField<T> >::iterator it =
                std::lower_bound(fields.begin(), fields.end(), value);
        return int(it - fields.begin());
    }
    int upperBound(const T& value) {
        typename std::vector<ListField<T> >::iterator it =
                std::upper_bound(fields.begin(), fields.end(), value);
        return int(it - fields.begin()) - 1;
    }

    void markAsMainQueryColumn() {
        isMainColumn = true;
        visited.resize(this -> constraintRange.length(), false);
    }

    int fillRowWithValueAndGetNextFieldId(int valueIndex, Row* row) {
        if(isVisited(valueIndex)) {
            return -1;
        }

        std::list<T> result;
        bool hasAnyFieldInRange = false;
        while(fields[valueIndex].isLastElement == false) {
            hasAnyFieldInRange |= this -> constraintRange.isInRange(valueIndex);
            addValueToResult(valueIndex, result);
            valueIndex = fields[valueIndex].nextFieldId;
        }
        hasAnyFieldInRange |= this -> constraintRange.isInRange(valueIndex);
        addValueToResult(valueIndex, result);

        if(hasAnyFieldInRange == false) {
            return -1;
        }

        row -> set<std::list<T> >(this -> columnId, result);
        return fields[valueIndex].nextFieldId;
    }
};

#endif /* PROTOTYPES_BOB_SRC_COLUMNS_LISTCOLUMN_H_ */
