#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

class Solution {
public:
	int minSwapsCouples(vector<int>& row) {
		int ans = 0;
		unordered_map<int, int> id;
		for (int i = 0; i < row.size(); i++)
		{
			id[row[i]] = i;
		}
		for (int i = 0; i < row.size(); i += 2)
		{
			int pair = 0;
			if (row[i] % 2 == 0)
			{
				pair = row[i] + 1;
			}
			else
			{
				pair = row[i] - 1;
			}
			if (row[i + 1] == pair) continue;
			id[row[i + 1]] = id[pair];
			swap(row[i + 1], row[id[pair]]);
			ans++;
		}
		return ans;
	}
};;

int main()
{
	int m;
	cin >> m;
	vector<int> row(m);
	for (int i = 0; i < m; i++)
	{
		cin >> row[i];
	}
	cout << Solution().minSwapsCouples(row);
}
