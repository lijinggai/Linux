#pragma once
#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;

template<class T>
class UnionFindSet {
public:
	int Find(T val)
	{
		int index = _find_index[val];
		while (_v[index] >= 0)
			index = _v[index];
		return index;
	}
	bool Union(T x, T y)
	{
		int xi = Find(x);
		int yi = Find(y);
		//是否已经联合了
		if (xi == yi)
			return false;
		_v[xi] += _v[yi];
		_v[yi] = xi;
		return true;
	}
	//有多少个集合
	int Count()
	{
		int count = 0;
		for (auto e : _v)
		{
			if (e < 0)
				count++;
		}
		return count;
	}
public:
	UnionFindSet(const vector<T>& tmp)
	{
		for (int i = 0; i < tmp.size(); i++)
			_find_index[tmp[i]] = i;
		_v.resize(tmp.size(), -1);
	}
	~UnionFindSet()
	{}
private:
	//
	vector<T> _v;
	//哈希数组的数据和下标
	unordered_map<T, int> _find_index;
};