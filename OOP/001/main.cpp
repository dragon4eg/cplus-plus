#include "Point.h" 


int main ()
{
	std::cout << std::boolalpha << true<<std::endl;
	Point a;
	std::cout<<"cout a:"<<a<<std::endl;
	Point b(1,1);
	std::cout<<"temp sum: "<<a+b<<std::endl;
	std::cout<<"cout a:"<<a<<std::endl;
	Point c(a);
	std::cout<<"cout c:"<<c<<std::endl;

	std::cout<<"a == c : "<< (a==c) << std::endl;

	a+=b;
	std::cout<<a<<std::endl;

	std::cout<<"a == c : "<< (a==c)<<std::endl;
	std::cout<<"a == b : "<< (a==b)<<std::endl;
	a+=b+c;
	std::cout<<a<<std::endl;

	std::cout<<"a!=b : "<< (a!=b) <<std::endl;
	return 0;
}

