TARGET = matmup
LIBS = 
CC = g++
CFLAGS = -Wall -pedantic -fopenmp -std=c++11 -O3 -mavx -fopt-info-vec
;CC = icc
;CFLAGS = -mmic -openmp -Ofast -vec-report
.PHONY: default all clean

default: $(TARGET)
all: default
run: $(TARGET)
	-./$(TARGET)

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)