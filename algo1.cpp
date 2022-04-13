#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <fstream>
#include <sstream>

#include <chrono>

using namespace std::chrono;
using namespace std;

void dfs(int u, int p, int **memo, int *lev, int log, vector<int> *g) {
	memo[u][0] = p;
	for (int i = 1; i <= log; i++) {
        memo[u][i] = memo[memo[u][i - 1]][i - 1];
    }
	for (int v : g[u]) {
		if (v != p) {
			lev[v] = lev[u] + 1;
			dfs(v, u, memo, lev, log, g);
		}
	}
}


int lca(int u, int v, int log, int *lev, int **memo) {
	if (lev[u] < lev[v]) {
		swap(u, v);
    }
	for (int i = log; i >= 0; i--) {
		if ((lev[u] - pow(2, i)) >= lev[v]) {
			u = memo[u][i];
        }
    }
	if (u == v) {
		return u;
    }
	for (int i = log; i >= 0; i--) {
		if (memo[u][i] != memo[v][i]) {
			u = memo[u][i];
			v = memo[v][i];
		}
	}
	return memo[u][0];
}

int main() {

	ifstream input;
	ofstream output;
	ofstream outputtime;

	for(int k = 1; k <= 20; k++) {

		char buffer[50];
        sprintf(buffer,"in/test%d.in",k);

		char buffer2[50];
        sprintf(buffer2,"timeBL/time%d.out",k);


		input.open(buffer);
		output.open("test.out");
		outputtime.open(buffer2);


		int n;
		int m;
		input >> n;
		input >> m;
		vector<int> g[n + 1];
		int log = (int)ceil(log2(n));
		int **memo = new int *[n + 1];
		for (int i = 0; i < n + 1; i++) {
			memo[i] = new int[log + 1];
		}
		int *lev = new int[n + 1];
		for (int i = 0; i <= n; i++) {
			memset(memo[i], -1, sizeof memo[i]);
		}
		int n1;
		int n2;
		for(int i = 0 ; i < n-1; i++) {
			input >> n1;
			input >> n2;
			g[n1].push_back(n2);
			g[n2].push_back(n1);
		}

		auto start = high_resolution_clock::now();

		dfs(1, 1, memo, lev, log, g);
		for(int i = 0 ; i < m; i++) {
			input >> n1;
			input >> n2;
			output << lca(n1, n2, log, lev, memo) << endl;
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