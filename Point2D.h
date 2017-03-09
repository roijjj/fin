#pragma once
class Point2D
{
private:
	int _x, _y;
public:
	Point2D();
	Point2D(int x, int y);

	void set(int x, int y);

	//GETTERS
	int getX() { return _x; }
	int getY() { return _y; }

	//SETTERS
	void setX(int x) { _x = x; }
	void setY(int y) { _x = y; }
};

