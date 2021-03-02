#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

class Solution {
public:
	int minFallingPathSum(vector<vector<int>>& arr) {
		vector<vector<int>> dp(arr.size(), vector<int>(arr.size()));
		int min = -1;
		int secMin = -1;
		int minId = 0;
		for (int i = 0; i < arr.size(); i++)
		{
			if (min > arr[0][i] || min == -1)
			{
				secMin = min;
				min = arr[0][i];
				minId = i;
			}
			else if (secMin > arr[0][i] || secMin == -1)
			{
				secMin = arr[0][i];
			}
			dp[0][i] = arr[0][i];
		}
		for (int i = 1; i < arr.size(); i++)
		{
			int lastMin = min;
			int lastSecMin = secMin;
			int lastMinId = minId;
			min = secMin = -1;
			for (int j = 0; j < arr.size(); j++)
			{
				if (j == lastMinId)
				{
					dp[i][j] = arr[i][j] + lastSecMin;
				}
				else
				{
					dp[i][j] = arr[i][j] + lastMin;
				}

				if (min > dp[i][j] || min == -1)
				{
					secMin = min;
					min = dp[i][j];
					minId = j;
				}
				else if (secMin > dp[i][j] || secMin == -1)
				{
					secMin = dp[i][j];
				}
			}
		}
		return min;
	}
};;

int main()
{
	int n;
	cin >> n;
	vector<vector<int>> graph(n);
	for (int i = 0; i < n; i++)
	{
		graph[i] = vector<int>(n);
		for (int j = 0; j < n; j++)
		{
			cin >> graph[i][j];
		}
	}
	cout << Solution().minFallingPathSum(graph);
}