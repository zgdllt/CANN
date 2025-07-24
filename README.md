# CANN - C++ Artificial Neural Network

## 项目简介

CANN (C++ Artificial Neural Network) 是一个用C++实现的人工神经网络库，提供了完整的神经网络构建、推理和文件操作功能。该项目采用面向对象的设计模式，支持多层前馈神经网络的创建和管理，并在main.cpp中提供了一个简单的示例程序，演示了如何使用该库进行神经网络的构建和推理。

## 版本信息

- **版本**: v1.0
- **开发者**: zgdllt
- **开发日期**: 2025年7月13日 - 2025年7月24日
- **支持平台**: Windows (x64)
- **编译器**: Clang 20.1.8 with x86_64-pc-windows-msvc
- **C++标准**: C++14

## 项目结构

```
CANN/
├── README.md               # 项目说明文档
├── version.txt            # 版本信息
├── main.cpp              # 主程序入口
├── 核心类文件/
│   ├── Network.hpp/cpp    # 神经网络主类
│   ├── Layer.hpp/cpp      # 网络层类
│   ├── Neuron.hpp/cpp     # 神经元类
│   ├── Soma.hpp/cpp       # 细胞体类
│   └── Synapse.hpp/cpp    # 突触类
├── 功能模块/
│   ├── ActivationFunc.hpp/cpp    # 激活函数类
│   ├── ANNFilePorter.hpp/cpp     # ANN文件导入导出类
│   └── FilePorter.hpp            # 文件操作基类
├── 示例文件/
│   ├── simple.ANN         # 示例神经网络文件
│   └── output.ANN         # 输出文件
```

## 快速开始

### 编译方法

```bash
# 使用Clang编译器
clang++ -std=c++14 -Wall -o main.exe *.cpp

# 或使用GCC编译器
g++ -std=c++14 -Wall -o main.exe *.cpp

```

### 运行示例

```bash
# 直接运行主程序
./main.exe

# 或在Windows PowerShell中, 使用UTF-8编码设置以避免中文显示错误
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
./main.exe

```

## 核心类详解

### 1. Network类 - 神经网络主类

#### 功能概述
`Network`类是整个神经网络的核心管理类，负责网络的构建、层管理、前向传播等功能。

#### 构造函数
```cpp
Network();                           // 默认构造函数
Network(const Network& other);       // 拷贝构造函数
Network& operator=(const Network& other); // 赋值运算符重载
```

#### 网络构建方法
```cpp
// 添加网络层
void addLayer(Layer* layer);
void addLayer(int index);            // 在指定位置添加层

// 添加神经元
void addNeuron(int layerIndex, double bias = 0.0, int activationType = 0);

// 删除操作
void deleteLayer(int index);         // 删除指定层
void deleteNeuron(int layerIndex, int neuronIndex); // 删除指定神经元
```

#### 网络推理方法
```cpp
// 前向传播 - 核心推理方法
std::vector<std::vector<double>> forward(const std::vector<double>& inputs);
```

#### 网络配置方法
```cpp
// 权重设置
void setWeights(int layerIndex = 0, const std::vector<std::vector<double>>& weights = {});

// 网络验证
bool isValid() const;               // 检查网络结构是否有效

// 网络信息
void setName(const std::string& name);  // 设置网络名称
std::string getName() const;            // 获取网络名称
```

#### 信息查询方法
```cpp
// 获取网络组件
const Layer* getLayer(int index) const;        // 获取指定层
const std::list<Layer*>& getLayers() const;    // 获取所有层
int getLayerCount() const;                      // 获取层数

// 显示信息
void showInfo() const;               // 显示网络整体信息
void showLayer(int index) const;     // 显示指定层信息
void showLayers() const;             // 显示所有层信息
```

### 2. Layer类 - 网络层类

#### 功能概述
`Layer`类管理神经网络中的一层神经元，提供层级连接、神经元管理、权重设置等功能。

#### 构造函数
```cpp
Layer(int index = 0, int neuronCount = 0,
      std::vector<double> biases = std::vector<double>(),
      int activationFunctionType = 0);
```

#### 神经元管理
```cpp
void addNeuron(const Neuron& neuron);       // 添加神经元
void deleteNeuron(int index);               // 删除神经元
const std::vector<Neuron>& getNeurons() const; // 获取所有神经元
const Neuron& getNeuron(int index) const;   // 获取指定神经元
int getNeuronCount() const;                  // 获取神经元数量
```

#### 层连接管理
```cpp
void connectTo(Layer* nextLayer);           // 连接到下一层
void disconnect();                          // 断开连接
void disconnectFrom();                      // 断开与前一层的连接
Layer* getPreviousLayer() const;            // 获取前一层
Layer* getNextLayer() const;                // 获取下一层
bool isConnectedTo(const Layer& other) const; // 检查是否连接
```

#### 数据处理
```cpp
void setInput(const std::vector<double>& input);    // 设置输入（仅第一层）
void updateOutputs();                               // 更新所有神经元输出
std::vector<double> getOutputs() const;             // 获取层输出
void setWeights(const std::vector<std::vector<double>>& weights); // 设置权重
```

### 3. Neuron类 - 神经元类

#### 功能概述
`Neuron`类继承自`Soma`类，实现神经网络中的单个神经元功能。

#### 构造函数
```cpp
Neuron(std::vector<Synapse*> pre = {}, 
       double bias = 0.0, 
       int activationFunctionType = 0,
       int layerIndex = 0, 
       int idx = 0);
```

#### 连接管理
```cpp
void connectTo(Neuron* other, double weight = 1.0); // 连接到其他神经元
void disconnectTo(Neuron* other);                   // 断开连接
bool isConnectedTo(const Neuron& other) const;      // 检查连接状态
void showConnections() const;                       // 显示连接信息
```

#### 权重管理
```cpp
std::vector<double> getWeights() const;             // 获取权重
void setWeights(const std::vector<double>& weights); // 设置权重
void setWeight(int index, double weight);           // 设置单个权重
```

#### 信号处理
```cpp
void updateInput();                                 // 更新输入信号
void updateOutput() override;                       // 更新输出信号
```

#### 属性访问
```cpp
int getIndex() const;                              // 获取神经元索引
int getLayerIndex() const;                         // 获取层索引
void setLayerIndex(int newLayerIndex);             // 设置层索引
void setIndex(int newIndex);                       // 设置索引
void setBias(double newBias);                      // 设置偏置
```

### 4. ActivationFunc类 - 激活函数类

#### 功能概述
提供神经网络中常用的激活函数实现。

#### 可用激活函数
```cpp
static double sigmoid(double x);     // Sigmoid函数: 1/(1+e^-x)
static double tanh(double x);        // 双曲正切函数: tanh(x)
static double relu(double x);        // ReLU函数: max(0,x)
static double linear(double x);      // 线性函数: x (默认)
```

#### 激活函数类型编码
- `0`: Linear (线性函数，默认)
- `1`: Sigmoid 
- `2`: Tanh (双曲正切)
- `3`: ReLU

### 5. ANNFilePorter类 - 文件导入导出类

#### ANNImporter类 - 导入器
```cpp
ANNImporter(const std::string& filename);
Network import();                           // 从ANN文件导入网络
```

#### ANNExporter类 - 导出器  
```cpp
ANNExporter(const std::string& filename);
void exportNetwork(const Network& network); // 导出网络到ANN文件
```

## ANN文件格式规范

### 文件结构
ANN文件采用文本格式，使用特定的标记来定义网络结构：

```
# 注释行以#开头
G NetworkName           # G: 网络名称
N bias activationType   # N: 神经元定义（偏置值 激活函数类型）
L startIndex endIndex   # L: 层定义（起始神经元索引 结束神经元索引）
S fromNeuron toNeuron weight # S: 突触连接（源神经元 目标神经元 权重）
```

### 示例ANN文件
```
# simple.ANN 
G RotationNetwork 
# Six Neurons: zero bias, without activation function 
N 0.0 0  
N 0.0 0  
N 0.0 0  
N 0.0 0  
N 0.0 0  
N 0.0 0  
# Layer 0: Neuron 0 to 2 
L 0 2  
# Layer 1: Neuron 3 to 5 
L 3 5  
# Neuron 0 to 2: has one Dendrite 
S -1 0 1.0  
S -1 1 1.0  
S -1 2 1.0  
# Neuron 3 to 5: has one Axon 
S 3 -1 1.0  
S 4 -1 1.0  
S 5 -1 1.0  
# Dendrites from Neuron 0 to Neuron 3~5 
S 0 3 0.3536  
S 0 4 -0.5732  
S 0 5 0.7392  
# 更多连接...
```

## 使用示例

### 基本使用流程

#### 1. 从文件导入网络
```cpp
#include "ANNFilePorter.hpp"
#include "Network.hpp"

// 导入网络
ANNImporter importer("simple.ANN");
Network network = importer.import();

// 查看网络信息
network.showInfo();
```

#### 2. 执行前向传播
```cpp
// 准备输入数据
std::vector<double> inputs = {0.5, 0.8, 0.3};

// 执行前向传播
auto outputs = network.forward(inputs);

// 获取最终输出
auto finalOutput = outputs.back();
```

#### 3. 手动创建网络
```cpp
// 创建新网络
Network network;

// 创建第一层（输入层）
std::vector<double> biases1 = {0.0, 0.0, 0.0};
Layer* layer1 = new Layer(0, 3, biases1, 0);
network.addLayer(layer1);

// 创建第二层（输出层）
std::vector<double> biases2 = {0.0, 0.0};
Layer* layer2 = new Layer(1, 2, biases2, 1); // 使用sigmoid激活函数
network.addLayer(layer2);

// 设置权重
std::vector<std::vector<double>> weights = {
    {0.5, 0.3, 0.2},  // 第一个神经元的权重
    {0.4, 0.6, 0.1}   // 第二个神经元的权重
};
network.setWeights(1, weights);
```

#### 4. 导出网络
```cpp
// 导出到文件
ANNExporter exporter("output.ANN");
exporter.exportNetwork(network);
```

### 网络动态修改

#### 添加层和神经元
```cpp
// 向网络添加新层
network.addLayer(2); // 在位置2添加新层

// 向指定层添加神经元
network.addNeuron(1, 0.5, 1); // 层索引1，偏置0.5，sigmoid激活函数
```

#### 删除操作
```cpp
// 删除层
network.deleteLayer(2);

// 删除神经元
network.deleteNeuron(1, 0); // 删除第1层的第0个神经元
```

#### 查看网络状态
```cpp
// 查看整体信息
network.showInfo();

// 查看特定层
network.showLayer(0);

// 查看神经元连接
network.getLayer(0)->getNeuron(0).showConnections();
```

## 注意事项

### 编码问题
在Windows PowerShell中运行时，建议设置UTF-8编码以避免显示错误：
```powershell
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
```

### 内存管理
- 网络析构时会自动释放所有层和神经元的内存
- 避免手动删除Layer*指针，由Network类管理

### 错误处理
- 所有公共接口都包含参数验证
- 使用异常处理机制报告错误
- 建议在生产环境中添加try-catch块

## 扩展开发

### 添加新的激活函数
1. 在`ActivationFunc`类中添加新的静态方法
2. 更新激活函数类型枚举
3. 在`Soma::updateOutput()`中添加新的case

### 添加新的文件格式支持
1. 继承`FilePorter`基类
2. 实现相应的导入/导出逻辑
3. 按照现有的ANNFilePorter模式组织代码

### 添加训练功能
当前版本主要支持推理，可以扩展添加：
- 反向传播算法
- 梯度下降优化器
- 损失函数计算
- 训练循环管理

## 许可证

本项目可在GitHub上获取：https://github.com/zgdllt/CANN

## 更新历史

- **v1.0** (2025-07-24): 初始版本发布
  - 完整的神经网络构建和推理功能
  - ANN文件格式支持
  - 多种激活函数支持
  - 网络结构动态修改功能
  - 完整的API文档

- 开发时间：2025年7月13日 - 2025年7月24日
