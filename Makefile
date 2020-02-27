CXX = g++
DEBUG = -g -O2
CC_FLAGS = $(DEBUG) 

CC_FLAGS += -I./
CC_FLAGS += -std=c++11


LIBS = -lpthread


OBJS = main.o global.o util.o monitor.o


VERSION = 2.0.0
EXEC = server

$(EXEC): $(OBJS)
	$(CXX) $(CC_FLAGS) $(OBJS) -o $@  $(LIB_PATH) $(LIBS)


main.o: main.cpp
	$(CXX) $(CC_FLAGS) -c -o $@ main.cpp

global.o: global.cpp global.h
	$(CXX) $(CC_FLAGS) -c -o $@ global.cpp 

util.o: util.cpp util.h
	$(CXX) $(CC_FLAGS) -c -o $@ util.cpp 

monitor.o: monitor.cpp monitor.h
	$(CXX) $(CC_FLAGS) -c -o $@ monitor.cpp 

all: $(EXEC) $(TEST)
	
.PHONY:clean
clean:
	rm -rf $(OBJS) $(EXEC) 


