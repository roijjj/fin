#include "Point2D.h"


Point2D::Point2D(void)
{
}

Point2D::Point2D(int x, int y)
{
	_x = x;
	_y = y;
}

void Point2D::set(int x, int y)
{
	_x = x;
	_y = y;
}