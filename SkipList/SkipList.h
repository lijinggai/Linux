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
	//��Ա����
	T _val;
	vector<SkipNode<T>*> _v;
};

template<class T>
class SkipList {
	typedef SkipNode<T> Node;
public:
	//Ӧ�÷��ز�����
	pair<vector<Node*>, bool> FindPervNode(T val)
	{
		vector<Node*> findList(_head->_v.size(), nullptr);//������

		int level = _head->_v.size() - 1;
		Node* cur = _head;
		//�±�Ϊ-1��˵��û��Ԫ�أ�
		while (level >= 0)
		{
			//��һ��Ԫ��Ϊ�ջ��߱�Ҫ�ҵ�Ԫ�ش� �� ����
			if (cur->_v[level] == nullptr || cur->_val > val)
			{
				//ǰһ����Ӳ�����
				findList[level] = cur;
				level--;
			}
			//��һ��Ԫ�ر�Ҫ�ҵ�Ԫ��С������
			else if(cur->_val < val)
			{
				cur = cur->_v[level];
			}
			//���ڣ�return false
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
		//�����Ƿ����
		pair<vector<Node*>, bool> fpn = FindPervNode(val);
		 
		if (fpn.second == true)
			return false;

		vector<Node*> findList= fpn.first;
		//��ȡ����������½ڵ�
		int probaLevel = Random();
		Node* newNode = new Node(val,probaLevel);
		//ͷ�ĸ���С���½ڵ�ĸ���������
		if (_head->_v.size() < probaLevel)
		{
			_head->_v.resize(probaLevel, nullptr);
			findList.resize(probaLevel, _head);
		}
		
		//�½ڵ�ָ��findlist����һ����findlistָ���½ڵ�
		for (int i = 0; i < probaLevel; i++)
		{
			newNode->_v[i] = findList[i]->_v[i];
			findList[i]->_v[i] = newNode;
		}
	}
	bool Erase(T val)
	{
		//�����Ƿ����
		pair<vector<Node*>, bool> fpn = FindPervNode(val);

		if (fpn.second == false)
			return false;

		vector<Node*> findList = fpn.first;
		Node* prev = findList[0], * cur = prev->_v[0];
		//ǰһ����ָ��  ɾ���ڵ�ĺ�һ���ڵ�
		for (int i = 0; i < cur->_v.size(); i++)
		{
			prev->_v[i] = cur->_v[i];
		}
		delete cur;

		// ���ɾ����߲�ڵ㣬��ͷ�ڵ�Ĳ���Ҳ��һ��
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
			// ��ӡÿ��ÿ��cur�ڵ�
			for (auto e : cur->_v)
			{
				printf("%2s", "��");
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
			//ɾ�����нڵ�
		}
	}

private:
	//ͷ�ڵ�
	Node* _head;
	//ÿ��һ��ĸ���
	float _proba;
	//������
	size_t _maxLevel;
};