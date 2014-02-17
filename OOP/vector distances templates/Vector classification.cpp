// Vector classification.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
using namespace std;


static int global_error = 999999;

inline bool checkSizeOk (vector<int> a, vector<int> b)
{
	if (a.size() == b.size())
	return true;
	else
	{
		cout<<"different vector sizes, cannot compare!"<<'\n';
		return false;
	}
}

const unsigned int manhattanDistance (vector<int> a, vector<int> b)
{
	if (checkSizeOk(a,b) == false)
	{
		return global_error;
	}
	unsigned int dist = 0;
	for (unsigned int i = 0; i< a.size(); i++)
	{
		dist+=abs(a.at(i) - b.at(i));
	}
	return dist;
}

const double euclideanDistance (vector<int> a, vector<int> b)
{
	if (checkSizeOk(a,b) == false)
	{
		return global_error;
	}
	double dist = 0;
	for (unsigned int i = 0; i< a.size(); i++)
	{
		dist+=abs(a.at(i) - b.at(i)) * abs(a.at(i) - b.at(i));
	}
	return sqrt(dist);
}

const unsigned int hemingDistance(vector<int> a, vector<int> b) //editing distance here we can get a number of unequal elements
{
	if (checkSizeOk(a,b) == false)
	{
		return global_error;
	}
	unsigned int dist = 0;
	for (unsigned int i = 0; i< a.size(); i++)
	{
		if (a.at(i) != b.at(i))
		{
			dist++;
		}
	}
	return dist;
}
int main()
{
	const int arr[] = {1, 2, 3};
	const int arr2[] = {1, 4, 6};
	vector<int> a(arr, arr+sizeof(arr) / sizeof(arr[0]));
	vector<int> b(arr2, arr2+sizeof(arr2) / sizeof(arr2[0])) ;
	cout<<"vector a: ";
	copy(a.begin(), a.end(), ostream_iterator<int>(cout, " "));
	cout<<'\n';
	cout<<"vector b: ";
	copy(b.begin(), b.end(), ostream_iterator<int>(cout, " "));
	cout<<'\n';

	//********************************************************************//

	cout<<"manhattan distance of vectors = "<<manhattanDistance(a,b)<<'\n';
	
	cout<<"euclidean distance of vectors = "<<euclideanDistance(a,b)<<'\n';

	cout<<"heming distance of vectors = "<<hemingDistance(a,b)<<'\n';

	return 0;
}

