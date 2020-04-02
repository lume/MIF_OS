CC=g++
CFLAGS=-std=c++17 -pthread

test:
	g++ -IRM/RM.Headers $(CFLAGS) RM/RM.Tests/main.cpp RM/cpu.cpp RM/memcontrol.cpp RM/IOControl.cpp RM/RM.Tests/rmTest.cpp -o rmTests -g -D DEBUG

debug:
	g++ -IRM/RM.Headers $(CFLAGS) RM/main.cpp RM/UI.cpp RM/cpu.cpp RM/memcontrol.cpp RM/IOControl.cpp RM/Clock.cpp -o rmDebug -g -D DEBUG

release:
	g++ -IRM/RM.Headers $(CFLAGS) RM/main.cpp RM/UI.cpp RM/cpu.cpp RM/memcontrol.cpp RM/IOControl.cpp RM/Clock.cpp -o rmRelease

pedantic:
	g++ -IRM/RM.Headers $(CFLAGS) RM/main.cpp RM/UI.cpp RM/cpu.cpp RM/memcontrol.cpp RM/IOControl.cpp RM/Clock.cpp -o rmPedantic -Wall -pedantic

compiler:
	g++ -ICompiler/ Compiler/main.cpp Compiler/compiler.cpp -o compiler