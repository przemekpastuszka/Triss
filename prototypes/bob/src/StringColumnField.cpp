#include "StringColumnField.h"

int StringColumnField::compare(ColumnField *other) {
    if (other -> getType() == STRING) {
        return value.compare(static_cast<StringColumnField*>(other) -> getValue());
    }
    return -2;
}

bool StringColumnField::equals(ColumnField *other) {
    if (other -> getType() == STRING) {
        return value.compare(static_cast<StringColumnField*>(other) -> getValue()) == 0;
    }
    return false;
}
