# Spring

> main reference: [二哥java](https://javabetter.cn/sidebar/sanfene/spring.html)

[toc]

## 基础

### Spring定义 特性 模块

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

## Spring

### Spring介绍

Spring 是一个**轻量级、非入侵式**的**控制反转 (IoC) 和面向切面 (AOP)** 的**框架**

**Spring核心特性/优势：**

* **IoC容器**：Spring通过控制反转，**维护所有对象的创建和依赖关系**。开发者只需要定义好Bean及其依赖关系，**Spring容器负责创建和管理这些对象**。
  * 依赖关系是指某对象A(eg bus)依赖对象B(eg wheels)
* **AOP**：面向切面编程，允许开发者定义**横切关注点**，例如事务管理、安全控制等，**独立于业务逻辑的代码**。通过AOP，可以将这些关注点模块化，提高代码的可维护性和**可重用性**。
* **事务管理**：Spring提供了**一致的事务管理接口**，支持声明式和编程式事务。开发者可以轻松地进行事务管理，而无需关心具体的事务API。
* **MVC框架**：Spring MVC是一个基于Servlet API构建的Web框架，采用了模型-视图-控制器（MVC）架构。它支持灵活的URL到页面控制器的映射，以及多种视图技术。
  * servlet是一个java类，用于处理web请求并生成响应，通常运行在Web服务器（如Tomcat、Jetty）上；核心api: `HttpServlet, HttpServletRequest...`

### 介绍一下IoC

* **IoC(Inversion of Control)控制反转**：是一种创建和获取对象的思想，**依赖注入DI(Dependency Injection)是实现这种技术的一种方式**。传统开发，我们需要通过**new关键字**来创建对象。使用IoC思想开发方式的话，我们不通过new关键字创建对象，而是**通过IoC容器来帮我们实例化对象**。
  * > 以前是我们想要什么就自己创建什么，现在是我们需要什么容器就帮我们送来什么。
  * IOC解决了繁琐的对象生命周期的操作，**解耦**了对象之间的耦合度，也解耦了代码
  * 所谓反转，其实就是反转的**控制权**，使用对象时，由主动new产生对象转换为由"外部"(Spring提供的IoC容器)提供对象

---

作图理解：

原本是我们自个儿new，现在有专门的IoC容器`ApplicationContext`来**控制**对象，控制是指：

* **创建对象**：原来是new一个，现在是由Spring容器创建
* **初始化对象**：原来是对象自己通过构造器或者setter方法给依赖的对象赋值，现在是由**Spring容器自动注入**
  * 如何自动注入来初始化呢？基于`@Autowired`, `@Resource`, 或者基于XML配置，或者基于构造函数（啊？
* **销毁对象**：原来是直接给对象赋值null或做一些销毁操作，现在是Spring容器**管理生命周期负责销毁对象**。

![picture 3](../images/f3f0a36302d30f5560121ceecc5fb247e9c5f7e8ae780adefdce0109a4c64fa8.png)  

#### IoC实现机制

* **反射**：Spring IOC容器利用反射机制动态地加载类、创建对象实例及调用对象方法，**反射允许在运行时检查类、方法、属性等信息，以及可以运行时创建对象和调用方法**，使得对象实例化更为灵活。
* **依赖注入**：IOC的核心概念是依赖注入，即容器负责管理应用程序组件之间的依赖关系。Spring通过**构造函数注入、属性注入或方法注入**，将组件之间的依赖关系描述在配置文件中或使用注解。
* **设计模式 - 工厂模式**：IOC容器通常采用工厂模式来管理对象的创建和生命周期。**容器作为工厂**负责**实例化Bean并管理它们的生命周期**。
* **容器实现**：IOC容器是实现IOC的核心，通常使用`BeanFactory`或`ApplicationContext`来管理Bean。
  * `BeanFactory`是IOC容器的**基本形式**，提供基本的IOC功能；
  * `ApplicationContext`是BeanFactory的**扩展**，提供更多企业级功能。

### 介绍一下AOP

AOP(Aspect Oriented Programing): 

在面向切面编程的思想里面，把功能分为两种

* **核心业务**：登陆、注册、增、删、改、查
* **周边功能**：**日志、事务管理**这些次要的业务

在面向切面编程中，核心业务功能和周边功能是分别独立进行开发，两者不是耦合的，然后把切面功能和核心业务功能 "编织" 在一起，这就叫AOP。

---

面向切面编程能够将那些**与核心业务(CRUD登录注册..)无关**，却**为业务模块所共同调用的逻辑(周边功能：日志、事务管理)**封装起来，以**减少系统的重复代码，降低模块间的耦合度**，并且有利于扩展和维护。

> 用于在不修改原始代码的情况下，给现有方法/代码添加增强功能。

AOP中的概念：

简而言之：

1. JoinPoint连接点是一堆执行方法
2. PointCut通过表达式来匹配joinpoint得到切入点
3. Advice通知是在切入点"处"(前|后|环绕)执行具体的操作
4. Aspect切面是将多个Advice和PointCut结合在一起的模块/类

详而言之：

* JoinPoint：**连接点**，指程序执行过程中可以插入AOP逻辑的特定点，springAOP中连接点通常是**执行方法**
  * 在SpringAOP中**仅支持执行方法**的连接点，其他其实还有抛出异常、设置变量等
* `@PointCut`: 定义**切入点**，一个用于匹配**连接点**的表达式；可以理解为**连接点的过滤器**，用来定义在那些连接点应用AOP逻辑；
  * 当方法/**连接点匹配上了定义的规则**，就成为了切入点，就可以在这些点上插入Advice
* Advice：**通知**，在**切入点处**(即匹配了的PointCut)执行的**具体操作**
  * 分为前置通知`@Before`, `@After`, 环绕通知`@Around`
* `@Aspect`：声明一个**切面**类，包含了通知(advice)和切入点(pointcut)的对应关系
  * advice和pointcut的对应管理：**切面类中通过切入点@Pointcut标识出需要增强的目标方法，通过通知@Before指定在什么时机执行增强逻辑**

```java
// 一个AOP切面类
@Aspect // 声明一个切面，描述通知和切入点的关系
@Component
public class LoggingAspect {
    // 切入点
    @Pointcut("execution(* com.example.demo.service.UserService.addUser(..))")
    public void userServiceMethods() {} // 切入点方法内容为空，因为它仅用于定义切入点

    @Before("userServiceMethods()")
    // 你也可以直接把切入点写到切面中
    // @Before("execution(* com.example.demo.service.UserService.addUser(..))") 
    public void logBefore() {
        System.out.println("Before method execution: Logging aspect");
    }

    @After("userServiceMethods()")
    public void logAfter() {
        // ...
    }
}
```

#### AOP实现机制

## SpringMVC

### SpringMVC的核心组件

* `DispatcherServlet`：前端控制器，核心的中央处理器，负责接收请求、分发，并给予客户端响应；是整个流程控制的**核心**，控制与调度
* `HandlerMapping`：处理器映射器，根据请求URL去匹配查找能处理的**Handler**，并会**将请求涉及到的拦截器和Handler一起封装**成一个`HandlerExecutionChain`处理器执行链对象
  * `Handler`: 处理器，完成具体的业务逻辑
  * `HandlerInterceptor`: 处理器拦截器，是个接口，可以拦截一些请求；你可以额外添加拦截器
    * 定义拦截器时，需要在配置类中实现`addInterceptors`方法注册拦截器，并设定拦截的路径范围
    * 可以创建多个拦截器，然后按照注册顺序进行执行
    * HandlerInterception中有三个方法
      * `preHandle`: 在请求处理程序执行之前调用。用于**执行权限验证**、日志记录等操作。如果该方法返回**false，则请求将被中断，后续的拦截器和处理程序将不会被执行**(很妙)
      * `postHandle`: 在**请求处理程序执行之后、视图渲染之前**调用。可以对请求的结果进行修改或添加额外的模型数据。
      * `afterCompletion`: 在整个请求完成之后调用，包括视图渲染完毕。可用于进行**资源清理**等操作。
* `HandlerAdapter`：处理器适配器，**Handler执行业务方法之前，需要进行一系列的操作**，包括表单数据的验证、数据类型的转换、将表单数据封装到JavaBean等，这些操作都是由交给他，开发者只需将注意力集中业务逻辑的处理上，DispatcherServlet通过HandlerAdapter执行不同的Handler
<!-- * `ModelAndView`: 装载了模型数据和视图信息，作为Handler的处理结果，返回给DispatcherServlet -->
* `ViewResolver`：视图解析器，根据Handler返回的逻辑视图，解析并渲染真正的视图，并传递给DispatcherServlet响应客户端

![picture 2](../images/7c7b615ad6412412e4ef5c8d68ac0163a54f11a304b01a3da882d75a4bf3262d.png)  


### SpringMVC工作原理 / 流程

![picture 1](../images/4dc74e7703cf69f845e608769181ee77faaf9e2672a6fbab7a13d1d56b50aca1.png)

1. **用户请求**：用户/客户端/浏览器发送请求，DispatcherServlet拦截请求
2. **请求拦截**：DispatcherServlet接收到请求后，并不直接处理，而是将请求信息传递给HandlerMapping。HandlerMapping根据请求的URL来查找能处理该请求的Handler（我们常说的Controller），并会**将请求涉及到的Interceptor和Handler一起封装**为一个处理器执行链对象`HandlerExecutionChain`
3. **处理器适配**：DispatcherServlet调用HandlerAdapter适配器执行具体的Handler
   1. HandlerAdapter通过适配器模式，使得不同类型的处理器可以被统一调用
4. **业务逻辑处理**：Handler完成对用户请求的处理后，会返回一个ModelAndView对象给DispatcherServlet
   1. ModelAndView顾名思义，包含了数据模型以及相应的视图的信息。Model是返回的数据对象，View是个逻辑上的View
5. **视图解析**：ViewResolver会根据逻辑View查找实际的View
6. **渲染视图**：DispatcherServlet使用解析得到的视图对象，将模型数据填充到视图中，完成视图的渲染，并将结果返回给用户
7. 把View返回给请求者（浏览器）
