#include <iostream>
#include <unordered_map>

using namespace std;

// Global variables

int dim = 3;          							// Number of Dimensions - (x, y, z)
int nodes;            							// Number of Nodes
unordered_map <int, int> connectivity;			// Connectivity Matrix
int **node_values;								// Node Values

struct Node
{
	int value;
	double x;
	double y;
	double z;
	Node *next;
	Node (int val, double valX, double valY, double valZ) : value(val), x(valX), y(valY), z(valZ), next(NULL) {}
};

int main()
{
	Node *head = NULL, *l = NULL;
	int i = 0, j = 0, flag = 1;

	cout << "\n Enter the number of nodes: ";
	cin >> nodes;

	node_values = new int*[nodes];
	
	for (i = 0; i < nodes; i++)
	{
		node_values[i] = new int[dim + 1];
	}	

	cout << "\n Enter the Node values: ";

	for (i = 0; i < nodes; i++)
	{
		cin >> node_values[i][0];
	}

	cout << "\n Enter the Coordinate values (x, y, z) - \n";

	for (i = 0; i < nodes; i++)
	{
		cout << " " << node_values[i][0] << " : "; 
		for (j = 1; j < dim + 1; j++)
		{
			cin >> node_values[i][j];
		}
	}	
	
	for (i = 0; i < nodes; i++)
	{
		Node *pointer = new Node(node_values[i][0], node_values[i][1], node_values[i][2], node_values[i][3]);
		
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

	l = head;

	cout << "\n The linked list is: ";

	while (l)
	{
		if (l->next == NULL)
			cout << " " << l->value;
		
		else
			cout << " " << l->value << "->";

		l = l->next;
	}

	cout << "\n\n Enter the Connectivity Matrix - \n";
	
	for (i = 0; i < nodes - 1; i++)
	{
		cout << " " << node_values[i][0] << " -> which node: ";
		cin >> connectivity[node_values[i][0]];
		cout << endl;
	}

	cout << "\n Connectivity Matrix - \n";

	for (i = 0; i < nodes - 1; i++)
	{
		cout << " " << node_values[i][0] << " -> " << connectivity[node_values[i][0]];
		cout << endl;
	}

	cout << endl;
}
