#ifndef INODEBLOCK_H
#define INODEBLOCK_H

#include <iostream>
#include <vector>
#include <sstream>
#include "Block.h"
#include "Inode.h"

// [[inode];[inode];[inode]]

class InodeBlock : public Block
{
public:
	InodeBlock(int inodes_per_block);
	InodeBlock(int inodes_per_block,const char* token);
	InodeBlock(int inodes_per_block,const std::string& token);
	bool isFull()const;
	const Inode& getNthInode(int n)const;
	virtual std::string toString()const override;
	virtual void initWithToken(const std::string& token)override;
	virtual void initWithToken(const char* token)override;
	bool fillNextInode(const Inode& inode);
	const std::vector<Inode>& getAllInodes()const;

private:
	InodeBlock(){};
	int inodes_per_block;
	int used;
	std::vector<Inode> inodes;
};


#endif