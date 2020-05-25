#include <iostream>
#include <fstream>

#include "Network.h"

using namespace std;

int main()
{
	Network test;
	test.fill("input.txt");
	test.print();
	cout << test.fordFulkerson("1", "5");
}