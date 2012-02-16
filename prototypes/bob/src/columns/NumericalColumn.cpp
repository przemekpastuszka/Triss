#include "NumericalColumn.h"

int NumericalColumn::NumericalField::compare(Field *other) {
    NumericalField *otherNumericalField = static_cast<NumericalField*>(other);
    if (value < otherNumericalField -> value) {
        return -1;
    }
    return value > otherNumericalField -> value;
}

void* NumericalColumn::NumericalField::getValue() {
    return &value;
}

void NumericalColumn::addField(double value, int nextFieldId) {
    NumericalField *field = new NumericalField();
    field -> value = value;
    field -> nextFieldId = nextFieldId;
    Column :: addField(field);
}

