#ifndef PROTOTYPES_BOB_SRC_LISTCOLUMNFIELD_H_
#define PROTOTYPES_BOB_SRC_LISTCOLUMNFIELD_H_

#include "ColumnField.h"

class ListColumnField : public ColumnField {
    private:
    ColumnField* value;
    bool lastListField;

    public:
    ListColumnField(ColumnField* value, ColumnField* nextField, bool lastElement) : ColumnField(nextField) {
        this -> value = value;
        this -> lastListField = lastElement;
    }
    virtual ~ListColumnField() {
        delete value;
    }

    const ColumnField* getValue() {
        return value;
    }

    virtual int compare(ColumnField* other) {
        return value -> compare(other);
    }
    virtual bool equals(ColumnField* other) {
        return value -> equals(other);
    }

    virtual ColumnFieldType getType() {
        return value -> getType();
    }

    virtual bool isLastListField() {
        return lastListField;
    }
};

#endif  // PROTOTYPES_BOB_SRC_LISTCOLUMNFIELD_H_
