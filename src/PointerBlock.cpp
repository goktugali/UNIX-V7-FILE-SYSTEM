#include "PointerBlock.h"

PointerBlock::PointerBlock(){}

PointerBlock::PointerBlock(const char* token)
{
	this->initWithToken(token);
}
PointerBlock::PointerBlock(const std::string& token)
{
	this->initWithToken(token);
}
int PointerBlock::getSize()const
{
	return this->used;
}
int PointerBlock::getNthPointer(int n)const
{
	return this->pointers[n];
}
void PointerBlock::push_pointer(int ptr)
{
	this->pointers.push_back(ptr);
	this->used++;
}
std::string PointerBlock::toString()const
{
	std::ostringstream out;
	out << "[";

	for (unsigned int i = 0; i < this->pointers.size(); ++i)
	{
		out << pointers[i];
		if(i!=pointers.size()-1)
			out << ",";
	}
	out << "]";

	return out.str();
}

void PointerBlock::initWithToken(const char* token)
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
	while(tokenize != NULL)
	{
		this->push_pointer(atoi(tokenize));
		tokenize = strtok_r(tokenizer,",",&tokenizer);
	}

	delete[] bckup_tknzr;
}

void PointerBlock::initWithToken(const std::string& token)
{
	this->initWithToken(token.c_str());
}
