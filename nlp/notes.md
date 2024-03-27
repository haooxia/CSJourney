# question

## Transformer

### 1.为什么使用multi-head attention？在相同维度下单头和多头的区别是什么？

单个头注意力机制的维度$d_k$乘上多头的个数$h$就等于模型的维度$d_{model}$。

1.为了解决模型在对当前位置的信息进行编码时，**会过度的将注意力集中于自身位置**的问题。
> 当进行进行注意力权重矩阵计算时，h越大那么Q,K,V就会被切分得越小，进而得到的**注意力权重分配方式越多**。让attention有更加丰富的层次，可以从不同的角度来看待问题。
2.一定程度上h越大整个模型的表达能力越强，越能提高模型对于注意力权重的合理分配。

### 2.为什么Q和K使用不同的权重矩阵生成，为何不能使用同一个值进行自身的点乘？

最简单的方式就是自身点乘也就是xx^T，其可以表征在本空间内的相似度。
但通常原空间的相似度是不够的，为了使得模型有更强的表征能力和泛化能力，我们需要其在不同空间进行投影。

### 3.Transformer计算attention的时候为何选择点乘而不是加法？

tbd

### 4.为什么在进行softmax之前需要对attention进行scaled（为什么除以dk的平方根），并使用公式推导进行讲解

原文解释：当维度dk比较大时，导致点积整体方差变大，进而dot products($QK^T$)出现极大值使得softmax梯度消失，所以需要通过scale空置房差，进而稳定梯度流，使得block堆叠时候模型仍然可以有效地训练。

为什么点积会增大比较快？
原文解释：To illustrate why the dot products get large, assume that the components of q and k are independent random variables with mean 0 and variance 1. Then their dot product, $q\cdot k = \sum_{i=1}^{d_k} q_ik_i$, has mean 0 and variance $d_k$;

> $d_k$ is the dimension of queries and keys, $d_v$ is the dimension of value;

证明：
> 1 两个独立随机变量点积的期望: $E[XY]=E[X] \cdot E[Y]$
> 2 两个独立随机变量点积的方差(当二者期望都为0时): $var[XY]=var[X]\cdot var[Y]$
> $E[q\cdot k]=E[\sum_{i=1}^{d_k} q_ik_i]=\sum_{i=1}^{d_k}E[q_ik_i]=\sum_{i=1}^{d_k}E[q_i]E[k_i]=0$
> $var[q\cdot k]=var[\sum_{i=1}^{d_k} q_ik_i]=\sum_{i=1}^{d_k}var[q_ik_i]=\sum_{i=1}^{d_k}var[q_i]var[k_i]=\sum_{i=1}^{d_k}1=d_k$

### 5.embedding方案

token的embedding通常有两种方式：
1.使用pre-trained的embedding并冻住，实际上就是一个查询映射表格。我的paper就这样尝试过，但很遗憾，由于bert的embedding似乎无法完全映射到一个单独的字符，这个embedding应该还有上下文信息。
2.随机初始化或使用pretrained embedding，然后设置为trainable，这是end-to-end的方式。（Transformer）

### 6.位置编码

由于$attention$**是不会有时序信息的**，“输出时value的加权和，权重是query和key之间的距离”，这跟序列信息无关。这意味着我给你一句话，任意打乱后（语义肯定变了），attention结果仍然一致（顺序会变，但值不变），这有问题。
> RNN是将上一时刻的输出作为本时刻的输入，so可时序。

**$attention$是在输入中加入时序信息**。我们用长为512位的向量表示embedding向量，同时用等长的向量表示$positional\ encodeing$,具体的值使用周期不同的sin和cos函数算出来的。
这是想让网络知道这个单词所在句子中的位置是什么，是想让网络做自注意力的时候，不但要知道注意力要聚焦在哪个单词上面，还想要知道单词之间的互相距离有多远

## BERT

所以我们可以从这里看出decoder有一个sequence mask，decoder在生成输出序列的每个位置的词语时，只能依赖已生成的部分序列，避免未来信息泄露，所以说decoder是单向的（gpt）；encoder是双向的，因为在处理输入sequence时，每个位置的token都可以考虑其上下文，有助于更好地捕获输入序列中的语义和语法信息（bert）。

### question

1. bert是怎么使用transformer的，为什么？
2. bert与transformer的encoder端的输入有什么区别？
   1. token embeddings + segment .. + position ..
   2. bert的三种embedding都是学习得到的，使用的都是nn.Embedding() layer.
3. bert pretrain有哪两项训练任务？Mask LM & Next Sentence Prediction
4. Mask LM的mask策略（样本是怎么造的），为什么？
5. NSP用来判断两个句子是否连续，作用是什么？样本是怎么造的？和albert有什么区别？
6. NSP作为二分类任务，和[CLS] token的关系是什么？
   1. BERT在第一句前会加一个[CLS]标志，最后一层该位对应向量可以作为整句话的语义表示，从而用于下游的分类任务等。
   2. 用于分类任务没错，但是说cls能作为整句话的语义表示差点意思，二分类任务并非是去编码整个句子的语义信息。
   3. Reimers等人之前已实验证明 context embeddings 取平均要优于[CLS] token的embedding
7. BERT的输入输出是什么？为什么我的paper中无法使用bert的token embedding作为character content的映射？
   1. bert的输出是输入各字符对应的融合了全文语义信息之后的向量表示，所以他并不是完全代表着输入content的vector。合理。
8. ELMo和BERT的异同？
9. bert可用于哪些下游任务？
   1. bert主要是NLU，可以做命名实体识别NER，classification，断句
   2. 也可以做question answer，不过是给定question和paragraph，然后它从paragraph中理解标记出答案。（bert是不能生成的，主要用于理解文本和提取特征）