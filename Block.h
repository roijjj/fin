#pragma once

enum BlockType
{
	BT_AIR,
	BT_WALL,

};

class Block
{
private:
	int _type;
	char _repr;

public:
	Block(void);
	Block::Block(char repr, int type);

	void init(char repr, int type);

	//GETTERS
	int getType() { return _type; }
	char getRepr() { return _repr; }

	//SETTERS
	void setType(int type);
	void setRepr(char repr);
};

