XX = g++
CXXFLAGS = -Wall

driver: joke.o driver.o
	$(CXX) $(CXXFLAGS) joke.o driver.o -o driver

joke.o: joke.cpp joke.h
	$(CXX) $(CXXFLAGS) -c joke.cpp

driver.o: driver.cpp
	$(CXX) $(CXXFLAGS) -c driver.cpp

clean:
	rm *.o
	rm *~
run:
	./driver
