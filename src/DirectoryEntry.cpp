#include "DirectoryEntry.h"


DirectoryEntry::DirectoryEntry()
{
	this->inode = -1;
	strcpy(this->filename,"null");
}
DirectoryEntry::DirectoryEntry(int inode, const std::string& filename)
{	
	if(filename.size() > 14){
		std::cerr << "Filename size is illegal, maximum is 14" << std::endl;
		exit(EXIT_FAILURE);
	}
	this->inode = inode;
	strcpy(this->filename,filename.c_str());
}
DirectoryEntry::DirectoryEntry(const char* token)
{
	this->initWithToken(token);
}
DirectoryEntry::DirectoryEntry(const std::string& token)
{
	this->initWithToken(token);
}

void DirectoryEntry::initWithToken(const char* token)
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

	char* tokenize = strtok_r(tokenizer,",",&tokenizer);
	if(strlen(tokenize) > 14){
		delete[] bckup_tknzr;
		std::cerr << "Maximum filename is 14 byte ! Exit.." << std::endl;
		exit(EXIT_FAILURE);
	}
	this->inode = atoi(tokenize);
	tokenize = strtok_r(tokenizer,",",&tokenizer);
	strcpy(this->filename,tokenize);

	delete[] bckup_tknzr;
}

void DirectoryEntry::initWithToken(const std::string& token)
{
	this->initWithToken(token.c_str());
}
void DirectoryEntry::setInode(int inode)
{
	this->inode = inode;
}
void DirectoryEntry::setFilename(const std::string& filename)
{
	strcpy(this->filename,filename.c_str());
}
void DirectoryEntry::setFilename(const char* filename)
{
	strcpy(this->filename,filename);
}
std::string DirectoryEntry::getFilename()const
{
	return filename;
}
int DirectoryEntry::getInode()const
{
	return inode;
}
std::string DirectoryEntry::toString()const
{
	std::ostringstream out;
	out << "[" << this->inode << "," << this->filename << "]";
	return out.str();
}