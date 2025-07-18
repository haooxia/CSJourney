# ML & AI

- [ML \& AI](#ml--ai)
  - [一、机器学习基础](#一机器学习基础)
    - [常见的监督/无监督学习算法](#常见的监督无监督学习算法)
    - [过拟合 vs 欠拟合](#过拟合-vs-欠拟合)
  - [二、深度学习核心](#二深度学习核心)
    - [CNN vs RNN](#cnn-vs-rnn)
    - [Transformer](#transformer)
      - [为什么出现Transformer](#为什么出现transformer)
      - [核心结构](#核心结构)
      - [BERT \& GPT](#bert--gpt)
    - [LLM](#llm)
      - [pretrain vs finetuning](#pretrain-vs-finetuning)
      - [大模型优化技巧](#大模型优化技巧)
      - [LLM伦理和安全问题](#llm伦理和安全问题)

更多详细知识参考repo:
Machine Learning: [link](https://github.com/haooxia/MachineLearning-Notes)
Deep Learning: [link](https://github.com/haooxia/DeepLearning-Notes)

## 一、机器学习基础

### 常见的监督/无监督学习算法

- **监督**：线性回归、逻辑回归、SVM、决策树、XGBoost、神经网络  
  - 线性回归：建模线性关系，适用于回归问题
  - 逻辑回归：二分类问题，输出概率值
  - SVM：最大间隔分类器，适用于高维数据
  - 决策树：树形结构，易解释，适用于分类和回归
  - KNN：基于距离的分类器，简单易用
  - 神经网络：多层感知机，适用于复杂非线性问题，适用于复杂的数据，如图像、文本、序列
- **无监督**：K-means、PCA、AutoEncoder
  - K-means：聚类算法，基于距离
  - PCA主成分分析：降维算法，保留数据主要特征
  - Autoencoder：自编码器，神经网络自编码器，学习数据的低维表示

### 过拟合 vs 欠拟合

- **过拟合**overfitting：模型在训练集上表现很好，但在测试集上效果差，说明泛化能力弱。
  - 常见原因：模型过于复杂，数据量太小，训练时间过长
  - 缓解方法：正则化、数据增强、减少模型复杂度、early stopping, dropoup, 交叉验证
- **欠拟合**：模型在训练集和测试集上都表现不好，说明模型能力不足。
  - 常见原因：模型太简单、特征不足、训练不充分
  - 缓解方法：增加模型复杂度、增加训练轮数、减小正则化强度

Q: 正则化为什么可以缓解过拟合
A: 正则化通过**在损失函数中加入惩罚项**，限制模型的自由度，防止其过度拟合训练数据中的噪声和偶然性。鼓励模型学习“**更简单、更平滑**”的函数

## 二、深度学习核心

### CNN vs RNN

- **CNN**：卷积神经网络
  - 核心思想：利用卷积核（局部感受野）提取空间局部特征
  - 结构组成：卷积层、池化层、激活函数、全连接层
  - 应用：图像分类(ResNet, VGG)，目标检测(Faster R-CNN, YOLO)，图像分割(U-Net)
- **RNN**：循环神经网络
  - 核心思想：通过隐藏状态传递，捕捉序列中时间上的依赖关系
  - 缺陷：难以捕捉长期依赖（梯度消失/爆炸）；无法并行训练
    - 改进：LSTM长短期记忆网络，GRU门控循环单元
  - 应用：自然语言处理（文本生成、机器翻译），时间序列预测（气温、股价），语音识别


### Transformer

#### 为什么出现Transformer

- RNN难以捕捉长期依赖，训练无法并行
- Transformer基于attention机制，可以并行计算，捕捉长距离依赖

#### 核心结构

1. input embedding: 将输入序列转换为向量表示，eg word2vec
2. position encoding: 添加位置信息，eg 正弦/余弦位置编码
3. self-attention:  对每个token，计算其与所有token的关联程度（相关性）
   1. $$\text{Attention}(Q, K, V) = \text{softmax} \left( \frac{QK^T}{\sqrt{d_k}} \right)V$$
4. 多头注意力机制：从不同视角理解数据，将QKV矩阵分别投影到多个子空间，计算多个注意力分数，然后拼接，可以增强模型的表达能力

#### BERT & GPT

BERT属于自编码模型，GPT属于自回归模型

- BERT（Bidirectional Encoder Representations from Transformers）基于Transformer的**encoder**
  - 预训练任务：
    - Masked Language Model（MLM）：随机mask掉一些token，预测被mask的token
    - Next Sentence Prediction（NSP）：判断两个句子是否相邻
  - 特点：同时考虑上下文，注重**语义理解**
  - 应用：文本分类、命名实体识别NER、情感分析等

- GPT（Generative Pre-trained Transformer）基于Transformer的**decoder**
  - 预训练任务：语言模型，预测下一个token（自回归语言建模）
    - **无监督预训练与有监督微调相结合**
    - 微调任务：文本分类、相似性对比、多项选择等(GPT-1)
  - 特点：单向（左到右），注重**文本生成**
  - 应用：聊天机器人，代码生成等

- ViT（Vision Transformer）
  - 结构：图像 → 分割为 Patch → 线性嵌入 → 输入标准 Transformer
  - 核心：不使用卷积，完全依赖attention提取视觉特征
  - 应用：图像分类检测分割

- **CLIP**：图文匹配，训练目标是最大化正样本的相似度

### LLM

#### pretrain vs finetuning

* 预训练：一般用大规模**无标签**数据**从头**训练模型，使其学会语言的通用知识。
* 微调：用特定任务的**带标签数据**对预训练模型进行微调，提高其在特定任务上的性能。
  * GPT-1就是这个思想，即**无监督预训练与有监督微调相结合**

#### 大模型优化技巧

* 混合精度训练（FP16）降低显存占用，加速训练
  * FP16: 16-bit flating point, 比32-bit flating point节省50%显存
* 权重共享减少参数量
* 梯度累计、剪枝、蒸馏

#### LLM伦理和安全问题

* 偏见与歧视：
  * LLM可能会学习到训练数据中的偏见和歧视，导致生成的内容不公正或有害
  * 解决方案：**多样化训练数据**，过滤偏见数据
* 虚假信息生成
  * LLM可能生成虚假或误导性的信息
  * 解决方案：**验证生成内容的真实性**，使用外部知识库进行验证