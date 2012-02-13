#ifndef PROTOTYPES_BOB_SRC_NUMERICCOLUMNFIELD_H_
#define PROTOTYPES_BOB_SRC_NUMERICCOLUMNFIELD_H_

#include "ColumnField.h"

class NumericColumnField : public ColumnField  {
    private:
    double value;

    public:
    NumericColumnField(double value, ColumnField* nextField) : ColumnField(nextField) {
        this -> value = value;
    }

    double getValue() {
        return value;
    }

    virtual int compare(ColumnField* other);
    virtual bool equals(ColumnField* other);

    virtual ColumnFieldType getType() {
        return NUMERIC;
    }
};

#endif  // PROTOTYPES_BOB_SRC_NUMERICCOLUMNFIELD_H_
