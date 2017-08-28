// reuben1.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <vector>
using namespace std;

int main()
{
	//an array of vector
	//supose only record 100 nodes in an axon,so the array has 100 elements
	int no_cells=1;
	int no_steps = 100;
	for (int k = 0; k < no_cells; k++)
	{
	  vector <double> coordinates[no_steps];
	  double z=0.0;
	  for (int i = 0; i < no_steps; i++)
	{
                double x= rand() % 100;
                double y= rand() % 100;
                double z=z+rand() % 10;
		coordinates[i].push_back (x);
		coordinates[i].push_back (y);
		coordinates[i].push_back (z);
		cout <<"Coordinate of node "<<i<<":  "<<x<<", "<<y<<", "<<z<<endl; 
	}}
	return 0;
}
