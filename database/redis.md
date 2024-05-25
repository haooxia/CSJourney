# Redis

Redis: Remote dictionary server
是一个开源的基于内存的数据存储系统，可以用作数据库DB，缓存Cache和消息队列MQ等各种场景。是最热门NoSQL之一。

> 磁盘IO太慢了，so Redis. 性能极高。

支持数据类型
![picture 0](../images/9a31812320d0a698e62808fb73c322c2f19392807f90d5711a37fe97ffb7a776.png)  

使用途径

* CLI: Command Line Interface
* API: Application Programming Interface
* GUI: Graphical User Interface

## install

Linux
`apt install redis`
`redis-server`: open
`redis-cli`: open redis client

windows
wsl, or Docker, or msi file

## use

string
默认使用string存储数据，区分大小写

```sql
SET key value
GET key
DEL key
EXISTS key
keys *: show all keys
flushall: del all keys 
TTL key: time to live 过期时间; -1未设置 -2已过期
EXPIRE key seconds: set ttl
```

List：有序？

```sql
lpush key value [value ...]: 头插法; 
rpush key value [value ...]: 尾插法;  
lpop key; ()
rpop
lrange key start stop: show; (list)
llen key: length of len
ltrim letter start stop: 裁剪两侧
# 可以配合实现一个消息队列
```

Set: 无序、不重复、集合

```sql
sadd key member [member ...]
smembers key: show
sismember key member: is member in set
srem key member: remove
# 交并差
sinter
sunion
sdif
```

SortedSet: 有序（通过给每个元素关联一个socre实现，有点丑系那个）、不重复集合

```sql
zadd key score member [score member]
zrange key start stop: show member; (with scores)
zscore key member: show score of member
zrank key member: show rank of member
zrevrank: reverse rank
zrem
```

Hash: 键值对的集合，这才是真map，要sortedset还有何用(中介？)

```sql
hash key file value
hget key
hdel
hexists
```

发布订阅
缺点：无法记录历史信息 无法持久化

```sql
publish(这tm不是服务端)
subscribe(这tm不就是客户端，可多个)
```
