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

### 5.