# Spring

> main reference: [二哥java](https://javabetter.cn/sidebar/sanfene/spring.html)

[toc]

## 基础

### Spring定义 特性 模块

Spring 是一个**轻量级、非入侵式的**控制反转 (IoC) 和面向切面 (AOP) 的框架

**Spring优势：**

* IoC和DI的支持：Spring 的核心就是一个大的**工厂容器**，可以维护**所有对象的创建和依赖关系**，Spring工厂用于生成Bean，并且管理Bean的生命周期，实现**高内聚低耦合**的设计理念
* AOP编程的支持：可以方便的实现对程序进行权限拦截、运行监控等切面功能
* 声明式事务的支持：支持通过配置就来完成对事务的管理，而不需要通过硬编码的方式，以前重复的一些事务提交、回滚的JDBC代码，都可以不用自己写了
* 快捷测试的支持：可以通过注解快捷地测试 Spring程序
* 快速集成功能：提供了对各种优秀框架（如：Struts、Hibernate、MyBatis、Quartz 等）的直接支持

**Spring主要模块：**

* Spring Core: 框架最基础的部分，提供IoC和DI特性
* Spring Context: 上下文容器，是BeanFactory功能加强的一个子接口
* Spring Web: 提供Web应用开发的支持
* Spring MVC: 针对Web应用中MVC思想的实现
* Spring DAO: 提供对JDBC抽象层，简化了JDBC编码
* Spring AOP: 即面向切面编程，它提供了与AOP联盟兼容的编程实现
* Spring ORM：它支持用于流行的ORM框架的整合，比如：Spring+Hibernate、Spring+iBatis、Spring+JDO的整合等

### Spring Annotation

please see: "java\SpringAnnotation.xmind"

### Spring设计模式

暂略
