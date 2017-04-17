#include "stdafx.h"
#include <CppUnitTest.h>  
#include "DataSet.h"
#include "Templates.cpp"
#include <vector>
#include <cassert>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {

			static wstring stringToWstring(string str) {
				std::wstring str2(str.length(), L' ');
				std::copy(str.begin(), str.end(), str2.begin());
				return str2;
			}

			template<>
			static wstring ToString<set<string>>(const set<string> & coord) {
				string str = "{ ";
				for_each(coord.begin(), coord.end(), [&str](string s) -> void { str += s + ", "; });
				str += "}";
				return stringToWstring(str);
			}
			template<>
			static wstring ToString<set<int>>(const set<int> & coord) {
				string str = "{ ";
				for_each(coord.begin(), coord.end(), [&str](int i) -> void { str += to_string(i) + ", "; });
				str += "}";
				return stringToWstring(str);
			}
			template<>
			static wstring ToString<vector<int>>(const vector<int> & coord) {
				string str = "{ ";
				for_each(coord.begin(), coord.end(), [&str](int i) -> void { str += to_string(i) + ", "; });
				str += "}";
				return stringToWstring(str);
			}
			template<>
			static wstring ToString<vector<vector<int>>>(const vector<vector<int>> & coord) {
				wstring str = L"{ ";
				for_each(coord.begin(), coord.end(), [&str](vector<int> i) -> void { str += ToString(i) + L", "; });
				str += L"}";
				return str;
			}
		}
	}
}

namespace TimeSeriesToolkit
{

	TEST_CLASS(TimeSeries_tests)
	{
	public:
		TEST_METHOD(Correct_labels_creation_test)
		{
			DataSet data = DataSet(vector<vector<int>>{
			    vector<int>{ 1, 2, 3 },
				vector<int>{ 4, 5, 6 },
				vector<int>{ 7, 8, 9 }
			});
			Assert::AreEqual(set<string> { "1", "4", "7" }, data.paramsForColumn(0));
			Assert::AreEqual(set<string> { "2", "5", "8" }, data.paramsForColumn(1));
			Assert::AreEqual(set<string> { "3", "6", "9" }, data.paramsForColumn(2));
		}

		TEST_METHOD(Correct_labels_creation_test_with_repetitive_labels)
		{
			DataSet data = DataSet(vector<vector<int>>{
				vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 1, 2 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 1, 2 }
			});
			Assert::AreEqual(set<string> { "1", "2" }, data.paramsForColumn(0));
			Assert::AreEqual(set<string> { "1" }, data.paramsForColumn(1));
			Assert::AreEqual(set<string> { "2" }, data.paramsForColumn(2));
		}

		TEST_METHOD(Single_values_label_test)
		{
			DataSet data = DataSet(vector<vector<int>>{
				vector<int>{ 1, 10, 100 }
			});
			Assert::AreEqual(set<string> { "1" }, data.paramsForColumn(0));
			Assert::AreEqual(set<string> { "10" }, data.paramsForColumn(1));
			Assert::AreEqual(set<string> { "100" }, data.paramsForColumn(2));
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
	};
}