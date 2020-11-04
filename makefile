SOURCE_CPP = src/graph.cpp src/oracle.cpp src/PRNG.cpp

all: src/*.cpp src/*.h
	mkdir -p bin/
	gcc-9 -o bin/main src/main.cpp $(SOURCE_CPP) -lstdc++ -lm -lcrypto -O3 -Wall -funroll-loops -std=c++2a

test: src/*.cpp src/*.h
	mkdir -p bin/
	gcc-9 -o bin/test src/project3_tests.cpp $(SOURCE_CPP) -lstdc++ -lm -lcrypto -O3 -Wall -funroll-loops -std=c++2a


clean:
	rm -rfv bin/
