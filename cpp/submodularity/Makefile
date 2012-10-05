compile_command=g++ -std=c++0x

all: Submodularity
	$(compile_command) main.cpp Submodularity.o -o submodularity.out
Submodularity:
	$(compile_command)  -c Submodularity.cpp
clean: 
	rm *.o submodularity.out
