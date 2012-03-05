CC = g++
CFLAGS = -lpthread
TARGETS = benchmark

all: $(TARGETS)

benchmark: src/main.cpp BobTable.o Table.o
	$(CC) src/main.cpp BobTable.o Table.o -o benchmark $(CFLAGS)

BobTable.o: ../bob/src/BobTable.cpp Table.o
	$(CC) -c ../bob/src/BobTable.cpp Table.o -o BobTable.o $(CFLAGS)

Table.o: ../common/src/Table.cpp
	$(CC) -c ../common/src/Table.cpp -o Table.o $(CFLAGS)

clean:
	rm -f *.o

distclean:
	rm -f *.o
	rm -f $(TARGETS)
