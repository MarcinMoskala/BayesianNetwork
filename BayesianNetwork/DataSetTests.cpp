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
			static wstring ToString<vector<string>>(const vector<string> & coord) {
				string str = "{ ";
				for_each(coord.begin(), coord.end(), [&str](string s) -> void { str += s + ", "; });
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

		TEST_METHOD(Mapping_test)
		{
			Assert::AreEqual(vector<int> { 2, 3 }, map<int, int>(vector<int>{1, 2}, [](int i) -> int { return i + 1; }));
			Assert::AreEqual(vector<int> { 2, 4, 6 }, map<int, int>(vector<int>{1, 2, 3}, [](int i) -> int { return i * 2; }));
			Assert::AreEqual(vector<int> { 1, 4, 9 }, map<int, int>(vector<int>{1, 2, 3}, [](int i) -> int { return i * i; }));
		}

		TEST_METHOD(Vector_mapping_test)
		{
			Assert::AreEqual(vector<vector<int>> { {3, 4}, { 5, 6 } }, map<vector<int>, vector<int>>(vector<vector<int>>{ {2, 3}, { 4, 5 } }, [](vector<int> v) -> vector<int> {
				return map<int, int>(v, [](int i) -> int { 
					return i + 1;
				});
			}));
		}

		TEST_METHOD(Uniquify_test)
		{
			std::vector<int> v = { 1, 1, 2, 2 };
			v.erase(uniquify(v.begin(), v.end()), v.end());
			Assert::AreEqual(vector<int> { 1, 2 }, v);
		}

		TEST_METHOD(Custom_uniquify_test)
		{
			std::vector<int> v = { 1, 1, 2, 2 };
			v = uniquify(v);
			Assert::AreEqual(vector<int> { 1, 2 }, v);
		}
	};
}