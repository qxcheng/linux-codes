/*
给定四个包含整数的数组列表 A , B , C , D ,计算有多少个元组 (i, j, k, l) ，使得 
A[i] + B[j] + C[k] + D[l] = 0。

为了使问题简单化，所有的 A, B, C, D 具有相同的长度 N，且 0 ≤ N ≤ 500 。所有整数的范围在 
-2^28 到 2^28- 1 之间，最终结果不会超过 2^31 - 1 。

把 A 和 B 的两两之和都求出来，在哈希表中建立两数之和与其出现次数之间的映射；
遍历 C 和 D 中任意两个数之和，只要看哈希表存不存在这两数之和的相反数就行了。

*/

// 时间复杂度: O(n^2)
// 空间复杂度: O(n^2)
class Solution {
public:
    int fourSumCount(vector<int>& A, vector<int>& B, vector<int>& C, vector<int>& D) {
        unordered_map<int,int> hashtable;
        for(int i = 0 ; i < A.size() ; i ++){
            for(int j = 0 ; j < B.size() ; j ++){
                 hashtable[A[i]+B[j]] += 1;
            }
        }
        int res = 0;
        for(int i = 0 ; i < C.size() ; i ++){
            for(int j = 0 ; j < D.size() ; j ++){
                if(hashtable.find(-C[i]-D[j]) != hashtable.end()){
                    res += hashtable[-C[i]-D[j]];
                }
            }
        }
        return res;
    }
};