# Text-to-Speech

reference:

* https://zhuanlan.zhihu.com/p/691709656
* https://github.com/cnlinxi/book-text-to-speech
* ChatTTS: [link](https://github.com/panyanyany/Awesome-ChatTTS), [tutorial](https://www.bilibili.com/video/BV1Ui421v7JU/?share_source=copy_web&vd_source=b576499af9e9962cb710c58072253ab6)
## Background

TTS: Text to Speech, or Speech Synthesis (语音合成)

* 广泛应用于语音对话系统：智能语音助手(e.g., Siri)，聊天机器人(e.g., ChatGPT)，朗读(e.g., 剪映)，车载导航，语音翻译，语音识别等等
* 语音合成系统分为：文本前端和后端
  * 文本前端：将输入的文本转换为语音合成系统可以处理的格式
    * 主要任务：分词，词性标注，音素转换，韵律分析
  * 语音合成后端：将文本前端处理后的数据转换为语音信号
    * 主要任务：声码器（Vocoder）模型

> 暂且省略一些语言学的基础知识，以及语音合成的发展历史

