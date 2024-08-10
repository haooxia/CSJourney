package sorting;

import java.util.Arrays;

public class QuickSort {
    public static int partition(int[] nums, int low, int high) { // 左闭右闭
        int pivot = nums[low];
        while (low < high) {
            while (low < high && nums[high] >= pivot) --high; // 一直找到 < pivot的值
            nums[low] = nums[high];
            while (low < high && nums[low] <= pivot) ++low; // 一直找到 > pivot的值
            nums[high] = nums[low];
        }
        nums[low] = pivot;
        return low; // 最终应该返回pivot的位置 而非其值
    }

    public static void quickSort(int[] nums, int low, int high) {
        // note: 每次选一个pivot, 让左侧小于该pivot
        if (low < high) {
            int pivot = partition(nums, low, high);
            quickSort(nums, low, pivot-1);
            quickSort(nums, pivot+1, high);
        }
    }

    public static void main(String[] args) {
        int[] nums = {10,3,8,5,4};
        quickSort(nums, 0, nums.length-1);
        System.out.println(Arrays.toString(nums));
    }
}
