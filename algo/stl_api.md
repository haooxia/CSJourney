# STL API

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

## stack

- no begin() & end()
- empty()
- size()
- push()
- pop()
- top()

## queue

- no begin() & end(): 因为其访问受限 FIFO 不支持随机访问
- empty()
- size()
- push()
- pop()
- front(): get the first element of queue
- back()

## vector

algo:

- std::max_element(vec.begin(), vec.end())
  - return: Iterator to the greatest element in the range [first, last).

## pair

- pair(x, y): construct
- p.first: reference to the first element of pair
- p.second: reference
- make_pair(x, y): 创建一个pair