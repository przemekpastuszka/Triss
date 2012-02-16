#ifndef PROTOTYPES_BOB_SRC_NUMERICALCOLUMN_H_
#define PROTOTYPES_BOB_SRC_NUMERICALCOLUMN_H_

#include "Column.h"

class NumericalColumn : public Column {
    protected:
    class NumericalField : public Field {
        public:
        double value;

        virtual int compare(Field* other);
        void* getValue();
    };

    public:
    void addField(double value, int nextFieldId);
    void add(void* value, int nextFieldId) { addField(*static_cast<double*>(value), nextFieldId); }
};

#endif  // PROTOTYPES_BOB_SRC_NUMERICALCOLUMN_H_
