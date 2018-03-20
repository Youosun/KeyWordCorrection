INC_DIR:= ./include	
SRC_DIR:= ./src
SRCS:=$(wildcard ./src/*.cc)
OBJS:= $(patsubst ./src/%.cc, ./src/%.o, $(wildcard ./src/*.cc))
LIBS:= -lpthread -llog4cpp

CSRC:=./src/client.c

CXX:=g++

CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) -Wno-deprecated

EXE:=./bin/server.out
CEXE:=./bin/client.out

$(EXE):$(SRCS) $(CSRCS)
	$(CXX) -o $(EXE) $(SRCS) $(CXXFLAGS)
	$(CXX) -o $(CEXE) $(CSRC)

clean:
	rm -rf $(EXE)
	rm -rf $(CEXE)
	rm -rf $(OBJS)
