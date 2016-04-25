CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=-pthread -lstdc++
SOURCES=source/sim_main.cpp source/processip.cpp source/singlebatt.cpp source/setbatt.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=battbalancesim
all: clean build

build: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	$(CC) $(CFLAGS) $< -o $@ $(LINKFLAGS)

clean:
	rm -fr ./*/*.o $(EXECUTABLE)
