//-------------------------------------------------------------------------------------------------------------------
// 【文件名】Network.hpp
// 【功能模块和目的】神经网络类的声明，定义了完整的人工神经网络结构和管理功能
// 【开发者及日期】李孟涵 2025年7月13日
// 【更改记录】2025年7月21日 完善了网络层的添加、删除、前向传播等功能
// 【更改记录】2025年7月23日 增加拷贝构造函数和赋值运算符重载，使用addLayer等函数从基础结构重新构建网络，防止浅拷贝导致的指针错误
// 【更改记录】2025年7月24日 增加网络名称的设置和获取功能
// 【更改记录】2025年7月29日 删除私有成员变量layerCount，改为使用layers.size()获取层数
//-------------------------------------------------------------------------------------------------------------------

#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "Layer.hpp" // 层类所在头文件
#include <list>      // 链表所在头文件
#include <string>    // 字符串所在头文件

//-------------------------------------------------------------------------------------------------------------------
// 【类名】Network
// 【功能】管理完整的神经网络，包括层的添加删除、前向传播、权重设置、网络验证、文件导入导出等功能
// 【接口说明】提供网络构建、推理、持久化等完整的神经网络操作接口
//   - Network(): 默认构造函数，初始化网络层数为0
//   - Network(const Network& other): 拷贝构造函数
//   - Network& operator=(const Network& other): 赋值运算符重载
//   - void addLayer(Layer* layer): 添加新的网络层
//   - std::vector<std::vector<double>> forward(const std::vector<double>& inputs): 执行前向传播
//   - bool isValid() const: 验证网络结构合法性
//   - void setWeights(int layerIndex, const std::vector<std::vector<double>>& weights): 设置权重
//   - void deleteLayer(int index): 删除指定索引的网络层
//   - void addLayer(int index): 在指定索引处添加新层
//   - void addNeuron(int layerIndex, double bias, int activationType): 添加神经元
//   - void deleteNeuron(int layerIndex, int neuronIndex): 删除神经元
//   - void showLayer(int index) const: 显示指定层信息
//   - void showLayers() const: 显示所有层信息
//   - void setName(const std::string& name): 设置网络名称
//   - std::string getName() const: 获取网络名称
//   - void showInfo() const: 显示网络整体信息
//   - ~Network(): 析构函数
//   - const Layer* getLayer(int index) const: 获取指定层
//   - const std::list<Layer*>& getLayers() const: 获取所有层列表
//   - int getLayerCount() const: 获取网络层数
// 【开发者及日期】李孟涵 2025年7月13日
// 【更改记录】2025年7月21日 添加了网络层的添加、删除、前向传播等功能
// 【更改记录】2025年7月23日 增加拷贝构造函数和赋值运算符重载，使用addLayer等函数从基础结构重新构建网络，防止浅拷贝导致的指针错误
// 【更改记录】2025年7月24日 增加网络名称的设置和获取功能
// 【更改记录】2025年7月29日 删除私有成员变量layerCount，改为使用layers.size()获取层数
//-------------------------------------------------------------------------------------------------------------------
class Network {
public:
    Network();                                                   // 默认构造函数，初始化网络层数为0
    Network(const Network& other);                              // 拷贝构造函数，使用addLayer等函数从基础结构重新构建网络
    Network& operator=(const Network& other);                   // 赋值运算符重载，使用addLayer等函数从基础结构重新构建网络
    void addLayer(Layer* layer);                                // 添加新的网络层
    std::vector<std::vector<double>> forward(const std::vector<double>& inputs); // 执行神经网络的前向传播，计算每一层的输出
    bool isValid() const;                                       // 验证网络结构的合法性
    void setWeights(int layerIndex = 0, const std::vector<std::vector<double>>& weights = {}); // 设置指定层的权重
    void deleteLayer(int index);                                // 删除指定索引的网络层
    void addLayer(int index);                                   // 在指定索引处添加新的网络层
    void addNeuron(int layerIndex, double bias = 0.0, int activationType = 0); // 向指定层添加一个新的神经元
    void deleteNeuron(int layerIndex, int neuronIndex);         // 删除指定层的指定神经元
    void showLayer(int index) const;                            // 显示指定层的详细信息
    void showLayers() const;                                    // 显示所有层的详细信息
    void setName(const std::string& name);                      // 设置网络名称
    std::string getName() const;                                // 获取网络名称
    void showInfo() const;                                      // 显示网络的整体信息
    ~Network();                                                 // 析构函数，释放网络层内存
    const Layer* getLayer(int index) const;                     // 获取指定索引的网络层
    const std::list<Layer*>& getLayers() const;                 // 获取所有网络层的列表
    int getLayerCount() const;                                  // 获取网络层数
private:                                    // 记录网络层数
    std::list<Layer*> layers;                                   // 用链表存储所有网络层的指针
    std::string networkName;                                    // 网络名称
};

#endif // NETWORK_HPP
