#include "Point.h"
int Point::freeID = 0;

Point::Point (double x, double y): _x(x), _y(y), pointID(++freeID)
{
	
#ifdef NDEBUG
	cout<<pointID<<": created "<<*this<<endl;
#endif
	return;
}

Point::Point (const Point & u): pointID(++freeID)
{
	this->_x = u._x;
	this->_y = u._y;
	return;
}

Point::~Point ()
{

#ifdef NDEBUG
	cout<<pointID<<": deleted "<<*this<<endl;
#endif
	return;
}

Point& Point::operator=(const Point & u)
{
	this->_x = u._x;
	this->_y = u._y;
	return *this;
}

double& Point::x() {return _x;}

double& Point::y() {return _y;}

const double& Point::x()const {return _x;}

const double& Point::y()const {return _y;}

const int Point::getID() const {return pointID;}

int Point::amount() {return freeID;}
/******************************************************************/
ostream& operator<<(ostream & os, const Point & u)
{
	os<<"Point: "<<"("<<u.x()<<", "<<u.y()<<")"<<endl;
	return os;

}
const Point operator+ (const Point & u, const Point & v)
{

	return Point (u.x()+v.x(), u.y()+v.y());
}

Point& operator+=(Point & u, const Point & v)
{
	u.x() += v.x();
	u.y() += v.y();
	return u;
}

bool operator==(const Point & u, const Point &v)
{
	return ( (u.x()==v.x()) && (u.y()==v.y()) );
}

bool operator!=(const Point & u, const Point &v)
{
	return !(u == v);
}

