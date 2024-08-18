# Database

- [Database](#database)
  - [MySQL](#mysql)
    - [Begin](#begin)
    - [安装](#安装)
      - [导入导出](#导入导出)
    - [SQL](#sql)
      - [DDL](#ddl)
      - [DML](#dml)
      - [DQL](#dql)
      - [DCL](#dcl)
    - [函数](#函数)
    - [多表设计](#多表设计)
      - [一对多](#一对多)
      - [一对一](#一对一)
      - [多对多](#多对多)
    - [多表查询](#多表查询)
      - [内连接](#内连接)
      - [外连接](#外连接)
      - [子查询/嵌套查询](#子查询嵌套查询)
    - [事务](#事务)
    - [索引](#索引)
  - [进阶](#进阶)
    - [存储引擎](#存储引擎)
  - [一些指令](#一些指令)


## MySQL

![picture 1](../images/60ed56107759295fa2d2ccc198cc609f2a205459f89f7c0288a5dc570af1473e.png)  

![picture 0](../images/666b21c4693b11f56e68ed0f112fe3ce7aafb9178142c2e6258ded70ef9e502e.png)

![picture 3](../images/0c802f215506964e398299038b2438ea40a4cdc650304d0be3e488260a345ea9.png)  

### Begin

关系型数据库管理系统(RDBMS)：使用二维表存储数据，支持表与表之间的关系。比如MySQL, ORACLE, PostgreSQL
SQL (Structured Query Language)是RDBMS的语言。
非关系型数据库管理系统(NON-RDBMS)：使用非结构化或面向文档的数据模型。比如Redis, mongoDB, neo4j, cassandra.

* DBMS下有多个数据库，数据库下对应对张表
* 表格的一行在java中使用一个对象表示

SQL分类:

* DDL 数据定义语言(数据库设计) definition: CREATE, DROP, ALTER, TRUNCATE
* DML 数据操作语言(增删改) manipulation: INSERT, UPDATE, DELETE, CALL
* DQL 数据查询语言 query: SELECT
* DCL 数据控制语言 control: GRANT, REVOKE

注意：

* 分号结尾
* 关键词建议大写，但都行
* 单行注释： `--` or `#`
* 多行注释：`/* test */`

### 安装

**Linux**
`sudo apt install mysql-server`: 安装后自动启动
`mysql --version`
`systemctl status mysql`: 查看mysql服务状态，如果没启动：
`systemctl start mysql`

linux系统安装完成之后默认有密码（手动修改）
`sudo cat /etc/mysql/debian.cnf`: 查看密码
`mysql -u debian-sys-maint -p`: 使用默认用户名和密码登录
修改密码
version > 5.7.9

```mysql
alter user 'root'@'localhost' identified with mysql_native_password by 'xiahao';
flush privileges;
# 'root'@'localhost'
```

> 报错参考 [url](https://blog.csdn.net/q258523454/article/details/84555847)

version < 5.7.9

```mysql
update user set password=Password("xiahao") where user='root';
update user set authentication_string=PASSWORD("xiahao") where user='root';
flush privileges;
```

> windows安装时需要手动设置密码

`show variable like '%port%';`: 查看端口
`use mysql; select user, host from user;`: 查看用户访问权限，如果root的host是localhost，表示这个用户只能在本地访问，使用
`update user set host='%' where user='root';`: 改为%，就可以在任何情况下访问了（不建议生产环境下配置，有一定风险）

默认情况下mysql只监听本地ip地址，从外部无法连接到mysql服务，修改如下：

```bash
sudo vim /etc/mysql/mysql.conf.d/mysqld.cnf
# 修改：
bind-address            = 127.0.0.1 # 默认只监听本机IP地址
mysqlx-bind-address     = 127.0.0.1
# 为：
bind-address            = 0.0.0.0 # 修改为监听所有的ip地址
mysqlx-bind-address     = 0.0.0.0
```

windows修改root密码

```bash
use mysql;  
update user set authentication_string=password('xiahao') where user='root' and Host='localhost';
```

windows使用
**mysql是一个数据服务器，监听3306端口；命令行终端或图形化工具(eg navicat)或java程序是client，通过网络连接到服务器.**

* 如果没写-h默认是本机localhost；如果没写-P默认是3306；
* 为了避免被攻击，一般会修改端口号

```bash
net start mysql # 启动mysql服务器后client才可以连接
net stop mysql  # close 
mysql -u root -p
# 全称
mysql -h host/IP -P port -u user -ppassword # -p后无空格 
# example
mysql -h 127.0.0.1 -P 3306 -u root -pxiahao
```

#### 导入导出

```sql
mysqldump -u root -p game > game.sql # 将数据库导出到文件中，文件内是一些sql创建插入语句
mysql -u root -p game < game.sql # 从文件导入数据
```

### SQL

#### DDL

```sql
# 数据库操作
show databases;
create database/schema game;   # add
drop database game;     # delete
use game;               # change database
```

```sql
# 表操作
show tables;            # show all tables of current database
desc tableName;         # 查看指定表结构：字段、类型等
show create table tableName;    # 查看指定表的建表语句
# 创建表 (重点)
create table player (
    id INT DEFAULT 1 comment '编号', # 常用约束包括default, null, not null, unique等, 以及主键约束(每个表只能有一个主键，主键不为空)，外键约束(一个表的外键必须是另一个表的主键)等
    name VARCHAR(100) comment '姓名', # 变长字符串 size=100
    level INT,
    exp INT,
    gold DECIMAL(10, 2) # 十进制数值数据
) comment '用户表';
drop table player; # 删除table
```

**用于限制表中字段的规则**
![picture 2](../images/75f2b86768af2064fb65ac4924d12b2772b83fce7204dd733eb5d254d374f7bb.png)  

数据类型：

* **数值**类型: tinyint(1B), smallint(2B), mediumint(3B), int(integer)(4B), bigint(8B), float(4B), double(8B), decimal(字符串处理小数)
  * 默认是有符号类型，可以通过unsigned指定为无符号；eg int unsigned
  * double(5,2)表示2位小数，总共5位数
* **字符串**类型: 定长字符串char(0-255B), 变长varchar(0-65535B)(指定长度为最大占用长度), tinyblob(0-255B)(binary large object存储不超过255B的二进制数据), tinytext(0-255B)(短文本字符串), blob/text(0-65535B), mediumblob/mediumtext(0-1600wB), longblob/longtext(0-40亿B)
  * char(10)表示最多只能存10字符，不足也占用10B；而varchar(10)不足时按实际情况存储；char更高效，浪费空间。
* **日期时间**类型: **date**(YYYY-MM-DD), time(HH:MM:SS), year(YYYY), **datetime**(YYYY-MM-DD HH:MM:SS), timestamp

```sql
# 修改表 (不重要 可视化操作就完了)
alter table player add column nickname varchar(20); # 给player表添加字段; 似乎可以省略column
alter table player modify column name VARCHAR(200); # 修改player表中name列数据结构
alter table player change column name to nick_name; # 修改player表中name名称为nick_name;
alter table player drop column name; # 删除name列
rename table player to new_name; # 修改表名
```

#### DML

数据操作语言：**增删改** insert delete update
![picture 4](../images/ce82de55f514dde10b9dbf2cdddb2473bfc7933b9b67dcbc1ded0bb34fcadef2.png)  
![picture 5](../images/d2e243c788de2ae0828085024c11604bf498a7758051864ed30118947ae7f6dc.png)  
![picture 7](../images/d8416e7bf56fd9ec2042399b90e4b318bcd7cf997ac8a9f4bfaeec57ae5e8982.png)  

```sql
insert into player (id, name, level, exp, gold) values (1, '张三', 1, 1, 1); # 所有字段都写的话可省略name
select * from player;
insert into player (id, name) values (2, '李四'), (3, '王二麻子'); # 插入多条数据
update player set level=1, exp=0, gold=0 where name='李四'; # update
delete from player where gold=0; # 删除gold=0的玩家
```

#### DQL

在一个正常的业务系统中，查询操作的频次是要远高于增删改的
![picture 8](../images/6f31bf9ec283691264f868a07eb23963f39df7876196c4f8e9faf78602718cfb.png)  

```sql
# 基础查询
select 字段1, 字段2 from 表名;
select * from tableName;                    # 实际开发中少用，低效且不直观
select 字段 as 别名 from ...;                # 设置字段别名
select 字段2 别名2, 字段2 别名2 from ...;     # as可省略
select distinct 字段列表 from ...;           # 去重  
```

```sql
# 条件查询
select column_list from table_name where 条件列表;
```

常用比较运算符：`>, >=, =, <>, !=, between ... and ..., in(...), like 占位符(模糊匹配(_单字符，%任意字符)), is null`
常用逻辑运算符：`and / &&, or / ||, not / !`

```sql
# 聚合函数(将一列数据作为整体进行纵向计算)
select 聚合函数(字段列表) from table_name;
select count(idcard) from emp; -- 统计idcard字段不为null的记录数
select avg(age) from emp;      -- 统计该企业员工的平均年龄
```

常用聚合函数：`count(不统计null), max, min, avg, sum`
统计行数: `count(字段), count(常量), count(*)(推荐，底层有特别优化)`都行

```sql
# 分组查询
select gender, avg(age) from emp group by gender ; -- 根据性别分组 , 统计男性员工 和 女性员工的平均年龄
select workaddress, count(*) address_count from emp where age < 45 group by workaddress having address_count >= 3; 
-- 查询年龄小于45的员工 , 并根据工作地址分组 , 获取员工数量大于等于3的工作地址
```

![picture 9](../images/e580fc603c3dd1725afed050cc00f537c865ef60898a981e0500bb49f70a9ec9.png)  

```sql
# 排序查询
select column_list from table_name order by 字段1 排序方式1, 字段2 排序方式2;
# ASC (默认升序), DESC
```

```sql
# 分页查询
select column from table limit 起始索引, 查询记录数;
# 查询第1页员工数据, 每页展示10条记录
select * from emp limit 0,10;
select * from emp limit 10;
# 查询第2页员工数据, 每页展示10条记录
select * from emp limit 10,10;
```

起始索引 = (页码 - 1) * 每页展示记录数

```sql
UNION: 并集；union two select result, and keep unique.
INTERSECT: 交集；
EXCEPT：差集；
```

执行顺序：
![picture 0](../images/531e9a4fb1557984ed739f4c09706c0bb93c8f756f54fa8b9779fadd3593a381.png)  

> **select是在分组之后的，只能查询分组字段或汇聚函数**

#### DCL

在MySQL中需要通过User@Host来唯一标识一个用户

```sql
# 管理用户
select * from mysql.user;   -- 查询用户
select Host, User from mysql.user;   -- 查询用户
create user 'userName'@'hostName' identified by 'password'; -- 创建用户
alter user 'userName'@'hostName' identified with mysql_native_password by 'new_password'; -- 修改用户密码
drop user 'userName'@'hostName'; -- 删除用户
# 示例
create user 'itcast'@'localhost' identified by '123456'; -- A. 创建用户itcast, 只能够在当前主机localhost访问, 密码123456;
create user 'heima'@'%' identified by '123456'; -- B. 创建用户heima, 可以在任意主机访问该数据库, 密码123456;
```

```sql
# 权限相关
show grants from 'userName'@'hostName'; -- 查询权限
grant 权限列表 on 数据库名.表名 to 'userName'@'hostName'; -- 授予权限
revoke 权限列表 on 数据库名.表名 from 'userName'@'hostName'; -- 撤销权限
# */all/all privileges 代表所有权限
```

![picture 1](../images/b0523272b743c7ea88b5f5c38511f036d8e798fb9eb0dc197571897e4d92f9d5.png)  

### 函数

* 字符串函数
  * concat(s1,s2)
  * lower(s), upper(s)
  * lpad(s, n, pad), rpad() # left/right padding, 使字符串长度为n
  * trim(s): 去掉头尾的空格
  * substring(s, start, len)
* 数值函数
  * ceil(x)
  * floor(x)
  * mod(x,y)
  * rand(): return random value in 0-1
  * round(x,y): 四舍五入 y位小数
* 日期函数
  * curdate()
  * curtime()
  * now(): 返回data和time
  * year(date)
  * month(date)
  * day(date)
  * data_add(date, interval 70 years)
  * datediff(date1, date2)
* 流程函数
  * ![picture 2](../images/babcada535efd00e2c23436086053a8e855d9416c518f2d97c904713346854dc.png) 
  * ![picture 3](../images/9dfa58cbdc71c081524f10868bbe60828bfaf38a138b7ea6ada56ce4c2a14f8c.png)  

### 多表设计

#### 一对多

在多的一方添加外键

![picture 10](../images/862a8e3c58b3dc57b751af991212b70b4d2e814f0d96f2a87c479d6418fa0103.png)  
![picture 11](../images/acb2321476d07e626c4106fb62d235bccb5c4836759933befbcd5bcfcc0a0bd8.png)
![picture 12](../images/1b0daea6521a3c4a55cf872f558d88911e51aef05c1a1ea3ee99e063de8e4ebd.png)  
![picture 13](../images/f55ffbc901b8e90895805715ed4e2725c98d403591b8768515a19e19713cd573.png)
> 项目开发中很少使用物理外键，甚至被禁止

#### 一对一

![picture 14](../images/58d44f5b9955172dd53264407a87c1387d1c11ac32c539512b1d45f7ebb0f94a.png)  

#### 多对多

![picture 15](../images/4b687bf2ce500f0c2c55a7ee02491aaa487b503d47d55512c1eb647e98ef8b00.png)  

### 多表查询

如果直接`select * from tableA, tableB`将返回笛卡尔积：类似于矩阵外积，向量外积，CLIP的language和image的乘积。
直接这么搞是没意义的。**必须要消除那些无效的笛卡尔积**。

![picture 16](../images/6f9a784313a27ae3cfe11d2c7ceca5e812c3a82c0eb8da02b4411f2c533eb052.png){width=300}

---

* 基于连接类型的查询
  * inner join（内连接）: 返回两个表中匹配条件的**交集**记录
  * **left join（左外连接）**: 返回左表的所有记录和右表中满足条件的记录
  * **right join（右外连接）**: 返回右表的所有记录和左表中满足条件的记录
    * 右外调个顺序就变成左外了: `A right join B -> B left join A`
  * full join（全连接）: 返回两个表中的所有记录，不论是否匹配。（MySQL 通过 UNION 实现）

![picture 23](../images/47ea5e100170562d7b0755920e99efcac575e55676b596cb4dec798e3bee7099.png){width=50%}
<!-- UNION 合并查询: 将多个 SELECT 语句的结果集合并为一个结果集。 -->



#### 内连接

![picture 18](../images/cc761828a0fb47e7607b4ef7126380dd37c423f88dc9301f31dceb78c9fb6bd5.png)  
> 可以给表起别名简化

```sql
-- 查询员工姓名 和 对应部门名称
-- 隐式内连接
select tb_emp.name, tb_dept.name from tb_dept, tb_emp where tb_emp.dept_id = tb_dept.id;
-- 显式内连接
select tb_emp.name, tb_dept.name from tb_emp inner join tb_dept on tb_emp.dept_id = tb_dept.id;
-- 别名
select e.name, d.name from tb_emp e inner join tb_dept d on e.dept_id=d.id;
```

#### 外连接

![picture 19](../images/23aac632861b62d086764ba4ccff43fc8828a1579536fd9a2f2344c9269aa047.png)  

```sql
-- 查询员工表*所有*员工的姓名 和 对应的部门名称 (左外连接: 会完全包含左表数据，交集 + 包括null(左表有 右表无))
select tb_emp.name, tb_dept.name from tb_emp left outer join tb_dept on tb_emp.dept_id=tb_dept.id;
-- 查询部门表*所有*部门的名称 和 对应的员工名称 (右外连接: 包含交集 + 右表部门表有 左表员工表无)
select d.name, e.name from tb_emp e right join tb_dept d on e.dept_id=d.id; -- 注意右表放部门表
-- 右外可以完全替换成左外（实际开发中一般使用左外）
select d.name, e.name from tb_dept d left join tb_emp e on e.dept_id=d.id;
```

#### 子查询/嵌套查询

一个查询结果作为另一个查询的条件；
![picture 20](../images/730ac90a750c8ff401806f3679c98a20a331d874076f4e766d813700a1f2fa57.png)

```sql
-- 标量子查询
-- 查询"教研部"的所有员工信息 (分解为：a:查询教研部对应id；b:找到员工id等于该id的条目)
select * from tb_emp where dept_id = (select id from tb_dept where name='教研部');
-- 查询"方东白"员工入职之后的员工信息 (分解为：a:查询方东白入职时间；b:...)
select * from tb_emp where entrydate >= (select entrydate from tb_emp where name = '方东白');

-- 列子查询
-- 查询"教研部"和"咨询部"的所有员工信息
select * from tb_emp where dept_id in (select id from tb_dept where name = '教研部' or name = '咨询部');

-- 行子查询
-- 查询与"韦一笑"的*入职日期及职位*都相同的员工 (两次子查询)
select * from tb_emp where entrydate = (select entrydate from tb_emp where name = '韦一笑')
                       and job = (select job from tb_emp where name = '韦一笑');
-- 优化一手 支持这种写法(A,B)=  (一次子查询)
select * from tb_emp where (entrydate, job) = (select entrydate, job from tb_emp where name = '韦一笑');

-- 表子查询 (将子查询作为临时表使用)
-- 查询入职日期是"2006-01-01"之后的员工信息，及其部门名称
-- 先查到日期符合的员工作为临时表来使用
select tmp.*, tb_dept.name from (select * from tb_emp where entrydate > '2006-01-01') tmp, tb_dept
    where tmp.dept_id=tb_dept.id;

```

### 事务

事务是一组操作的集合，它是一个不可分割的工作单位，事务会把所有的操作作为一个整体一起向系 统提交或撤销操作请求，即这些操作**要么同时成功，要么同时失败**。（原子性）

```sql
# 手动控制事务方法一（关闭事务自动提交）
select @@autocommit; -- 查看事务提交方式，1表示自动提交
set @@autocommit = 0; -- 设置为手动（session参数，只针对当前窗口有效）
# 设置为手动之后就需要使用commit手动提交事务到数据库
commit;     -- 提交事务
# 设置手动之后 如果执行出错需要回滚（why
rollback;   -- 回滚事务
```

```sql
# 手动控制事务方法二（显式开启事务）[推荐]
start transaction; -- 开启事务 或者begin
-- 事务操作
commit; -- 提交
rollback; -- 回滚
```

### 索引

```sql
-- 创建普通索引
create index index_name on table_name(column_name) 
-- 创建唯一索引
create unique index index_name on table_name(column_name) 
-- 创建复合索引
create index index_name on table_name(column1, column2) 
-- 创建全文索引
create fulltext index index_name on table_name(column_name) 
-- 创建前缀索引 (提取前n个字符构建索引)
create index index_name on table_name (column_name(n))
```

## 进阶

### 存储引擎

![picture 1](../images/7c922e1806f8c01041f1ac185a89437eeca49df4307ec87aaca92abc06c2c5e6.png)  

* 存储引擎：存储数据、建立索引、更新查询数据等技术的实现方式；
* 存储引擎基于表，而非库，所以又名为**表类型**。
* mysql5.5之后默认存储引擎InnoDB，早起是MyISAM；MEMORY存储引擎存在内存中。
* 查询存储引擎`show engines;`

InnoDB：
**support transactions, row-level locking, and foriegn keys**

![picture 3](../images/f589de149a1ecfa4d2f1ad6407cba3933e7b6f75866932369dd50ed88777d1da.png)  

![picture 2](../images/ec7aff723755704ce2c89589cfd16523d72df398f163d4de1708d7b990807b88.png)  

MyISAM：

特点：

* 不支持事务，不支持外键
* 支持表锁，不支持行锁
* 访问速度快

文件：

* xxx.sdi：存储表结构信息
* xxx.MYD: 存储数据
* xxx.MYI: 存储索引

Memory:

* 内存存放 快
* hash索引（默认）
* 文件：xxx.sdi：存储表结构信息

![picture 4](../images/dce6b078797cdf1eed55ea3608bbff912c6cc5d044bf566fd8b09256d9e30cc3.png)  

如何选择存储引擎？
![picture 5](../images/7889dd100a0a0c1d1b9b6d45323894e9968139f29a19f399547802c65fc1c420.png)  
MyISAM现在被MongoDB替代，Memory被Redis替代

## 一些指令

```sql
-- 查询当前数据库的增删改查频率
show [session | global] status like 'com_______';
```

可以通过慢查询日志来查看哪些sql语句的执行时间超过了指定参数; 还可以通过profiles查看每条语句的时间（需要先配置开启）

```sql
-- 查询每条sql的耗时
show profiles;
-- 查询指定id的sql的具体耗时
show profiles for query id;
explain + sql语句 -- 查询该语句执行计划 挺好用的
```

`show processlist;`: 查看有多少客户端连接了mysql服务
`kill connect +id`: kill客户端连接
`show variables like 'max_connections';`: 查询客户端连接数，默认151个

```sql
SELECT @@TRANSACTION_ISOLATION; -- 查看事务隔离级别
SET [ SESSION | GLOBAL ] TRANSACTION ISOLATION LEVEL { READ UNCOMMITTED | READ COMMITTED | REPEATABLE READ | SERIALIZABLE } -- 设置事务隔离级别
```