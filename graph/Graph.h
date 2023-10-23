#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<queue>
#include"UnionFindSet.h"

using namespace std;

//V顶点类型，W权值类型
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
			//错误顶点
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
		//贪心思想，选权值最小的边，使用并查集判断是否构成回路
		W Kruskal(Self& minTree)
		{
			//初始化生成树
			int n = _vertexs.size();
			minTree._vertexs = _vertexs;
			minTree._find_index = _find_index;
			minTree._matrix.resize(n, vector<W>(n, W_MAX) );

			//优先级队列保存边
			priority_queue<Edge<W>, vector<Edge<W>>, greater<Edge<W>>> pq;

			//添加所有边进优先级队列
			for (int i = 0; i < n; i++)
			{
				for (int j = i + 1; j < n; j++)
				{
					if (_matrix[i][j] != W_MAX)
						pq.push(Edge<W> (i, j, _matrix[i][j]) );
				}
			}

			//有n-1条边
			int count = 0;
			UnionFindSet<V> ufs(_vertexs);
			int totalW = 0;
			while (!pq.empty())
			{
				Edge<W> eg= pq.top();
				pq.pop();
				bool ret = ufs.Union(_vertexs[eg._src], _vertexs[eg._des]);
				if (ret == true){//不构成回路
					cout << _vertexs[eg._src] << ' ' << _vertexs[eg._des] << ' ' << eg._weight << endl;
					minTree.AddEdge(_vertexs[eg._src], _vertexs[eg._des], eg._weight);
					count++;
					totalW += eg._weight;
				}
				else {//
					cout << "构成环  " << _vertexs[eg._src] << ' ' << _vertexs[eg._des] << ' ' << eg._weight << endl;
				}
			//构成的总权值
			}
			if (count == n - 1)//能构成生成树
				return totalW;
			else//不能构成
				return W();
		}
		//贪心思想，按已选择的点延伸，使用set保存已使用，不使用使用过的就一定不会构环
		//不使用并查集是因为已使用节点都是连通的
		W Prim(Self &minTree, const V& val)
		{
			int n = _vertexs.size();
			//初始化mintree
			minTree._vertexs = _vertexs;
			minTree._find_index = _find_index;
			minTree._matrix.resize(n, vector<W>(n, W_MAX));
			
			int pos = _find_index[val];
			//保存已使用
			unordered_set<V> visited;
			visited.insert(pos);

			priority_queue<Edge<W>, vector<Edge<W>>, greater<Edge<W>> > pq;//
			for (int j = 0; j < n; j++) {
				if(_matrix[pos][j] != W_MAX)
				pq.push(Edge<W>(pos, j, _matrix[pos][j]));
			}
			int count = 0;//边数量
			W TotalW = 0;//权值大小
			while (!pq.empty())
			{
				//获取当前最小
				Edge<W> dg = pq.top();
				pq.pop();
				int des = dg._des;
				if(visited.count(des))//存在,不可用
					cout << "构成环  " << _vertexs[dg._src] << ' ' << _vertexs[dg._des] << ' ' << dg._weight << endl;
				else {
					for (int j = 0; j < n; j++)//添加新边
					{
						if(_matrix[des][j] != W_MAX)
						pq.push(Edge<W>(des, j, _matrix[des][j]));
					}
					visited.insert(des);//已使用
					minTree._matrix[dg._src][dg._des] = dg._weight;//生成树添边
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
		//最短路径
		void Dijkstra(const V& v, vector<W>& dest, vector<int>& parentPath )
		{
			int n = _vertexs.size();
			//起始点下标
			int src = _find_index[v];
			//和初始点的最短路径
			dest.resize(n, W_MAX);
			//顶点父亲下标
			parentPath.resize(n, -1);

			dest[src] = W();//起始点置为0
			vector<bool> visited(n, false);
			//执行n次
			for (int i = 0; i < n; i++)
			{
				int minW = W_MAX, minI = src;
				//找到当前最小顶点且未使用
				for (int j = 0; j < n; j++)
				{
					if (visited[j] == false && dest[j] < minW)
					{
						minI = j;
						minW = dest[j];
					}
				}
				//设为已使用的值，不可能更小
				visited[minI] = true;

				//延伸是否能找到更小顶点权值
				for (int j = 0; j < n; j++)
				{
					//满足未使用，两个顶点右边
					if (visited[j] == false && _matrix[minI][j] != W_MAX)
					{
						//新路径值更小
						if (dest[minI] + _matrix[minI][j] < dest[j])
						{
							//设置路径值和父顶点下标
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
			int src = _find_index[v];//初始位置

			dest.resize(n, W_MAX);
			parentPath.resize(n, -1);

			//初始化初始位置
			dest[src] = 0;

			//为什么是n-1次：任意顶点到其他所有顶点最多n-1次，再多说明构成负权回路
			for (int k = 0; k < n - 1; k++)
			{
				bool quit = true;
				//遍历权值邻接矩阵
				for (int i = 0; i < n; i++)
				{
					for (int j = 0; j < n; j++)
					{
						if (_matrix[i][j] != W_MAX && dest[i] != W_MAX && dest[i] + _matrix[i][j] < dest[j])
						{
							//设置更小的路径值和父顶点下标
							dest[j] = dest[i] + _matrix[i][j];
							parentPath[j] = i;
							quit = false;
						}
					}
				}
				if (quit)
					break;
			}

			//判断是否构成负权回路
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
			//初始化
			for (int i = 0; i < n; i++)
			{
				vvDest[i].resize(n, W_MAX);
				vvParentPath[i].resize(n, -1);
			}
			//直接相连的顶点添加入二维数组
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
			
			//动规思想：i 到 j之间存在一个k，并且i到k + k到j  < i 到 j
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
			//初始化顶点集合和  顶点和下标的映射
			for (int i = 0; i < n; i++)
			{
				_vertexs[i] = v[i];
				_find_index[v[i]] = i;
			}
			//初始邻接矩阵
			_matrix.resize(n, vector<W>(n, W_MAX));
		}
		Graph() = default;
		~Graph()
		{}
	private:
		//顶点集合
		vector<V> _vertexs;
		//查找顶点下标
		unordered_map<V, int> _find_index;
		vector<vector<W>> _matrix;

	};
}
namespace List {
	template<class W>
	class Node {
	public:
		int _des;
		//权值
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
			//获取下标
			int si = Find(src);
			int di = Find(des);
			//不存在元素，fail；
			if (si == -1 || di == -1)
				return false;
			//邻接表添加边
			//先判断是否已经添加
			if (_table[si] == nullptr){
				//还未添加过
				Node<W>* new_node = new Node<W>(di, weight);
				new_node->_next = _table[si];
				_table[si] = new_node;
				//无向图
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
				//还未添加过
				Node<W>* new_node = new Node<W>(di, weight);
				new_node->_next = _table[si];
				_table[si] = new_node;
				//无向图
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
		//顶点集合
		vector<V> _vertexs;
		//顶点和下标的哈希
		unordered_map<V, int> _find_index;
		//邻接表
		vector<Node<W>*> _table;
	};
}

