// growth.cpp : model of microTENN growth
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip> // needed for formatting

using namespace std;

/* Define global variables */
int ndim = 3;          // number of dimensions
int nnodes;            // number of nodes
int ncells;            // number of cells
double *coordinates = NULL;   // nodal coordinates
//std::vector<double> coordinates;
int *connectivity;     // connectivity matrix
int *nelements;        // number of elements per cell
int nsteps;            // number of time steps
int *node_to_cell;     // maps a given node to a cell
int max_elements_per_cell = 3; // max elements per cell

/* Define function prototypes */
void plotVTU(int);

int main()
{
	//an array of vector
	//supose only record 100 nodes in an axon,so the array has 100 elements
	ncells = 1;     // number of cells
	nsteps = 1; // number of steps
  double x, y, z;
	int old_nodes;
	double *tmp = NULL;

  // initialize coordinates and connectivity
  //initially the number of coordinates = ncells, all at x,y,z =  0,0,0
	coordinates = new double[ncells*ndim];
  node_to_cell = new int[ncells];
	nelements = new int[ncells];
	connectivity = new int[ncells*max_elements_per_cell];

  for (int k = 0; k < ncells; k++){
		nnodes = nnodes + 1;
	  x = 0.0;
    y = 0.0;
    z = 0.0;
    coordinates[ndim*(nnodes-1)+0]=x;
    coordinates[ndim*(nnodes-1)+1]=y;
    coordinates[ndim*(nnodes-1)+2]=z;
    // assign current node to current cell
    node_to_cell[nnodes-1]=k;

		//initialize connectivity to -1's
		for (int j = 0; j < max_elements_per_cell; j++){
			connectivity[ncells*k*max_elements_per_cell+j+k]=-1;
		}

		// store 1st element point at 0,0,0
		//  0 hard coded b/c it is element 0
		std::cout << "counter= " << ncells*k*max_elements_per_cell+0+k<< '\n';
    connectivity[ncells*k*max_elements_per_cell+0+k]=nnodes-1;
		nelements[k]=nelements[k]+1;
		std::cout << "connectivity[ncells*k*max_elements_per_cell+0+k] = "
			<< connectivity[ncells*k*max_elements_per_cell+0+k] <<'\n';
    //plotVTU(0);
	}

 	std::cout << "-nnodes = "<<nnodes<< " nnodes*ndim= " << nnodes*ndim<< '\n';
	// grow the microTENN
	for (int i = 0; i < nsteps; i++){
		for (int k = 0; k < ncells; k++){
    	//create a segment (aka element), need one additional node (unless branching)
			// reallocate arrays to make room for new data
			old_nodes = nnodes;
			nnodes = nnodes + 1;
			std::cout << "nnodes = "<<nnodes<< " nnodes*ndim= " << nnodes*ndim<< '\n';

		  double *tmp = new double[nnodes*ndim];
			for (int j = 0; j < old_nodes*ndim; j++){
				tmp[j] = coordinates[j];
			}
      //delete [] coordinates;
			//double *coordinates = new double[nnodes*ndim];
			coordinates = tmp;
			delete [] tmp;
//if (k==1)exit(0);
			//std::cout << "nnodes  =" << nnodes<< '\n';
      x = rand() % 100;
      y = rand() % 100;
    	z = z+rand() % 10;
			coordinates[ndim*(nnodes-1)+0]=x;
			coordinates[ndim*(nnodes-1)+1]=y;
			coordinates[ndim*(nnodes-1)+2]=z;

    			// now add new node to form a element
      //nelements[k] is the number of element in current cell
			std::cout << "counter= " << ncells*k*max_elements_per_cell+nelements[k]+k<< '\n';
			connectivity[ncells*k*max_elements_per_cell+nelements[k]+k]=nnodes-1;
			std::cout << "connectivity[ncells*k*max_elements_per_cell+0+k]= "
				<< connectivity[ncells*k*max_elements_per_cell+0+k] <<'\n';
			std::cout << "connectivity[ncells*k*max_elements_per_cell+nelements[k]+k]= "
				<< connectivity[ncells*k*max_elements_per_cell+nelements[k]+k] <<'\n';
			nelements[k]=nelements[k]+1;

    	plotVTU(i);
			//if (k==1)exit(0);
		}
	}
	return 0;
}

void plotVTU(int time_step){
	double x, y, z;

  // file corresponding to this time step
  std::ostringstream var;
  var << "utenn_" << time_step+1 <<".vtu";
  ofstream outFile;
	std::string filename = var.str();
	outFile.open(filename.c_str());
	if (outFile.fail()){
		cout << "The file was not successfully opened" << endl;
		exit(1);
	}

	// set the output file stream formats
	outFile << setiosflags(ios::fixed)
					<< setiosflags(ios::showpoint)
					<< std::setw(10)
					<< setprecision(8);

  outFile << "<?xml version=\"1.0\"?>\n";
  outFile << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
  outFile << "\t<UnstructuredGrid>\n";
  outFile << "\t\t<Piece NumberOfPoints=\"" << nnodes << "\" NumberOfCells=\"" << ncells<< "\">\n";

  /* Point Data */
  outFile << "\t\t\t<Points>\n";
  outFile << "\t\t\t\t<DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n";

  // write out all coordinates for this time step
	for (int i = 0; i < nnodes; i++){
		x=coordinates[ndim*i+0];
		y=coordinates[ndim*i+1];
		z=coordinates[ndim*i+2];
		outFile << "\t\t\t\t\t" << x <<"  "<<y<<"  "<<z<<endl;
	}
	outFile << "\t\t\t\t</DataArray>\n";
  outFile << "\t\t\t</Points>\n";

  /** Cell Data */
	// print connectivity
  outFile << "\t\t\t<Cells>\n";
  outFile << "\t\t\t\t<DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">\n";
  for (int k = 0; k < ncells; k++){
		outFile << "\t\t\t\t\t";
		for (int j = 0; j < max_elements_per_cell; j++){
			//if (connectivity[ncells*max_elements_per_cell*k+j] != -1){
				outFile << connectivity[ncells*k*max_elements_per_cell+k+j]  << " " ;
		//	}
		}
		outFile << endl;
	}
	outFile << "\t\t\t\t</DataArray>\n";

  // print offsets (needed by vtu/paraview)
	outFile << "\t\t\t\t<DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">\n";
	for (int k = 0; k < ncells; k++){
		outFile << "\t\t\t\t\t" << "2"  << "\n";
	}
	outFile << "\t\t\t\t</DataArray>\n";

 	// print cell types, using polyline
	// see http://www.vtk.org/wp-content/uploads/2015/04/file-formats.pdf
	outFile << "\t\t\t\t<DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">\n";
	for (int k = 0; k < ncells; k++){
		outFile << "\t\t\t\t\t" << "3" << endl;
	}
	outFile << "\t\t\t\t</DataArray>\n";

  outFile << "\t\t\t</Cells>\n";
  outFile << "\t\t</Piece>\n";
  outFile << "\t</UnstructuredGrid>\n";
  outFile << "</VTKFile>\n";

	outFile.close();
	//cout << "The file " << filename
	//	<< " has been successfully written." << endl;

 	return;
}
