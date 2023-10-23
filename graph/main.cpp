#include<string>
#include<vector>
#include"Graph.h"


void test()
{
vector<string> a{ "����", "����", "����", "����", "����" };
List::Graph<string, int, false> g1(a);
g1.AddEdge("����", "����", 100);
g1.AddEdge("����", "����", 200);
g1.AddEdge("����", "����", 30);
g1.AddEdge("����", "����", 30);
g1.Print();
g1.BFS("����");

g1.DFS("����");
g1.Print();
}
void TestGraphMinTree()
{
	const char* ch = "abcdefghi";
	vector<char> v;
	for (int i = 0; i < strlen(ch); i++)
	{
		v.push_back(ch[i]);
	}
	Matrix::Graph<char, int> g(v);
	g.AddEdge('a', 'b', 4);
	g.AddEdge('a', 'h', 8);
	g.AddEdge('b', 'c', 8);
	g.AddEdge('b', 'h', 11);
	g.AddEdge('c', 'i', 2);
	g.AddEdge('c', 'f', 4);
	g.AddEdge('c', 'd', 7);
	g.AddEdge('d', 'f', 14);
	g.AddEdge('d', 'e', 9);
	g.AddEdge('e', 'f', 10);
	g.AddEdge('f', 'g', 2);
	g.AddEdge('g', 'h', 1);
	g.AddEdge('g', 'i', 6);
	g.AddEdge('h', 'i', 7);

	//Matrix::Graph<char, int> kminTree;
	//cout << "Prim:" << g.Prim(kminTree, 'a') << endl;
	//cout << "Kruskal:" << g.Kruskal(kminTree) << endl;
	//kminTree.Print();
}
void TestGraphDijkstra()
{
	const char* ch = "syztx";
	vector<char> v;
	for (int i = 0; i < strlen(ch); i++)
	{
		v.push_back(ch[i]);
	}
	Matrix::Graph<char, int, true, INT_MAX > g(v);
	g.AddEdge('s', 't', 10);
	g.AddEdge('s', 'y', 5);
	g.AddEdge('y', 't', 3);
	g.AddEdge('y', 'x', 9);
	g.AddEdge('y', 'z', 2);
	g.AddEdge('z', 's', 7);
	g.AddEdge('z', 'x', 6);
	g.AddEdge('t', 'y', 2);
	g.AddEdge('t', 'x', 1);
	g.AddEdge('x', 'z', 4);

	vector<int> dist;
	vector<int> parentPath;
	g.Dijkstra('s', dist, parentPath);
	//g.PrintShortPath('s', dist, parentPath);

	//// ͼ�д��и�Ȩ·��ʱ��̰�Ĳ�����ʧЧ�ˡ�
	//// ���Խ�����Կ���s->t->y֮������·��û���³���
	//const char* str = "sytx";
	//Matrix::Graph<char, int, INT_MAX, true> g(str, strlen(str));
	//g.AddEdge('s', 't', 10);
	//g.AddEdge('s', 'y', 5);
	//g.AddEdge('t', 'y', -7);
	//g.AddEdge('y', 'x', 3);
	//vector<int> dist;
	//vector<int> parentPath;
	//g.Dijkstra('s', dist, parentPath);
	//g.PrintShortPath('s', dist, parentPath);
}
void TestGraphBellmanFord()
{
	const char* ch = "syztx";
	vector<char> v;
	for (int i = 0; i < strlen(ch); i++)
	{
		v.push_back(ch[i]);
	}
	Matrix::Graph<char, int, true, INT_MAX> g(v);
	g.AddEdge('s', 't', 6);
	g.AddEdge('s', 'y', 7);
	g.AddEdge('y', 'z', 9);
	g.AddEdge('y', 'x', -3);
	g.AddEdge('z', 's', 2);
	g.AddEdge('z', 'x', 7);
	g.AddEdge('t', 'x', 5);
	g.AddEdge('t', 'y', 8);
	g.AddEdge('t', 'z', -4);
	g.AddEdge('x', 't', -2);
	vector<int> dist;
	vector<int> parentPath;
	if (g.BellmanFord('s', dist, parentPath))
	{
		//g.PrinrtShotPath('s', dist, parentPath);
	}
	else
	{
		cout << "���ڸ�Ȩ��·" << endl;
	}
	// ΢��ͼ�ṹ�����и�Ȩ��·�Ĳ���
	//const char* str = "syztx";
	//Graph<char, int, INT_MAX, true> g(str, strlen(str));
	//g.AddEdge('s', 't', 6);
	//g.AddEdge('s', 'y', 7);
	//g.AddEdge('y', 'x', -3);
	//g.AddEdge('y', 'z', 9);
	//g.AddEdge('y', 'x', -3);
	//g.AddEdge('y', 's', 1); // ����
	//g.AddEdge('z', 's', 2);
	//g.AddEdge('z', 'x', 7);
	//g.AddEdge('t', 'x', 5);
	//g.AddEdge('t', 'y', -8); // ����
	//g.AddEdge('t', 'z', -4);
	//g.AddEdge('x', 't', -2);
	//vector<int> dist;
	//vector<int> parentPath;
	//if (g.BellmanFord('s', dist, parentPath))
	//{
	// g.PrinrtShotPath('s', dist, parentPath);
	//}
	//else
	//{
	// cout << "���ڸ�Ȩ��·" << endl;
	//}
}
void TestFloydWarShall()
{
	const char* ch = "12345";
	vector<char> v;
	for (int i = 0; i < strlen(ch); i++)
	{
		v.push_back(ch[i]);
	}
	Matrix::Graph<char, int, true, INT_MAX> g(v);
	g.AddEdge('1', '2', 3);
	g.AddEdge('1', '3', 8);
	g.AddEdge('1', '5', -4);
	g.AddEdge('2', '4', 1);
	g.AddEdge('2', '5', 7);
	g.AddEdge('3', '2', 4);
	g.AddEdge('4', '1', 2);
	g.AddEdge('4', '3', -5);
	g.AddEdge('5', '4', 6);
	vector<vector<int>> vvDist;
	vector<vector<int>> vvParentPath;
	g.FloydWarShall(vvDist, vvParentPath);

}
int main()
{
	//test1();
	//TestGraphMinTree();
	//TestGraphDijkstra()
	//TestGraphBellmanFord();
	TestFloydWarShall();
	return 0;
}