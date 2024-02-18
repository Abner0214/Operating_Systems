#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <sys/mman.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <signal.h>
#include <fcntl.h> 
#include <cstring>

#define FILE_SIZE 0x100 
#define true 1
#define false 0;

using namespace std;

int main(int argc,char* const argv[])
{
	int fd = open("myfile.txt",O_RDWR | O_CREAT, S_IRUSR | S_IWUSR );
	if (fd < 0) {  // open fd failed
		cout << "Open file failed!\n";
		return -1;
	}
	char* map_memory = (char*)mmap(0, FILE_SIZE , PROT_WRITE , MAP_SHARED , fd , 0 );  // position to map is 0, size you want to map
	// the place you map can be written, the properties of the position, the file you map, from the first start to map
	if (map_memory == MAP_FAILED) {  // if success it will return the address of the position
		cout << "Map memory failed!\n";
		close(fd);
		return -1;
	}
	while (true) {
		struct stat file = {0};
		if (fstat(fd, &file) == -1) {  // fd's state write to the file(struct stat), get the information from fd
			cout << "Get file state failed.\n";
			return -1;
		}
		int size = (int) file.st_size;
		char set_string[FILE_SIZE];
		cout << "Please type a string : ";
		fgets(set_string, sizeof(set_string), stdin);
		cout << "File updated.\n";
		int len = strlen(set_string);
		lseek(fd, len+size, SEEK_SET);  // make the position to the offset
		write(fd, "", 1);
		lseek(fd, 0, SEEK_SET);  // make position to the begining
		sprintf(map_memory + size , "%s" , set_string);  // correct the file
	}
	if (munmap(map_memory,FILE_SIZE) == -1) {
		cout << "Unmap failed.\n";
		close(fd);
		return -1;
	}
	close(fd);
	cout << "[exit]\n";
	return 0;  
}
