#pragma once
#include<iostream>
#include<vector>

using namespace std;

template<class T>
struct SkipNode
{
	SkipNode(T val, size_t n):_val(val)
	{
		_v.resize(n, nullptr);
	}
	//成员变量
	T _val;
	vector<SkipNode<T>*> _v;
};

template<class T>
class SkipList {
	typedef SkipNode<T> Node;
public:
	//应该返回查找链
	pair<vector<Node*>, bool> FindPervNode(T val)
	{
		vector<Node*> findList(_head->_v.size(), nullptr);//查找链

		int level = _head->_v.size() - 1;
		Node* cur = _head;
		//下标为-1，说明没有元素；
		while (level >= 0)
		{
			//下一个元素为空或者比要找的元素大 ， 向下
			if (cur->_v[level] == nullptr || cur->_val > val)
			{
				//前一个添加查找链
				findList[level] = cur;
				level--;
			}
			//下一个元素比要找的元素小，向右
			else if(cur->_val < val)
			{
				cur = cur->_v[level];
			}
			//等于，return false
			else
				return make_pair(findList, true);
		}
		return make_pair(findList, false);
	}
	int Random()
	{
		int level = 1;
		while (rand() <= _proba * RAND_MAX && level < _maxLevel)
			level++;
		return level;
	}
	bool Insert(T val)
	{
		//查找是否存在
		pair<vector<Node*>, bool> fpn = FindPervNode(val);
		 
		if (fpn.second == true)
			return false;

		vector<Node*> findList= fpn.first;
		//获取随机层数和新节点
		int probaLevel = Random();
		Node* newNode = new Node(val,probaLevel);
		//头的个数小于新节点的个数，扩容
		if (_head->_v.size() < probaLevel)
		{
			_head->_v.resize(probaLevel, nullptr);
			findList.resize(probaLevel, _head);
		}
		
		//新节点指向findlist的下一个，findlist指向新节点
		for (int i = 0; i < probaLevel; i++)
		{
			newNode->_v[i] = findList[i]->_v[i];
			findList[i]->_v[i] = newNode;
		}
	}
	bool Erase(T val)
	{
		//查找是否存在
		pair<vector<Node*>, bool> fpn = FindPervNode(val);

		if (fpn.second == false)
			return false;

		vector<Node*> findList = fpn.first;
		Node* prev = findList[0], * cur = prev->_v[0];
		//前一个节指向  删除节点的后一个节点
		for (int i = 0; i < cur->_v.size(); i++)
		{
			prev->_v[i] = cur->_v[i];
		}
		delete cur;

		// 如果删除最高层节点，把头节点的层数也降一下
		int i = _head->_v.size() - 1;
		while (i >= 0)
		{
			if (_head->_v[i] == nullptr)
				--i;
			else
				break;
		}
		_head->_v.resize(i + 1);
		return true;
	}
	void Print()
	{

		Node* cur = _head;
		while (cur)
		{
			printf("%2d\n", cur->_val);
			// 打印每个每个cur节点
			for (auto e : cur->_v)
			{
				printf("%2s", "↓");
			}
			printf("\n");

			cur = cur->_v[0];
		}
	}
public:
	SkipList()
	{
		srand( (unsigned int) time(nullptr) );
		_head = new Node(T(), 1);
		_proba = 0.5;
		_maxLevel = 32;
	}
	~SkipList()
	{
		if (_head)
		{
			//删除所有节点
		}
	}

private:
	//头节点
	Node* _head;
	//每多一层的概率
	float _proba;
	//最大层数
	size_t _maxLevel;
};