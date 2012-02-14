#ifndef PROTOTYPES_BOB_SRC_COLUMN_H_
#define PROTOTYPES_BOB_SRC_COLUMN_H_

class Column {
    protected:
    class Field {
        public:
        int nextFieldId;

        virtual int compare(Field* other) = 0;
    };
    void addField(Field* field);

    private:
    Field** fields;
    int currentSize;
    static bool compare(Field* a, Field* b);

    public:
    Column() { currentSize = 0; }
    virtual ~Column();
    void init(int capacity) {
        fields = new Field*[capacity];
    }

    void sort();
    int getSize() { return currentSize; }
};

#endif  // PROTOTYPES_BOB_SRC_COLUMN_H_
