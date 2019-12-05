/*
给定一个字符串，请你找出其中不含有重复字符的 最长子串 的长度。

建立一个 HashMap ，建立每个字符和其最后出现位置之间的映射，然后再定义两个变量 res 和 left ，其中 
res 用来记录最长无重复子串的长度，left 指向该无重复子串左边的起始位置的前一个，一开始由于是前一个，
所以在初始化时就是 -1。

接下来遍历整个字符串，对于每一个遍历到的字符，如果该字符已经在 HashMap 中存在了，并且如果其映射值大
于 left 的话，那么更新 left 为当前映射值，然后映射值更新为当前坐标 i，这样保证了 left 始终为当前边
界的前一个位置，然后计算窗口长度的时候，直接用 i-left 即可，用来更新结果 res 。

*/

#include <iostream>
#include <map>
using namespace std;

int lengthOfLongestSubstring(string s)
{
    int res = 0, left = -1, n = s.size();
    map<int, int> m;
    for (int i = 0; i < n; ++i) {
        if (m.count(s[i]) && m[s[i]] > left) {
            left = m[s[i]];  
        }
        m[s[i]] = i;
        res = max(res, i - left);            
    }
    return res;
}

int main()
{
    string s = "bcddehicgbb";
    int length = lengthOfLongestSubstring(s);
    cout << "Result: " << length << endl;
    return 0;
}