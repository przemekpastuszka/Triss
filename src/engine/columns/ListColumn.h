/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef TRISS_ENGINE_SRC_COLUMNS_LISTCOLUMN_H_
#define TRISS_ENGINE_SRC_COLUMNS_LISTCOLUMN_H_

#include <algorithm>
#include <list>
#include <vector>
#include "TypedColumn.h"

template <class T>
    class ListColumn : public TypedColumn<T> {
        private:
        std::vector<ListField<T> > fields;

        virtual TypedListColumnQueryState<T>* getTypedListState(ColumnQueryState* state) const {
                return static_cast<TypedListColumnQueryState<T>*>(state);
            }

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

        bool shouldBeVisited(int valueIndex, TypedListColumnQueryState<T>* state) const {
            return state -> isMainColumn && state -> constraintRange.isInRange(valueIndex);
        }

        bool isVisited(int valueIndex, TypedListColumnQueryState<T>* state) const {
            return shouldBeVisited(valueIndex, state)
                    && state -> visited.size() > valueIndex - state -> constraintRange.left
                    && state -> visited[valueIndex - state -> constraintRange.left];
        }

        void addValueToResult(int valueIndex, std::list<T>& result, TypedListColumnQueryState<T>* state) const {
            if(shouldBeVisited(valueIndex, state)) {
                int relativeValueIndex = valueIndex - state -> constraintRange.left;
                state -> visited[relativeValueIndex] = true;
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

        ColumnQueryState* prepareColumnForQuery() const {
            TypedListColumnQueryState<T>* typedListState = new TypedListColumnQueryState<T>();
            typedListState -> isMainColumn = false;
            typedListState -> visited.clear();
            return typedListState;
        }

        int lowerBound(const T& value) const {
            typename std::vector<ListField<T> >::const_iterator it =
                    std::lower_bound(fields.begin(), fields.end(), value);
            return int(it - fields.begin());
        }
        int upperBound(const T& value) const {
            typename std::vector<ListField<T> >::const_iterator it =
                    std::upper_bound(fields.begin(), fields.end(), value);
            return int(it - fields.begin()) - 1;
        }

        void markAsMainQueryColumn(ColumnQueryState* state) const {
            TypedListColumnQueryState<T>* typedListState = getTypedListState(state);
            typedListState -> isMainColumn = true;
            typedListState -> visited.resize(typedListState -> constraintRange.length());
        }

        int fillRowWithValueAndGetNextFieldId(int valueIndex, Row* row, ColumnQueryState* state, bool fill) const {
            valueIndex -= this -> globalPosition;

            TypedListColumnQueryState<T>* typedListState = getTypedListState(state);
            if(isVisited(valueIndex, typedListState)) {
                return -1;
            }

            std::list<T> result;
            bool hasAnyFieldInRange = false;
            while(this -> globalPosition <= fields[valueIndex].nextFieldId &&
                    fields[valueIndex].nextFieldId < this -> globalPosition + fields.size()) {
                hasAnyFieldInRange |= state -> constraintRange.isInRange(valueIndex);
                if(fill) {
                    addValueToResult(valueIndex, result, typedListState);
                }
                valueIndex = fields[valueIndex].nextFieldId - this -> globalPosition;
            }
            hasAnyFieldInRange |= state -> constraintRange.isInRange(valueIndex);
            if(fill) {
                addValueToResult(valueIndex, result, typedListState);
            }

            if(hasAnyFieldInRange == false) {
                return -1;
            }

            if(fill) {
                row -> set<std::list<T> >(this -> columnId, result);
            }
            return fields[valueIndex].nextFieldId;
        }
    };
#endif /* TRISS_ENGINE_SRC_COLUMNS_LISTCOLUMN_H_ */
