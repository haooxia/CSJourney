# STL API

## vector

- begin() & end()
- front() & back(): reference to the first and last element
- empty()
- size()
- push_back()
- pop_back()
- emplace_back()
- at(size_t pos)
- insert(iter pos, val)
- erase(iter pos)
- erase(iter first, iter last)

algo:

- sort(vec.begin(), vec.end(), comp);
  - comp默认为升序排列
  - 降序方法：
    - `sort(vec.begin(), vec.end(), std::greater<int>());`
      - 这个greater函数对象返回bool：表示第一个参数是否严格大于第二个参数
    - `sort(vec.begin(), vec.end(), [](int a, int b) {return a > b});`
      - 当然你写个comp实名函数也可以啦
      - **comp的返回值表示是否将第一个参数放在第二个参数之前**。
- reverse(vec.begin(), vec.end());
- find(vec.begin(), vec.end(), value); // return position_iter
- count(vec.begin(), vec.end(), value); // count the number `value`
- max_element(vec.begin(), vec.end()); return the position_iter
- accumulate(vec.begin(), vec.end(), 0); // he last argument's type is used not just for the initial value, but for the type of the result as well. e.g., `0`, `0.0`; `#include <numeric>`

## list

- front() & back(): reference of elem
- empty()
- size()
- push_back()
- pop_back()
- push_front()
- pop_front()
- insert(iter pos_iter, val)

## stack

- no begin() & end()
- empty()
- size()
- push()
- pop()
- top()

## queue

- no begin() & end(): 因为其访问受限 FIFO 不支持随机访问
- front() & back(): reference to elem
- empty()
- size()
- push()
- pop()

## pair

- pair(x, y): construct
- p.**first**: reference to the first element of pair
- p.second: reference
- make_pair(x, y): 创建一个pair

对一个`vector<pair<int,int>>`根据pair的second元素倒序排序

`sort(vec.begin(), vec.end(), [](const pair<int,int>& a, const pair<int,int>&b){return a.second > b.second;});`

## unordered_set

- begin(), end(): return iterator
- empty()
- size()
- insert(key_name)
- erase(key_name)
- clear(): clean all values
- find(key_name)

## unordered_map

- begin()
- end()
- empty()
- size()
- insert({key_name, value_name})
  - or insert(std::make_pair(key_name, value_name)); // the types can be deduced automatically
- erase(key_name)
- clear()
- find(key_name)

## priority_queue

```cpp
std::priority_queue<int, vector<int>, std::less<int>> pq;
// 默认容器类型是vector，排序类型是std::less降序排列：大顶堆: 最大元素显示为top()
std::priority_queue<int, vector<int>, std::greater<int>> pq;
// 小顶堆：最小元素显示为top()
```

> 这里的greater似乎和`sort(vec.begin(), vec.end(), std::greater<int>());`有些不同，我似乎需要去学一下所谓的谓词`greater`.

接口似乎跟stack一模一样

- empty()
- size()
- top()
- push()
- pop()