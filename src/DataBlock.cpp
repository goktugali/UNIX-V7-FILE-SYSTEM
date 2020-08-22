#include "DataBlock.h"


DataBlock::DataBlock(const char* token)
{
	this->initWithToken(token);
}
DataBlock::DataBlock(const std::string& token) 
{
	this->initWithToken(token);
}
void DataBlock::initWithToken(const std::string& token)
{
	this->initWithToken(token.c_str());
}
void DataBlock::initWithToken(const char* token)
{
	char* cpy_token = new char[strlen(token) + 1];
	char* bckp_cpy = cpy_token;
	strcpy(cpy_token,token);

	cpy_token = &cpy_token[1];
	cpy_token[strlen(cpy_token)-1] = '\0';

	this->contents = cpy_token;

	delete[] bckp_cpy;
}
void DataBlock::setContent(const char* contents)
{
	this->contents = contents;
}
void DataBlock::setContent(const std::string& contents)
{
	this->contents = contents;
}
const std::string& DataBlock::getContent()const
{
	return this->contents;
}
std::string DataBlock::toString()const 
{
	std::ostringstream out;
	out << "[" << this->contents << "]";

	return out.str();
}