#ifndef PROTOTYPES_BOB_SRC_NUMERICALCOLUMN_H_
#define PROTOTYPES_BOB_SRC_NUMERICALCOLUMN_H_

#include "Column.h"

class NumericalColumn : public Column {
    protected:
    class NumericalField : public Field {
        public:
        double value;

        virtual int compare(Field* other);
    };

    public:
    void addField(double value, int nextFieldId);
};

#endif  // PROTOTYPES_BOB_SRC_NUMERICALCOLUMN_H_
