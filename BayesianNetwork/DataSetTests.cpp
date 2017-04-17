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
			vector<vector<int>> theArray = vector<vector<int>> {
				vector<int>{ 1,2,3 },
				vector<int>{ 4,5,6 },
				vector<int>{ 7,8,9 }
			};
			DataSet data = DataSet(theArray);
			Assert::AreEqual(set<string> { "1", "4", "7" }, data.paramsForColumn(0));
			Assert::AreEqual(set<string> { "2", "5", "8" }, data.paramsForColumn(1));
			Assert::AreEqual(set<string> { "3", "6", "9" }, data.paramsForColumn(2));
		}	
	};
}