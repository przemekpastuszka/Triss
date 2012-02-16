#include "StringColumn.h"

int StringColumn::StringField::compare(Field *other) {
    StringField *otherStringField = static_cast<StringField*>(other);
    return value.compare(otherStringField-> value);
}

void StringColumn::addField(const std :: string& value, int nextFieldId) {
    StringField *field = new StringField();
    field -> value = value;
    field -> nextFieldId = nextFieldId;
    Column :: addField(field);
}

