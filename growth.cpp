// reuben1.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <vector>
using namespace std;

int main()
{
	//an array of vector
	//supose only record 100 nodes in an axon,so the array has 100 elements
	int nodes = 120;
	vector <double> coordinates[nodes];
	for (int i = 0; i < nodes; i++)
	{
                double x= rand() % 100;
                double y= rand() % 100;
                double z=i*1.0;
		coordinates[i].push_back (x);
		coordinates[i].push_back (y);
		coordinates[i].push_back (z);
		cout <<"Coordinate of node "<<i<<":  "<<x<<", "<<y<<", "<<z<<endl; 
	}
	return 0;
}
