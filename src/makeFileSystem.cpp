/* cse312 part 2 
* 161044018
*/

#include <iostream>
#include "FileSystem.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void usageErr()
{
	std::cerr << "BAD USAGE : Run with ./makeFileSystem 4 400 disk.data" << std::endl;
	exit(EXIT_FAILURE); 
}

int main(int argc, char const *argv[])
{
	
	FileSystem fs;
	if(argv[1] == NULL || argv[2] == NULL || argv[3] == NULL)
		usageErr();

	fs.createFileSystem(atoi(argv[1]),atoi(argv[2]),argv[3]);

	return 0;
}