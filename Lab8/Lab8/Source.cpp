#include<iostream>
#include<fstream>
#include<mutex>
#include<future>
using namespace std;
ifstream fin("hamilton.in");
int a[205][205], sol,n;
mutex mtx;

void tipar(int p,int st[])
{
	int k;
	cout << sol << endl;
	for (k = 1; k <= p; k++)
		cout << st[k] << " ";
	cout << st[1];


}
void back(int vf,int st[],int viz[])
{
	int k;
	vector<future<void>> futures;
	for (k = 1; k <=n && sol == 0; k++)
	{
		if (a[st[vf - 1]][k] == 1 && viz[k] == 0)
		{
			st[vf] = k;
			viz[k] = 1;
			if (vf == n)
			{
				if (a[st[vf]][1] == 1)
				{
					sol = 1; tipar(vf,st);
				}
			}
			else {
				if (k % 2 == 1)
					futures.push_back(async(back, vf + 1, st, viz));
				else
					back(vf + 1, st, viz);
			}viz[k] = 0;
		}

	}
	for (size_t i = 0; i < futures.size(); i++)
	{
		futures[i].wait();
	}
}
int main()
{
	fin >> n;
	int  viz[10] = {0}, st[20];
	int x, y;
	while (fin >> x >> y)
	{
		if (a[x][y] == 0)
		{
			a[x][y] =a[y][x]= 1;
		}
	}
	st[1] = 1;
	back(2,st,viz);
	if (sol == 0)
		cout << 0;
}