#include "Block.h"


Block::Block()
{
	setBlocknum(-1);
}
void  Block::setBlocknum(int blockval)
{
	this->block_num = blockval;
}
int Block::getBlocknum()const
{
	return block_num;
}