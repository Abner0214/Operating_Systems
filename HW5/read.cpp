#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define true 1
#define false 0
#define FILE_SIZE 0x100

using namespace std;

int main()
{
	
	int fd = open("myfile.txt", O_RDWR, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		cout << "myfile.txt hasn't created yet.\n";
		return -1;
	}
	char* map_memory = (char*) mmap(0, FILE_SIZE, PROT_READ, MAP_SHARED, fd, 0);
	struct stat file = {0};
	if (fstat(fd,&file) == -1) {
		cout << "Get file state failed.\n";
		return -1;
	}
	if (file.st_size == 0) {
		cout << "File is nothing to read.\n";			
		return -1;
	}
	if (map_memory == MAP_FAILED) {
		cout << "Map memory failed.\n";
		close(fd);
		return -1;
	}
	cout << "Below is file myfile.txt\n";
	cout << "+++++++ START +++++++\n";
		
	for (int i = 0;  i < file.st_size;  i++)
		cout << map_memory[i];
	if (munmap(map_memory,FILE_SIZE) == -1) {
			cout << "Unmap failed.\n";
			close(fd);
			return -1;
	}
	close(fd);
	cout << "++++++++ END ++++++++\n";
	return 0;
}
