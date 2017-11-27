#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <fstream>
#include <string>
#include <random>

using namespace std;

// Global variables

int dim = 3;          							// Number of Dimensions - (x, y, z)
int cells;								// Number of Cells
int *nodes = NULL;            						// Number of Nodes
unordered_map <int, int> connectivity;					// Connectivity Matrix
unordered_map <int, int> check;

struct Node
{
	int value;
	double x;
	double y;
	double z;
	Node *next;
	Node (int val, double valX, double valY, double valZ) : value(val), x(valX), y(valY), z(valZ), next(NULL) {}
};

void printList (Node* head);

void printConnectivity (Node* head);

void updateConnectivity (Node *head);

Node* createNodes (Node* head);

void plotVTU (int time_step, Node* head);

int randomGen (int low, int high);

int main()
{
	Node *head = NULL;

	head = createNodes(head);
	
	updateConnectivity(head);
	
	printConnectivity(head);
	
	plotVTU (123, head);

	cout << endl;
}

void printList (Node* head)
{
	Node *l = head;	

	cout << "\n The linked list is: ";

	while (l)
	{
		if (l->next == NULL)
			cout << " " << l->value;
		
		else
			cout << " " << l->value << "->";

		l = l->next;
	}
}

void printConnectivity (Node* head)
{
	Node *l = head;
	int addOff = 0, i = 0, counter = 0;

	cout << "\n Connectivity Matrix - \n";

	for (i = 0; i < cells; i++)
	{
		cout <<"\n Cell " << i + 1 << " - \n";
		
		if (i != 0)
			addOff += nodes[i - 1];

		for (counter = 1; counter < nodes[i]; counter++)
		{
			if (l && l->next)
				cout << " " << l->value + addOff << " -> " << connectivity[l->value + addOff] << endl;

			l = l->next;
		}

		if (l)
			l = l->next;
	}

}

void updateConnectivity (Node *head)
{
	Node *l = head;
	int addOff = 0, i = 0, counter = 0;

	for (i = 0; i < cells; i++)
	{
		if (i != 0)
			addOff += nodes[i - 1];

		for (counter = 1; counter < nodes[i]; counter++)
		{
			if (l && l->next)
				connectivity[l->value + addOff] = l->next->value + addOff;

			l = l->next;
		}

		if (l)
			l = l->next;
	}
}

Node* createNodes (Node* head)
{
	Node *l = NULL;
	int x = 0, k = 0, i = 0, j = 0, flag = 1, temp[3], z_coord = 0, step = 0;

	cout << "\n Enter the number of cells: ";
	cin >> cells;

	nodes = new int[cells];
	
	for (i = 0; i < cells; i++)
	{
		cout << "\n Enter the number of nodes for cell " << i + 1 << ": ";
		cin >> nodes[i];
	}

	for (k = 0; k < cells; k++)
	{
		x = nodes[k];
		check.clear();
		z_coord = 0;
		step = 2 * (x + 10) / x;

		for (i = 0; i < x; i++)
		{
			j = 0;
			z_coord += step;
			temp[0] = randomGen(1, x);
			check[temp[0]] = 1;

			for (j = 1; j <= 2; j++)
				temp[j] = randomGen(-10 - x, x + 10);

			Node *pointer = new Node(temp[0], temp[1], temp[2], z_coord);
			
			if (flag == 1)
			{
				head = pointer;
				l = pointer;
				flag = 0;
			}

			else
			{
				l->next = pointer;
				l = pointer;
			}
		}
	}
	
	return head;
}

int randomGen (int low, int high)
{
	int random;
	int range = high - low + 1;

	random = low + range * (rand() / (RAND_MAX + 1.0));

	while (check[random])
		random = low + range * (rand() / (RAND_MAX + 1.0));
		
	return random;
}

void plotVTU(int time_step, Node* head)
{

	Node *l = head;

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
	
	for (i = 0; i < cells; i++)
	{
		if (i != 0)
			offset[i] = nodes[i] + offset[i - 1];
		
		else
			offset[i] = nodes[i];

		total += nodes[i];
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

	while (l)
	{
		outFile << "\t\t\t\t\t" << l->x << "  " << l->y << "  " << l->z << endl;
		l = l->next;
	}

	outFile << "\t\t\t\t</DataArray>\n";
	outFile << "\t\t\t</Points>\n";

	outFile << "\t\t\t<Cells>\n";
	outFile << "\t\t\t\t<DataArray type=\"Int64\" Name=\"connectivity\" format=\"ascii\">";

	l = head;
	
	for (i = 0; i < cells; i++)
	{
		outFile <<"\n\t\t\t\t\t";
		
		if (i != 0)
			addOff += nodes[i - 1];

		outFile << l->value + addOff - 1;

		for (counter = 1; counter < nodes[i]; counter++)
		{
			if (l && l->next)
				outFile << " " << connectivity[l->value + addOff] - 1;

			l = l->next;
		}

		if (l)
			l = l->next;
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
