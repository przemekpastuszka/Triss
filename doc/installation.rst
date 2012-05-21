======================
Installation
======================

Prerequisites
================

* `google sparsehash`_ >= 2.0
* `google test`_ >= 1.6 (for unit tests)
* scons_ >= 2.1 (build tool)
* boost_

Installation
=============

Git clone the project::

    $ git clone git://github.com/rtshadow/Triss.git


Build and install (requires super user privileges)::

    $ cd Triss
    $ scons install --release

*release* flag is optional and will cause the library to be complied with *-O2*
optimization flag. We advise to use *release* for usual usage and omit it only
for debugging purposes (the project will be compiled with *-g* and *-O0* flags then)


After *install* following artifacts should be copied:

* Shared library *libtriss.so* to */usr/lib/*
* Triss header files to */usr/include/triss*

Uninstallation
===============
Simply do (as super user)::

    $ scons install --clean

For developers
===============
As a developer you may want to run unit tests provided with Triss.
Invoking ``scons`` without any arguments will compile some ready-to-run test executables in *build* directory.
Name of each one contains *_test* postfix. Run them as usual programs.


.. _google sparsehash: http://code.google.com/p/sparsehash/
.. _google test: http://code.google.com/p/googletest/
.. _scons: http://www.scons.org/
.. _boost: http://www.boost.org/
