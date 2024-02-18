SRCS = philosopher_monitor.cpp
EXT  = .cpp
OBJS = $(SRCS:$(EXT)=.o)
EXEC = philosopher_monitor
CXX  = g++
#CXXFLAGS = -std=c99 -Wall -g -D _DEBUG -pthread
CXXFLAGS = -Wall -g -pthread
DEP  = dep

all:$(EXEC) clean

$(EXEC):$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS)

%.o:%$(EXT)
	$(CXX) $(CXXFLAGS) -c $<

dep:
	$(CXX) $(CXXFLAGS) -M $(SRCS) > $(DEP)

clean:
	rm -rf $(OBJS) $(DEP)
