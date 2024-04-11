# 基于人工碱基的序列重构方案  工作进展

### 人工碱基作为分隔符进行序列重构

记两个人工碱基为 M N，进行如下编码 { A:MM  ,  C:MN  , G:NM   , T : NN}，相较于直接取单个人工碱基作为分隔符，可以将考虑将两个人工碱基组合作为分隔符的原因如下：

- 两个人工碱基两两组合可以对应表示ACGT四种信息，两个碱基作为分隔符的同时可以携带一个碱基的信息，冗余量与仅采取一个碱基作为单纯的分隔符相同

- 采用一个人工碱基作为分隔符时，若该人工碱基被删除，则不能确定边界，采用两个碱基作为分隔符，当其中一个被删除时，仍具有分界作用。

具体的策略如下：

![](C:\Users\emily\AppData\Roaming\marktext\images\2024-04-11-21-55-02-8f50d93ff91bb25aab0f0be165e6a9f.jpg)

### 工作计划

#### 1.实现已有算法

- 删除重构/DNA重构: Reconstruction Algorithms for DNA-Storage Systems  

- 序列重构系统：Trace reconstruction from noisy polynucleotide sequencerreads  

- ~~单读重构:Single-Read Reconstruction for DNA Data StorageUsing Transformers~~

#### 2.使用人工碱基作为分隔符，进行算法上的修改

将原本的长度为L的输入序列按照长度k进行分段，每段的最后一个碱基用两个人工碱基表示，直接调用已实现的算法处理长为k的序列，相当于将原本的一个簇，分为了`L/k` 个簇进行处理，然后再将结果进行拼接，主要需要按照上述图中的思想处理分簇、以及拼接的过程。

#### 3.进行对比实验

- 衡量标准：准确率、算法复杂度

- 比较维度：
  
  相同算法，不同信道(t-删除  t-替换  编辑错误)
  
  相同信道，不同算法
  
  相同算法、相同信道、相同信息，添加分隔符前后对比

### 代码实现进度说明

地址：[https://github.com/EmilyZHANG00/RECONSTRUCTION]()

###### :white_check_mark: Trace reconstruction from noisy polynucleotide sequencerreads

`(专利) Parikshit S.Gopalan,PaloAlto,CA(US);Sergey Yekhanin.Redmond WA( US );Siena Dumas Ang`

- 基本按照论文里面的算法思想实现了代码，但是还有很多细节的地方可以完善(如果有需要的话) 。

- 目前只是有一个输出重构后的结果序列的功能，如果这部分有一些展示重构过程的需求的话，或许还需要再添加一些别的功能，比如分步骤展示具体在哪个序列的哪个位置进行了错误类型的判定、判定结果为替换、插入还是删除等

###### Reconstruction Algorithms for DNA-Storage Systems

`Omer Sabary, Alexander Yucovich, Guy Shapira, and Eitan Yaakobi`

- 没开始

- 这篇论文里面的算法过程都给出了一些伪代码，再加上目前已经写好了一部分，以及一些框架性的东西，实现起来应该比较快。

### 目前结果示例

![](C:\Users\emily\AppData\Roaming\marktext\images\2024-04-11-21-18-35-image.png)

【 解释 】：

左边是输入文件，每个簇之间用空白行分隔，用“=”行 来references和reads，输入文件总共包含了20个簇，基本每个簇中的reads都是包含了对应的references的，包含的错误的数量基本在个位数。(ps：这个文件是直接使用了`《Robust Multi-Read Reconstruction from
Contaminated Clusters Using Deep Neural Network
for DNA Storage》(Yun Qin, Fei Zhu)`这篇公开代码里面给出了示例文件)

右边是程序输出结果，对比了重构算法输出的结果和references，对于20个簇的重构结果都和原序列相同。

### 
