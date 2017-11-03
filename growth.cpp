#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <fstream>
#include <string>
#include <random>

using namespace std;

// Global variables

int dim = 3;          							// Number of Dimensions - (x, y, z)
int nodes;            							// Number of Nodes
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

	cout << "\n Connectivity Matrix - \n";

	while (l && l->next)
	{
		cout << " " << l->value << " -> " << connectivity[l->value];
		cout << endl;
		l = l->next;
	}

}

void updateConnectivity (Node *head)
{
	Node *l = head;
	
	while (l)
	{
		if (l && l->next)
			connectivity[l->value] = l->next->value;

		l = l->next;
	}
}

Node* createNodes (Node* head)
{
	Node *l = NULL;
	int i = 0, j = 0, flag = 1, temp[4];

	cout << "\n Enter the number of nodes: ";
	cin >> nodes;

	for (i = 0; i < nodes; i++)
	{
		j = 0;

		temp[0] = randomGen(1, nodes);
		check[temp[0]] = 1;

		for (j = 1; j <= 3; j++)
		{
			temp[j] = randomGen(-10 - nodes, nodes + 10);
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

	outFile << "<?xml version=\"1.0\"?>\n";
	outFile << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
	outFile << "\t<UnstructuredGrid>\n";
	outFile << "\t\t<Piece NumberOfPoints=\"" << nodes << "\" NumberOfCells=\"" << "1" << "\">\n";

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
	outFile << "\t\t\t\t<DataArray type=\"Int64\" Name=\"connectivity\" format=\"ascii\">\n";

	l = head;
	outFile << "\t\t\t\t\t" << l->value - 1;
	while (l)
	{
		if (l->next != NULL)
		{
			outFile << " ";
			outFile << connectivity[l->value] - 1;
		}
		l = l->next;
	}

	outFile << "\n\t\t\t\t</DataArray>\n";

	outFile << "\t\t\t\t<DataArray type=\"Int64\" Name=\"offsets\" format=\"ascii\">\n";
	outFile << "\t\t\t\t\t" << nodes;

	outFile << "\n\t\t\t\t</DataArray>\n";

	outFile << "\t\t\t\t<DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">\n";
	outFile << "\t\t\t\t\t4";

	outFile << "\n\t\t\t\t</DataArray>\n";

	outFile << "\t\t\t</Cells>\n";
	outFile << "\t\t</Piece>\n";
	outFile << "\t</UnstructuredGrid>\n";
	outFile << "</VTKFile>\n";

	outFile.close();
}
