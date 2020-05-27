#include <iostream>
#include "Network.h"

using namespace std;

int main()
{
	Network test;
	test.fill("input.txt");
	test.print();
	for (unsigned int i = 0; i < 10; ++i)
	{
		unsigned int source = 1 + rand() % 10;
		unsigned int outlet = 1 + rand() % 10;
		while (outlet == source)
			outlet = 1 + rand() % 10;
		cout << "\n\nSource: " << source << ", outlet: " << outlet << ":" << endl;
		float weight = test.fordFulkerson(to_string(source), to_string(outlet));
		cout << endl << "Weight: " << weight;
	}
}