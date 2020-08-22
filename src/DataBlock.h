#ifndef DATABLOCK_H
#define DATABLOCK_H

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#include "Block.h"

class DataBlock : public Block
{
public:
	DataBlock(){};
	DataBlock(const char* token);
	DataBlock(const std::string& token);
	void setContent(const char* contents);
	void setContent(const std::string& contents);
	const std::string& getContent()const;
	virtual std::string toString()const override;
	virtual void initWithToken(const std::string& token)override;
	virtual void initWithToken(const char* token)override;

private:
	std::string contents;
};





#endif