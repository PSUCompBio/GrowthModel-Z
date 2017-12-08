#include <iostream>
#include <list>
#include <iomanip>
#include <fstream>
#include <string>
#include <random>

using namespace std;

int cells;

class node
{
	// Overloading the "<<" operator. Marked as friend for easier access.
	friend ostream &operator << (ostream &, const node &);

	public:

		// Data Members
		double x;
		double y;
		double z;

		// Constructors
		node();
		node(const node &);

		// Destructor
		~node(){};

		// Overloaded Operators
		node &operator = (const node &temp);
		int operator == (const node &temp) const;
		int operator < (const node &temp) const;
};

node::node()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

node::node(const node &copy)
{                             
	x = copy.x;
	y = copy.y;
	z = copy.z;
}

ostream &operator << (ostream &output, const node &temp)
{
	output << " " << temp.x << " " << temp.y << " " << temp.z << endl;
	return output;
}

node& node::operator = (const node &temp)
{
	this->x = temp.x;
	this->y = temp.y;
	this->z = temp.z;
	return *this;
}

int node::operator == (const node &temp) const
{
	if ((this->x != temp.x) || (this->y != temp.y) || (this->z != temp.z))
		return 0;
	
	else
		return 1;
}

// Required for certain built-in functions
int node::operator < (const node &temp) const
{
	if(this->x == temp.x && this->y == temp.y && this->z < temp.z)
		return 1;
	
	if(this->x == temp.x && this->y < temp.y)
		return 1;
	
	if(this->x < temp.x)
		return 1;
	
	return 0;
}

double randomGen (double low, double high);

void plotVTU (int time_step, list<node> united[]);

int main()
{
	srand(time(NULL));

	cout << "\n Enter the number of cells: ";
	cin >> cells;

	int counter = 0, yRange = 0, time_step = 0;
	double zStep = 10.0;

	node temp;
	list <node> united[cells];
	list <node>::reverse_iterator rit;
	
	while (counter < cells)
	{	
		yRange = counter * 100;
		
		temp.x = 0;
		
		temp.y = randomGen(yRange - 25, yRange + 25);
		
		if (counter % 2)
			temp.z = 0;
		
		else
			temp.z = 1000;
		
		united[counter].push_back(temp);
		
		counter += 1;
	}
	
	for (time_step = 0; time_step <= 100; time_step++)
	{
		plotVTU (time_step, united);

		counter = 0;
		
		while (counter < cells)
		{	
			rit = united[counter].rbegin();

			yRange = counter * 100;
			
			temp.x = 0;
			
			temp.y = randomGen(yRange - 25, yRange + 25);
			
			if (counter % 2)
				temp.z = rit->z + zStep;
			
			else
				temp.z = rit->z - zStep;

			united[counter].push_back(temp);
			
			counter += 1;
		}
	}
	
	cout << endl;
}

double randomGen (double low, double high)
{
	double random;
	double range = high - low + 1;

	random = low + range * (rand() / (RAND_MAX + 1.0));
	
	return random;
}


void plotVTU(int time_step, list<node> united[])
{

	string var = "utenn_";
	var += to_string(time_step);
	var += ".vtu";

	ofstream outFile;
	outFile.open(var);
	
	if (outFile.fail())
	{
		cout << "The file was not opened" << endl;
		exit(1);
	}

	outFile << setiosflags(ios::fixed) << setiosflags(ios::showpoint) << setw(10) << setprecision(8);

	int addOff = 0, i = 0, counter = 0, total = 0, *offset;
	float step = 0.0, color = 0.0;
	offset = new int[cells];
	list <node>::iterator it;

	for (i = 0; i < cells; i++)
	{
		if (i != 0)
			offset[i] = united[i].size() + offset[i - 1];
		
		else
			offset[i] = united[i].size();

		total += united[i].size();
	}

	step = 1.0 / cells;

	outFile << "<?xml version=\"1.0\"?>\n";
	outFile << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
	outFile << "\t<UnstructuredGrid>\n";
	outFile << "\t\t<Piece NumberOfPoints=\"" << total << "\" NumberOfCells=\"" << cells << "\">\n";
	
	outFile << "\t\t\t<CellData Scalars=\"cell_scalars\">\n";
	outFile << "\t\t\t\t<DataArray type=\"Float32\" Name=\"cell_scalars\" format=\"ascii\">\n";
	outFile << "\t\t\t\t\t";
	
	for (i = 0; i < cells; i++)
	{
		if (i != 0)
			color += step;

		if (i != cells - 1)
			outFile << color << " ";
		
		else
			outFile << color << "\n";
	}
	outFile << "\t\t\t\t</DataArray>\n";
	outFile << "\t\t\t</CellData>\n";
	outFile << "\t\t\t<Points>\n";
	outFile << "\t\t\t\t<DataArray type=\"Float32\" Name=\"Points\" NumberOfComponents=\"3\" format=\"ascii\">\n";

	for(i = 0; i < cells; i++)
	{
		it = united[i].begin();
		while (it != united[i].end())
		{
			outFile << "\t\t\t\t\t" << *it;
			it++;
		}
	}
	
	outFile << "\t\t\t\t</DataArray>\n";
	outFile << "\t\t\t</Points>\n";

	outFile << "\t\t\t<Cells>\n";
	outFile << "\t\t\t\t<DataArray type=\"Int64\" Name=\"connectivity\" format=\"ascii\">";
	
	for (i = 0; i < cells; i++)
	{
		outFile <<"\n\t\t\t\t\t";
		counter = united[i].size() * i;
		
		while (counter < (united[i].size() * (i + 1)))
		{
			if (counter == (united[i].size() * (i + 1)) - 1)
				outFile << counter;
			
			else
				outFile << counter << " ";
			
			counter += 1;
		}
	}
	
	outFile << "\n\t\t\t\t</DataArray>\n";

	outFile << "\t\t\t\t<DataArray type=\"Int64\" Name=\"offsets\" format=\"ascii\">\n";
	outFile << "\t\t\t\t\t";
	
	for (i = 0; i < cells; i++)
	{
		if (i != cells - 1)
			outFile << offset[i] << " ";
		else
			outFile << offset[i];
	}

	outFile << "\n\t\t\t\t</DataArray>\n";

	outFile << "\t\t\t\t<DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">\n";
	outFile << "\t\t\t\t\t";

	for (i = 0; i < cells; i++)
	{
		if (i != cells - 1)
			outFile << "4" << " ";
		else
			outFile << "4";	
	}

	outFile << "\n\t\t\t\t</DataArray>\n";

	outFile << "\t\t\t</Cells>\n";
	outFile << "\t\t</Piece>\n";
	outFile << "\t</UnstructuredGrid>\n";
	outFile << "</VTKFile>\n";

	outFile.close();
}
