/*
* Copyright 2012 Michał Rychlik
*/
#ifndef PROTOTYPES_COMMON_SRC_CONSTRAINT_H_
#define PROTOTYPES_COMMON_SRC_CONSTRAINT_H_

class Constraint {
    public:
    enum ConstraintType {
        EQUALS, CONTAINS, GREATER_OR_EQUAL, LESS_OR_EQUAL
    };

    protected:
    Constraint(ConstraintType type, int column) : type(type), column(column) {};
    
    private:
    ConstraintType type;
    int column;
    
    public:
    ConstraintType getConstraintType() const { return type; }
    int getAffectedColumn() const { return column; }
};

template <class T>
class TypedConstraint : public Constraint {
    private:
    T constraint;
    TypedConstraint(Constraint::ConstraintType type, int column, T constraint) :
        Constraint(type, column), constraint(constraint) {};
    
    public:
        static TypedConstraint* equals(int column, T constraint) {
            return new TypedConstraint(Constraint::EQUALS, column, constraint);
        };
        
        static TypedConstraint* contains(int column, T constraint) {
            return new TypedConstraint(Constraint::CONTAINS, column,
                                       constraint);
        };
        
        static TypedConstraint* greaterOrEqual(int column, T constraint) {
            return new TypedConstraint(Constraint::GREATER_OR_EQUAL, column, constraint);
        };
        
        static TypedConstraint* lessOrEqual(int column, T constraint) {
            return new TypedConstraint(Constraint::LESS_OR_EQUAL, column, constraint);
        };

        T getConstraintValue() const { return constraint; }
};


#endif  // PROTOTYPES_COMMON_SRC_CONSTRAINT_H_

