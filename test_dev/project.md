# 基于Pytest的自动化测试框架

## 开篇

接口测试：

1. **获取接口文档**
2. 如何测试：
   1. **单接口测试**：验证某一个接口在不同参数下的结果是否符合预期
      1. 正向测试：正确参数
      2. 反向测试：错误参数，看看能否处理异常
      3. 边界测试：eg 固定长度的参数(身份证)，测测不同长度下的响应
   2. **业务逻辑测试**：验证**多个接口之间的协同工作**。例如，商品下单流程涉及多个接口，如库存检查、订单创建、支付确认等。这些接口之间存在依赖关系，测试时需要保证每个步骤的返回值都正确传递给下一个接口。
3. postman到CI持续集成
   1. 可以用postman快速验证接口的功能，即可以**快速人工手动做接口测试**
   2. **自动化测试**：将手动测试过程脚本化，eg，使用python的requests编写**自动化测试脚本**，并将脚本**集成到CI系统**中(eg Jenkins)。然后设定每天定时执行测试，并将测试结果推送到团队的沟通工具（eg 钉钉），所有成员就即时知道项目的健康状态了。

---

介绍一下接口文档：

1. 接口文档应当包括：
   1. 接口名: `创建新用户`
   2. 接口地址: `/api/v1/users`
   3. 请求方法: `post`; （get, post, put..）
   4. 请求头信息: `Content-Type: application/json`
   5. 请求参数
      1. `username：字符串，用户名称，长度 3-20 字符，仅限字母和数字，必填`
      2. `email：字符串，邮箱地址，需符合标准格式（如 user@example.com），必填`
      3. eg ```{  "username": "testuser",  "email": "test@example.com"}```
   6. 返回值及其数据结构
      1. 成功：HTTP 状态码 201
         1. ```{  "status": "success",  "data": {    "userId": 1001  },  "message": "用户创建成功"}``` // userId是用户唯一标识符
      2. 失败：HTTP 状态码 400
         1. ```{  "status": "error",  "errorCode": "INVALID_INPUT",  "message": "用户名或邮箱格式不正确"}```

## Python.Requests

Requests库用于简化http请求的发送和响应处理。

so easy

发送post

```python
import requests

# 设置请求的 URL 和参数
url = 'http://127.0.0.1:8787/login'
headers = {'Content-Type': 'application/x-www-form-urlencoded'}
data = {'username': 'testuser', 'password': 'testpass'}
# 发起 POST 请求
response = requests.post(url, headers=headers, data=data)
# 查看响应状态码和内容
print(response.status_code)  # 200 表示请求成功
print(response.text)  # 返回服务器响应的文本内容
```

* requests有get put post delete head patch request...
* `requests.Session()`创建会话对象，可以搞一个持久化链接，避免重复登录等操作
* 处理响应结果
  * `response.text`: 返回文本格式
  * `response.json()`: **解析**json格式
  * `response.content`: 获取二进制内容，eg 图片/文件

## 基于YAML文件进行API测试

基于YAML管理测试用例

YAML是一种简单的配置文件格式，适合**数据交换**和**测试用例管理**，比json更人性化简单吧。

1. 用yaml文件存储接口信息和测试用例（接口包含url, 请求方法，头部信息），测试用例就是请求参数和**预期结果**（正向和反向）
   1. 参考`test_dev\proj_test\login.yaml`
2. python读取yaml文件，调用`requests.requests(method, url, data, heads...)`，然后循环遍历测试用例即可，`assert`一下返回结果是否包含预期结果就行。
   1. 然后接口请求的返回结果也可以直接写入到yaml文件
3. 前面的yaml是硬编码，硬编码不好替换嘛，而且有时候我们需要动态生成一些东西，eg 对密码使用md5算法动态加密，这时候我们就可以用在占位符那塞一个函数

## JUnit {补充}

Junit是一个单元测试框架

单元测试：针对方法进行正确性测试

* 原来的问题：1.在main()中编写测试代码；2.无法实现自动化测试；3.无法得到测试报告
* junit可以一键对所有方法自动化测试，遇到bug不会中断（低级自动）；自动有测试报告

junit使用：
导入jar包 -> 为需要测试的类**定义对应的测试类**，**并为每个业务方法编写对应测试方法(测试方法需要public, 无参, 无返回值)**

junit底层原理：首先找到带有`@Test`注解的测试方法(这个就涉及到反射`Method.getAnnotations()`)，然后创建测试类的实例(通过反射获取测试类的构造函数，并调用`Constructor.newInstance()`)，然后利用反射调用测试方法(`Method.invoke`)


## Pytest

* junit是java单元测试框架
* pytest是一个基于python的测试框架，更灵活扩展，可以与requests, selenium集成，实现web自动化测试
  * 可以和Allure结合生成美观的测试报告
  * 可以和Junkins集成，实现自动化测试和部署



pytest核心功能：fixture，参数化，断言机制