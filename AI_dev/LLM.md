# LLM

- [LLM](#llm)
  - [DeepSeek \& RAG](#deepseek--rag)
    - [网页ds vs. 本地ds](#网页ds-vs-本地ds)
    - [幻觉: SFT vs. RLHF vs.RAG](#幻觉-sft-vs-rlhf-vsrag)
      - [SFT vs. RLHF vs. RAG](#sft-vs-rlhf-vs-rag)
      - [微调还是RAG?](#微调还是rag)
    - [本地部署deepseek + RAG](#本地部署deepseek--rag)
      - [悦动探针智慧客服](#悦动探针智慧客服)
  - [SFT](#sft)
    - [LoRA](#lora)
    - [LLama-Factory](#llama-factory)
    - [FastAPI暴露接口](#fastapi暴露接口)
    - [后端调用](#后端调用)
  - [RAG更进一步？](#rag更进一步)


## DeepSeek & RAG

### 网页ds vs. 本地ds

* 绝对的隐私保护：用网页ds需要将数据上传到ds服务器
* 个性化知识库构建：虽然网页ds可以上传文件，但有限制；而且你每次都要上传，想修改删除附件都很麻烦；而且一般这种经常需要付费
* 本地ds可以结合RAG技术
  * 基于开源框架RAGFlow
  * 本地部署Embedding大模型（或者直接部署自带Embedding模型的RAGFlow版本）；
  * 虽然云ds也可以结合RAG：但一般需要付费，且还是没保护隐私嘛，知识方便RAG了而已

### 幻觉: SFT vs. RLHF vs.RAG

llm在回答自己不太懂的问题时候会出现幻觉问题，生成内容看起来合理，但实际不真实。

原因：

* gpt的目标是预测下一个词，以生成连贯的文本。即追求的是“看起来像真的”，而非“真的是真的”
* 有的训练数据有问题
* **缺少垂直领域的数据**

Q: 如何避免幻觉?

* RAG
* Finetune
* 模型对齐：RLHF

#### SFT vs. RLHF vs. RAG

* **SFT (Supervised Fine-Tuning) 有监督微调**: 在预训练模型上，基于特定任务的数据集(人工标注)进一步进行训练，改变了权重。（微调是**考前复习**，模型通过训练，消化吸收了这些知识然后给你回复）
* **RLHF (Reinforcement Learning from Human Feedback) 强化学习**：模型根据人类反馈区调整自己的生成结果，可以生成更符合人类偏好的回答。
  * DPO (Direct Preference Optimization): 直接偏好优化：人类对比选择
  * PPO (Proximal Policy Optimization): 近端策略优化：奖励信号（点赞、点踩）
* **RAG(Retrieval-Augmented Generation)**: 不改变模型权重，在生成回答之前，检索外部知识库找到和问题相关的知识，增强生成过程的信息来源，从而提升生成的质量。（RAG是**开卷考试**，模型看到你的问题，开始翻你的知识库，以实时生成更准确的答案）
  * 检索（Retrieval）：当用户提出问题时，系统会**从外部的知识库(文档、数据库或互联网)中检索出与用户输入相关的内容**。
  * 基于Embedding模型(eg BERT)都Embedding一手，然后基于向量相似性搜索(eg 余弦相似度)寻找（这不跟我推荐系统一样吗~
  * embedding模型简单来说就是把自然语言 -> 高维向量
  * 增强（Augmentation）：系统将检索到的信息与用户的输入**结合**，**扩展模型的上下文**(也就是**模型的记忆/思考背景**，比如可以通过搜到的数据增强嘛)。这让生成模型可以利用外部知识，使生成的答案更准确和丰富。
  * 生成（Generation）：生成模型基于增强后的输入生成最终的回答。弥补了传统生成模型**知识更新慢**、**可能产生“幻觉”**的不足。

#### 微调还是RAG?

* 微调：
  * 适合：拥有**非常充足**的数据
  * 能直接提升模型的固有能力，无需依赖外部检索
* RAG：
  * 适合：只有**非常非常少**的数据；适合**动态更新/更新频率高**的数据
  * 但每次回答需要检索，**速度慢**，且回答很依赖检索系统的质量
* 总结：
  * 少量企业私有知识：最好微调和RAG都做；资源不足时优先 RAG；
  * 会动态更新的知识：RAG
  * 大量垂直领域知识：微调

### 本地部署deepseek + RAG

deepseek:

1. download ollama (app/cmd)
2. ollama list: show models
3. download deepseek: https://ollama.com/library/deepseek-r1:1.5b: `ollama run deepseek-r1`

RAG:

1. 下载RAGflow源码 & Docker，通过Docker来本地部署RAGflow
   1. ref: https://github.com/infiniflow/ragflow
   2. ref: https://www.bilibili.com/video/BV1WiP2ezE5a?t=1116.6
   3. 进入`localhost:8080`: 我用的8080端口，大概ragflow前端
2. 在RAGflow中构建个人知识库
   1. 由Grok生成“悦动探针”100个店铺（名称，描述，5条评价），作为个人/公司/项目知识库：`java\project\recommend\dianping_from_Grok.md`
      1. 这个描述可以大规模扩展，每个店铺内的商品信息都可以往上塞，**经常更新都无所谓**

> 当然你也可以不本地部署ollama-deepseek，你可以直接去RAGFlow选择一个在线模型即可

![picture 2](../images/08569920e0d2de0895f70c3c936a5cf13a5c7bf779c1f5dc6c6bc74ff9ddc7b3.png)  

![picture 3](../images/c9017f17e83d172ddf188f2949e55b1f316530c569a655b2d73667c7c7603a57.png)  


#### 悦动探针智慧客服


1. 信息查询类需求
   1. “极速飞鱼游泳俱乐部提供什么服务？”
   2. “xxx评价如何”
   3. “xxx地址在哪儿”
2. 活动推荐类需求
   1. “推荐一个适合亲子活动的场所”
   2. “有什么适合团队建设的运动？”
   3. “我想尝试水上运动，有什么推荐？”
   4. “有什么周末的户外活动？”
3. 装备相关需求
   1. “哪里可以买到专业的滑雪装备？”
   2. “攀岩装备哪里质量好？”
4. 课程/培训类需求
   1. “我想学游泳，有什么地方推荐？”
   2. “我想学滑翔伞，有什么建议？”
5. 个性化定制需求
   1. “我想找一个安静点的运动场所。”
   2. “有什么适合老年人的活动？”

---

后续还可以提供**具体地址、商品价格、联系方式、营业时间**。

1. 商品与价格咨询
   1. “旋风乒乓球馆的球拍多少钱？”
   2. “租一辆水上摩托要多少费用？”
   3. “预算100元，能玩什么？”
   4. “哪里有滑雪课程的优惠？”
2. 营业时间咨询
   1. “xxx周末营业吗？”
   2. “我想周三晚上学瑜伽，有地方开课吗？”




## SFT

通过**人工标注**的数据，进一步训练预训练模型，使其在特定任务上表现更好。

微调算法分为：

* 全参数微调(Full-parameter fine-tuning)
  * 性能最佳；但需要大量计算资源 & 容易过拟合
* 部分参数微调(Partial-parameter fine-tuning)：**LoRA算法**
  * 相反

### LoRA

LoRA: Low-Rank Adaptation of Large Language Models

矩阵的秩：矩阵中线性无关的行/列向量个数；即矩阵所包含的有效信息量。

![picture 0](../images/93a8501c42f4ef304b34b737ae600ce9dc51e1b9cc9f7ac8e11bdf8102b79878.png)  

### LLama-Factory

* 一个页面微调平台（低代码）
* 搞点符合格式的训练集，用lora训就完了
* 预训练模型来自huggingface

### FastAPI暴露接口

* 将SFT后的模型路径，基于`transformers`库加载，然后基于FASTapi就可以暴露一个API接口，随意操控这个api内部处理动作了
* FastAPI是一个基于python的现代web API框架（似乎就是python世界的springboot
* 这个调用的api是（**本地访问**） `localhost:8000/generate`
  * 可以使用cURL / Postman测试该API
  * 如果想要**公网访问**，那就部署到服务器即可，`your-server-ip:8000/generate`

```python
from fastapi import FastAPI
from transformers import AutoModelForCausalLM, AutoTokenizer
import torch

app = FastAPI()
# SFT后的模型路径
model_path = "/root/autodl-tmp/Models/deepseek-r1-1.5b-merged"
tokenizer = AutoTokenizer.from_pretrained(model_path)
device = "cuda" if torch.cuda.is_available() else "cpu"
model = AutoModelForCausalLM.from_pretrained(model_path).to(device)
@app.get("/generate") # NOTE：表示接收GET请求，路径为`/generate`
async def generate_text(prompt: str):
  # 解析用户提问 -> model -> 解析输出
    inputs = tokenizer(prompt, return_tensors="pt").to(device)
    outputs = model.generate(inputs["input_ids"], max_length=150)
    generated_text = tokenizer.decode(outputs[0], skip_special_tokens=True)
    return {"generated_text": generated_text}
```

### 后端调用

1. 假如你直接部署/sft了一个模型，那么暴露API就可以借助FastAPI框架: `http://localhost:8000/generate`
2. 如果你直接Ollama部署ds，ollama**自带了**api访问: `"http://localhost:11434/api/generate"`
   1. [reference](https://www.runoob.com/ollama/ollama-api.html)
3. 然后java后端访问API的话很简单了，我用的是apache的**httpClient5**工具，直接访问api就行了，交给ai吧
   1. 同样的搞个control(一个api)，然后走到下面的逻辑即可
4. 那我们ragflow中rag增强后的ollama的ds，那个api是什么？
   1. 大概来说，需要curl创建知识库，再基于知识库创建角色，再就可以拿到api，角色和秘钥了。然后故事就差不多了，暂不深究了吧


<!-- import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.hc.client5.http.classic.methods.HttpPost;
import org.apache.hc.client5.http.impl.classic.CloseableHttpClient;
import org.apache.hc.client5.http.impl.classic.HttpClients;
import org.apache.hc.core5.http.ContentType;
import org.apache.hc.core5.http.io.entity.StringEntity;
import org.apache.hc.core5.http.io.entity.EntityUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory; -->

<!-- dependency: httpclient5, sfl4j-simple, jackson-databind -->


```java
public class OllamaApiClient {
    private static final String API_URL = "http://localhost:11434/api/generate";
    private static final String MODEL_NAME = "deepseek-r1:32b";
    private static final String PROMPT = "你好，9.9和9.10谁更大？";
    private static final ObjectMapper MAPPER = new ObjectMapper();
    private static final Logger LOGGER = LoggerFactory.getLogger(OllamaApiClient.class);
    public static void main(String[] args) {
        try {
            String response = sendRequest(PROMPT);
            LOGGER.info("Response: {}", response);
        } catch (Exception e) {
            LOGGER.error("Error calling API", e);
        }
    }
    public static String sendRequest(String prompt) throws Exception {
        try (CloseableHttpClient client = HttpClients.createDefault()) {
            HttpPost post = new HttpPost(API_URL);
            // 使用 Jackson 构造 JSON，避免手动拼接
            JsonNode request = MAPPER.createObjectNode()
                    .put("model", MODEL_NAME)
                    .put("prompt", prompt)
                    .put("stream", false);
            post.setEntity(new StringEntity(MAPPER.writeValueAsString(request), ContentType.APPLICATION_JSON));
            post.setHeader("Content-type", "application/json");

            return client.execute(post, response -> {
                String jsonResponse = EntityUtils.toString(response.getEntity());
                JsonNode root = MAPPER.readTree(jsonResponse);
                return root.path("response").asText(); // 自动解码 Unicode
            });
        }
    }
}
```

<!-- 

RAGflow API调用：


1. 创建知识库，获取id
```shell
curl --request POST \
     --url http://localhost:8080/api/v1/datasets \
     --header 'Content-Type: application/json' \
     --header 'Authorization: Bearer ragflow-EyYzhiN2M0ZjlhOTExZWY5ZTMwMDI0Mm' \
     --language 'Chinese'\
     --data '{
        "name": "悦动探针_curl"
     }'
```

获得返回的{id}:"2d697672f9c211efa44d0242ac120006"


2. 创建聊天助手
```shell
curl --request POST \
     --url http://localhost:8080/api/v1/chats \
     --header 'Content-Type: application/json' \
     --header 'Authorization: Bearer <KEY>' \
     --data '{
        "name": "my_chat_assistant",
        "dataset_ids": ["<DATASET_ID>"]
     }'
```


```shell
curl --request POST \
     --url http://localhost:8080/api/v1/chats \
     --header 'Content-Type: application/json' \
     --header 'Authorization: Bearer <KEY>' \
     --data '{
        "name": "我的中文助手",
        "dataset_ids": ["<DATASET_ID>"],
        "language": "Chinese",
        "prompt": {
            "opener": "你好！我是你的中文智能助手，有什么可以帮助你的吗？",
            "variables": [{"key": "knowledge", "optional": false}],
            "prompt": "你是一个智能助手，请总结知识库的内容来回答问题，请列举知识库中的数据详细回答。当所有知识库内容都与问题无关时，你的回答必须包括“知识库中未找到您要的答案！”这句话。回答需要考虑聊天历史。以下是知识库：{knowledge}以上是知识库。"
        },
     }'
```

大概最终的角色是这样聊天的

```shell
curl --request POST \
     --url http://localhost:8080/api/v1/chats/b1f2f15691f911ef81180242ac120003/completions \
     --header 'Content-Type: application/json' \
     --header 'Authorization: Bearer <KEY>' \
     --data '{
        "question": "请告诉我关于人工智能的知识",
        "stream": false
     }'
``` -->

## RAG更进一步？

