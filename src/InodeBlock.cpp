#include "InodeBlock.h"


InodeBlock::InodeBlock(int inodes_per_block)
{	
	this->inodes_per_block = inodes_per_block;
	used = 0;
}

InodeBlock::InodeBlock(int inodes_per_block,const char* token)
{	
	this->used = 0;
	this->inodes_per_block = inodes_per_block;
	this->initWithToken(token);
}

InodeBlock::InodeBlock(int inodes_per_block,const std::string& token)
{
	this->used = 0;
	this->inodes_per_block = inodes_per_block;
	this->initWithToken(token);
}

bool InodeBlock::fillNextInode(const Inode& inode)
{
	if(isFull())
		return false;

	inodes.push_back(inode);
	used++;
	return true;
}

void InodeBlock::initWithToken(const std::string& token)
{
	this->initWithToken(token.c_str());
}

void InodeBlock::initWithToken(const char* token)
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

	char* tokenize = strtok_r(tokenizer,";",&tokenizer);

	while(tokenize !=NULL )
	{
		Inode tmp(tokenize);
		inodes.push_back(tmp);
		tokenize = strtok_r(tokenizer,";",&tokenizer);
		++used;
	}

	delete[] bckup_tknzr;
}


bool InodeBlock::isFull()const
{	
	return used == inodes_per_block;
}
const Inode& InodeBlock::getNthInode(int n)const
{
	return this->inodes[n];
}

std::string InodeBlock::toString()const
{
	std::ostringstream out;

	out << "[";
	for (int i = 0; i < used; ++i)
	{
		out << inodes[i].toString();
		if(i+1 != used)
			out << ";";
	}
	out << "]";

	return out.str();
}

const std::vector<Inode>& InodeBlock::getAllInodes()const
{
	return this->inodes;
}