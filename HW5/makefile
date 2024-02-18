all:	write read dep

write:	write.cpp
	g++ -Wall -c write.cpp
	g++ -Wall -o write write.o

read:	read.cpp
	g++ -Wall -c read.cpp
	g++ -Wall -o read read.o

dep:
	gcc -M *.cpp > dep

clean:
	rm write read dep 
