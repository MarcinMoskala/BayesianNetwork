#include "stdafx.h"
#include <algorithm>
#include <vector>
#include <numeric>
#include <iterator>

template<class _Ty, class _It, class _Fn1>
std::vector<_Ty> map(_It iterable, _Fn1 _Func)
{
	std::vector<_Ty> v(iterable.begin(), iterable.end());
	for (int i = 0; i < v.size(); i++) {
		v[i] = _Func(v[i]);
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