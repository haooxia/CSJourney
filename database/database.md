# Database

## 基础语句

select: 检索
distinct: 检索列内不同值
limit: 检索前几个

```sql
SELECT DISTINCT prod_name 
FROM products
LIMIT 5;
# 单行注释
/* 多行
注释 */
```

order by: 根据某列排序，默认升序
desc: 逆序
where: 过滤数据
between A and B: 介于指定区间
!=, <>: 不等于
and, or, not: 组合逻辑
in ('A', 'B'): 属于，in一般比or快
like: 通配符过滤
%: 任意字符出现任意次数，包括0
_: 任意单个字符
[]: 用字符集匹配指定位置的一个字符

计算字段/函数
Concat(): 拼接
as: alias
trim(): 删除两侧空白格
ltrim(): 删除左侧空格
lower()
substr()
sin()
abs()
sqrt()

聚集函数
avg()
count()
max()
sum()

数据分组：group by必须在where之后 order by之前
group by: 分组
where: 在数据分组之前过滤，过滤掉的行不包括在分组内
having: 在数据分组之后过滤

```sql
SELECT ...      # 要返回的列或表达式
FROM ...        # 被检索数据表
WHERE ...       # 行级过滤
GROUP BY ...    # 分组
HAVING ...      # 组级过滤
ORDER BY ...    # 输出排序顺序
```

