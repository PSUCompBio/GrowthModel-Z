#include <iostream>
#include <unordered_map>

using namespace std;

// Global variables

int dim = 3;          							// Number of Dimensions - (x, y, z)
int nodes;            							// Number of Nodes
unordered_map <int, int> connectivity;			// Connectivity Matrix

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

void fillConnectivity (Node *head);

Node* enterData (Node* head);

int main()
{
	Node *head = NULL;

	head = enterData(head);
	
	fillConnectivity(head);
	
	printConnectivity(head);
	
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

	cout << "\n Connectivity Matrix - \n";

	while (l && l->next)
	{
		cout << " " << l->value << " -> " << connectivity[l->value];
		cout << endl;
		l = l->next;
	}

}

void fillConnectivity (Node *head)
{
	Node *l = head;
	
	while (l)
	{
		if (l && l->next)
			connectivity[l->value] = l->next->value;

		l = l->next;
	}
}

Node* enterData (Node* head)
{
	Node *l = NULL;
	int i = 0, j = 0, flag = 1, temp[4];

	cout << "\n Enter the number of nodes: ";
	cin >> nodes;	

	cout << "\n Enter Data \n";

	for (i = 0; i < nodes; i++)
	{
		j = 0;

		cout << "\n Node Value: ";

		cin >> temp[0];

		cout << "\n Enter the Coordinate values (x, y, z) for Node " << temp[0] << ": ";

		for (j = 1; j <= 3; j++)
		{
			cin >> temp[j];
		}

		Node *pointer = new Node(temp[0], temp[1], temp[2], temp[3]);
		
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

	return head;
}

/*
void plotVTU(int time_step, Node* head)
{

	Node *l = head;

	std::ostringstream var;
	ofstream outFile;

	var << "utenn_" << time_step+1 << ".vtu";
	std::string filename = var.str();

	outFile.open(filename.c_str());
	
	if (outFile.fail())
	{
		cout << "The file was not opened" << endl;
		exit(1);
	}

	outFile << setiosflags(ios::fixed) << setiosflags(ios::showpoint) << std::setw(10) << setprecision(8);

	outFile << "<?xml version=\"1.0\"?>\n";
	outFile << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
	outFile << "\t<UnstructuredGrid>\n";
	outFile << "\t\t<Piece NumberOfPoints=\"" << nodes << "\" NumberOfCells=\"" << nodes << "\">\n";

	outFile << "\t\t\t<Points>\n";
	outFile << "\t\t\t\t<DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n";

	// write out all coordinates for this time step
	while (l)
	{
		outFile << "\t\t\t\t\t" << l->x << "  " << l->y << "  " << l->z << endl;
		l = l->next;
	}

	outFile << "\t\t\t\t</DataArray>\n";
	outFile << "\t\t\t</Points>\n";

	/ Cell Data *
	// print connectivity
	outFile << "\t\t\t<Cells>\n";
	outFile << "\t\t\t\t<DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">\n";
	
	for (int k = 0; k < ncells; k++)
	{
		outFile << "\t\t\t\t\t";

		for (int j = 0; j < max_elements_per_cell; j++)
			outFile << connectivity[ncells*k*max_elements_per_cell+k+j]  << " " ;

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
}
*/
