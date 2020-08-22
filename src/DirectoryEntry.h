#ifndef DIRECTORYENTRY_H
#define DIRECTORYENTRY_H

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

// [inode,filename]
class DirectoryEntry
{
public:
	DirectoryEntry();
	DirectoryEntry(int inode, const std::string& filename);
	DirectoryEntry(const char* token);
	DirectoryEntry(const std::string& token);
	void initWithToken(const char* token);
	void initWithToken(const std::string& token);
	void setInode(int inode);
	void setFilename(const std::string& filename);
	void setFilename(const char* filename);
	std::string getFilename()const;
	int getInode()const;
	std::string toString()const;
private:
	int inode;
	char filename[15];
	
};



#endif