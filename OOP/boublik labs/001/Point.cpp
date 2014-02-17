#include "Point.h"
int Point::_freeID = 0;

Point::Point (double x, double y): _x(x), _y(y), _pointID(++_freeID)
{
#ifdef NDEBUG
	cout<<_pointID<<": created "<<*this<<std::endl;
#endif
	return;
}

Point::Point (const Point & u): _pointID(++_freeID), _x(u.x()), _y(u.y())
{
	return;
}

Point::~Point ()
{
#ifdef NDEBUG
	cout<<_pointID<<": deleted "<<*this<<std::endl;
#endif
	return;
}

Point& Point::operator=(const Point & u)
{
	x() = u.x();
	y() = u.y();
	return *this;
}

double& Point::x() {return _x;}

double& Point::y() {return _y;}

const double& Point::x()const {return _x;}

const double& Point::y()const {return _y;}

const int Point::getID() const {return _pointID;}

int Point::freeID() {return _freeID;}
/******************************************************************/
std::ostream& operator<<(std::ostream & os, const Point & u)
{
	os<<"Point: "<<"("<<u.x()<<", "<<u.y()<<")"<<std::endl;
	return os;
}

const Point operator+ (const Point & u, const Point & v)
{
	return Point (u.x()+v.x(), u.y()+v.y());
}

Point& operator+=(Point & u, const Point & v)
{
	u = u + v;
	return u;
}

const bool operator==(const Point & u, const Point &v)
{
	return (u.x()==v.x())&&(u.y()==v.y());
}

const bool operator!=(const Point & u, const Point &v)
{
	return !(u == v);
}

