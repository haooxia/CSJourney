# LLM

- [LLM](#llm)
  - [DeepSeek \& RAG](#deepseek--rag)
    - [网页ds vs. 本地ds](#网页ds-vs-本地ds)
    - [幻觉: SFT vs. RLHF vs.RAG](#幻觉-sft-vs-rlhf-vsrag)
      - [SFT vs. RLHF vs. RAG](#sft-vs-rlhf-vs-rag)
      - [微调还是RAG?](#微调还是rag)
    - [本地部署deepseek + RAG](#本地部署deepseek--rag)
      - [悦动探针智慧客服](#悦动探针智慧客服)
    - [RAGflow底层 / RAG底层](#ragflow底层--rag底层)
      - [改进: 暴力搜索 -\> ANN](#改进-暴力搜索---ann)
      - [改进: 混合检索 Hybrid Search](#改进-混合检索-hybrid-search)
      - [Milvus向量数据库](#milvus向量数据库)
  - [SFT](#sft)
    - [LoRA](#lora)
    - [LLama-Factory](#llama-factory)
    - [FastAPI暴露接口](#fastapi暴露接口)
    - [后端调用](#后端调用)
  - [Prompt技巧](#prompt技巧)
      - [RTF框架](#rtf框架)
      - [思考链模式](#思考链模式)
  - [Agent](#agent)
    - [客服改进](#客服改进)
    - [Tool Calling](#tool-calling)
  - [MCP](#mcp)


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

* gpt的目标是预测下一个词，以生成连贯的文本。即**追求的是“看起来像真的”，而非“真的是真的”**
* 有的训练数据有问题
* **缺少垂直领域的数据**

Q: 如何避免幻觉?

* RAG：让模型有据可依
* Finetune：垂直数据，也是有据可依
* 模型对齐：RLHF：让他在不知道的时候承认不知道

#### SFT vs. RLHF vs. RAG

* **SFT (Supervised Fine-Tuning) 有监督微调**: 在预训练模型上，基于特定任务的数据集(人工标注)进一步进行训练，改变了权重。（微调是**考前复习**，模型通过训练，消化吸收了这些知识然后给你回复）
* **RLHF (Reinforcement Learning from Human Feedback) 强化学习**：模型根据人类反馈区调整自己的生成结果，可以生成更符合人类偏好的回答。
  * DPO (Direct Preference Optimization): 直接偏好优化：人类对比选择
  * PPO (Proximal Policy Optimization): 近端策略优化：奖励信号（点赞、点踩）
* **RAG(Retrieval-Augmented Generation)**: **不改变模型权重**，在生成回答之前，检索外部知识库找到和问题相关的知识，增强生成过程的信息来源，从而提升生成的质量。（RAG是**开卷考试**，模型看到你的问题，开始翻你的知识库，以实时生成更准确的答案）
  * 检索（Retrieval）：当用户提出问题时，系统会**从外部的知识库(文档、数据库或互联网)中检索出与用户输入相关的内容**。
  * 基于Embedding模型(eg BERT)都Embedding一手，然后基于向量相似性搜索(eg 余弦相似度)寻找（~~这不跟我推荐系统一样吗~~
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

---

### RAGflow底层 / RAG底层

我上传了一些资料，**Chat**模型选择了Deepseek-r1:32b, embedding模型选择了`BAAI/bge-large-zh-1.5`，RAGflow底层做了什么？


1. 资料文档会被切分为**小段落/块/chunk**，每段通常**几百个字**。分块一般采用滑动窗口的方式，**每个块之间有重叠**，避免**上下文断裂**。
   1. 按句子分割会有点短，语义不完整
   2. 文本分割很重要，影响检索
2. 每个chunk经过embedding模型编码为高维向量，向量存储到**向量数据库**，且系统会建立倒排索引啥的进行加速
   1. 向量数据库：可以作为LLM的Long-term Memory；eg. mulvus, FAISS, ES, postgreSQL
   2. 传统数据库(mysql)主要用于存储结构化数据（行列），侧重于**精准匹配**；向量数据库主要用于存储非结构化数据（文本、图片、音频等）经过特征提取后的特征表示，侧重于**相似性搜索**
3. 提出问题 -> retrieval(问题被embedding -> 找到语义最相似的chunk) -> augmentation(拼接在提问之前形成新prompt) -> generation (新prompt送入ds模型生成回答)
   1. 检索细节：找到与query最近的k个chunk，然后reranking得到context；eg 欧氏距离，余弦相似度，点积相似度

#### 改进: 暴力搜索 -> ANN

> 暴力搜索 -> ANN -> 构建索引 -> HNSW

Q: 近似最近邻算法（Approximate Nearest Neighbor, ANN）是什么？

在处理大规模高维数据（如图像、文本 embedding）时，我们通常需要快速查找与某个向量最相近的向量。这就用到了最近邻搜索（Nearest Neighbor Search）。

1. 精确最近邻（Exact Nearest Neighbor）存在的问题
   1. 暴力搜索（Brute-force Search） 是精确最近邻的一种实现方式：
      1. 每次查询都与所有向量计算相似度（如欧氏距离或余弦相似度）
      2. 简单粗暴，计算成本非常高。
   2. 高维空间下效率低：精确搜索面临维度灾难（Curse of Dimensionality），计算会变得非常慢。

2. ANN：在准确率与效率之间的平衡，通过**构建索引**，实现**近似**的快速搜索，**牺牲少量精度换取巨大速度提升**。
   1. 向量索引就是将大量高维向量按某种逻辑（如聚类、图结构）组织起来，加速相似度搜索。在你的场景中，比如把 100 个店铺的文档拆分成 chunk，转成 embedding，然后用 HNSW 建立索引。
   2. 常见的ANN实现算法/**构建向量索引**vector indexing：
      1. LSH（Locality Sensitive Hashing）：基于哈希
      2. HNSW（Hierarchical Navigable Small World）：基于图结构
      3. FAISS（Facebook AI Similarity Search）：集成多种高效索引算法
3. **Vector Indexing**是 ANN 的核心技术手段，目标是**让相似的向量尽可能靠近**。
   1. Hash-based: 局部敏感哈希算法LSH：将高维向量映射到低维空间，使用哈希函数将相似的向量映射到同一个桶中
   2. Tree-based: Spotify的Annoy算法
   3. Graph-based: HNSW
   4. Cluster-based: K-means，将搜索空间控制到某个小范围内

4. HNSW：一种高效的图结构索引方法; Hierarchical Navigable Small World Graph（多层导航小世界图）
   1. **多层图结构**：
      1. **高层图**由较少的节点组成(稀疏)，支持快速的大范围跳跃，负责粗略搜索
      2. **低层图**包含更多的节点(密集)，负责精确搜索，最底层的图包括所有点
         1. 有点像redis skiplist


![picture 6](../images/3cf8c70d95421c55221d22d074bdacb7ee5d607d8aac4d92dc1f9d0825d8ed04.png)  

---

捋一下总体流程：

1. 提前计算并存储店铺的embedding：（离线计算，系统启动之间）
   1. 计算好所有店铺的特征向量embedding，使用text2vec模型
2. 将这些店铺的向量存储到Milvus中。
3. 创建索引（HNSW）：
   1. 在插入所有店铺向量之后，你需要为这些向量创建索引。可以显著加速之后的搜索过程。
4. 查询时进行近似最近邻搜索：（实时查询）
   1. 当用户发送查询请求时，同样embdding，**然后会被输入到Milvus中**，系统会基于创建的索引（如HNSW）来搜索与用户向量最相似的店铺向量。
5. 返回最相似的店铺：
   1. Milvus会根据用户向量和店铺向量的相似度（如**余弦相似度**）返回最匹配的店铺，并将它们按相似度排序，给出前N个最相似的推荐。

---

* milvus本身可以进行相似度计算，milvus基于HNSW向量索引加速相似度计算过程。当用户发送查询时，HNSW 索引会先通过高层图进行粗略筛选，从而快速定位到可能包含相似向量的区域。然后，通过低层图进行更精细的搜索，从而找到与查询向量最相似的向量。
* mivlus在查询的时候，会自动将一部分索引和数据库加载到内存，加速
* 然后原始向量数据和索引数据存在磁盘中
* mivlus还支持分布式存储，可以管理大规模数据

code reference: `project\recommend\recommend.ipynb`

#### 改进: 混合检索 Hybrid Search

* **keyword search/关键词匹配/稀疏embedding**: 通过精确的关键词匹配来检索相关文档。
  * 缺陷：只能检索到包含关键词的文档，无法处理**同义词、拼写错误**等变体。eg “狗”和“汪星人”
  * model: eg BM25
* **vector search/向量搜索/稠密embedding**：基于文本语义进行查找，可以处理同义词、拼写错误等变体。
  * 缺陷：在处理缩写、产品编号、人名或员工号特定短语时，可能会出现问题。这正是关键词搜索的优势
  * model: eg BERT
* 融合：eg 加权平均


![picture 7](../images/44e5a3517e9f95981011ff371c6b991755fc273a8ea836fd345883cb018efb12.png)

![picture 8](../images/7b03312fb9ab6f4764c7b70c28e2074a9ee77bf74889159e86a19c29ef0ffe08.png)  

#### Milvus向量数据库

ref: `project\recommend\recommend.ipynb`

---

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


## Prompt技巧

#### RTF框架

* Role：指定模型的角色：程序员、数据分析师、讲解员、记者等
* Task：指定模型的任务：编写代码、分析数据、撰写文章等
* Format：指定模型的输出格式：JSON、Markdown、表格等

#### 思考链模式

* prompt末尾加上“让我们逐步思考”即可。适合复杂问题

## Agent

> 各个公司(openai, google..)对Agent都有自己的解释

* Key components:
  * **LLM**(大脑)
  * **multi-modal inputs**(感官): 眼镜，耳朵；text, image, voice
    * 编码后送入LLM
  * **tools**(手脚): web search, DB operation, External Knowledge
    * LLM可以通过 **==tool-calling==** 来调用tools，拿到信息反哺到LLM，LLM作进一步处理（eg chatgpt的web search, cursor创建新文件..）
    * 这个tools其实就是一个个function call吧
    * 数据库操作，文件操作中的写操作需要human approval (human in the loop)
  * memory(记忆)：有选择地存储上下文
    * short-term memory: 任务相关的上下文（eg gpt当前会话的上下文，chat history）
    * long-term memory: 任务无关的上下文（eg cross-conversation）
  * evaluation(评估): 评估当前的任务是否完成（可能在output之前/后，一般是benchmark时候用吧
  * **output**: 任务的最终结果
* 该图是一个 **==loop==**，agent之所以强大，是因为**他可以自己决定什么时候停止**。

> RAG你可以理解为一个“文档检索”的外部tools，你也可以理解为一种long-term memory

![picture 4](../images/6def87060c25fae3fb16b2f572eb93ede525e1409d8052ede56f3eeb493d6a51.png){width=70%}


Agent framework: LangGraph, LangChain...

### 客服改进

目前RAG + LLM更多是在**回答问题**
加入Agent之后，可以**解决问题**：eg 自动下单、退单（基于functionCall功能）

### Tool Calling

* 通过instructions/description模型可以知道去call哪个function/tool，同时我们就知道了这个function需要哪些parameters（不够的话反过来找user要），然后我们收集够了就可以去call。然后返回结果。
  * i.e., Mapping: `{prompt/human language : function_name}`
  * 这个tool是什么时候给LLM的呢？这个就是一个function schema，通过function calling接口明确传递给LLM的

![picture 5](../images/b01a502cbbe46abc28ca5dab5a664f4063f251c8cfbf52f0d6a4c386d05dffb8.png)  
> refer [link](https://www.bilibili.com/video/BV1CQ91YVEx5?t=4075.9); function schema可以参考，openai, langchain啥的写法不太一样吧都

function schema大概长这样
```json
{
  "model": "gpt-4",
  "messages": [ 
    { "role": "user", "content": "我想查一下上海今天的天气" }
  ],
  "tools": [
    {
      "type": "function",
      "function": {
        "name": "get_weather",
        "description": "获取指定城市的天气信息",
        "parameters": {
          "type": "object",
          "properties": {
            "city": {
              "type": "string",
              "description": "需要查询天气的城市名称"
            }
          },
          "required": ["city"]
        }}}]
}
```

## MCP

由于tools缺乏统一性，不好复用。所以Anthropic提出了MCP协议

MCP: Model Context Protocol, 一个社区共建的开放协议，提供一个通用的开放标准，用于连接LLM和外部数据、工具或者行为。

MCP的主要特点：

* 统一的工具调用格式：不同的工具都遵循相同的调用格式，方便复用和维护
* 上下文管理：提供统一的上下文管理机制，让模型更好地理解和使用工具
* 可扩展性：可以方便地添加新的工具和功能
* 跨平台兼容：不同的LLM平台都可以使用相同的协议

MCP的工作流程：

1. 定义工具：使用统一的格式定义工具的功能、参数等
2. 上下文构建：将工具信息和其他上下文信息组织成标准格式
3. 模型调用：模型根据上下文信息调用相应的工具
4. 结果处理：统一处理工具返回的结果

相比传统的Tool Calling，MCP的优势：

* 更好的工具复用：统一的格式让工具可以在不同的场景下复用
* 更清晰的上下文：标准化的上下文管理让模型更容易理解任务
* 更容易维护：统一的协议让系统更容易维护和升级
* 更好的生态：开放协议促进了工具和功能的共享
