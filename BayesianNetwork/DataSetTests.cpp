#include "stdafx.h"
#include <CppUnitTest.h>  
#include "DataSet.h"
#include "Templates.cpp"
#include "TestHelpers.cpp"
#include <vector>
#include <cassert>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

#pragma once
namespace BayesianNetwork
{

	TEST_CLASS(DataSet_tests)
	{
	public:
		TEST_METHOD(Correct_labels_creation_test)
		{
			DataSet data = DataSet(vector<vector<int>>{
				vector<int>{ 1, 2, 3 },
					vector<int>{ 4, 5, 6 },
					vector<int>{ 7, 8, 9 }
			});
			Assert::AreEqual(vector<string> { "1", "4", "7" }, data.paramsForColumn(0));
			Assert::AreEqual(vector<string> { "2", "5", "8" }, data.paramsForColumn(1));
			Assert::AreEqual(vector<string> { "3", "6", "9" }, data.paramsForColumn(2));
		}

		TEST_METHOD(Correct__reversed_order_labels_creation_test)
		{
			DataSet data = DataSet(vector<vector<int>>{
				vector<int>{ 9, 8, 7 },
				vector<int>{ 6, 5, 4 },
				vector<int>{ 3, 2, 1 }
			});
			Assert::AreEqual(vector<string> { "9", "6", "3" }, data.paramsForColumn(0));
			Assert::AreEqual(vector<string> { "8", "5", "2" }, data.paramsForColumn(1));
			Assert::AreEqual(vector<string> { "7", "4", "1" }, data.paramsForColumn(2));
		}

		TEST_METHOD(Correct_labels_creation_test_with_repetitive_labels)
		{
			DataSet data = DataSet(vector<vector<int>>{
				vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 1, 2 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 1, 2 }
			});
			Assert::AreEqual(vector<string> { "1", "2" }, data.paramsForColumn(0));
			Assert::AreEqual(vector<string> { "1" }, data.paramsForColumn(1));
			Assert::AreEqual(vector<string> { "2" }, data.paramsForColumn(2));
		}

		TEST_METHOD(Single_values_label_test)
		{
			DataSet data = DataSet(vector<vector<int>>{
				vector<int>{ 1, 10, 100 }
			});
			Assert::AreEqual(vector<string> { "1" }, data.paramsForColumn(0));
			Assert::AreEqual(vector<string> { "10" }, data.paramsForColumn(1));
			Assert::AreEqual(vector<string> { "100" }, data.paramsForColumn(2));
		}

		TEST_METHOD(Data_with_next_values_are_returning_next_values)
		{
			DataSet data = DataSet(vector<vector<int>>{
				vector<int>{ 0 },
				vector<int>{ 1 },
				vector<int>{ 2 },
				vector<int>{ 3 }
			});
			Assert::AreEqual(
				vector<vector<int>> { vector<int> { 0, 1, 2, 3} },
				data.valuesInRow(0)
			);
		}

		TEST_METHOD(Data_for_single_line_are_filled_with_zeroes)
		{
			DataSet data = DataSet(vector<vector<int>>{
				vector<int>{ 1, 10, 100 }
			});
			Assert::AreEqual(
				vector<vector<int>> { vector<int> { 0 }, vector<int> { 0 }, vector<int> { 0 } },
				data.valuesInRow(0)
			);
		}

		TEST_METHOD(Data_for_single_two_line_are_filled_correctly_with_zeroes_and_ones)
		{
			DataSet data = DataSet(vector<vector<int>>{
				vector<int>{ 1, 2 },
					vector<int>{ 2, 2 },
					vector<int>{ 1, 1 }
			});
			Assert::AreEqual(vector<string> { "1", "2" }, data.paramsForColumn(0));
			Assert::AreEqual(vector<string> { "2", "1" }, data.paramsForColumn(1));

			Assert::AreEqual(
				vector<vector<int>> { vector<int> { 0, 1, 0 }, vector<int> { 0, 0, 1 } },
				data.valuesInRow(0)
			);
		}
	};
}