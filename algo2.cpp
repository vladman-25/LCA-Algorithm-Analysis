#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <fstream>
#include <sstream>
// CPP code to find LCA of given
// two nodes in a tree

#include <chrono>

using namespace std::chrono;
using namespace std;


#define left 2 * i + 1
#define right 2 * i + 2
using namespace std;

const int maxn = 1000000;

vector<vector<int>> g(maxn);

int level[maxn];

vector<int> e;
vector<int> l;
int h[maxn];

int st[5 * maxn];

void leveling(int src) {
	for (int i = 0; i < g[src].size(); i++) {
			int des = g[src][i];
			if (!level[des]) {
			level[des] = level[src] + 1;
			leveling(des);
		}
	}
}

bool visited[maxn];

void dfs(int src) {
	e.push_back(src);
	visited[src] = 1;
	for (int i = 0; i < g[src].size(); i++) {
		int des = g[src][i];
		if (!visited[des]) {
			dfs(des);
			e.push_back(src);
		}
	}
}


int RMQ(int ss, int se, int qs, int qe, int i) {
	if (ss > se)
		return -1;

	if (se < qs || qe < ss)
		return -1;

	if (qs <= ss && se <= qe)
		return st[i];

	int mid = (ss + se) >> 1;
	int st = RMQ(ss, mid, qs, qe, left);
	int en = RMQ(mid + 1, se, qs, qe, right);

	if (st != -1 && en != -1) {
		if (l[st] < l[en])
		return st;
		return en;
	} else if (st != -1)
		return st;
	else if (en != -1)
		return en;
	return 0;
}

void SegmentTreeConstruction(int ss, int se, int i) {
	if (ss > se)
		return;
	if (ss == se)
	{
		st[i] = ss;
		return;
	}
	int mid = (ss + se) >> 1;

	SegmentTreeConstruction(ss, mid, left);
	SegmentTreeConstruction(mid + 1, se, right);

	if (l[st[left]] < l[st[right]])
		st[i] = st[left];
	else
		st[i] = st[right];
}

int LCA(int x, int y) {
	if (h[x] > h[y])
		swap(x, y);
	return e[RMQ(0, l.size() - 1, h[x], h[y], 0)];
}

int main() {
	int n;
	int m;
	
	ifstream input;
	ofstream output;
	ofstream outputtime;

	for(int k = 1; k <= 20; k++) {
		char buffer[50];
        sprintf(buffer,"in/test%d.in",k);

		char buffer2[50];
        sprintf(buffer2,"timeRMQ/time%d.out",k);


		input.open(buffer);
		output.open("test.out");
		outputtime.open(buffer2);

		input >> n;
		input >> m;
		int n1;
		int n2;
		for(int i = 0 ; i < n-1; i++) {
			input >> n1;
			input >> n2;
			g[n1].push_back(n2);
			g[n2].push_back(n1);
			
		}

		auto start = high_resolution_clock::now();

		level[1] = 1;
		leveling(1);

		dfs(1);

		for (int i = 0; i < e.size(); i++) {
			l.push_back(level[e[i]]);
		}

		for (int i = 0; i <= n; i++) {
			h[i] = -1;
		}
		for (int i = 0; i < e.size(); i++) {
			if (h[e[i]] == -1)
			h[e[i]] = i;
		}

		SegmentTreeConstruction(0, l.size() - 1, 0);

		for(int i = 0 ; i < m; i++) {
			input >> n1;
			input >> n2;
			output << LCA(n1, n2) << endl;
		}

		auto finish = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(finish - start);

		outputtime << duration.count() << " ms" << endl;

		input.close();
		output.close();
		outputtime.close();
	}
	return 0;
}
