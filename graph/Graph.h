#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<queue>
#include"UnionFindSet.h"

using namespace std;

//V�������ͣ�WȨֵ����
namespace Matrix {
	template<class V, class W, bool Direction = false, W W_MAX = INT_MAX>
	class Graph {
	public:
		typedef Graph<V, W, Direction, W_MAX> Self;
		int Find(const V& v)
		{
			auto it = _find_index.find(v);
			if (it == _find_index.end())
				return -1;
			return it->second;
		}
		bool AddEdge(const V& src, const V& des, const W& weight)
		{
			int si = Find(src);
			int di = Find(des);
			//���󶥵�
			if (si == -1 || di == -1)
				return false;
			_matrix[si][di] = weight;
			if (Direction == false)
				_matrix[di][si] = weight;
			return true;
		}
		void Print()
		{
			for (int i = 0; i < _matrix.size(); i++)
			{
				for (int j = 0; j < _matrix.size(); j++)
				{
					if (_matrix[i][j] == W_MAX)
						printf("%5c", '*');
					else
						printf("%5d", _matrix[i][j]);
				}
				cout << endl;
			}
		}
	public:
		template<class W>
		struct Edge
		{
			bool operator>(const Edge<W>& eg)const 
			{
				return _weight > eg._weight;
			}
			Edge(int s,int d, int w):_src(s), _des(d), _weight(w)
			{}
			int _src;
			int _des;
			W _weight;
		};
		//̰��˼�룬ѡȨֵ��С�ıߣ�ʹ�ò��鼯�ж��Ƿ񹹳ɻ�·
		W Kruskal(Self& minTree)
		{
			//��ʼ��������
			int n = _vertexs.size();
			minTree._vertexs = _vertexs;
			minTree._find_index = _find_index;
			minTree._matrix.resize(n, vector<W>(n, W_MAX) );

			//���ȼ����б����
			priority_queue<Edge<W>, vector<Edge<W>>, greater<Edge<W>>> pq;

			//������б߽����ȼ�����
			for (int i = 0; i < n; i++)
			{
				for (int j = i + 1; j < n; j++)
				{
					if (_matrix[i][j] != W_MAX)
						pq.push(Edge<W> (i, j, _matrix[i][j]) );
				}
			}

			//��n-1����
			int count = 0;
			UnionFindSet<V> ufs(_vertexs);
			int totalW = 0;
			while (!pq.empty())
			{
				Edge<W> eg= pq.top();
				pq.pop();
				bool ret = ufs.Union(_vertexs[eg._src], _vertexs[eg._des]);
				if (ret == true){//�����ɻ�·
					cout << _vertexs[eg._src] << ' ' << _vertexs[eg._des] << ' ' << eg._weight << endl;
					minTree.AddEdge(_vertexs[eg._src], _vertexs[eg._des], eg._weight);
					count++;
					totalW += eg._weight;
				}
				else {//
					cout << "���ɻ�  " << _vertexs[eg._src] << ' ' << _vertexs[eg._des] << ' ' << eg._weight << endl;
				}
			//���ɵ���Ȩֵ
			}
			if (count == n - 1)//�ܹ���������
				return totalW;
			else//���ܹ���
				return W();
		}
		//̰��˼�룬����ѡ��ĵ����죬ʹ��set������ʹ�ã���ʹ��ʹ�ù��ľ�һ�����ṹ��
		//��ʹ�ò��鼯����Ϊ��ʹ�ýڵ㶼����ͨ��
		W Prim(Self &minTree, const V& val)
		{
			int n = _vertexs.size();
			//��ʼ��mintree
			minTree._vertexs = _vertexs;
			minTree._find_index = _find_index;
			minTree._matrix.resize(n, vector<W>(n, W_MAX));
			
			int pos = _find_index[val];
			//������ʹ��
			unordered_set<V> visited;
			visited.insert(pos);

			priority_queue<Edge<W>, vector<Edge<W>>, greater<Edge<W>> > pq;//
			for (int j = 0; j < n; j++) {
				if(_matrix[pos][j] != W_MAX)
				pq.push(Edge<W>(pos, j, _matrix[pos][j]));
			}
			int count = 0;//������
			W TotalW = 0;//Ȩֵ��С
			while (!pq.empty())
			{
				//��ȡ��ǰ��С
				Edge<W> dg = pq.top();
				pq.pop();
				int des = dg._des;
				if(visited.count(des))//����,������
					cout << "���ɻ�  " << _vertexs[dg._src] << ' ' << _vertexs[dg._des] << ' ' << dg._weight << endl;
				else {
					for (int j = 0; j < n; j++)//����±�
					{
						if(_matrix[des][j] != W_MAX)
						pq.push(Edge<W>(des, j, _matrix[des][j]));
					}
					visited.insert(des);//��ʹ��
					minTree._matrix[dg._src][dg._des] = dg._weight;//���������
					TotalW += dg._weight;
					count++;
					cout << _vertexs[dg._src] << ' ' << _vertexs[dg._des] << ' ' << dg._weight << endl;
				}


			}
			if (count == n - 1)
				return TotalW;
			else
				return W();
		}
	public:
		//���·��
		void Dijkstra(const V& v, vector<W>& dest, vector<int>& parentPath )
		{
			int n = _vertexs.size();
			//��ʼ���±�
			int src = _find_index[v];
			//�ͳ�ʼ������·��
			dest.resize(n, W_MAX);
			//���㸸���±�
			parentPath.resize(n, -1);

			dest[src] = W();//��ʼ����Ϊ0
			vector<bool> visited(n, false);
			//ִ��n��
			for (int i = 0; i < n; i++)
			{
				int minW = W_MAX, minI = src;
				//�ҵ���ǰ��С������δʹ��
				for (int j = 0; j < n; j++)
				{
					if (visited[j] == false && dest[j] < minW)
					{
						minI = j;
						minW = dest[j];
					}
				}
				//��Ϊ��ʹ�õ�ֵ�������ܸ�С
				visited[minI] = true;

				//�����Ƿ����ҵ���С����Ȩֵ
				for (int j = 0; j < n; j++)
				{
					//����δʹ�ã����������ұ�
					if (visited[j] == false && _matrix[minI][j] != W_MAX)
					{
						//��·��ֵ��С
						if (dest[minI] + _matrix[minI][j] < dest[j])
						{
							//����·��ֵ�͸������±�
							dest[j] = dest[minI] + _matrix[minI][j];
							parentPath[j] = minI;
						}
					}
				}
			}
		}
		bool BellmanFord(const V& v, vector<W>& dest, vector<int>& parentPath)
		{
			int n = _vertexs.size();
			int src = _find_index[v];//��ʼλ��

			dest.resize(n, W_MAX);
			parentPath.resize(n, -1);

			//��ʼ����ʼλ��
			dest[src] = 0;

			//Ϊʲô��n-1�Σ����ⶥ�㵽�������ж������n-1�Σ��ٶ�˵�����ɸ�Ȩ��·
			for (int k = 0; k < n - 1; k++)
			{
				bool quit = true;
				//����Ȩֵ�ڽӾ���
				for (int i = 0; i < n; i++)
				{
					for (int j = 0; j < n; j++)
					{
						if (_matrix[i][j] != W_MAX && dest[i] != W_MAX && dest[i] + _matrix[i][j] < dest[j])
						{
							//���ø�С��·��ֵ�͸������±�
							dest[j] = dest[i] + _matrix[i][j];
							parentPath[j] = i;
							quit = false;
						}
					}
				}
				if (quit)
					break;
			}

			//�ж��Ƿ񹹳ɸ�Ȩ��·
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					if (_matrix[i][j] != W_MAX && dest[i] != W_MAX && dest[i] + _matrix[i][j] < dest[j])
						return false;
				}
			}
			return true;
		}
		void FloydWarShall(vector<vector<W>>& vvDest, vector<vector<int>>& vvParentPath)
		{
			int n = _vertexs.size();
			vvDest.resize(n);
			vvParentPath.resize(n);
			//��ʼ��
			for (int i = 0; i < n; i++)
			{
				vvDest[i].resize(n, W_MAX);
				vvParentPath[i].resize(n, -1);
			}
			//ֱ�������Ķ���������ά����
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					if (_matrix[i][j] != W_MAX)
					{
						vvDest[i][j] = _matrix[i][j];
						vvParentPath[i][j] = i;
					}
					else if (i == j)
						vvDest[i][j] = 0;
				}
			}
			
			//����˼�룺i �� j֮�����һ��k������i��k + k��j  < i �� j
			for (int k = 0; k < n; k++)
			{
				for (int i = 0; i < n; i++)
				{
					for (int j = 0; j < n; j++)
					{
						if (vvDest[i][k] != W_MAX && vvDest[k][j] != W_MAX && vvDest[i][k] + vvDest[k][j] < vvDest[i][j])
						{
							
							vvDest[i][j] = vvDest[i][k] + vvDest[k][j];
							vvParentPath[i][j] = vvParentPath[k][j];
						}
					}
				}
			}

		}
	public:
		Graph(const vector<V>& v)
		{
			int n = v.size();
			_vertexs.resize(n);
			//��ʼ�����㼯�Ϻ�  ������±��ӳ��
			for (int i = 0; i < n; i++)
			{
				_vertexs[i] = v[i];
				_find_index[v[i]] = i;
			}
			//��ʼ�ڽӾ���
			_matrix.resize(n, vector<W>(n, W_MAX));
		}
		Graph() = default;
		~Graph()
		{}
	private:
		//���㼯��
		vector<V> _vertexs;
		//���Ҷ����±�
		unordered_map<V, int> _find_index;
		vector<vector<W>> _matrix;

	};
}
namespace List {
	template<class W>
	class Node {
	public:
		int _des;
		//Ȩֵ
		W _weight;
		Node<W>* _next;
		Node(int des, W w):_des(des),_weight(w),_next(nullptr)
		{}
	};
	template<class V, class W, bool Direction = false>
	class Graph{
	public:
		int Find(const V& v)
		{
			auto it = _find_index.find(v);
			if (it == _find_index.end())
				return -1;
			return it->second;
		}
		bool AddEdge(const V& src, const V& des, const W& weight)
		{
			//��ȡ�±�
			int si = Find(src);
			int di = Find(des);
			//������Ԫ�أ�fail��
			if (si == -1 || di == -1)
				return false;
			//�ڽӱ���ӱ�
			//���ж��Ƿ��Ѿ����
			if (_table[si] == nullptr){
				//��δ��ӹ�
				Node<W>* new_node = new Node<W>(di, weight);
				new_node->_next = _table[si];
				_table[si] = new_node;
				//����ͼ
				if (Direction == false) {
					Node<W>* new_node = new Node<W>(si, weight);
					new_node->_next = _table[di];
					_table[di] = new_node;
				}

			}
			else {
				Node<W>* nd = _table[si];
				while (nd)
				{
					if (nd->_des == di)
						return false;
					else
						nd = nd->_next;
				}
				//��δ��ӹ�
				Node<W>* new_node = new Node<W>(di, weight);
				new_node->_next = _table[si];
				_table[si] = new_node;
				//����ͼ
				if (Direction == false) {
					Node<W>* new_node = new Node<W>(si, weight);
					new_node->_next = _table[di];
					_table[di] = new_node;
				}
			}
			return true;

		}
		void Print()
		{
			for (int i = 0; i < _table.size(); i++)
			{
				cout << _vertexs[i] << ": ";
				Node<W>* nd = _table[i];
				while (nd)
				{
					cout << _vertexs[nd->_des] << " " << nd->_weight << "   ";
					nd = nd->_next;
				}
				cout << endl;
			}
		}
		void _DFS(int pos, vector<bool>& visited)
		{
			visited[pos] = true;
			Node<W>* nd = _table[pos];
			while (nd)
			{
				cout << _vertexs[pos] <<  _vertexs[nd->_des] << "  ";
				if (visited[nd->_des] == false)
					_DFS(nd->_des, visited);
				nd = nd->_next;
			}
		}
		void DFS(const V& v)
		{
			int pos = _find_index[v];
			vector<bool> visited(_table.size(), false);
			_DFS(pos, visited);

		}
		void BFS(const V& v)
		{
			int pos = _find_index[v];
			vector<bool> visited(_table.size(), false);
			queue<int> q;
			q.push(pos);
			while (!q.empty())
			{
				int size = q.size();
				while (size--)
				{
					pos = q.front();
					visited[pos] = true;
					q.pop();
					Node<W>* nd = _table[pos];
					while (nd)
					{
						cout << _vertexs[pos] << _vertexs[nd->_des] << "  ";
						if (visited[nd->_des] == false)
							q.push(nd->_des);
						nd = nd->_next;
					}
					
				}
			}
		}
	public:
		Graph(const vector<V>& v)
		{
			int n = v.size();
			_vertexs.reserve(n);
			for (int i = 0; i < n; i++)
			{
				_vertexs.push_back(v[i]);
				_find_index[v[i]] = i;
			}
			_table.resize(n, nullptr);
		}
	private:
		//���㼯��
		vector<V> _vertexs;
		//������±�Ĺ�ϣ
		unordered_map<V, int> _find_index;
		//�ڽӱ�
		vector<Node<W>*> _table;
	};
}

