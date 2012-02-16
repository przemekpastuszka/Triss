#ifndef PROTOTYPES_BOB_SRC_NUMERICALLISTCOLUMN_H_
#define PROTOTYPES_BOB_SRC_NUMERICALLISTCOLUMN_H_

#include <string>
#include <list>
#include "NumericalColumn.h"

class NumericalListColumn : public NumericalColumn {
    protected:
    class NumericalListField : public NumericalField {
        public:
        bool lastListElement;

        virtual void updateNextFieldUsingMapping(int* mapping);
    };

    public:
    void addField(std::list<double>& ls, int nextFieldId);

    private:
    void addNumericalListField(double value, int nextField, bool isLastElement);
};

#endif  // PROTOTYPES_BOB_SRC_NUMERICALLISTCOLUMN_H_
