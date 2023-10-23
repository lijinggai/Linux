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
		//�Ƿ��Ѿ�������
		if (xi == yi)
			return false;
		_v[xi] += _v[yi];
		_v[yi] = xi;
		return true;
	}
	//�ж��ٸ�����
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
	//��ϣ��������ݺ��±�
	unordered_map<T, int> _find_index;
};