#pragma once
#include<iostream>
#include<vector>

using namespace std;

template<class T, size_t N>
class Node {

public:
	Node()
	{
		//孩子最多为N个，关键字比孩子少一个
		//需要多开辟一个，来判断数组元素满
		_key.resize(N);
		_children.resize(N+1, nullptr);
		_parent = nullptr;
		_size = 0;
	}
	~Node()
	{

	}
public:
	//关键字
	vector<T> _key;
	//孩子数据
	vector<Node<T, N>*> _children;
	//父亲节点
	Node<T, N>* _parent;
	//有效个数
	size_t _size;
};
template<class T, size_t N>
class BTree {
public:
	typedef Node<T, N> Node;
	//返回节点和下标
	pair<Node*, int> Find(T val)
	{
		Node* parent = nullptr;
		Node* cur = _root;
		
		//找到合适位置，从左向右找 比当前元素大，判断是否有左孩子
		while (cur)
		{
			size_t i = 0;
			while (i < cur->_size)
			{
				//找到比我大的了
				if ( val < cur->_key[i] )
					break;
				else if ( val > cur->_key[i] )
					i++;
				else//元素存在了，退出
					return make_pair(cur, i);
			}
			//保存父亲
			parent = cur;
			cur = cur->_children[i];
		}
		//不存在，返回父亲和-1
		return make_pair(parent, -1);
	}
	void _Insert(T val, Node* parent, Node* child)
	{
		int end = parent->_size - 1;
		//从最后一个元素挪数据
		while (end >= 0)
		{
			//目标元素更小，挪
			if (val < parent->_key[end])
			{
				parent->_key[end + 1] = parent->_key[end];
				parent->_children[end + 2] = parent->_children[end + 1];//挪右孩子
				end--;
			}
			else
				break;
		}
		//合适的插入位置
		parent->_key[end + 1] = val;
		parent->_children[end + 2] = child;//右孩子

		if (child)//指向父亲
			child->_parent = parent;
		parent->_size++;
	}
	bool Insert(T val)
	{
		//第一个插入的元素
		if (_root == nullptr)
		{
			_root = new Node;
			_root->_key[0] = val;
			_root->_size++;
		}
		else
		{
			//查找是否存在
			pair<Node*, int> pr = Find(val);
			if (pr.second >= 0)//元素已存在，不用在插入
				return false;

			//该插入的位置
			Node* parent = pr.first;
			Node* child = nullptr;
			_Insert(val, parent, child);
			
			while (true)
			{
				if (parent->_size < N)
					return true;
				else//数组满，分离
				{
					//分裂 [l, mid-1]   [mid+1, r]
					int mid = N / 2;
					size_t i = 0;
					size_t j = mid + 1;

					//分离兄弟节点
					Node* brother = new Node;
					for (; j < N; j++, i++)
					{
						brother->_key[i] = parent->_key[j];
						brother->_children[i] = parent->_children[j];
						//有孩子节点,brother做新父亲
						if (parent->_children[j])
							parent->_children[j]->_parent =  brother;

						//分裂给兄弟的关键字和孩子置空
						parent->_children[j] = nullptr;
						parent->_key[j] = T();
					}

					//还有一个右孩子给brother
					brother->_children[i] = parent->_children[j];
					if (parent->_children[j])
						parent->_children[j]->_parent = brother;
					parent->_children[j] = nullptr;

					//分裂完处理个数；
					brother->_size = i;
					parent->_size -= (i + 1);//减去兄弟和中间

					//中间值向上插入
					T midKey = parent->_key[mid];
					parent->_key[mid] = T();

					//头节点
					if (parent->_parent == nullptr)
					{
						_root = new Node;
						_root->_key[0] = midKey;
						_root->_children[0] = parent;
						_root->_children[1] = brother;
						_root->_size = 1;

						//孩子的父亲为头
						parent->_parent = _root;
						brother->_parent = _root;
						break;
					}
					else
					{
						//非头重新插入
						_Insert(midKey, parent->_parent, brother);
						//继续循环，看上一层是否满了
						parent = parent->_parent;

					}


				}
			}
				
		}
	}
public:
private:
	Node* _root = nullptr;
};