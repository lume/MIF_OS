CC=g++
INC=-I/RM -I/RM.Tests
CFLAGS=-std=c++17 -pthread
DEPS = Clock.h cpu.h memcontrol.h IOControl.h rmTest.h

test:
	g++ -IRM/RM.Headers $(CFLAGS) RM/RM.Tests/main.cpp RM/cpu.cpp RM/memcontrol.cpp RM/IOControl.cpp RM/Clock.cpp RM/RM.Tests/rmTest.cpp -o rmTests -g -D DEBUG

debug:
	g++ -IRM/RM.Headers $(CFLAGS) RM/main.cpp RM/cpu.cpp RM/memcontrol.cpp RM/IOControl.cpp RM/Clock.cpp -o rmDebug -g -D DEBUG

release:
	g++ -IRM/RM.Headers $(CFLAGS) RM/main.cpp RM/cpu.cpp RM/memcontrol.cpp RM/IOControl.cpp RM/Clock.cpp -o rmRelease

pedantic:
	g++ -IRM/RM.Headers $(CFLAGS) RM/main.cpp RM/cpu.cpp RM/memcontrol.cpp RM/IOControl.cpp RM/Clock.cpp -o rmPedantic -Wall -pedantic

