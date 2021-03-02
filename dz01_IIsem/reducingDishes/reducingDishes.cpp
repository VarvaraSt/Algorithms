#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

class Solution {
public:
	int maxSatisfaction(vector<int>& satisfaction) {
		sort(satisfaction.begin(), satisfaction.end());
		int sum = 0;
		int ans = 0;
		for (int i = satisfaction.size() - 1; i >= 0; i--)
		{
			if (satisfaction[i] + sum > 0)
			{
				sum += satisfaction[i];
				ans += sum;
			}
			else
			{
				break;
			}
		}
		return ans;
	}
};;

int main()
{
	int m;
	cin >> m;
	vector<int> satisfaction(m);
	for (int i = 0; i < m; i++)
	{
		cin >> satisfaction[i];
	}
	cout << Solution().maxSatisfaction(satisfaction);
}
