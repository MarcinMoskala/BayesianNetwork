#include "stdafx.h"
#include <CppUnitTest.h>  
#include "DataSet.h"
#include "Templates.cpp"
#include <vector>
#include <cassert>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

#ifndef CUSTOM_TEST_HELPERS
#define CUSTOM_TEST_HELPERS

#pragma once
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

#endif
