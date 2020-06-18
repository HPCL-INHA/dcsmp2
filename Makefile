.SUFFIXES : .cpp .o

TARGET = dcsmp2

LIBS = -ljsoncpp -lmosquitto
OBJS = dcsmp2.o user.o xyz.o dc_mqtt.o

CXX = g++
CXXFLAGS = -std=c++11 -Wall -pedantic -c
LDFLAGS = -Wall -pedantic

all : CXXFLAGS += -O3
all : executable

debug : CXXFLAGS += -g -O0 -fsanitize=undefined
debug : LDFLAGS += -fsanitize=undefined
debug : executable

test : CXXFLAGS += -DTEST
test : debug

executable : $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $<

clean :
	rm -f *.o $(TARGET)