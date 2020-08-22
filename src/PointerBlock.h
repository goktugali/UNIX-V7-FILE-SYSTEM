#ifndef POINTERBLOCK_H
#define POINTERBLOCK_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>

#include "Block.h"


class PointerBlock : public Block
{
public:
	PointerBlock();
	PointerBlock(const char* token);
	PointerBlock(const std::string& token);
	int getNthPointer(int n)const;
	int getSize()const;
	void push_pointer(int ptr);
	virtual std::string toString()const override;
	virtual void initWithToken(const std::string& token)override;
	virtual void initWithToken(const char* token)override;
private:
	std::vector<int> pointers;
	int used=0;
	
};

#endif