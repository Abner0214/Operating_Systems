SRCS = philosopher.c
EXT  = .c
OBJS = $(SRCS:$(EXT)=.o)
EXEC = philosopher
CXX  = gcc
CXXFLAGS = -std=c99 -Wall -g -pthread
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
