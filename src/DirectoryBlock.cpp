#include "DirectoryBlock.h"

int DirectoryBlock::total_directories = 0;

DirectoryBlock::DirectoryBlock(int direntry_per_block)
{
	this->direntry_per_block = direntry_per_block;
	used = 0;
	++total_directories;
}
DirectoryBlock::DirectoryBlock(int direntry_per_block,const char* token) : DirectoryBlock(direntry_per_block)
{
	this->direntry_per_block = direntry_per_block;
	this->initWithToken(token);
}

DirectoryBlock::DirectoryBlock(int direntry_per_block,const std::string& token) : DirectoryBlock(direntry_per_block)
{
	this->direntry_per_block = direntry_per_block;
	this->initWithToken(token);
}

void DirectoryBlock::initWithToken(const std::string& token)
{	
	this->initWithToken(token.c_str());
}
void DirectoryBlock::initWithToken(const char* token)
{	

	if(token == NULL || strlen(token) <= 0)
		return;

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

	while(tokenize != NULL)
	{	
		DirectoryEntry tmp(tokenize);
		directory_entries.push_back(tmp);
		tokenize = strtok_r(tokenizer,";",&tokenizer);
		++used;
	}

	delete[] bckup_tknzr;
}
bool DirectoryBlock::fillNextDirectory(const DirectoryEntry& direntry)
{
	if(isFull())
		return false;

	directory_entries.push_back(direntry);
	used++;
	return true;
}

bool DirectoryBlock::isFull()const
{
	return used == direntry_per_block;
}

int DirectoryBlock::getInodeByFilename(const char* filename)const
{
	for (unsigned int i = 0; i < directory_entries.size(); ++i)
		if(strcmp(directory_entries[i].getFilename().c_str(),filename) == 0)
			return directory_entries[i].getInode();
	return -1;
}
int DirectoryBlock::getInodeByFilename(const std::string& filename)const
{
	return this->getInodeByFilename(filename.c_str());
}

bool DirectoryBlock::containsDirectory(const char* filename)const
{
	for (unsigned int i = 0; i < directory_entries.size(); ++i)
		if(strcmp(directory_entries[i].getFilename().c_str(),filename) == 0)
			return true;
	return false;
}
bool DirectoryBlock::containsDirectory(const std::string& filename)const
{
	return this->containsDirectory(filename.c_str());
}

const std::vector<DirectoryEntry>& DirectoryBlock::getContents()const
{
	return this->directory_entries;
}
std::string DirectoryBlock::toString()const 
{
	std::ostringstream out;
	out << "[";
	for (int i = 0; i < used; ++i)
	{
		out << directory_entries[i].toString();
		if(i+1 != used)
			out << ";";
	}
	out << "]";
	return out.str();
}
int DirectoryBlock::getTotalDirectories()
{
	return DirectoryBlock::total_directories;
}