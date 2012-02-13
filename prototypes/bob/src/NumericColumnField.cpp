#include "NumericColumnField.h"

int NumericColumnField::compare(ColumnField* other) {
    if (other -> getType() == NUMERIC) {
        double otherValue = static_cast<NumericColumnField*>(other) -> getValue();
        if (value < otherValue) {
            return -1;
        }
        if (value > otherValue) {
            return 1;
        }
        return 0;
    }
    return -2;
}

bool NumericColumnField::equals(ColumnField* other) {
    if (other -> getType() == NUMERIC) {
        double otherValue = static_cast<NumericColumnField*>(other) -> getValue();
        return value == otherValue;
    }
    return false;
}
