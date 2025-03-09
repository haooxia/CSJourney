# AI智能客服

* deepseek的训练成本是openai o1的3% - 5%，558万美元就训练好了基础模型V3，而o1需要数十亿美元。
* deepseek开源了预训练模型权重，可以免费商用。openai并没有开源权重（gpt-3以来）

---

* deepseek v3支持function call，而r1不支持。当然r1仅仅本地对话是可以的
* 调用模型的时候你可以借助阿里云端大模型api（需要一个key嘛），也可以采用ollama本地部署离线使用
  * 云端不太稳定，本地满血版很难嘛（资源不够嘛）
  * 我在rtx3090上(24G)跑了32B的deepseek r1，占显存20GB
  * 阿里云你可以用qwen-max,也可以deepseek-v3
* springAI会有很多官方Starter，比如`spring-ai-openai-spring-boot-starter`, `spring-ai-alibaba`, `spring-ai-ollama`(本地部署用这个就成)，

---

Q: 大模型为什么会有幻觉？

A: 幻觉: 生成内容看起来合理，但实际不真实。

* gpt的目标是预测下一个词，以生成连贯的文本。即追求的是“看起来像真的”，而非“真的是真的”
* 有的训练数据有问题
* **缺少垂直领域的数据**

Q: 如何避免幻觉

* 引入知识库，让模型在生成的时候能够**检索和参考外部知识库**。例如RAG（检索增强生成）
* 模型对齐：采用RLHF人类反馈强化学习，引导其生成更真实的数据
* 优化prompt，要求模型基于试试回答

Q: 什么是FunctionCall?




---

* 预设提示词（**预设角色**）：系统消息defaultSystem（客户看不见，系统发给大模型的）
  * 智能助手，主要职责：给用户退订单，买订单
  * """
        您是“图灵”航空公司的客户聊天支持代理。请以友好、乐于助人且愉快的方式来回复。
        您正在通过在线聊天系统与客户互动。
        在提供有关预订或取消预订的信息之前，您必须始终从用户处获取以下信息：预订号、客户姓名。
        在询问用户之前，请检查消息历史记录以获取此信息。
        在更改预订之前，您必须确保条款允许这样做。
        如果更改需要收费，您必须在继续之前征得用户同意。
        使用提供的功能获取预订详细信息、更改预订和取消预订。
        如果需要，可以调用相应函数调用完成辅助动作。
        请讲中文。
        今天的日期是 {current_date}.
    """

* 取消订单的方法: 函数有订单号 + 姓名两个参数，所以需要用户提供
  * 为了实现这个业务需求，也提前写好**预设提示词**：“在提供有关预订或取消预订的信息之前，您必须始终从用户处获取以下信息：预订号、客户姓名。在询问用户之前，请检查消息历史记录以获取此信息。”
```java
// 取消预定航班
public void cancelBooking(String bookingNumber, String name) {
    var booking = findBooking(bookingNumber, name);
    if (booking.getDate().isBefore(LocalDate.now().plusDays(2))) {
        throw new IllegalArgumentException("Booking cannot be cancelled within 48 hours of the start date.");
    }
    booking.setBookingStatus(BookingStatus.CANCELLED);
}
```
* 然后他让我提供订单号+姓名，我给他了，然后后台怎么调用“取消订单”方法呢？这个方法的两个参数是怎么提取到的呢？答：FunctionCall
  * 1 **你告诉大模型，什么样的对话，需要调用functioncall**，这个通过`@Description()`注解来实现，我们这里就是：`@Description("取消机票预订")`，这就是告诉大模型，用户在取消机票预订的时候，来调用这个FunctionCall
  * 2 那他怎么知道提取预定号和姓名呢 


```java
@Description("取消机票预定")
	public Function<CancelBookingRequest, String> cancelBooking() {
		return request -> {
			flightBookingService.cancelBooking(request.bookingNumber(), request.name());
			return "";
		};
	}
```
