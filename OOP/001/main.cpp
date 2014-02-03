#include "Point.h" 


int main ()
{
	std::cout << std::boolalpha << true<<endl;
	Point a;
	cout<<"cout a:"<<a<<endl;
	Point b(1,1);
	cout<<"temp sum: "<<a+b<<endl;
	cout<<"cout a:"<<a<<endl;
	Point c(a);
	cout<<"cout c:"<<c<<endl;

	cout<<"a == c : "<< (a==c) << endl;

	a+=b;
	cout<<a<<endl;

	cout<<"a == c : "<< (a==c)<<endl;
	cout<<"a == b : "<< (a==b)<<endl;
	a+=b+c;
	cout<<a<<endl;

	cout<<"a!=b : "<< (a!=b) <<endl;
	return 0;
}

