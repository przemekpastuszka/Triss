CC = g++ 
CFLAGS = -lgtest -lpthread
TARGETS = test
 
all: $(TARGETS)

test: BobTable.o BobTableTest.o ColumnTest.o NumericalColumnTest.o NumericalListColumnTest.o
	$(CC) tests/test.cpp BobTable.o BobTableTest.o ColumnTest.o NumericalColumnTest.o NumericalListColumnTest.o -o test $(CFLAGS)
    
BobTable.o: src/BobTable.cpp Table.o
	$(CC) -c src/BobTable.cpp Table.o -o BobTable.o $(CFLAGS)

Table.o: ../common/src/Table.cpp
	$(CC) -c ../common/src/Table.cpp -o Table.o $(CFLAGS)
    
BobTableTest.o: tests/BobTableTest.cpp
	$(CC) -c tests/BobTableTest.cpp -o BobTableTest.o $(CFLAGS)
    
ColumnTest.o: ../common/tests/columns/ColumnTest.cpp
	$(CC) -c ../common/tests/columns/ColumnTest.cpp -o ColumnTest.o $(CFLAGS)
    
NumericalColumnTest.o: ../common/tests/columns/NumericalColumnTest.cpp
	$(CC) -c ../common/tests/columns/NumericalColumnTest.cpp -o NumericalColumnTest.o $(CFLAGS)

NumericalListColumnTest.o: ../common/tests/columns/NumericalListColumnTest.cpp
	$(CC) -c ../common/tests/columns/NumericalListColumnTest.cpp -o NumericalListColumnTest.o $(CFLAGS)


clean:
	rm -f *.o

distclean:
	rm -f *.o
	rm -f $(TARGETS)