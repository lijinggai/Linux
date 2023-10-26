#pragma once
#include<iostream>
#include<vector>

using namespace std;

template<class T, size_t N>
class Node {

public:
	Node()
	{
		//�������ΪN�����ؼ��ֱȺ�����һ��
		//��Ҫ�࿪��һ�������ж�����Ԫ����
		_key.resize(N);
		_children.resize(N+1, nullptr);
		_parent = nullptr;
		_size = 0;
	}
	~Node()
	{

	}
public:
	//�ؼ���
	vector<T> _key;
	//��������
	vector<Node<T, N>*> _children;
	//���׽ڵ�
	Node<T, N>* _parent;
	//��Ч����
	size_t _size;
};
template<class T, size_t N>
class BTree {
public:
	typedef Node<T, N> Node;
	//���ؽڵ���±�
	pair<Node*, int> Find(T val)
	{
		Node* parent = nullptr;
		Node* cur = _root;
		
		//�ҵ�����λ�ã����������� �ȵ�ǰԪ�ش��ж��Ƿ�������
		while (cur)
		{
			size_t i = 0;
			while (i < cur->_size)
			{
				//�ҵ����Ҵ����
				if ( val < cur->_key[i] )
					break;
				else if ( val > cur->_key[i] )
					i++;
				else//Ԫ�ش����ˣ��˳�
					return make_pair(cur, i);
			}
			//���游��
			parent = cur;
			cur = cur->_children[i];
		}
		//�����ڣ����ظ��׺�-1
		return make_pair(parent, -1);
	}
	void _Insert(T val, Node* parent, Node* child)
	{
		int end = parent->_size - 1;
		//�����һ��Ԫ��Ų����
		while (end >= 0)
		{
			//Ŀ��Ԫ�ظ�С��Ų
			if (val < parent->_key[end])
			{
				parent->_key[end + 1] = parent->_key[end];
				parent->_children[end + 2] = parent->_children[end + 1];//Ų�Һ���
				end--;
			}
			else
				break;
		}
		//���ʵĲ���λ��
		parent->_key[end + 1] = val;
		parent->_children[end + 2] = child;//�Һ���

		if (child)//ָ����
			child->_parent = parent;
		parent->_size++;
	}
	bool Insert(T val)
	{
		//��һ�������Ԫ��
		if (_root == nullptr)
		{
			_root = new Node;
			_root->_key[0] = val;
			_root->_size++;
		}
		else
		{
			//�����Ƿ����
			pair<Node*, int> pr = Find(val);
			if (pr.second >= 0)//Ԫ���Ѵ��ڣ������ڲ���
				return false;

			//�ò����λ��
			Node* parent = pr.first;
			Node* child = nullptr;
			_Insert(val, parent, child);
			
			while (true)
			{
				if (parent->_size < N)
					return true;
				else//������������
				{
					//���� [l, mid-1]   [mid+1, r]
					int mid = N / 2;
					size_t i = 0;
					size_t j = mid + 1;

					//�����ֵܽڵ�
					Node* brother = new Node;
					for (; j < N; j++, i++)
					{
						brother->_key[i] = parent->_key[j];
						brother->_children[i] = parent->_children[j];
						//�к��ӽڵ�,brother���¸���
						if (parent->_children[j])
							parent->_children[j]->_parent =  brother;

						//���Ѹ��ֵܵĹؼ��ֺͺ����ÿ�
						parent->_children[j] = nullptr;
						parent->_key[j] = T();
					}

					//����һ���Һ��Ӹ�brother
					brother->_children[i] = parent->_children[j];
					if (parent->_children[j])
						parent->_children[j]->_parent = brother;
					parent->_children[j] = nullptr;

					//�����괦�������
					brother->_size = i;
					parent->_size -= (i + 1);//��ȥ�ֵܺ��м�

					//�м�ֵ���ϲ���
					T midKey = parent->_key[mid];
					parent->_key[mid] = T();

					//ͷ�ڵ�
					if (parent->_parent == nullptr)
					{
						_root = new Node;
						_root->_key[0] = midKey;
						_root->_children[0] = parent;
						_root->_children[1] = brother;
						_root->_size = 1;

						//���ӵĸ���Ϊͷ
						parent->_parent = _root;
						brother->_parent = _root;
						break;
					}
					else
					{
						//��ͷ���²���
						_Insert(midKey, parent->_parent, brother);
						//����ѭ��������һ���Ƿ�����
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