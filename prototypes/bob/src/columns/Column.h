#ifndef PROTOTYPES_BOB_SRC_COLUMN_H_
#define PROTOTYPES_BOB_SRC_COLUMN_H_

#include <vector>
#include <list>
#include <algorithm>

template <class T>
class Field {
    public:
    T value;
    int nextFieldId;

    bool operator<(const Field<T>& other) const {
        return value < other.value;
    }

    virtual void updateNextFieldUsingMapping(int* currentColumnMapping, int* nextColumnMapping) {
        nextFieldId = nextColumnMapping[nextFieldId];
    }

    virtual bool isLastListElement() {
        return true;
    }
};

template <class T>
class ListField : public Field<T> {
    public:
    bool isLastElement;

    virtual void updateNextFieldUsingMapping(int* currentColumnMapping, int* nextColumnMapping) {
        if(isLastElement) {
            this -> nextFieldId = nextColumnMapping[this -> nextFieldId];
        }
        else {
            this -> nextFieldId = currentColumnMapping[this -> nextFieldId];
        }
    }

    bool isLastListElement() {
        return isLastElement;
    }
};

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

template <class T>
class ListColumn : public TypedColumn<T> {
    private:
    std::vector<ListField<T> > fields;

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
    void addList(std::list<T> *ls, int nextFieldId) {
        typename std::list<T>::iterator left = ls-> begin(), right = ls -> begin();
        right++;
        for(;right != ls -> end(); left++, right++){
            addField(*left, fields.size() + 1, false);
        }
        addField(*left, nextFieldId, true);
    }

    public:
    int getSize() const { return fields.size(); }
    void sort() {
        std::sort(fields.begin(), fields.end());
    }

    void add(void *value, int nextFieldId) {
        std::list<T> *ls = static_cast<std::list<T>*>(value);
        addList(ls, nextFieldId);
    }
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
