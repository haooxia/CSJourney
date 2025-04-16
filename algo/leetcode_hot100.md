# Leetcode hot 100

> 总结leetcode-hot-100，以及常见面试考题

- [Leetcode hot 100](#leetcode-hot-100)
  - [哈希表](#哈希表)
    - [1. 两数之和](#1-两数之和)
    - [2. 字母异位词分组](#2-字母异位词分组)
    - [3. 最长连续序列](#3-最长连续序列)
  - [双指针](#双指针)
    - [4. 移动零](#4-移动零)
    - [5. 盛最多水的容器](#5-盛最多水的容器)
    - [6. 三数之和](#6-三数之和)
  - [滑动窗口](#滑动窗口)
    - [7. 无重复字符的最长子串](#7-无重复字符的最长子串)
    - [8. 找到字符串中所有的字母异位词 ☆](#8-找到字符串中所有的字母异位词-)
  - [普通数组](#普通数组)
    - [9. 最大子数组和](#9-最大子数组和)
    - [10. 合并区间 ☆](#10-合并区间-)
    - [11. 轮转数组](#11-轮转数组)
    - [12. 除自身以外数组的乘积](#12-除自身以外数组的乘积)
  - [链表](#链表)
    - [13. 相交链表](#13-相交链表)
    - [16. 环形链表](#16-环形链表)
    - [17. 环形链表 II](#17-环形链表-ii)
    - [18. 合并两个有序链表](#18-合并两个有序链表)
    - [19. 两数相加](#19-两数相加)
    - [20. 删除链表倒数第k个节点](#20-删除链表倒数第k个节点)
    - [22. 随机链表的复制](#22-随机链表的复制)
    - [23. 排序链表](#23-排序链表)
    - [14. 反转链表](#14-反转链表)
    - [15. 回文链表](#15-回文链表)
    - [21. 两两交换链表节点](#21-两两交换链表节点)
  - [二叉树](#二叉树)
    - [1. 二叉树前序遍历](#1-二叉树前序遍历)
    - [2. 二叉树后序遍历](#2-二叉树后序遍历)
    - [3. 二叉树中序遍历](#3-二叉树中序遍历)
    - [4. 翻转二叉树](#4-翻转二叉树)
    - [5. 对称二叉树](#5-对称二叉树)
    - [6. 二叉树的最大深度](#6-二叉树的最大深度)
    - [7. 二叉树的最小深度](#7-二叉树的最小深度)
    - [8. 二叉树的直径](#8-二叉树的直径)
    - [9. 二叉树的最近公共祖先](#9-二叉树的最近公共祖先)
    - [10. 二叉树的层序遍历](#10-二叉树的层序遍历)
    - [11. 验证二叉搜索树](#11-验证二叉搜索树)
    - [12. 二叉搜索树中第K小的元素](#12-二叉搜索树中第k小的元素)
    - [13. 二叉树展开为链表](#13-二叉树展开为链表)
    - [14. 将有序数组转换为平衡二叉搜索树 (构建二叉树)](#14-将有序数组转换为平衡二叉搜索树-构建二叉树)

TODO:

* 接雨水
* 和为K的子数组
* LRU

## 哈希表

### 1. 两数之和

题目：给定数组nums，目标值tar，找到数组中和为tar的两个数的下标。
思路：搞一个map记录数组内的值和索引，判断是否记过了tar-nums[i]即可

```java
public int[] twoSum(int[] nums, int target) {
    Map<Integer, Integer> map = new HashMap<>();
    for (int i=0; i<nums.length; ++i) {
        if (map.containsKey(target - nums[i]))
            return new int[]{i, map.get(target-nums[i])};
        map.put(nums[i], i);
    }
    return new int[2];
}
```

### 2. 字母异位词分组

题目：给你一个`string[]`，你把字母异位词分组，返回一个`List<List<String>>`。
思路：把每个字符串排序，作为key，value是原字符串。

```java
public List<List<String>> groupAnagrams(String[] strs) {
    Map<String, List<String>> map = new HashMap<>();
    for (String str:strs) {
        char[] arr = str.toCharArray();
        Arrays.sort(arr);
        String key = new String(arr);
        if (map.get(key) == null) map.put(key, new ArrayList<>());
        map.get(key).add(str);
    }
    List<List<String>> result = new ArrayList<>();
    for (Map.Entry<String, List<String>> entry : map.entrySet())
        result.add(entry.getValue());
    return result;
}
```

### 3. 最长连续序列

题目：给一个无序数组，找出数字连续的最长序列的长度。O(n)时间复杂度。
思路：用一个set记录元素，然后循环，如果set有n-1就跳过，因为我们要从连续的最小值开始找。

```java
public int longestConsecutive(int[] nums) {
    Set<Integer> set = new HashSet<>();
    for (int num:nums) set.add(num);
    int result = 0;
    for (int num:set) {
        if (set.contains(num-1)) continue;
        int sum = 1;
        while (set.contains(num+1)) {
            num++;
            sum++;
        }
        result = Math.max(result, sum);
    }
    return result;
}
```

## 双指针

### 4. 移动零

题目：将数组中所有的0移动到数组末尾，保持非零元素的相对顺序。
思路：双指针，一个指向0，一个指向非0，交换即可。

```java
public void moveZeroes(int[] nums) {
    int i=0, j=0;
    while (j<nums.length) {
        if (nums[j] != 0) {
            int tmp = nums[i];
            nums[i] = nums[j];
            nums[j] = tmp;
            i++;
        }
        j++;
    }
}
```

### 5. 盛最多水的容器

题目：数组每个元素表示高度，找出两个柱子，盛最多的水。
思路：左右指针向中间逼近，然后贪心，每次先挪矮的即可。

```java
public int maxArea(int[] height) {
    int n = height.length;
    int left = 0, right = n - 1;
    int result = 0;
    while (left < right) {
        int h = Math.min(height[left], height[right]);
        result = Math.max(result, (right - left) * h);
        if (height[left] <= height[right]) left++;
        else right--;
    }
    return result;
}
```

### 6. 三数之和

题目：给一个数组，找到和为0的ijk三元组，返回这三个数（不能重复）。
思路：先排序，第一个指针遍历，然后内层左右双指针，和<0左指针右移，否则右指针左移。（核心在于去重）

```java
public List<List<Integer>> threeSum(int[] nums) {
    Arrays.sort(nums);
    int n = nums.length;
    List<List<Integer>> result = new ArrayList<>();
    for (int i = 0; i < n; ++i) {
        // 去重a
        if (i-1>=0 && nums[i] == nums[i-1]) continue;
        int left = i + 1, right = n - 1;
        while (left < right) {
            int sum = nums[left] + nums[right] + nums[i];
            if (sum == 0) {
                result.add(Arrays.asList(nums[i], nums[left], nums[right]));
                // 去重bc
                while (left+1 < n && nums[left+1] == nums[left]) left++;
                while (right - 1 > 0 && nums[right-1] == nums[right]) right--;
                left++; right--;
            } else if (sum < 0) left++;
            else right--;
        }
    }
    return result;
}
```

<!-- ## 单调栈

### 接雨水
 -->

## 滑动窗口

### 7. 无重复字符的最长子串

题目：返回str中不含重复字符的最长子串的长度。
思路：滑动窗口，使用set记录滑动窗口内的字符，如果right已经出现在set中，那left++，同时删掉set中对应left值。

```java
public int lengthOfLongestSubstring(String s) {
    Set<Character> set = new HashSet<>();
    int left = 0, right = 0;
    int result = 0;
    while (right < s.length()) {
        while (set.contains(s.charAt(right))) {
            set.remove(s.charAt(left));
            left++;
        }
        set.add(s.charAt(right));
        result = Math.max(result, right - left + 1);
        right++;
    }
    return result;
}
```

### 8. 找到字符串中所有的字母异位词 ☆

题目：给定两个字符串s和p，找到s中所有p的异位词子串，返回这些子串的开始索引。
思想：先存子串p到map，然后滑动窗口，map[right]如果=0，说明p没这个字符，map[left]++（++是核心，慢慢体会吧，其实和上面非常一致
> 如果map[right]=0，说明right字符不在p中，此时需要移动left，并且要先恢复被移除的字符，直到窗口可能再次有效
> 如果map[right]>0，说明right字符在p中，此时窗口可能有效，也就需要right继续走，**将新字符加入窗口（加入窗口等价于map技术-1）**

```java
public List<Integer> findAnagrams(String s, String p) {
    int[] map = new int[26];
    for (char c:p.toCharArray()) map[c-'a']++;
    int left = 0, right = 0;
    List<Integer> result = new ArrayList<>();
    while (right < s.length()) {
        while (map[s.charAt(right)-'a'] == 0) {
            map[s.charAt(left) - 'a']++;
            left++;
        }
        map[s.charAt(right)-'a']--;
        if (right-left == p.length() - 1) result.add(left);
        right++;
    }
    return result;
}
```

## 普通数组

### 9. 最大子数组和

题目：找到数组中和最大的子数组(连续)，返回该和。
思路：贪心，如果和<0，立马放弃。

```java
public int maxSubArray(int[] nums) {
    int sum = 0;
    int result = Integer.MIN_VALUE;
    for (int i=0; i<nums.length; ++i) {
        sum += nums[i];
        result = Math.max(result, sum);
        if (sum < 0) sum = 0;
    }
    return result;
}
```

### 10. 合并区间 ☆

题目：给定区间数组(二维数组)，合并重叠区间。返回合并后的区间数组。
思路：按照区间左端点排序，然后遍历，如果当前区间的右侧<=下一个区间的左侧，说明可以合并。
> 但是需要注意是add还是get修改右端点，而且一般还要取出来合并后的最后一个区间。代码再看看吧

```java
public int[][] merge(int[][] intervals) {
    List<int[]> result = new ArrayList<>();
    Arrays.sort(intervals, (a, b) -> {
        return a[0] - b[0];
    });
    for (int i=0; i<intervals.length; ++i) {
        int left = intervals[i][0], right = intervals[i][1];
        int k = result.size() - 1;
        if (result.size() == 0 || result.get(k)[1] < intervals[i][0])
            result.add(new int[]{left, right});
        else
            result.get(k)[1] = Math.max(right, result.get(k)[1]);
    }
    int[][] res = new int[result.size()][2];
    for (int i=0; i<result.size(); ++i) res[i] = result.get(i);
    return res;
}
```

### 11. 轮转数组

题目：将数组中元素向右轮转k个位置。
思路：先reverse前n-k,再reverse后k，再reverse所有

```java
public void reverse(int[] nums, int left, int right) { // [l, r]
    while (left <= right) {
        int temp = nums[left];
        nums[left] = nums[right];
        nums[right] = temp;
        ++left; --right;
    }
}
public void rotate(int[] nums, int k) {
    int n = nums.length;
    k = k % n; // 轮转多于n 相当于原样嘛
    reverse(nums, 0, n-k-1);
    reverse(nums, n-k, n-1);
    reverse(nums, 0, n-1);
}
```

### 12. 除自身以外数组的乘积

题目：返回新数组，每个元素值为除自身以外的乘积。
思路：先用result从左往右累积，再从右往左累积(用一个right变量即可)。

```java
public int[] productExceptSelf(int[] nums) {
    int n = nums.length;
    int[] result = new int[n];
    // 先用result从左往右累积
    result[0] = 1;
    for (int i=1; i<n; ++i) 
        result[i] = result[i-1] * nums[i-1]; 
    // right从右往左累积
    int rightVal = 1;
    for (int i=n-2; i>=0; --i) {
        rightVal = rightVal * nums[i+1];
        result[i] = rightVal * result[i];
    }
    return result;
}
```

## 链表

### 13. 相交链表

题目：找到两个链表的交点。给定两个链表的头结点headA和headB。
思路：先用set遍历一个即可。或者双指针基于长度

```java
public ListNode getIntersectionNode(ListNode headA, ListNode headB) {
    Set<ListNode> set = new HashSet<>();
    for (ListNode p = headA; p!=null; p=p.next) set.add(p);
    for (ListNode p = headB; p!=null; p=p.next) {
        if (set.contains(p)) return p;
    }
    return null;
}
```

### 16. 环形链表

题目：判断一个链表是否有环。给定head。
思路：1. 用set存；2. 快慢指针，一个走两步一个走一步，肯定会相遇（可以数学证明）。

```java
public boolean hasCycle(ListNode head) {
    ListNode slow = head, fast = head;
    while (fast != null && fast.next != null) {
        slow = slow.next;
        fast = fast.next.next;
        if (slow == fast) return true;
    }
    return false;
}
```

### 17. 环形链表 II

题目：返回环形链表的入口节点。没环的话返回null。
思路：1. 用set；2. 快慢指针，相遇后，一个从head，一个从fast出发，再次相遇即为入口。

```java
public ListNode detectCycle(ListNode head) {
    ListNode slow = head, fast = head;
    while (fast != null && fast.next != null) {
        slow = slow.next;
        fast = fast.next.next;
        if (slow == fast) {
            ListNode p = head, q = fast; // q不必要了;用slow/fast均可
            while (p != q) {
                p = p.next;
                q = q.next;
            }
            return p;
        }
    }
    return null;
}
```

### 18. 合并两个有序链表

题目：合并两个有序链表。给定head1和head2。
思想：搞个dummy，然后双指针比大小接到dummy后面即可。

```java
public ListNode mergeTwoLists(ListNode list1, ListNode list2) {
    ListNode dummyNode = new ListNode(0);
    ListNode p = list1, q = list2;
    ListNode pp = dummyNode;
    while (p != null && q != null) {
        if (p.val <= q.val) {
            pp.next = p;
            p = p.next;
        } else {
            pp.next = q;
            q = q.next;
        }
        pp = pp.next;
    }
    if (p == null) pp.next = q;
    if (q == null) pp.next = p;
    return dummyNode.next;
}
```

### 19. 两数相加

题目：将两个**逆序**的链表相加，每个节点是一个数字，返回一个新链表。eg 243 + 564 -> 708。（即逆序相加，向后进位）
思路：dummy，然后双指针，加个carry变量即可。

```java
public ListNode addTwoNumbers(ListNode l1, ListNode l2) {
    ListNode dummyNode = new ListNode(0);
    ListNode p = l1, q = l2;
    ListNode pp = dummyNode;
    int carry = 0;
    while (p != null || q != null) {
        int sum = carry + (p==null?0:p.val) + (q==null?0:q.val);
        int val = sum % 10;
        carry = sum / 10;
        pp.next = new ListNode(val);
        pp = pp.next;
        if (p!=null) p=p.next;
        if (q!=null) q=q.next;
    }
    if (carry == 1) pp.next = new ListNode(1);
    return dummyNode.next;
}
```

### 20. 删除链表倒数第k个节点

题目：删除链表倒数第k个节点。
思路：快慢指针，一个先走k步，然后一块走。

```java
public ListNode removeNthFromEnd(ListNode head, int n) {
    ListNode dummyNode = new ListNode(0, head);
    ListNode slow = dummyNode, fast = dummyNode;
    for (int i=0; i<n; ++i) fast = fast.next;
    while (fast.next != null) {
        slow = slow.next;
        fast = fast.next;
    }
    slow.next = slow.next.next;
    return dummyNode.next;
}
```

### 22. 随机链表的复制

题目：给定一个链表，节点包括next指针和random指针，你需要深拷贝得到一个全新的val和指针一样的链表。
思路：用map存源节点和新节点的映射（在复制next的时候），然后加上random指针。

```java
public Node copyRandomList(Node head) {
    HashMap<Node, Node> map = new HashMap<>();
    Node dummyNode = new Node(0);
    Node p = head;
    Node pp = dummyNode;
    while (p != null) {
        Node newNode = new Node(p.val);
        pp.next = newNode;
        map.put(p, newNode);
        p = p.next;
        pp = pp.next;
    }
    for (Node oldNode : map.keySet())
        map.get(oldNode).random = map.get(oldNode.random); // 新节点的ran指向old节点.ran的对应新节点
    return dummyNode.next;
}
```

### 23. 排序链表

题目：排序无序链表。
思路1(O(n)空间)：存到ArrayList，然后`Collections.sort(list)`，然后存回链表。
思路2(O(1)空间)：归并排序，找到中点，然后递归排序，最后合并。（十分之优雅）

```java
public ListNode sortList(ListNode head) {
    if (head == null || head.next == null) return head;
    ListNode fast = head.next, slow = head;
    while (fast != null && fast.next != null) {
        slow = slow.next;
        fast = fast.next.next;
    }
    ListNode tmp = slow.next;
    // slow.next = null;
    ListNode left = sortList(head);
    ListNode right = sortList(tmp);
    // 上面是按照中点分割，下面是合并
    ListNode t = new ListNode(0);
    ListNode res = t;
    while (left != null && right != null) {
        if (left.val < right.val) {
            t.next = left;
            left = left.next;
        } else {
            t.next = right;
            right = right.next;
        }
        t = t.next;
    }
    t.next = left != null ? left : right;
    return res.next;
}
```

### 14. 反转链表

题目：反转一个单链表。给定头结点head。
思路：双指针：cur，pre，tmp指针，无需dummy。

```java
public ListNode reverseList(ListNode head) {
    if (head == null || head.next == null) return head;
    ListNode cur = null;
    ListNode pre = head;
    while (pre != null) {
        ListNode tmp = pre.next;
        pre.next = cur;
        cur = pre;
        pre = tmp;
    }
    return cur;
}
```

### 15. 回文链表

题目：判断一个链表是否是回文链表。
思路：找到中点，反转后半部分，然后双指针比较。（当然你可以先用list存储）

```java
public boolean isPalindrome(ListNode head) {
    int n = 0;
    for (ListNode p=head; p!=null; p=p.next) n++;
    ListNode p = head;
    for (int i=0; i<(n+1)/2; ++i) p=p.next;
    ListNode cur = null, pre = p;
    while (pre != null) {
        ListNode tmp = pre.next;
        pre.next = cur;
        cur = pre;
        pre = tmp;
    }
    ListNode p1 = head, p2 = cur;
    while (p2 != null) {
        if (p1.val != p2.val) return false;
        p1 = p1.next;
        p2 = p2.next;
    }
    return true;
}
```

### 21. 两两交换链表节点

题目：两两交换链表节点。
思路：dummy，依然是双指针(cur, pre, tmp)。method_image_link: [link](https://www.notion.so/haooxia/1453e076b37f8191841aee7a29a9158e?pvs=4#19e3e076b37f805fab93ecb3560221f5)

```java
public ListNode swapPairs(ListNode head) {
    if (head == null || head.next == null) return head;
    ListNode dummyNode = new ListNode(0, head);
    ListNode cur = dummyNode, pre = head;
    while (pre != null && pre.next != null) { // 后面有俩元素才考虑交换
        ListNode tmp= pre.next;
        cur.next = tmp;
        pre.next = tmp.next;
        tmp.next = pre;
        // change the position
        cur = pre;
        pre = pre.next;
    }
    return dummyNode.next;
}
```

## 二叉树

### 1. 二叉树前序遍历

思路1：递归法

```java
List<Integer> result = new ArrayList<>();
public List<Integer> preorderTraversal(TreeNode root) {
    if (root == null) return result;
    result.add(root.val);
    preorderTraversal(root.left);
    preorderTraversal(root.right);
    return result;
}
```

思路2：迭代法：借助stack，不断pop后，先入右孩子

```java
public List<Integer> preorderTraversal(TreeNode root) {
    List<Integer> result = new ArrayList<>();
    if (root == null) return result;
    ArrayDeque<TreeNode> stack = new ArrayDeque<>();
    stack.push(root);
    while (!stack.isEmpty()) {
        TreeNode tmp = stack.pop();
        result.add(tmp.val);
        if (tmp.right != null) stack.push(tmp.right);
        if (tmp.left != null) stack.push(tmp.left);
    }
    return result;
}
```

### 2. 二叉树后序遍历

思路：迭代法：借助stack，先左后右，最后reverse即可

```java
public List<Integer> postorderTraversal(TreeNode root) {
    List<Integer> result = new ArrayList<>();
    if (root == null) return result;
    ArrayDeque<TreeNode> stack = new ArrayDeque<>();
    stack.push(root);
    while (!stack.isEmpty()) {
        TreeNode tmp = stack.pop();
        result.add(tmp.val);
        if (tmp.left != null) stack.push(tmp.left);
        if (tmp.right != null) stack.push(tmp.right);
    }
    Collections.reverse(result);
    return result;
}
```

### 3. 二叉树中序遍历

思路：还是借助stack，借助cur指针先一直往左走持续push，走到最左侧，cur=s.pop()，回收，然后cur指针往右走

```java
public List<Integer> inorderTraversal(TreeNode root) {
    List<Integer> result = new ArrayList<>();
    if (root == null) return result;
    Deque<TreeNode> stack = new ArrayDeque<>();
    TreeNode cur = root;
    while (!stack.isEmpty() || cur != null) {
        // 一直往左走持续push，直到最左侧（中左右嘛）
        if (cur != null) {
            stack.push(cur);
            cur = cur.left;
        } else { // 走到了最左侧之后 
            cur = stack.pop();
            result.add(cur.val);
            cur = cur.right;
        }
    }
    return result;
}
```

### 4. 翻转二叉树

题目：将每个节点的左右孩子交换。
思路：先序递归交换即可。（后序应该也没毛病）

```java
public TreeNode invertTree(TreeNode root) {
    if (root == null) return root;
    TreeNode tmp = root.left;
    root.left = root.right;
    root.right = tmp;
    invertTree(root.left);
    invertTree(root.right);
    return root;
}
```

### 5. 对称二叉树

题目：判断一个二叉树是否轴对称。
思路：递归，递归时候传入对称的节点，一个外侧，一个内侧递归。

```java
public boolean recur(TreeNode p, TreeNode q) {
    if (p == null && q == null) return true;
    if (p == null || q == null) return false;
    if (p.val != q.val) return false;
    boolean outer = recur(p.left, q.right);
    if (!outer) return false;
    boolean inner = recur(p.right, q.left);
    if (!inner) return false;
    return true; // return outer && inner
}
public boolean isSymmetric(TreeNode root) {
    if (root == null) return true;
    return recur(root.left, root.right);
}
```

### 6. 二叉树的最大深度

题目：返回最大深度：root到最远叶子的节点数。
思路1：自底向上递归。

```java
public int maxDepth(TreeNode root) {
    if (root == null) return 0;
    int left = maxDepth(root.left);
    int right = maxDepth(root.right);
    return Math.max(left, right) + 1;
}
```

思路2：自顶向下递归；搞一个全局变量记录

```java
int max_depth = 0;
public void dfs(TreeNode root, int depth) {
    if (root == null) return;
    max_depth = Math.max(max_depth, depth);
    dfs(root.left, depth+1);
    dfs(root.right, depth+1);
    // return;
}
public int maxDepth(TreeNode root) {
    dfs(root, 1);
    return max_depth;
}
```

### 7. 二叉树的最小深度

题目：最小深度：root到叶子。
思路1：自顶向下全局变量统计：一定要叶子节点才进行统计计算。

```java
int min_depth = Integer.MAX_VALUE;
public void dfs(TreeNode root, int depth) {
    if (root == null) return;
    if (root.left == null && root.right == null) // 核心
        min_depth = Math.min(min_depth, depth);
    dfs(root.left, depth+1);
    dfs(root.right, depth+1);
}
public int minDepth(TreeNode root) {
    dfs(root, 1);
    return min_depth==Integer.MAX_VALUE?0:min_depth;
}
```

思路2：自底向上(归)：核心依然是叶子节点才进行统计计算。

```java
public int minDepth(TreeNode root) {
    if (root == null) return 0;
    else if (root.left == null && root.right != null) return minDepth(root.right)+1;
    else if (root.left != null && root.right == null) return minDepth(root.left)+1;
    else return Math.min(minDepth(root.left), minDepth(root.right))+1;
}
```

### 8. 二叉树的直径

题目：找到二叉树中最长的路，是否经过root都行。
思路：后序递归，用全局变量记录最大路径，返回时返回长边侧，max(l, r)+1;

```java
int result = 0;
public int traverse(TreeNode root) {
    if (root == null) return 0;
    int left = traverse(root.left);
    int right = traverse(root.right);
    result = Math.max(result, left+right+1); // 记录连接的最长节点
    return Math.max(left, right) + 1; // 返回长的那边
}
public int diameterOfBinaryTree(TreeNode root) {
    if (root == null) return 0;
    traverse(root);
    return result - 1;
}
```

### 9. 二叉树的最近公共祖先

题目：找到二叉树中两个指定节点的最近公共祖先（一个节点也是自身的祖先）
思想：暴力一点：一个函数用于判断root是否是p的祖先，然后后序遍历，如果root同时是p和q的祖先，那就找到了最近的

```java
public boolean isAncestor(TreeNode root, TreeNode p) {
    // 判断root是否是p的祖先（看从root能否遍历到p即可）
    if (root == null) return false;
    if (root == p) return true;
    boolean left = isAncestor(root.left, p);
    boolean right = isAncestor(root.right, p);
    return left || right;
}
public TreeNode lowestCommonAncestor(TreeNode root, TreeNode p, TreeNode q) {
    if (root == null) return null;
    TreeNode left = lowestCommonAncestor(root.left, p, q);
    if (left != null) return left;
    TreeNode right = lowestCommonAncestor(root.right, p, q);
    if (right != null) return right;
    if (isAncestor(root, p) && isAncestor(root, q)) return root;
    return null;
}
```

更优解：[link](https://www.notion.so/haooxia/1453e076b37f81758bfbe25fe197237d?pvs=4#1453e076b37f815388bfc3221d0e6c80)

### 10. 二叉树的层序遍历

题目：一层一层输出二叉树节点。
思想：借助queue，每次把一层的节点offer进去，下一次把一层的pull出来。

```java
public List<List<Integer>> levelOrder(TreeNode root) {
    List<List<Integer>> result = new ArrayList<>();
    if (root == null) return result;
    Deque<TreeNode> queue = new ArrayDeque<>();
    queue.offer(root);
    while (!queue.isEmpty()) {
        int size = queue.size(); // 把一层都出了
        List<Integer> list = new ArrayList<>();
        for (int i=0; i<size; ++i) {
            TreeNode tmp = queue.poll();
            list.add(tmp.val);
            if (tmp.left != null) queue.offer(tmp.left);
            if (tmp.right != null) queue.offer(tmp.right);
        }
        result.add(list);
    }
    return result;
}
```

思路：递归法 [link](https://www.notion.so/haooxia/1453e076b37f81f9ab00ddc718ea142e?pvs=4#1453e076b37f81f0aec4eea84fd6257c)

### 11. 验证二叉搜索树

题目：判断一颗二叉树是否是严格BST（left < root < right）
思路1：BST的中序序列是递增的。（借助空间）

```java
public void traversal(TreeNode root, List<Integer> list) {
    if (root == null) return;
    traversal(root.left, list);
    list.add(root.val);
    traversal(root.right, list);
    return;
}
public boolean isValidBST(TreeNode root) {
    List<Integer> list = new ArrayList<>();
    traversal(root, list);
    for (int i=0; i<list.size()-1; ++i)
        if (list.get(i) >= list.get(i+1)) return false;
    return true;
}
```

思路2：搞一个全局变量，按照中序遍历并进行判断

```java
long val = Long.MIN_VALUE;
public boolean isValid(TreeNode root) {
    if (root == null) return true;
    boolean left = isValid(root.left);
    if (!left) return false;
    if (val >= root.val) return false;
    val = root.val;
    boolean right = isValid(root.right);
    if (!right) return false;
    return left && right;
}
public boolean isValidBST(TreeNode root) {
    return isValid(root);
}
```

### 12. 二叉搜索树中第K小的元素

思路1：遍历得到一个list，返回即可、
思路2：中序遍历，全局变量统计第几个以及返回值

```java
int cnt = 0;
int result = -1;
public int dfs(TreeNode root, int k) {
    if (root == null) return 0;
    dfs(root.left, k);
    cnt++;
    if (cnt == k) {
        result = root.val;
        return result; // 找到了直接返回 不再遍历右子树
    }
    dfs(root.right, k);
    return result;
}
public int kthSmallest(TreeNode root, int k) {
    return dfs(root, k);
}
```

### 13. 二叉树展开为链表

### 14. 将有序数组转换为平衡二叉搜索树 (构建二叉树)

题目：给定升序数组，将其转换为平衡二叉搜索树
思路：就是划分数组递归建树，root, root.left=, root.right=.(固定套路)。因为均匀分割，所以自动就平衡了。

```java
public TreeNode dfs(int[] nums, int left, int right) { // [l, r] 
    if (left > right) return null;
    int mid = left + (right - left) / 2; // mid作为root
    TreeNode root = new TreeNode(nums[mid]);
    root.left = dfs(nums, left, mid-1);
    root.right = dfs(nums, mid+1, right);
    return root;
}
public TreeNode sortedArrayToBST(int[] nums) {
    return dfs(nums, 0, nums.length-1);
}
```