CXX=g++
CXXFLAGS=--std=c++11 -W -Wall -O3 -DNDEBUG

SRCS=connect4.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

connect4:$(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $<

clean:
	rm -f *.o connect4




