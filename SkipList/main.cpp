#include"SkipList.h"


int main()
{
	SkipList<int> sl;
	//int a[] = { 5, 2, 3, 8, 9, 6, 5, 2, 3, 8, 9, 6, 5, 2, 3, 8, 9, 6 };
	int a[] = { 5, 2, 3, 8, 9, 6 };
	for (auto e : a)
	{
		//sl.Print();
		//printf("--------------------------------------\n");

		sl.Insert(e);
	}
	sl.Print();

	int x;
	cin >> x;
	sl.Erase(x);

	return 0;
}