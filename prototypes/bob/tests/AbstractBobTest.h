#include <prototypes/bob/src/BobTable.h>
#include <prototypes/bob/src/columns/TypedColumn.h>

namespace Bob {
    class AbstractBobTest {
        public:
        template <class T>
        Field<T>* getField(BobTable* table, int columnId, int fieldId) {
            TypedColumn<T>* column = static_cast<TypedColumn<T>*>(table -> columns[columnId]);
            return column -> getField(fieldId);
        }
    };
}
