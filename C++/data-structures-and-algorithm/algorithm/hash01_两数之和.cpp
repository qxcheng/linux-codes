/*

给定一个整数数组 nums 和一个目标值 target，请你在该数组中找出和为目标值的那两个整数，并返回他们的数组下标。
你可以假设每种输入只会对应一个答案。但是，你不能重复利用这个数组中同样的元素。

示例:
给定 nums = [2, 7, 11, 15], target = 9
因为 nums[0] + nums[1] = 2 + 7 = 9
所以返回 [0, 1]

解析:
首先设置一个 map 容器 record 用来记录元素的值与索引，然后遍历数组 nums 。
每次遍历时使用临时变量 complement 用来保存目标值与当前值的差值
在此次遍历中查找 record ，查看是否有与 complement 一致的值，如果查找成功则返回查找值的索引值与当前变量的值i
如果未找到，则在 record 保存该元素与索引值 i

*/

#include <iostream>
#include <vector>
#include <map>
using namespace std;

// 1. Two Sum
// 时间复杂度：O(n)
// 空间复杂度：O(n)
vector<int> twoSum(vector<int>& nums, int target) 
{
    map<int,int> record;
    for(int i = 0 ; i < nums.size() ; i++){
        int complement = target - nums[i];
        if(record.find(complement) != record.end()){
            int res[] = {i, record[complement]};
            return vector<int>(res, res + 2);
        }
        record[nums[i]] = i;
    }
}

int main()
{
    vector<int> nums = vector<int>{2, 7, 11, 15};
    vector<int> result = twoSum(nums, 9);
    cout << "Result: " << result[0] << " " << result[1] << endl; 
    
    return 0;
}



