#include <string>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

// GRAPH
class AdjacencyList
{
private:
	map<string, vector<pair<string, double>>> list;

public:
	void insertVertex(string from, string to)
	{
		list[from].push_back(make_pair(to, -1));
		list[to];
	};
	void PageRank(int power)
	{
		// calculate rank values
		rankCalc();

		// initialize r column vector
		double rank = (double) 1 / list.size();
		map<string, double> r;
		auto itr = list.begin();
		for (itr; itr != list.end(); itr++)
			r[itr->first] = rank;

		map<string, double> powerItr;
		int p = 1;
		do
		{
			// sets the power iteration as base column vector r
			if (power == 1)
			{
				for (itr = list.begin(); itr != list.end(); itr++)
					powerItr[itr->first] = r[itr->first];
				break;
			}
			// calculate the power iteration
			for (itr = list.begin(); itr != list.end(); itr++)
			{
				powerItr[itr->first];
				for (int i = 0; i < itr->second.size(); i++)
				{
					double rVector = r[itr->first];
					double rankValue = itr->second[i].second;
					powerItr[itr->second[i].first] += rVector * rankValue;
				}
			}
			// update the column vector r and reset the powerItr;
			if (p < power - 1)
			{
				for (itr = list.begin(); itr != list.end(); itr++)
				{
					r[itr->first] = powerItr[itr->first];
					powerItr[itr->first] = 0;
				}
			}
			p++;
		} 
		while (p < power);
	
		for (auto it = powerItr.begin(); it != powerItr.end(); it++)
			printf("%s %0.2f\n", it->first.c_str(), it->second);

	};
	void rankCalc()
	{
		auto itr = list.begin();
		for (itr; itr != list.end(); itr++)
		{
			for (int i = 0; i < itr->second.size(); i++)
				itr->second[i].second = (double) 1 / itr->second.size();
		}
	};
};

// INPUT READER
int main()
{
	int lines, power;
	AdjacencyList g;
	std::string from, to;
	std::cin >> lines;
	std::cin >> power;
	for (int i = 0; i < lines; i++)
	{
		std::cin >> from;
		std::cin >> to;
		g.insertVertex(from, to);
	}
	g.PageRank(power);

	return 0;
};