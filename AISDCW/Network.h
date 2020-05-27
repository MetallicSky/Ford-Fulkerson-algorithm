#pragma once

#include <string>
#include <limits>
#include <cstdlib>
#include <fstream>
#include "List.h"

struct Network
{
	void fill(string fileName);

	void newNode(string name);

	void newEdge(string departure, string destination, float capacity);

	void print();

	bool contains(string name);

	float fordFulkerson(string sourceName, string outletName);
	float fordFulkerson(unsigned int source, unsigned int outlet);

private:

	void newEdge(unsigned int departure, unsigned int destination, float capacity);

	unsigned int nameToIndex(string input);

	struct Node
	{
		struct Edge
		{
			Node* destination;
			float capacity;
			float remain;

			Edge(Node* destination = nullptr, unsigned int capacity = NULL)
			{
				this->destination = destination;
				this->capacity = capacity;
				this->remain = capacity;
			}
		};

		void addEdge(Edge* newEdge)
		{
			this->connections.push_back(newEdge);
		};

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

	List<Node*> allNodes;
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
			newNode(dot1);
		tempS.clear();

		file >> tempC;
		while (tempC != ';')
		{
			tempS += tempC;
			file >> tempC;
		}
		string dot2 = tempS;
		if (contains(dot2) == false)
			newNode(dot2);
		tempS.clear();

		file >> tempC;
		while (tempC != '\n' && !file.eof())
		{
			tempS += tempC;
			file >> tempC;
		}

		float weight = stof(tempS);
		if (weight <= 0)
			throw exception("Nevative edge weight value");
		newEdge(dot1, dot2, weight);
	}
}

void Network::newNode(string name)
{
	for (unsigned int i = 0; i < allNodes.GetSize(); ++i)
		if (allNodes[i]->name == name)
			throw exception("Attempt to add a new dot with the same name");
	Node * temp = new Node(name);
	allNodes.push_back(temp);
}

void Network::newEdge(unsigned int departure, unsigned int destination, float capacity)
{
	if (departure > allNodes.GetSize() - 1 || destination > allNodes.GetSize() - 1)
		throw exception("Out of range");
	Node::Edge * temp = new Node::Edge(allNodes[destination], capacity);
	allNodes[departure]->addEdge(temp);
}

void Network::newEdge(string nameDeparture, string nameDestination, float capacity)
{
	newEdge(nameToIndex(nameDeparture), nameToIndex(nameDestination), capacity);
}

void Network::print()
{
	for (unsigned int i = 0; i < allNodes.GetSize(); ++i)
	{
		if (allNodes[i]->name != "")
			cout << allNodes[i]->name;
		else
			cout << i;
		cout << " - ";
		for (unsigned int j = 0; j < allNodes[i]->connections.GetSize(); ++j)
		{
			if (allNodes[i]->connections[j]->destination->name != "")
				cout << allNodes[i]->connections[j]->destination->name;
			else
				cout << allNodes[i]->connections[j]->destination;

			cout << " (" << allNodes[i]->connections[j]->capacity << ")";
			if (j < allNodes[i]->connections.GetSize() - 1)
				cout << ", ";
			else
				cout << endl;
		}
		cout << endl;
	}
}

bool Network::contains(string name)
{
	for (unsigned int i = 0; i < allNodes.GetSize(); ++i)
		if (allNodes[i]->name == name)
			return true;
	return false;
}

float Network::fordFulkerson(string sourceName, string outletName)
{
	return fordFulkerson(nameToIndex(sourceName), nameToIndex(outletName));
}

float Network::fordFulkerson(unsigned int sourceIndex, unsigned int outletIndex)
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

	if (sourceIndex >= allNodes.GetSize() || outletIndex >= allNodes.GetSize())
		throw exception("Out of range");

	Node * source = allNodes[sourceIndex];
	Node * outlet = allNodes[outletIndex];

	List<pathway*> pathways;

	source->flow = numeric_limits<float>::infinity();

	Node * current = source; // step 1: set current as source
	while (1)
	{
		Node available; // step 2: get a list of traversal available edges from current
		for (unsigned int i = 0; i < current->connections.GetSize(); ++i)
		{
			if (current->connections[i]->remain > 0)
				if (current->connections[i]->destination->flow == NULL)
				{
					Node::Edge* p = current->connections[i];
					available.addEdge(p);
				}
		}
		if (available.connections.GetSize() == 0)
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
				source->flow = 0;

				for (unsigned int i = 0; i < allNodes.GetSize(); ++i)
					for (unsigned int j = 0; j < allNodes[i]->connections.GetSize(); ++j)
						allNodes[i]->connections[j]->remain = allNodes[i]->connections[j]->capacity;

				return total; // the end
			}
			else
			{
				Node* temp = current->from; // step 4: jammed situation, get back and block this path
				for (unsigned int i = 0; i < temp->connections.GetSize(); ++i)
					if (temp->connections[i]->destination == current)
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
			unsigned int max = 0; // step 3: finding the next node to flow in
			for (unsigned int i = 0; i < available.connections.GetSize(); ++i)
				if (available.connections[i]->remain > available.connections[max]->remain)
					max = i;

			Node * next = available.connections[max]->destination;
			next->from = current;
			if (current->flow > available.connections[max]->remain)
			{
				next->flow = available.connections[max]->remain;
				available.connections[max]->remain = 0;
			}
			else
			{
				next->flow = current->flow;
				available.connections[max]->remain -= current->flow;
			}

			if (next == outlet) // step 5: reached outlet, tracking path, resetting flows, lowering remains and going back to source
			{
				pathways.push_back(new pathway(*next));
				Node* p = next;
				while (p != source) // clearing 'from' and 'flow' in Nodes, so only 'remain' in Edges adjusts
				{
					for (unsigned int i = 0; i < p->from->connections.GetSize(); ++i)
						if (p->from->connections[i]->destination == p)
						{
							p->from->connections[i]->remain += (p->flow - next->flow);
							if (p != next)
								p->flow = NULL;
							Node * temp = p->from;
							p->from = nullptr;
							p = temp;
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
	for (unsigned int i = 0; i < allNodes.GetSize(); ++i)
	{
		if (allNodes[i]->name == input)
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