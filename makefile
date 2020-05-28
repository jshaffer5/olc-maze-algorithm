src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

CXX = g++ 
CXXFLAGS = -std=c++1y 
LDFLAGS = -lX11 -lGL -lpthread -lpng

myprog: $(obj)
	$(CXX) -o $@ $^ $(LDFLAGS)
	./myprog

.PHONY: clean
clean:
	rm -f *.o myprog

