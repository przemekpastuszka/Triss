#ifndef PROTOTYPES_BOB_SRC_COLUMNFIELD_H_
#define PROTOTYPES_BOB_SRC_COLUMNFIELD_H_

class ColumnField {
    private:
    ColumnField* nextField;

    public:
    explicit ColumnField(ColumnField* nextField) {
        this -> nextField = nextField;
    }
    virtual ~ColumnField() {}

    enum ColumnFieldType { NUMERIC, STRING };

    virtual int compare(ColumnField* other) = 0;
    virtual bool equals(ColumnField* other) = 0;

    virtual ColumnFieldType getType() = 0;

    virtual ColumnField* getNextFieldFromSameRow() {
        return nextField;
    }
};

#endif  // PROTOTYPES_BOB_SRC_COLUMNFIELD_H_
