// reuben1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;


int main()
{
	int nodes = 100;
	int z = 0;
	//supppose the range of the random position is 0-100
	//suppose record the postion at each z point
	for (int i = 0; i <= nodes; i++)
	{
		int x= rand() % 100;
		int y= rand() % 100;
		int z=i;
	}
    return 0;
}

int main()
{
	//an array of vector
	//supose only record 100 nodes in an axon,so the array has 100 elements
	int nodes = 100;
	vector <int> position[100];
	for (int i = 0; i < nodes; i++)
	{
		position[i].push_back (rand() % 100);
		position[i].push_back (rand() % 100);
		position[i].push_back (i);
	}
	return 0;
}
