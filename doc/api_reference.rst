===========
API Reference
===========

Basic Usage
=============
* Adapt chosen prototype to alpha version

    * *Done:*
        * architecture review
        * support NULLs
        * support empty lists
        * not equals, not contains support
        * less, greater constraints support
        * check system using valgrind

    * *In progress:*
        * validate user interactions (system cannot crash when wrong input was given)
        * save / restore metadata (tables and their schemas) to / from disk
        * columns accessible by their names, not ids
        * full set of datatypes, e.g. short, int, long, float, double, char...
        * multiple tables

    * *Moved to future:*
        * memory / time optimizations

    * *Dropped:*
        * storage compression

* Add capability to query TRISS through network, server / client side


Examples
=============
* add SQL interface
* create full documentation
* create installer with .deb and .rpm packages

* add OR operator
* bindings to languages other than C++
* ORDER BY clause
* MERGE operation

* To be completed in unforeseeable future:
    * website
    * replication / sharding
    * INSERT / DELETE / UPDATE
