# ML & AI

## 一、机器学习基础

### 1. 常见的监督/无监督学习算法

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

### 2. 过拟合 vs 欠拟合

- **过拟合**overfitting：模型在训练集上表现很好，但在测试集上效果差，说明泛化能力弱。
  - 常见原因：模型过于复杂，数据量太小，训练时间过长
  - 缓解方法：正则化、数据增强、减少模型复杂度、early stopping, dropoup, 交叉验证
- **欠拟合**：模型在训练集和测试集上都表现不好，说明模型能力不足。
  - 常见原因：模型太简单、特征不足、训练不充分
  - 缓解方法：增加模型复杂度、增加训练轮数、减小正则化强度

Q: 正则化为什么可以缓解过拟合
A: 正则化通过**在损失函数中加入惩罚项**，限制模型的自由度，防止其过度拟合训练数据中的噪声和偶然性。鼓励模型学习“**更简单、更平滑**”的函数

## 二、深度学习核心

### 1. CNN vs RNN

- **CNN**：卷积神经网络
  - 核心思想：利用卷积核（局部感受野）提取空间局部特征
  - 结构组成：卷积层、池化层、激活函数、全连接层
  - 应用：图像分类(ResNet, VGG)，目标检测(Faster R-CNN, YOLO)，图像分割(U-Net)
- **RNN**：循环神经网络
  - 核心思想：通过隐藏状态传递，捕捉序列中时间上的依赖关系
  - 缺陷：难以捕捉长期依赖（梯度消失/爆炸）；无法并行训练
    - 改进：LSTM长短期记忆网络，GRU门控循环单元
  - 应用：自然语言处理（文本生成、机器翻译），时间序列预测（气温、股价），语音识别


### 4. Transformer

#### 为什么出现Transformer

- RNN难以捕捉长期依赖，训练无法并行
- Transformer基于attention机制，可以并行计算，捕捉长距离依赖

#### 核心结构

1. input embedding: 将输入序列转换为向量表示，eg word2vec
2. position encoding: 添加位置信息，eg 正弦位置编码
3. self-attention:  对每个token，计算其与所有token的关联程度（相关性）
   1. $$\text{Attention}(Q, K, V) = \text{softmax} \left( \frac{QK^T}{\sqrt{d_k}} \right)V$$

#### BERT

- BERT（Bidirectional Encoder Representations from Transformers）基于Transformer的encoder
  - 预训练任务：
    - Masked Language Model（MLM）：随机mask掉一些token，预测被mask的token
    - Next Sentence Prediction（NSP）：判断两个句子是否相邻
  - 特点：同时考虑上下文，注重**语义理解**
  - 应用：文本分类、命名实体识别NER、情感分析等

- GPT（Generative Pre-trained Transformer）基于Transformer的decoder
  - 预训练任务：语言模型，预测下一个token（自回归语言建模）
  - 特点：单向（左到右），注重**文本生成**
  - 应用：聊天机器人，代码生成等

- ViT（Vision Transformer）
  - 结构：图像 → 分割为 Patch → 线性嵌入 → 输入标准 Transformer
  - 核心：不使用卷积，完全依赖attention提取视觉特征
  - 应用：图像分类检测分割

- **CLIP**：图文匹配，训练目标是最大化正样本的相似度

