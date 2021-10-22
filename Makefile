main:
	g++ -std=c++0x -o scheduler main.cpp Process.cpp RNG.cpp Process.cpp Scheduler.cpp helper.cpp

clean:
	rm -rf scheduler
