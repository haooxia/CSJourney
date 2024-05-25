# Java API

==How to choose?==

* 判断是单列还是键值对
  * 单列 -> Collection接口实现类
    * 允许重复 -> List接口实现类
      * 增删多 -> LinkedList (双向链表)
      * 改查多 -> ArrayList (可变数组)
    * 不允许重复 -> Set接口实现类
      * 无序数据 -> HashSet (使用HashMap维护一个哈希表(数组+链表/红黑树))
      * 有序数据 -> TreeSet
      * 插入取出顺序一致 -> LinkedHashSet (数组+双向链表)
  * 键值对 -> Map接口实现子类
    * 无序key -> HashMap
    * 有序key -> TreeMap
    * key插入取出顺序一致 -> LinkedHashMap
    * 读取文件 -> Properties

## ACM template

```java
import java.util.Scanner;
public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        while (scanner.hasNext()) {
            int a = scanner.nextInt();
            int b = scanner.nextInt();
            if (a == 0 && b == 0) break;
            System.out.println(a + b);
        }
    }
}
```

```cpp
#include <iostream>
using namespace std;
int main() {
    int a, b;
    while (cin >> a >> b) {
        if (a == 0 && b == 0) break;
        cout << a + b << endl;
    }
}
```

## HashSet / unordered_set

哈希表用来快速判断(O(1))一个元素是否出现在集合中

* `Set<String> set = new HashSet<>();`
  * 通常使用接口定义，而非`HashSet<String> set = new HashSet<>()`
* 下面的方法都是实现的Collection Interface
* `set.add("element")`
* `set.contains("element")`: 对应cpp中find
  * `set.find("element") != set.end()`
* `set.size()`
* `set.isEmpty()`
* `set.remove("element")`
* `set.clear()`

```java
// traverse
for (String ele : set) {
    sout(ele);
}
```

## HashMap / unordered_map

* `Map<String, Integer> map = new HashMap<>();`
* `map.put("key", 1)`
* `map.get("key")`
* `map.getOrDefault(key, 0): 获取key的value，如果不存在返回0`
* `map.containsKey("key")`
* `map.remove("key")`
* `map.size()`
* `map.isEmpty()`
* `map.containsValue(1)`
* `map.clear()`
* `Set<String> keys = map.keySet()`
* `Collection<Integer> values = map.values()`
* `Set<Map.Entry<String, Integer>> entries = map.entrySet()`

```java
// traverse
for (Map.Entry<String, Integer> entry : map.entrySet()) {
    String key = entry.getKey();
    String value = entry.getValue();
}
```

## String

* `str.length()`
* `str.charAt(idx)`
* `str.toCharArray()`