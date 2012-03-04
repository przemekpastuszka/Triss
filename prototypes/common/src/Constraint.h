/*
* Copyright 2012 Michał Rychlik
*/
#ifndef PROTOTYPES_COMMON_SRC_CONSTRAINT_H_
#define PROTOTYPES_COMMON_SRC_CONSTRAINT_H_

class Constraint {
    protected:
    enum ConstraintType {
        EQUALS, NOT_EQUALS, CONTAINS, NOT_CONTAINS, GREATER, LESS
    };
    Constraint(ConstraintType type, int column) : type(type), column(column) {};
    
    private:
    ConstraintType type;
    int column;
    
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
        
        static TypedConstraint* notEquals(int column, T constraint) {
            return new TypedConstraint(Constraint::NOT_EQUALS, column,
                                       constraint);
        };
        
        static TypedConstraint* contains(int column, T constraint) {
            return new TypedConstraint(Constraint::CONTAINS, column,
                                       constraint);
        };
        
        static TypedConstraint* not_contains(int column, T constraint) {
            return new TypedConstraint(Constraint::NOT_CONTAINS, column,
                                       constraint);
        };
        
        static TypedConstraint* greater(int column, T constraint) {
            return new TypedConstraint(Constraint::GREATER, column, constraint);
        };
        
        static TypedConstraint* less(int column, T constraint) {
            return new TypedConstraint(Constraint::LESS, column, constraint);
        };
};

#endif  // PROTOTYPES_COMMON_SRC_CONSTRAINT_H_

