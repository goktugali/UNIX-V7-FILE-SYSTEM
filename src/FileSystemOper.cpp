/**
* cse312 part 3
* 161044018
*/

#include <iostream>
#include "FileSystem.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;


int main(int argc, char const *argv[])
{
	
	const char* diskfile = argv[1];

	FileSystem fs;
	fs.initSystem(diskfile);

	if(strcmp(argv[2],"list") == 0){
		if(argv[3]== NULL || strlen(argv[3]) <= 0){
			fprintf(stderr, "Invalid argument for list operation.exit..\n");
			exit(EXIT_FAILURE);
		}
		fs.sys_list(argv[3]);
	}
	else if(strcmp(argv[2],"mkdir") == 0){
		if(argv[3] == NULL || strlen(argv[3]) <= 0){
			fprintf(stderr, "Invalid argument for mkdir operation.exit..\n");
			exit(EXIT_FAILURE);
		}
		fs.sys_mkdir(argv[3]);
	}
	else if(strcmp(argv[2],"write") == 0){
		if(argv[3] == NULL || strlen(argv[3]) <= 0){
			fprintf(stderr, "Invalid argument for write operation.exit..\n");
			exit(EXIT_FAILURE);
		}
		if(argv[4] == NULL || strlen(argv[4]) <= 0){
			fprintf(stderr, "Invalid argument for write operation.exit..\n");
			exit(EXIT_FAILURE);
		}
		fs.sys_write(argv[3],argv[4]);
	}
	else if(strcmp(argv[2],"read") == 0){
		if(argv[3] == NULL || strlen(argv[3]) <= 0){
			fprintf(stderr, "Invalid argument for read operation.exit..\n");
			exit(EXIT_FAILURE);
		}
		if(argv[4] == NULL || strlen(argv[4]) <= 0){
			fprintf(stderr, "Invalid argument for read operation.exit..\n");
			exit(EXIT_FAILURE);
		}
		fs.sys_read(argv[3],argv[4]);
	}
	else if(strcmp(argv[2],"dumpe2fs") == 0){
		fs.sys_dumpe2fs();
	}
	else if(strcmp(argv[2],"fsck") == 0){
		fs.sys_fsck();
	}
	else{
		fprintf(stderr, "> %s is not a valid operation.exit..\n", argv[2]);
		fprintf(stderr, "(del,rmdir,ln,lnsym operations are not supported.)\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
