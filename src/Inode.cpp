#include "Inode.h"


Inode::Inode()
{
	for (int i = 0; i < DIRECT_PTR_SIZE; ++i)
		direct_pointers[i] = -1;

	single_indirect = -1;
	double_indirect = -1;
	triple_indirect = -1;
}

Inode::Inode(const std::string& token)
{
	this->initWithToken(token);
}

Inode::Inode(const char* token)
{	
	this->initWithToken(token);
}

void Inode::initWithToken(const std::string& token)
{
	char* str = new char[200];
	char* bckupstr = str;
	strcpy(str,token.c_str());
	int len = strlen(str);

	str = &str[1];
	str[len-2] = '\0';

	char* tokenizer = new char[200];
	char* bckup_tknzr = tokenizer;
	strcpy(tokenizer,str);
	delete[] bckupstr;

	int found_ind = 0;
	for (int i = 0; tokenizer[i] != '>'; ++i)
		found_ind++;
	
	char mdata_initstr[80];
	strncpy(mdata_initstr,tokenizer,found_ind+1);
	

	this->mdata.initWithToken(mdata_initstr);
	
	tokenizer = &tokenizer[found_ind+2];
	
	char * tokenize = strtok(tokenizer,",");
	for (int i = 0; i < DIRECT_PTR_SIZE; ++i)
	{
		direct_pointers[i] = atoi(tokenize);
		tokenize = strtok(NULL,",");
	}

	single_indirect = atoi(tokenize);
	tokenize = strtok(NULL,",");
	double_indirect = atoi(tokenize);
	tokenize = strtok(NULL,",");
	triple_indirect = atoi(tokenize);

	delete[] bckup_tknzr;	
}
void Inode::initWithToken(const char* token)
{	
	char* str = new char[200];
	char* bckupstr = str;
	strcpy(str,token);
	int len = strlen(str);

	str = &str[1];
	str[len-2] = '\0';

	char* tokenizer = new char[200];
	char* bckup_tknzr = tokenizer;
	strcpy(tokenizer,str);
	delete[] bckupstr;

	int found_ind = 0;
	for (int i = 0; tokenizer[i] != '>'; ++i)
		found_ind++;
	
	char mdata_initstr[80];
	strncpy(mdata_initstr,tokenizer,found_ind+1);
	mdata_initstr[found_ind+1] = '\0';
	this->mdata.initWithToken(mdata_initstr);
	tokenizer = &tokenizer[found_ind+2];
		
	char * tokenize = strtok(tokenizer,",");
	for (int i = 0; i < DIRECT_PTR_SIZE; ++i)
	{	
		direct_pointers[i] = atoi(tokenize);
		tokenize = strtok(NULL,",");
	}

	single_indirect = atoi(tokenize);
	tokenize = strtok(NULL,",");
	double_indirect = atoi(tokenize);
	tokenize = strtok(NULL,",");
	triple_indirect = atoi(tokenize);

	delete[] bckup_tknzr;	
}

void Inode::setDirectPointers(const int* ptrs)
{
	for (int i = 0; i < DIRECT_PTR_SIZE; ++i)
	{
		direct_pointers[i] = ptrs[i];
	}
}
void Inode::setSingleIndirect(int single_indirect)
{
	this->single_indirect = single_indirect;
}
void Inode::setDoubleIndirect(int double_indirect)
{
	this->double_indirect = double_indirect;
}
void Inode::setTripleIndirect(int triple_indirect)
{
	this->triple_indirect = triple_indirect;
}

void Inode::setMetadata(const Metadata& mdata)
{
	this->mdata = mdata;
}
int* Inode::getDirectPointers()const
{
	int* direc_ptr_cpy = new int[DIRECT_PTR_SIZE];
	for (int i = 0; i < DIRECT_PTR_SIZE; ++i)
		direc_ptr_cpy[i] = direct_pointers[i];

	return direc_ptr_cpy;
}
int Inode::getSingleIndirect()const
{
	return single_indirect;
}
int Inode::getDoubleIndirect()const
{
	return double_indirect;
}
int Inode::getTripleIndirect()const
{
	return triple_indirect;
}
int Inode::getNthPointer(int n)const
{
	return direct_pointers[n];
}
const Metadata& Inode::getMetadata()const
{
	return this->mdata;
}
std::string Inode::toString()const
{
	std::ostringstream out;
	out << "[";
	out << (this->mdata).toString();
	out << ",";

	for (int i = 0; i < DIRECT_PTR_SIZE; ++i)
		out << direct_pointers[i] << ",";

	out << single_indirect;
	out << ",";
	out << double_indirect;
	out << ",";
	out << triple_indirect;
	out << "]";

	return out.str();
}