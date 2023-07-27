.PHONY: main overhead clean 

CXX = g++
CXXFLAGS = -std=c++20 -O3 -g
LDFLAGS = -pthread

DEPS = utility.hpp jacobi.hpp
MAIN_OBJ = main.o jacobi.o utility.o utimer.o
OVERHEAD_OBJ = overhead.o utimer.o

all: main overhead

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c -o $@ $<

main: $(MAIN_OBJ)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^

overhead: $(OVERHEAD_OBJ)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	@rm *.o