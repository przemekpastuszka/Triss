#ifndef PROTOTYPES_BOB_SRC_COLUMN_H_
#define PROTOTYPES_BOB_SRC_COLUMN_H_

class Column {
    protected:
    class Field {
        public:
        int nextFieldId;

        virtual int compare(Field* other) = 0;
        virtual void updateNextFieldUsingMapping(int* mapping);
    };
    void addField(Field* field);

    private:
    struct Position;

    Field** fields;
    int currentSize;

    static bool compare(Field* a, Field* b);
    static bool compareFieldsIdentifiedByPositions(const Position& a, const Position& b);

    public:
    Column() { currentSize = 0; }
    virtual ~Column();
    void init(int capacity) {
        fields = new Field*[capacity];
    }

    int getSize() { return currentSize; }

    void sort();
    int* getMappingFromCurrentToSortedPositions();
    void updateNextFieldIdsUsingMapping(int* mapping);

#ifdef TRISS_TEST
    Field** getFields() { return fields; }
#endif
};

#endif  // PROTOTYPES_BOB_SRC_COLUMN_H_
