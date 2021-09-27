#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <set>
#include <stack>

using namespace std;

class AdjacencyList
{
private:
	map<string, vector<pair<string, string>>> graph;

public:
	void makeGraph(vector<string>& list);
	void eulerPath();
};

void AdjacencyList::makeGraph(vector<string>& list)
{
	for (int i = 0; i < list.size(); i += 2)
	{
		// create 'from' vertex from the first length-1 bits
		string from = list[i].substr(0, list[i].size() - 1);
		for (int j = i; j < i + 2; j++)
		{
			// create 'to' vertex from the last bits;
			string to = list[j].substr(1);
			// create an edge using the 'from' vertex and the last bit of 'to'
			string edge = from + to[to.size() - 1];
			// insert the nodes with 'to' and the edge
			graph[from].push_back(make_pair(to, edge));
		}
	}
};

void AdjacencyList::eulerPath()
{
	// IMPLEMENTS Hierholzer's Algorithm

	// map of node and vector size
	unordered_map<string, int> edgeCount;
	// stack of the nodes accessed
	stack<string> cycle;
	// a vector of the complete node circuit
	vector<string> path;
	
	// initialize the number of edges for each node
	for (auto i = graph.begin(); i != graph.end(); i++)
		edgeCount[i->first] = i->second.size();

	auto itr = graph.begin();
	// pick and insert first node into stack
	string node = itr->first;
	cycle.push(node);

	while (!cycle.empty())
	{
		if (edgeCount[node] != 0)
		{
			// push into node into stack
			cycle.push(node);
			// get next node, decrease edges, and remove adjacent
			string nextNode = graph[node].back().first;
			edgeCount[node]--;
			graph[node].pop_back();
			// move to next node
			node = nextNode;
		}
		else
		{
			path.push_back(node);
			// back-track
			node = cycle.top();
			cycle.pop();
		}
	}
	
	string universalString = "";
	// create the string using the last digits of each vertex
	// iterated in reverse
	for (int i = path.size() - 1; i > 0; i--)
		universalString += path[i][path[i].length() - 1];
	cout << "k-Universal Circular String: " << universalString;
};


// recursive function to make all binary variations
void binaryStrings(char digits[], vector<string>& list, string s, int k)
{
	if (k == 0)
	{
		list.push_back(s);
		return;
	}
	for (int i = 0; i < 2; i++)
	{
		string binary;
		binary = s + digits[i];
		binaryStrings(digits, list, binary, k - 1);
	}
}

void universalString(int k)
{
	// create a list of all variations of binary length k
	vector<string> binaryList;
	char alphabet[2] = { '0', '1' };
	string s = "";
	binaryStrings(alphabet, binaryList, s, k);

	// create a graph using the binary list and print euler path
	AdjacencyList g;
	g.makeGraph(binaryList);
	g.eulerPath();
}

int main()
{
	int k;

	cout << "Enter integer k: ";
	cin >> k;

	universalString(k);

	return 0;
}