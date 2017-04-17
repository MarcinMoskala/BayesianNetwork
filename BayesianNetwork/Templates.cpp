#include "stdafx.h"
#include <algorithm>
#include <vector>
#include <set>
#include <numeric>
#include <iterator>

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

template<class Ty> std::vector<Ty> uniquify(std::vector<Ty> v) {
	v.erase(uniquify(v.begin(), v.end()), v.end());
	return v;
}

template<class It> It uniquify(It begin, It const end)
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

template<class _Ty, class _Ty2, class _Fn1>
std::vector<_Ty2> map(std::vector<_Ty> iterable, _Fn1 _Func)
{
	std::vector<_Ty2> v = std::vector<_Ty2>();
	for (auto const& value : iterable) {
		v.push_back(_Func(value));
	}
	return v;
}

namespace std {
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