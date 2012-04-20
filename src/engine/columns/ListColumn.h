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
        TypedListColumnQueryState<T>* getTypedListState(ColumnQueryState* state) const {
            return static_cast<TypedListColumnQueryState<T>*>(state);
        }

        protected:
        bool hasBeenVisited(int valueIndex, int startPoint, TypedListColumnQueryState<T>* state) const {
            return state -> constraintRangeSet.ranges[0].left <= valueIndex && valueIndex < startPoint;
        }

        public:
        void updateNextFieldIdsUsingMapping(std::vector<int>& current, std::vector<int>& next, int indicesShift) {
            for(unsigned int i = 0; i < this -> fields.size(); ++i) {
                if(this -> fields[i].isLastElement) {
                    this -> fields[i].nextFieldId = next[this -> fields[i].nextFieldId] + indicesShift;
                }
                else {
                    this -> fields[i].nextFieldId = current[this -> fields[i].nextFieldId];
                }
            }
        }

        void add(const Row& row, int nextFieldId) {
            if(row.isNull(this -> columnId)) {
                this -> addNull(nextFieldId);
            }
            else {
                std::list<T>& ls = row.get<std::list<T> >(this -> columnId);

                typename std::list<T>::iterator left = ls.begin(), right = ls.begin();
                right++;
                for(;right != ls.end(); left++, right++) {
                    this -> addField(*left, this -> fields.size() + 1, false);
                }
                this -> addField(*left, nextFieldId, true);
            }
        }

        ColumnQueryState* prepareColumnForQuery() const {
            TypedListColumnQueryState<T>* typedListState = new TypedListColumnQueryState<T>();
            typedListState -> isMainColumn = false;
            return typedListState;
        }

        void markAsMainQueryColumn(ColumnQueryState* state) const {
            TypedListColumnQueryState<T>* typedListState = getTypedListState(state);
            typedListState -> isMainColumn = true;
        }

        bool isFieldInvalid(TypedListColumnQueryState<T>* typedListState, int valueIndex, int startPoint) const {
            return (typedListState -> isMainColumn && hasBeenVisited(valueIndex, startPoint, typedListState))
                        || (typedListState -> valueRangeSet != NULL && typedListState -> valueRangeSet -> isExcluded(this -> fields[valueIndex].value));
        }
        
        int fillRowWithValueAndGetNextFieldId(int valueIndex, int startPoint, Row* row, ColumnQueryState* state, const std::vector<ColumnDesc>& schema, bool fill) const {
            TypedListColumnQueryState<T>* typedListState = getTypedListState(state);
            if(isFieldInvalid(typedListState, valueIndex, startPoint)) {
                return -1;
            }

            std::list<T> result;
            bool hasAnyFieldInRange = false;
            while(this -> fields[valueIndex].nextFieldId < this -> fields.size()) {
                hasAnyFieldInRange |= state -> constraintRangeSet.isInRange(valueIndex);
                if(fill) {
                    result.push_back(this -> fields[valueIndex].value);
                }
                valueIndex = this -> fields[valueIndex].nextFieldId;
                if(isFieldInvalid(typedListState, valueIndex, startPoint)) {
                    return -1;
                }
            }
            hasAnyFieldInRange |= state -> constraintRangeSet.isInRange(valueIndex);
            if(fill) {
                result.push_back(this -> fields[valueIndex].value);
            }

            if(hasAnyFieldInRange == false) {
                return -1;
            }

            if(fill) {
                row -> set<std::list<T> >(this -> columnId, result, schema);
            }
            return this -> fields[valueIndex].nextFieldId;
        }
    };
#endif /* TRISS_ENGINE_SRC_COLUMNS_LISTCOLUMN_H_ */
