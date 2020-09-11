#g++ -std=c++1y graph.cpp grasp.cpp heuristics.cpp vnd.cpp vns.cpp main.cpp -o expKMIS -DIL_STD -m64 -lm -pthread -O3 -DLONG_MAX
CC		= g++ -std=c++1y -DIL_STD
FILES_CC	= graph.cpp grasp.cpp heuristics.cpp vnd.cpp vns.cpp main.cpp

KMIS_E_CARD	= 1
BINARY		= expKMIS-$(KMIS_E_CARD)

all: clean compile

clean:
	find . -name '*.o' -exec rm -f '{}' ';'
	rm -f $(BINARY);

compile:
	$(CC) -DKMIS_E_CARD=$(KMIS_E_CARD) $(FILES_CC) -o $(BINARY)

run:
	./$(BINARY)

