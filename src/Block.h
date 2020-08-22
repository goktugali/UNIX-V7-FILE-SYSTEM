#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <string>
#include <sstream>

class Block
{
	public:
		Block();
		virtual ~Block()=default;
		void setBlocknum(int blockval);
		int getBlocknum()const;
		virtual std::string toString()const = 0;
		virtual void initWithToken(const std::string& token)=0;
		virtual void initWithToken(const char* token)=0;
	private:
		int block_num;

};



#endif