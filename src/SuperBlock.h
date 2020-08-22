#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <iostream>
#include <sstream>
#include <cstring>


#include "Block.h"

// [block_size,free_indoes,inode_blocks_start,inode_blocks_end,directory_blocks_start,directory_blocks_end,inodes_per_block,direntry_per_block]

class SuperBlock : public Block
{
public:
	SuperBlock();
	SuperBlock(int block_size,int free_indoes,int inodes_per_block,int direntry_per_block);
	SuperBlock(const std::string& token);
	SuperBlock(const char* token);
	void setBlocksize(int block_size);
	void setFreeInodes(int free_indoes);
	void setInitInodes(int init_inode_size);
	void setInodesPerBlock(int inodes_per_block);
	void setDirentryPerBlock(int direntry_per_block);
	int getBlocksize()const;
	int getFreeInodes()const;
	int getInitInodes()const;
	int getInodesPerBlock()const;
	int getDirentryPerBlock()const;

	virtual std::string toString()const override;
	virtual void initWithToken(const std::string& token)override;
	virtual void initWithToken(const char* token)override;

private:
	int init_inode_size;
	int block_size;
	int free_indoes;
	int inodes_per_block;
	int direntry_per_block;
};


#endif