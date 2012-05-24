===========
API Reference
===========

Basic Usage
=============

Every application using Triss should be compiled with triss linked::

    $ g++ -ltriss example.cpp


.. cpp:class:: DataBase

    Central object to any application using Triss. Used to manage tables and query them.

.. cpp:function:: DataBase::DataBase()

    Argument-less constructor.

.. cpp:function:: void DataBase::createTable(std::string name, std::vector<ColumnDesc> &columns)

    Creates Table of given *name*, with given *schema*.

    :param name: table name
    :param columns: vector of ColumnDesc objects describing Table Schema

.. cpp:function:: void DataBase::loadTable(std::string name, std::string file, char delim = ',', char esc = '\\')

    Loads Table named *name* from CSV *file*. DataBase must be created first with call to *createTable*

    :param name: table name
    :param file: path to file containing data
    :param delim: file's columns delimiter
    :param esc: file's escape character

.. cpp:function:: void DataBase::dropTable(std::string name)

    Drops in-memory Table with *name*, erases assosiated file.

    :param name: table name

.. code-block:: guess

    #include <vector>
    #include <triss/src/db/db.h>

    int main(void) {
        DataBase db;
        std::vector<ColumnDesc> s;
        s.resize(1, ColumnDesc("made", Schema::STRING));
        db.createTable("cars", s);
        db.loadTable("cars", "./build/cars.csv", ';');
        db.dropTable("cars");
        return 0;
    }

.. cpp:function:: Result * DataBase::select(std::string name, Query &q)

    Selects items from table with *name* matching query *q*

    :param name: name of table to query
    :param q: Query object with constraints
    :rtype: pointer to Result object with items matching q

.. cpp:class:: ColumnDesc

    Simple container for description of single Table column.

.. cpp:function:: ColumnDesc::ColumnDesc(const std::string& n, Schema::DataType t)

    Creates ColumnDesc object with column name *n* of type *t*

    :param n: column name
    :param t: column type

    Currently supported types include:

    * Schema::NUMERICAL - any numerical castable to double
    * Schema::STRING - string of arbitrary length
    * Schema::NUMERICAL_LIST - unbounded list of numericals
    * Schema::STRING_LIST - unbounded list of strgins

.. cpp:class:: Query

    Simple container for column constraints

.. cpp:function:: Query::Query()

    Argument-less constructor, use addConstraint method to populate with constraints

.. cpp:function:: Query::addConstraint(Constraint* constraint)

    Ads Constraint *constraint* to Query

    :param constraint: pointer to Constraint object created by one of predefined static methods

    Available constraints (taking column number or name) include:

    * Constraint::equals<T>
    * Constraint::contains<T>
    * Constraint::greaterOrEqual<T>
    * Constraint::greater<T>
    * Constraint::less<T>
    * Constraint::lessOrEqual<T>
    * Constraint::notEqual<T>
    * Constraint::notContains<T>

.. cpp:function:: void Query::limit(unsigned int limit)

    Restricts number of items returned by query to *limit*

    :param limit: maximum number of items returned from table

.. code-block:: guess

    #include <string>
    #include <triss/src/db/db.h>

    int main(void) {
        Query q;
        q.addConstraint(Constraint::less<double>(0, 27));  // you may specify column by its number
        q.addConstraint(Constraint::equals<std::string>("two", "zzz"));  // or its name
        q.limit(10);
        return 0;
    }

.. cpp:class:: Result

    Container for items returned from Table

.. cpp:function:: bool Result::hasNext()

    True if not all items fetched yet. Most often used as loop termination condition.

.. cpp:function:: Row * Result::next()

    Returns pointer to next unfetched *Row* object

.. cpp:function:: std::list<Row*> * Result::fetchAll()

    Returns list of pointers to *Row* objects (no metter how many Rows were fetched before using next method, all items will be returned)

.. cpp:class:: Row

    Single Table row

.. cpp:function:: T& Row::get(int i) const

    Returns value in column numbered *i* from row

    :param i: column index

.. code-block:: guess

    #include <triss/src/db/db.h>

    int main(void) {
        /* fetching result omitted */
        while(result->hasNext()) {
            Row * row = result->next();
            std::cout << row->get<double>(0) << std::endl;
        }
        delete result;
        return 0;
    }

Examples
=============

Full example of typical interaction with Triss

.. code-block:: guess

    #include <vector>
    #include <triss/src/db/db.h>

    int main(void) {
        DataBase db;
        std::vector<ColumnDesc> s;
        s.resize(1, ColumnDesc("made", Schema::STRING));
        db.createTable("cars", s);
        db.loadTable("cars", "./build/cars.csv", ';');
        Query q;
        q.addConstraint(Constraint::equals<std::string>("made", "ford"));
        Result * result = db.select("cars", q);        
        while(result->hasNext()) {
            Row * row = result->next();
            std::cout << row->get<double>(0) << std::endl;
        }
        delete result;
        db.dropTable("cars");
        return 0;
    }


