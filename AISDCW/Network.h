#pragma once

#include <string>
#include <limits>
#include "List.h"

struct Network
{
	void fill(string fileName);

	void newDot(string name);

	void newEdge(string departure, string destination, float capacity);

	void print();

	bool contains(string name);

	float fordFulkerson(string sourceName, string outletName);

private:

	void newEdge(unsigned int departure, unsigned int destination, float capacity);

	unsigned int nameToIndex(string input);

	struct Edge
	{
		unsigned int destination;
		float capacity;
		float remain;

		Edge(unsigned int destination = NULL, unsigned int capacity = NULL)
		{
			this->destination = destination;
			this->capacity = capacity;
			this->remain = capacity;
		}
	};

	struct Node
	{
		string name;
		List<Edge*> connections;

		Node* from;
		float flow;

		Node(string name = "")
		{
			this->name = name;
			this->from = nullptr;
			this->flow = 0;
		}
	};

	List<Node*> allDots;
	List<Edge*> allEdges;
};

void Network::fill(string fileName)
{
	ifstream file;
	file.open(fileName);
	file.unsetf(ios::skipws);

	while (!file.eof())
	{
		string tempS;
		char tempC;
		file >> tempC;
		while (tempC != ';')
		{
			tempS += tempC;
			file >> tempC;
		}
		string dot1 = tempS;
		if (contains(dot1) == false)
			newDot(dot1);
		tempS.clear();

		file >> tempC;
		while (tempC != ';')
		{
			tempS += tempC;
			file >> tempC;
		}
		string dot2 = tempS;
		if (contains(dot2) == false)
			newDot(dot2);
		tempS.clear();

		file >> tempC;
		while (tempC != '\n' && !file.eof())
		{
			tempS += tempC;
			file >> tempC;
		}

		float price = stof(tempS);
		newEdge(dot1, dot2, price);
	}
}

void Network::newDot(string name)
{
	for (unsigned int i = 0; i < allDots.GetSize(); ++i)
		if (allDots[i]->name == name)
			throw exception("Attempt to add a new dot with the same name");
	Node * temp = new Node(name);
	allDots.push_back(temp);
}

void Network::newEdge(unsigned int departure, unsigned int destination, float capacity)
{
	if (departure > allDots.GetSize() - 1 || destination > allDots.GetSize() - 1)
		throw exception("Out of range");
	allDots[departure]->connections.push_back(new Edge(destination, capacity));
}

void Network::newEdge(string nameDeparture, string nameDestination, float capacity)
{
	newEdge(nameToIndex(nameDeparture), nameToIndex(nameDestination), capacity);
}

void Network::print()
{
	for (unsigned int i = 0; i < allDots.GetSize(); ++i)
	{
		if (allDots[i]->name != "")
			cout << allDots[i]->name;
		else
			cout << i;
		cout << " - ";
		for (unsigned int j = 0; j < allDots[i]->connections.GetSize(); ++j)
		{
			if (allDots[allDots[i]->connections[j]->destination]->name != "")
				cout << allDots[allDots[i]->connections[j]->destination]->name;
			else
				cout << allDots[i]->connections[j]->destination;

			cout << " (" << allDots[i]->connections[j]->capacity << ")";
			if (j < allDots[i]->connections.GetSize() - 1)
				cout << ", ";
			else
				cout << endl;
		}
		cout << endl;
	}
}

bool Network::contains(string name)
{
	for (unsigned int i = 0; i < allDots.GetSize(); ++i)
		if (allDots[i]->name == name)
			return true;
	return false;
}

float Network::fordFulkerson(string sourceName, string outletName)
{
	struct pathway
	{
		List<Node*> path;
		float flow;

		pathway(Node& finalNode)
		{
			this->flow = finalNode.flow;
			Node* current = &finalNode;
			while (current != nullptr)
			{
				path.push_front(current);
				current = current->from;
			}
		}
	};
	List<pathway*> pathways;

	if (nameToIndex(sourceName) >= allDots.GetSize() || nameToIndex(outletName) >= allDots.GetSize())
		throw exception("Out of range");

	Node * source = allDots[nameToIndex(sourceName)];
	Node * outlet = allDots[nameToIndex(outletName)];

	source->flow = numeric_limits<float>::infinity();

	Node * current = source; // step 1: set current as source
	while (1)
	{
		List<Edge*> available; // step 2: get a list of traversal available edges from current
		for (unsigned int i = 0; i < current->connections.GetSize(); ++i)
		{
			if (current->connections[i]->remain > 0)
			{
				Edge* p = current->connections[i];
				if (allDots[p->destination]->flow == NULL)
					available.push_back(p);
				else
					continue;
			}
		}
		if (available.GetSize() == 0)
		{
			if (current == source)
			{
				float total = 0; // step 6: finishing algorhytm
				for (unsigned int i = 0; i < pathways.GetSize(); ++i)
				{
					total += pathways[i]->flow;
					for (unsigned int j = 0; j < pathways[i]->path.GetSize(); ++j)
					{
						cout << pathways[i]->path[j]->name;
						if (j != pathways[i]->path.GetSize() - 1)
							cout << " - ";
					}
					cout << " (" << pathways[i]->flow << ")" << endl;
				}
				return total; // the end
			}
			else
			{
				unsigned int jamIndex; // step 4: jammed situation, get back and block this path
				for (unsigned int i = 0; i < allDots.GetSize(); ++i)
					if (allDots[i] == current)
					{
						jamIndex = i;
						break;
					}
				Node* temp = current->from;
				for (unsigned int i = 0; i < temp->connections.GetSize(); ++i)
					if (temp->connections[i]->destination == jamIndex)
					{
						temp->connections[i]->remain = 0;
						break;
					}
				current->from = nullptr;
				current->flow = NULL;
				current = temp;
				continue; // goto step 2
			}
		}
		else
		{
			Edge* max = available[0];
			for (unsigned int i = 0; i < available.GetSize(); ++i)// step 3: finding the next node to flow in
				if (available[i]->remain > max->remain)
					max = available[i];

			Node * next = allDots[max->destination];
			next->from = current;
			if (current->flow > max->remain)
			{
				next->flow = max->remain;
				max->remain = 0;
			}
			else
			{
				next->flow = current->flow;
				max->remain -= current->flow;
			}

			if (next == outlet) // step 5: reached outlet, tracking path, resetting flows, lowering remains and going back to source
			{
				pathways.push_back(new pathway(*next));
				Node* p = next;
				unsigned int pIndex = nameToIndex(outletName);
				while (p != source) // clearing 'from' and 'flow' in Nodes, so only 'remain' in Edges adjusts
				{
					for (unsigned int i = 0; i < p->from->connections.GetSize(); ++i)
						if (p->from->connections[i]->destination == pIndex)
						{
							p->from->connections[i]->remain += (p->flow - next->flow);
							if (p != next)
								p->flow = NULL;
							Node * temp = p->from;
							p->from = nullptr;
							p = temp;
							for (unsigned int i = 0; i < allDots.GetSize(); ++i)
								if (allDots[i] == p)
								{
									pIndex = i;
									break;
								}
							break;
						}
				}
				next->flow = NULL;
				current = source; // goto step 1
			}
			else
				current = next; // goto step 2
		}
	}
}

unsigned int Network::nameToIndex(string input)
{
	unsigned int answer;
	bool success = false;
	for (unsigned int i = 0; i < allDots.GetSize(); ++i)
	{
		if (allDots[i]->name == input)
		{
			answer = i;
			success = true;
			break;
		}
	}

	if (success == false)
		throw exception("Couldn't find such dot");
	return answer;
}