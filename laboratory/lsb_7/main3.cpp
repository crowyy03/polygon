#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int maxSlicesLinear(vector<int>& slices, int n) {
    int len = slices.size();
    vector<vector<int>> dp(len + 1, vector<int>(n + 1, 0));
    
    for (int i = 1; i <= len; ++i) {
        for (int j = 1; j <= n; ++j) {
            dp[i][j] = max(dp[i - 1][j], (i >= 2 ? dp[i - 2][j - 1] : 0) + slices[i - 1]);
        }
    }
    return dp[len][n];
}


int maxSizeSlices(vector<int>& slices) {
    int n = slices.size() / 3;
    vector<int> slices1(slices.begin(), slices.end() - 1); 
    vector<int> slices2(slices.begin() + 1, slices.end()); 

    return max(maxSlicesLinear(slices1, n), maxSlicesLinear(slices2, n));
}
int main() {
    vector<int> slices = {8, 9, 8, 6, 1, 1};
    cout << "Maximum sum of slices you can pick: " << maxSizeSlices(slices) << endl;
    return 0;
}
