/*
* Copyright 2012 Michał Rychlik
*/
#ifndef TRISS_SRC_COMMON_CONSTRAINT_H_
#define TRISS_SRC_COMMON_CONSTRAINT_H_

class Constraint {
    public:
    enum ConstraintType {
        EQUALS, CONTAINS, GREATER_OR_EQUAL, GREATER, LESS, LESS_OR_EQUAL
    };
    virtual ~Constraint() {};

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
    TypedConstraint(Constraint::ConstraintType type, int column, const T& constraint) :
        Constraint(type, column), constraint(constraint) {};
    
    public:
        static TypedConstraint* equals(int column, const T& constraint) {
            return new TypedConstraint(Constraint::EQUALS, column, constraint);
        };
        
        static TypedConstraint* contains(int column, const T& constraint) {
            return new TypedConstraint(Constraint::CONTAINS, column,
                                       constraint);
        };
        
        static TypedConstraint* greaterOrEqual(int column, const T& constraint) {
            return new TypedConstraint(Constraint::GREATER_OR_EQUAL, column, constraint);
        };
        
        static TypedConstraint* lessOrEqual(int column, const T& constraint) {
            return new TypedConstraint(Constraint::LESS_OR_EQUAL, column, constraint);
        };

        static TypedConstraint* greater(int column, const T& constraint) {
            return new TypedConstraint(Constraint::GREATER, column, constraint);
        };

        static TypedConstraint* less(int column, const T& constraint) {
            return new TypedConstraint(Constraint::LESS, column, constraint);
        };

        T getConstraintValue() const { return constraint; }
};


#endif  // TRISS_SRC_COMMON_CONSTRAINT_H_

