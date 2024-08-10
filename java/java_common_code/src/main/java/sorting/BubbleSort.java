package sorting;

import java.util.Arrays;

public class BubbleSort {
    public static void bubbleSort(int[] nums) {
        // note: 冒泡排序: n趟排序（每趟排序：从前往后，两两比较，若左>右，交换）使得最大的元素沉底。
        int n = nums.length;
        for (int i=0; i<n; ++i) { // 最多做n轮"冒泡操作"
            boolean flag = false; // note 优化：判断本轮是否做了交换操作
            for (int j=0; j<n-1; ++j) { // 注意冒泡的每一轮都需要从头开始 (因为最后才是有序的
                if (nums[j] > nums[j+1]) {
                    int tmp = nums[j];
                    nums[j] = nums[j+1];
                    nums[j+1] = tmp;
                    flag = true;
                }
            }
            if (!flag) return; // 说明本趟并未发生交换
        }
    }

    public static void main(String[] args) {
        int[] nums = {10,3,8,5,4};
        bubbleSort(nums);
        System.out.println(Arrays.toString(nums));
    }
}
