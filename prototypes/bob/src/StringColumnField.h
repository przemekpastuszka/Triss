#ifndef PROTOTYPES_BOB_SRC_STRINGCOLUMNFIELD_H_
#define PROTOTYPES_BOB_SRC_STRINGCOLUMNFIELD_H_

#include <string>
#include "ColumnField.h"

class StringColumnField : public ColumnField {
    private:
    std :: string value;

    public:
    StringColumnField(const std :: string& value, ColumnField* nextField)
        : ColumnField(nextField) {
        this -> value = value;
    }

    const std :: string& getValue() {
        return value;
    }

    virtual int compare(ColumnField* other);
    virtual bool equals(ColumnField* other);

    virtual ColumnFieldType getType() {
        return STRING;
    }
};

#endif  // PROTOTYPES_BOB_SRC_STRINGCOLUMNFIELD_H_
