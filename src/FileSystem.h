#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "SuperBlock.h"
#include "InodeBlock.h"
#include "DirectoryBlock.h"
#include "DataBlock.h"
#include "PointerBlock.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

#define BANNED_CHARACTERS ",;~[]<>\n "
#define INODE_SIZE 128
#define DIRECTORY_ENTRY_SIZE 32

#define INODE_BLOCK 0
#define DIRECTORY_BLOCK 1
#define DATA_BLOCK 2
#define POINTER_BLOCK 3

class FileSystem
{
	public:
		FileSystem();
		~FileSystem();
		/**
		* Creates empty file system with given parameters.
		*/
		void createFileSystem(int block_size,int free_inodes,const std::string& filename);
		void createFileSystem(int block_size,int free_inodes,const char* filename);

		/**
		* Initialize file system by disk file.
		*/
		void initSystem(const std::string& filename);
		void initSystem(const char* filename);

		/**
		* System methods that do operations on the disk.
		*/
		void sys_mkdir(const char* dirname);
		void sys_mkdir(const std::string& dirname);
		void sys_write(const char* filename,const char* source_file);
		void sys_write(const std::string& filename,const char* source_file);
		void sys_read(const char* filename,const char* target_file)const;
		void sys_read(const std::string filename,const char* target_file)const;
		void sys_list(const char* path)const;
		void sys_list(const std::string& path)const;
		void sys_dumpe2fs()const;
		void sys_fsck()const;

	private:
		void updateDisk();
		void decreaseFreeInode();
		static bool isValidFilename(const char* filename);
		std::string blockData;
		std::string filename;
		SuperBlock super_block;
		std::vector<InodeBlock*> inode_blocks;
		std::vector<Block*> blocks;
		int used_disk;
};




#endif