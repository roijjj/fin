#include "Block.h"


Block::Block(void)
{
	_type = BT_AIR;
	_repr = ' ';
}

Block::Block(char repr, int type)
{
	_type = type;
	_repr = repr;
}

void Block::init(char repr, int type)
{
	_type = type;
	_repr = repr;
}

void Block::setType(int type)
{
	_type = type;
}

void Block::setRepr(char repr)
{
	_repr = repr;
}