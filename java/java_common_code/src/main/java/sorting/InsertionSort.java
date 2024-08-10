package sorting;

import java.util.Arrays;

public class InsertionSort {
    public static void insertionSort(int[] nums) {
        // note: 插入排序：将无序关键字插入到有序序列中（即把有序序列不断往后移动
        int n = nums.length;
        for (int i = 1; i < n; ++i) { // 遍历无序列表
            int tmp = nums[i];
            int j;
            for (j = i-1; j >= 0 && tmp < nums[j]; --j) // 注意这个判断条件得写这儿, 写下面的话j会被多减
                nums[j+1] = nums[j];
            nums[j+1] = tmp; // j在不断左移 (可以画图看看
        }
    }

    public static void main(String[] args) {
        int[] nums = {10,3,8,5,4};
        insertionSort(nums);
        System.out.println(Arrays.toString(nums));
    }
}
