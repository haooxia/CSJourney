package sorting;

import java.util.Arrays;

public class QuickSort2 {
    public static void swap(int[] nums, int i, int j) {
        int tmp = nums[i];
        nums[i] = nums[j];
        nums[j] = tmp;
    }

    public static int partition(int[] nums, int left, int right) {
        int pivot = nums[right];
        int pointer = left; // 指向下一个待放入符合规则的位置（即在pointer之前的元素都是符合规则的
        //int i = left - 1; // 记录<=pivot的最后一个位置
        for (int j=left; j<right; ++j) {
            if (nums[j] <= pivot) {
                //++i; // <=pivot的区域增长
                swap(nums, pointer, j);
                pointer++;
            }
        }

        swap(nums, pointer, right);
        return pointer;
    }
    public static void quickSort(int[] nums, int left, int right) {
        if (left < right) {
            int pi = partition(nums, left, right); // pivot index
            quickSort(nums, left, pi-1);
            quickSort(nums, pi+1, right);
        }
    }

    public static void main(String[] args) {
        int[] nums = {10, 3, 8, 5, 4, 22};
        System.out.println(Arrays.toString(nums));
        quickSort(nums, 0, nums.length-1);
        System.out.println(Arrays.toString(nums));
    }
}
