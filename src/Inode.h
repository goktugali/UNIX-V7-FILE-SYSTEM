#ifndef INODE_H_
#define INODE_H_

#include "Metadata.h"
#include <string.h>
#include <string.h>
#include <iostream>
#include <sstream>

#define DIRECT_PTR_SIZE 10

class Inode
{
	public:
		Inode();
		Inode(const std::string& token);
		Inode(const char* token);
		void initWithToken(const std::string& token);
		void initWithToken(const char* token);
		void setMetadata(const Metadata& mdata);
		void setDirectPointers(const int* ptrs);
		void setSingleIndirect(int single_indirect);
		void setDoubleIndirect(int double_indirect);
		void setTripleIndirect(int triple_indirect);
		const Metadata& getMetadata()const;
		int* getDirectPointers()const;
		int getNthPointer(int n)const;
		int getSingleIndirect()const;
		int getDoubleIndirect()const;
		int getTripleIndirect()const;
		std::string toString()const;	

	private:
		Metadata mdata;
		int direct_pointers[DIRECT_PTR_SIZE];
		int single_indirect;
		int double_indirect;
		int triple_indirect;
};

#endif