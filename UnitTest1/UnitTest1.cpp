#include "pch.h"
#include "CppUnitTest.h"
#include "Network.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(newNode_contains)
		{
			Network test;
			test.newNode("1");
			test.newNode("2");
			test.newNode("3");
			test.newNode("4");
			Assert::AreEqual(false, test.contains("6"));
			Assert::AreEqual(true, test.contains("3"));
			test.newNode("6");
			Assert::AreEqual(true, test.contains("6"));
			try
			{
				test.newNode("3");
			}
			catch (const std::exception & ex)
			{
				Assert::AreEqual(ex.what(), "Attempt to add a new dot with the same name");
			}
		}

		TEST_METHOD(newEdge_fordFulkerson)
		{
			Network test;
			test.newNode("1");
			test.newNode("2");
			test.newNode("3");
			test.newNode("4");
			float compare = 0;
			Assert::AreEqual(compare, test.fordFulkerson(0, 1));
			test.newEdge("1", "2", 5);
			compare = 5;
			Assert::AreEqual(compare, test.fordFulkerson(0, 1));
			try
			{
				test.fordFulkerson(10, 1);
			}
			catch (const std::exception & ex)
			{
				Assert::AreEqual(ex.what(), "Out of range");
			}
			test.newEdge("1", "4", 10);
			test.newEdge("4", "2", 20);
			test.newEdge("2", "3", 90);
			test.newEdge("3", "4", 100);
			compare = 15;
			Assert::AreEqual(compare, test.fordFulkerson(0, 1));
			compare = 20;
			Assert::AreEqual(compare, test.fordFulkerson(2, 1));
			try
			{
				test.fordFulkerson("5", "1");
			}
			catch (const std::exception & ex)
			{
				Assert::AreEqual(ex.what(), "Couldn't find such dot");
			}
		}
	};
}