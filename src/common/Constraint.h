/*
* Copyright 2012 Michał Rychlik
*/
#ifndef TRISS_SRC_COMMON_CONSTRAINT_H_
#define TRISS_SRC_COMMON_CONSTRAINT_H_

#include <boost/serialization/base_object.hpp>

class Constraint {
    public:
    enum ConstraintType {
        EQUALS, CONTAINS, GREATER_OR_EQUAL, GREATER, LESS, LESS_OR_EQUAL,
        NOT_EQUAL, NOT_CONTAIN
    };
    virtual ~Constraint() {};

    protected:
    Constraint(ConstraintType type, unsigned int column) : type(type), column(column) {}
    
    private:
    ConstraintType type;
    unsigned int column;
    
    public:
    ConstraintType getConstraintType() const {
        return type;
    }
    unsigned int getAffectedColumn() const {
        return column;
    }
    template <typename Archive> void serialize(Archive& ar,
                                               const unsigned int version) {
        ar & type;
        ar & column;
    }
};

template <class T>
class TypedConstraint : public Constraint {
private:
    T constraint;
    TypedConstraint(
        Constraint::ConstraintType type, int column, const T& constraint
    ) : Constraint(type, column), constraint(constraint) {}
public:
    static TypedConstraint* equals(int column, const T& constraint) {
        return new TypedConstraint(Constraint::EQUALS, column, constraint);
    }
    static TypedConstraint* contains(int column, const T& constraint) {
        return new TypedConstraint(Constraint::CONTAINS, column, constraint);
    }    
    static TypedConstraint* greaterOrEqual(int column, const T& constraint) {
        return new TypedConstraint(Constraint::GREATER_OR_EQUAL, column,
                                   constraint);
    }
    static TypedConstraint* lessOrEqual(int column, const T& constraint) {
        return new TypedConstraint(Constraint::LESS_OR_EQUAL, column,
                                   constraint);
    }
    static TypedConstraint* greater(int column, const T& constraint) {
        return new TypedConstraint(Constraint::GREATER, column, constraint);
    }
    static TypedConstraint* less(int column, const T& constraint) {
        return new TypedConstraint(Constraint::LESS, column, constraint);
    }   
    static TypedConstraint* notEqual(int column, const T& constraint) {
        return new TypedConstraint(Constraint::NOT_EQUAL, column, constraint);
    }
    static TypedConstraint* notContains(int column, const T& constraint) {
        return new TypedConstraint(Constraint::NOT_CONTAIN, column,
                                   constraint);
    }
    T getConstraintValue() const {
        return constraint;
    }
    template <typename Archive> void serialize(Archive& ar,
                                               const unsigned int version) {
        ar & boost::serialization::base_object<Constraint>(*this);
        ar & constraint;
    }
};


#endif  // TRISS_SRC_COMMON_CONSTRAINT_H_

