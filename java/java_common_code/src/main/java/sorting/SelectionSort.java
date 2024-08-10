package sorting;

import java.util.Arrays;

public class SelectionSort {
    public static void selectionSort(int[] nums) {
        // note: 选择排序：每次从未排序序列中选择最小元素放到已排序序列末尾即可
        int n = nums.length;
        for (int i=0; i<n-1; ++i) {
            int min = i; // 记录下标而非nums[i] 下标作为key会有更多的信息
            for (int j=i+1; j<n; ++j) // 找到未排序序列的最小元素
                if (nums[j] < nums[min])
                    min = j;
            if (min != i) { // 如果i并非最小
                int tmp = nums[i]; // 和末尾交换即为放至末尾
                nums[i] = nums[min];
                nums[min] = tmp;
            }
        }
    }

    public static void main(String[] args) {
        int[] nums = {10,3,8,5,4};
        selectionSort(nums);
        System.out.println(Arrays.toString(nums));
    }
}
