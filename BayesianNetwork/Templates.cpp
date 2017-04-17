#include "stdafx.h"
#include <algorithm>
#include <vector>
#include <set>
#include <numeric>
#include <iterator>

#ifndef CUSTOM_TEMPLATES
#define CUSTOM_TEMPLATES

#pragma once

template<class Ty>
int indexOf(std::vector<Ty> v, Ty i) {
	auto it = std::find(v.begin(), v.end(), i);
	return it - v.begin();
}

struct target_less
{
	template<class It>
	bool operator()(It const &a, It const &b) const { return *a < *b; }
};

struct target_equal
{
	template<class It>
	bool operator()(It const &a, It const &b) const { return *a == *b; }
};

template<class Ty> 
std::vector<Ty> uniquify(std::vector<Ty> v) {
	v.erase(uniquify(v.begin(), v.end()), v.end());
	return v;
}

template<class It> 
It uniquify(It begin, It const end)
{
	std::vector<It> v;
	v.reserve(static_cast<size_t>(std::distance(begin, end)));
	for (It i = begin; i != end; ++i)
	{
		v.push_back(i);
	}
	std::sort(v.begin(), v.end(), target_less());
	v.erase(std::unique(v.begin(), v.end(), target_equal()), v.end());
	std::sort(v.begin(), v.end());
	size_t j = 0;
	for (It i = begin; i != end && j != v.size(); ++i)
	{
		if (i == v[j])
		{
			using std::iter_swap; iter_swap(i, begin);
			++j;
			++begin;
		}
	}
	return begin;
}

template<class _STy, class _Ty, class _Fn1>
_STy sumBy(std::vector<_Ty> iterable, _STy initial, _Fn1 _Func)
{
	_STy sum = initial;
	for (auto const& value : iterable) {
		sum += _Func(value);
	}
	return sum;
}

template<class _Ty>
int sum(std::vector<_Ty> iterable)
{
	int sum = 0;
	for (auto const& value : iterable) {
		sum += value;
	}
	return sum;
}

template<class _Ty, class _Ty2, class _Fn1>
std::vector<_Ty2> mapTo(std::vector<_Ty> iterable, _Fn1 _Func)
{
	std::vector<_Ty2> v = std::vector<_Ty2>();
	for (auto const& value : iterable) {
		_Ty2 newVal = _Func(value);
		v.push_back(newVal);
	}
	return v;
}

template<class _Ty, class _Ty2, class _Fn1>
std::vector<_Ty2> mapIndexed(std::vector<_Ty> iterable, _Fn1 _Func)
{
	std::vector<_Ty2> v = std::vector<_Ty2>();
	int index = 0;
	for (auto const& value : iterable) {
		_Ty2 newVal = _Func(value, index++);
		v.push_back(newVal);
	}
	return v;
}

template<class It, class _Fn1>
bool any(It iterable, _Fn1 _Func)
{
	for (auto const& value : iterable) {
		if (_Func(value)) return true;
	}
	return false;
}

namespace std {
	template<class _It, class _Func> inline
		void for_each_indexed(_It iterable , _Func _Func)
	{	
		int i = 0;
		auto it = iterable.begin();
		auto end = iterable.end();
		for (; it != end; ++it)
		{
			_Func(*it, i);
			i++;
		}
	}

	template<class _InIt, class _Func> inline
		void for_each_indexed(_InIt _First, _InIt _Last, _Func _Func)
	{	// perform function for each element
		_DEBUG_RANGE(_First, _Last);
		int i = 0;
		for (; _First != _Last; ++_First)
		{	// perform function for each element
			_Func(*_First, i);
			i++;
		}
	}
}

#endif
