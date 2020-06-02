#include <iostream>
#include "Network.h"

using namespace std;

int main()
{
	Graph test;
	test.fileRead("input.txt");
	test.print();
	for (unsigned int i = 0; i < 7; ++i)
	{
		unsigned int source = 1 + rand() % 10;
		unsigned int outlet = 1 + rand() % 10;
		while (outlet == source)
			outlet = 1 + rand() % 10;
		cout << "\n\nS: " << source << ", T: " << outlet << ":" << endl;
		float weight = test.FF(to_string(source), to_string(outlet));
		cout << endl << "Width: " << weight;
	}
}