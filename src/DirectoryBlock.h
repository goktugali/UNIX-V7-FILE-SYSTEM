#ifndef DIRECTORYBLOCK_H
#define DIRECTORYBLOCK_H

#include <iostream>
#include <vector>
#include <sstream>

#include "Block.h"
#include "DirectoryEntry.h"

class DirectoryBlock : public Block
{
public:
	DirectoryBlock(int direntry_per_block);
	DirectoryBlock(int direntry_per_block,const char* token);
	DirectoryBlock(int direntry_per_block,const std::string& token);
	bool isFull()const;
	virtual std::string toString()const override;
	virtual void initWithToken(const std::string& token)override;
	virtual void initWithToken(const char* token)override;
	bool fillNextDirectory(const DirectoryEntry& direntry);
	bool containsDirectory(const char* filename)const;
	bool containsDirectory(const std::string& filename)const;
	int getInodeByFilename(const char* filename)const;
	int getInodeByFilename(const std::string& filename)const;
	const std::vector<DirectoryEntry>& getContents()const;
	static int getTotalDirectories();
private:
	static int total_directories;
	DirectoryBlock(){}
	int direntry_per_block;
	int used;
	std::vector<DirectoryEntry> directory_entries;
	
};






#endif