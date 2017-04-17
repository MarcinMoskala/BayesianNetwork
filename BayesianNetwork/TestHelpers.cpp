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

			static wstring longDoubleToWString(long double ld) {
				std::stringstream ss;
				ss << ld;
				return stringToWstring(ss.str());
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
				return str + L"}";
			}

			template<>
			static wstring ToString<long double>(const long double & coord) {
				return longDoubleToWString(coord);
			}

			template<>
			static wstring ToString<vector<long double>>(const vector<long double> & coord) {
				wstring str = L"{ ";
				for_each(coord.begin(), coord.end(), [&str](long double i) -> void { str += longDoubleToWString(i) + L", "; });
				return str + L"}";
			}

			template<>
			static wstring ToString<vector<vector<long double>>>(const vector<vector<long double>> & coord) {
				wstring str = L"{ ";
				for_each(coord.begin(), coord.end(), [&str](vector<long double> i) -> void { str += ToString(i) + L", "; });
				return str + L"}";
			}
		}
	}
}

#endif
