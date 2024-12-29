STD=--std=c++17 -pthread
GCC=g++
OBJ=obj
BIN=bin

bin/search: src/main.cpp obj/search.o
	[ -d $(BIN) ] || mkdir -p $(BIN)
	${GCC} ${STD} -o bin/search src/main.cpp obj/search.o

obj/search.o: src/search.cpp src/search.h
	[ -d $(OBJ) ] || mkdir -p $(OBJ)
	${GCC} ${STD} -c -o obj/search.o src/search.cpp

.PHONY: doc run clean

run: bin/search
	./bin/search thread test_folder
doc:
	doxygen config
	cd latex && make

clean:
	rm -f obj/*.o
	rm -f bin/search
	rm -r -f bin
	rm -r -f obj
	rm -r -f html
	rm -r -f latex
