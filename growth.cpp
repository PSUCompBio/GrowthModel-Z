// reuben1.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip> // needed for formatting

using namespace std;

/* Define global variables */
int ndim = 3;       //number of dimensions

/* Define function prototypes */
void plotVTU(int,int,double []); 

int main()
{
	//an array of vector
	//supose only record 100 nodes in an axon,so the array has 100 elements
	int no_cells=1;     // number of cells
	int no_steps = 100; // number of steps 
	for (int k = 0; k < no_cells; k++)
	{
	  double coordinates[ndim*no_steps];
	  double z=0.0;
	  for (int i = 0; i < no_steps; i++)
	{
                double x= rand() % 100;
                double y= rand() % 100;
                double z=z+rand() % 10;
		coordinates[ndim*i+0]=x;
		coordinates[ndim*i+1]=y;
		coordinates[ndim*i+2]=z;
                plotVTU(i,no_cells,coordinates);
	}}
	return 0;
}

void plotVTU(int time_step, int no_cells, double coordinates[]){
        double x, y, z;

        // file corresponding to this time step
        std::ostringstream var;
        var << "utenn_" << time_step+1 <<".vtu";         
        ofstream outFile;
	std::string filename = var.str();
	outFile.open(filename.c_str());

	if (outFile.fail())
	{
		cout << "The file was not successfully opened" << endl;
		exit(1);
	}

	// set the output file stream formats
	outFile << setiosflags(ios::fixed)
		<< setiosflags(ios::showpoint)
		<< setprecision(2);

	// send data to the file
        // write out coordinates for this time step
	for (int k = 0; k < no_cells; k++)
        {
		for (int i = 0; i < time_step; i++)
	        {
			x=coordinates[ndim*i+0];
			y=coordinates[ndim*i+1];
			z=coordinates[ndim*i+2];
			outFile << x <<", "<<y<<", "<<z<<endl;	
		}
	}

	outFile.close();
	cout << "The file " << filename
		<< " has been successfully written." << endl;

 	return;
}
