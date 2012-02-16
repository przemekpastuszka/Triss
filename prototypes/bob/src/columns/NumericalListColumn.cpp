#include "NumericalListColumn.h"

using namespace std;

void NumericalListColumn::NumericalListField::updateNextFieldUsingMapping(int *mapping) {
    if(lastListElement) {
        nextFieldId = mapping[nextFieldId];
    }
}

void NumericalListColumn::addNumericalListField(double value, int nextField, bool isLastElement) {
    NumericalListField *field = new NumericalListField();
    field->value = value;
    field->nextFieldId = nextField;
    field->lastListElement = isLastElement;
    Column::addField(field);
}

void NumericalListColumn::addField(list<double>& ls, int nextFieldId) {
    list<double>::iterator left = ls.begin(), right = ls.begin();
    right++;
    for(; right != ls.end(); left++, right++) {
        addNumericalListField(*left, getSize() + 1, false);
    }
    addNumericalListField(*left, nextFieldId, true);
}

