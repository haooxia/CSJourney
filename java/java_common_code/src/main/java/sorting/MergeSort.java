package sorting;

import java.util.Arrays;

public class MergeSort {
    public static void merge(int[] nums, int left, int mid, int right) {
        // 将两个有序的子数组合并为一个有序数组 (左数组: [left, mid], 右数组[mid+1, right]
        int[] tmp = new int[right - left + 1];
        int i = left, j = mid + 1;
        int k = 0;
        // note 核心思想其实很简单：两个数组都从头遍历，每次拿出小的存入临时数组
        while (i <= mid && j <= right) {
            if (nums[i] <= nums[j]) tmp[k++] = nums[i++];
            else tmp[k++] = nums[j++];
        }
        while (i <= mid) tmp[k++] = nums[i++]; // 说明右数组走完了 把左数组复制过来即可
        while (j <= right) tmp[k++] = nums[j++];
        // note: 把临时数组中的k个数字 复制到原数组对应的区间
        for (int m=0; m<k; ++m) {
            nums[left+m] = tmp[m];
        }
        tmp = null;
    }

    public static void mergeSort(int[] nums, int left, int right) {
        if (left < right) {
            int mid = (left + right) / 2;
            mergeSort(nums, left, mid); // 左闭右闭: 对左半部分进行递归排序
            mergeSort(nums, mid+1, right);
            merge(nums, left, mid, right); // 核心：合并两部分
        }
    }
    public static void main(String[] args) {
        int[] nums = {10, 3, 8, 5, 4, 12, 10};
        System.out.println(Arrays.toString(nums));
        mergeSort(nums, 0, nums.length-1);
        System.out.println(Arrays.toString(nums));
    }
}
