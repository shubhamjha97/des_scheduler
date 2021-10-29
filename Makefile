main:
	g++ -std=c++0x -o scheduler main.cpp DES.cpp\
 	Event.cpp\
 	RNG.cpp\
 	Process.cpp\
 	FcfsScheduler.cpp\
 	LcfsScheduler.cpp\
 	PrePrioScheduler.cpp\
 	PrioScheduler.cpp\
 	RrScheduler.cpp\
 	SrtfScheduler.cpp\

clean:
	rm -rf scheduler
