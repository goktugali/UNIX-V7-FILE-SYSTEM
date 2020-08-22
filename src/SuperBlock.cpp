#include "SuperBlock.h"

SuperBlock::SuperBlock()
{
	this->block_size = 0;
	this->free_indoes = 0;
	this->init_inode_size = 0;
	this->inodes_per_block = 0;
	this->direntry_per_block = 0;
}

SuperBlock::SuperBlock(int block_size,int free_indoes,int inodes_per_block,int direntry_per_block)
{	
	this->block_size = block_size;
	this->free_indoes = free_indoes;
	this->init_inode_size = free_indoes;
	this->inodes_per_block = inodes_per_block;
	this->direntry_per_block = direntry_per_block;
}

SuperBlock::SuperBlock(const std::string& token)
{
	this->initWithToken(token);
}

SuperBlock::SuperBlock(const char* token)
{
	this->initWithToken(token);
}

void SuperBlock::initWithToken(const std::string& token)
{
	this->initWithToken(token.c_str());
}

void SuperBlock::initWithToken(const char* token)
{
	char* str = new char[strlen(token) + 1];
	char* bckupstr = str;
	strcpy(str,token);
	int len = strlen(str);

	str = &str[1];
	str[len-2] = '\0';

	char* tokenizer = new char[strlen(token) + 1];
	char* bckup_tknzr = tokenizer;
	strcpy(tokenizer,str);
	delete[] bckupstr;

	char * tokenize = strtok_r(tokenizer,",",&tokenizer);
	this->block_size = atoi(tokenize);
	tokenize = strtok_r(tokenizer,",",&tokenizer);
	this->init_inode_size = atoi(tokenize);
	tokenize = strtok_r(tokenizer,",",&tokenizer);
	this->free_indoes = atoi(tokenize);
	tokenize = strtok_r(tokenizer,",",&tokenizer);
	this->inodes_per_block = atoi(tokenize);
	tokenize = strtok_r(tokenizer,",",&tokenizer);
	this->direntry_per_block = atoi(tokenize);

	delete[] bckup_tknzr;
}
void SuperBlock::setBlocksize(int block_size)
{
	this->block_size = block_size;
}
void SuperBlock::setFreeInodes(int free_indoes)
{
	this->free_indoes = free_indoes;
}
void SuperBlock::setInitInodes(int init_inode_size)
{
	this->init_inode_size = init_inode_size;
}
void SuperBlock::setInodesPerBlock(int inodes_per_block)
{
	this->inodes_per_block = inodes_per_block;
}
void SuperBlock::setDirentryPerBlock(int direntry_per_block)
{
	this->direntry_per_block= direntry_per_block;
}
int SuperBlock::getBlocksize()const
{
	return block_size;
}
int SuperBlock::getFreeInodes()const
{
	return free_indoes;
}
int SuperBlock::getInitInodes()const
{
	return init_inode_size;
}
int SuperBlock::getInodesPerBlock()const
{
	return inodes_per_block;
}
int SuperBlock::getDirentryPerBlock()const
{
	return direntry_per_block;
}

std::string SuperBlock::toString()const
{
	std::ostringstream out;
	out << "[" << block_size << "," << init_inode_size << "," << free_indoes << "," << inodes_per_block
		<< "," << direntry_per_block << "]\n";

	return out.str();
}
