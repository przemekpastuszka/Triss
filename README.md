# TRISS
Active development year: 2012

## Summary
In-memory search system for persistent data with (mostly) conjunctive queries

## Motivation
We want to develop open source search system for RTB (real-time bidding) purposes.
Here's a sample of requirements for / characteristics of RTB system (note, that this is quite different than features, that traditional DBMSes offer):
* rapid response time (in tens of milliseconds max)
* tables usually have many columns (50 and more)
* most queries are conjunctive, i.e. they are in form of 't1 AND t2 AND ... AND tn', where t1 to tn are positive or negated terms
* queries contains constraints for many columns (usually for most of them)
* whole data set is few GB at most (it fits into memory easily)
* data set is updated rarely (which means we can afford to drop and recreate whole table instead of directly implementing inserts / updates / deletes)
* in most cases we are interested only in few first results (LIMIT is used extensively)

## Features

### Datatypes
* numerical
* string
* list (of numerical or strings)

### Queries
All queries are in form of: 't1 AND t2 AND ... AND tn LIMIT k', where ti is one of:
* equals
* not equals
* contains
* not contains 
* less
* greater
* less or equal
* greater or equal

## Build and installation

### Build
Build project with ```scons make [--release]```. ```--release``` flag is optional and will compile library with -O2 optimization flag (without this flag project is built with -g and -O0 flags).
This will generate ```libtriss.so``` shared library in ```build``` directory.

### Installation
Install project with ```scons install [--release]``` (requires super user privileges). Following files will be copied:
* shared library ```libtriss.so``` to ```/usr/lib/```
* Triss header files to ```/usr/include/triss```

### Uninstallation
Just do ```scons install --clean``` as super user

## Usage
Please, refer to: http://cluster014.ovh.net/~rtshadow/trissdoc/api_reference.html

## Performance

### Running benchmark
Build the benchmark with ```scons benchmark --release```, the executable will be placed under ```build/```. You can check available benchmark options by doing ```./build/benchmark --help```

## Maturity
This project was never used commercially (though it was evaluated for production by at least one company) and was not tested beyond the benchmarking and tests under ```triss/tests```. We do not consider it stable by any means and we do not plan to develop it further in the near future (so fork and play with it!).

## Development

### Running unit tests
You need to build the tests first with ```scons target_test```, where target is one of:
* engine
* common
* utils
* client

The executables will be placed under ```build/``` directory and can be run as an usual programs.

### Editing source
We've used mostly vim and eclipse (but we provide no eclipse project files here).

## About project
Project was done as the Bachelor's degree project at Wrocław University, Poland.

### Name
TRISS stands for "The Revolution In Search Systems". Also, Triss is the name of brown-haired sorceress from Sapkowski's novels

### Authors
* Olchawa Piotr (http://github.com/2easy)
* Pastuszka Przemysław (http://github.com/rtshadow)
* Rychlik Michał 
