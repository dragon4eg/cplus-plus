//#define NDEBUG
#pragma once
#include <iostream>

//*******************************************
//Every point posesses its unique ID.
//In debug mode both constructor and destructor
//report the ID and the coordinates
//of a point created or resp. deleted
//
//Developed by .....................
//on ....................
//Version 1.0
//*******************************************
 
class Point
{
	private:
		static int _freeID;
		const int _pointID;
		double _x;
		double _y;
	public:
		explicit Point (double x=0, double y=0);
		Point (const Point &);
		~Point();
		Point& operator=(const Point &);
		double& x();
		double& y();
		const double& x()const;
		const double& y()const;
		const int getID() const;
		static int freeID();
};

std::ostream& operator<<(std::ostream&, const Point&);
const Point operator+ (const Point & u, const Point & v);
const Point& operator+=(Point &, const Point&);
const bool operator==(const Point & u, const Point &v);
const bool operator!=(const Point & u, const Point &v);
