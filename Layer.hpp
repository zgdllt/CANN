//-------------------------------------------------------------------------------------------------------------------
//【文件名】Layer.hpp
//【功能模块和目的】神经网络层类的声明，定义了人工神经网络中一层神经元的组织和管理功能
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

#ifndef LAYER_HPP
#define LAYER_HPP

#include "Neuron.hpp"// 包含神经元类的头文件
#include <vector>// vector所在头文件
class Network;
//-------------------------------------------------------------------------------------------------------------------
// 【类名】Layer
// 【功能】管理神经网络中的一层神经元，提供层级连接、神经元管理、权重设置、信号传播等功能
// 【接口说明】提供神经元添加、层连接管理、权重设置、输入输出处理等公有接口
// 【开发者及日期】李孟涵 2025年7月13日
// 【更改记录】2025年7月21日 新增删除所有连接功能，便于network中deleteLayer实现
// 【更改记录】2025年7月29日 删除索引变量，改为基于网络内位置的动态计算
//-------------------------------------------------------------------------------------------------------------------
class Layer{
public:
    Layer(Network* network, int neuronCount = 0,
          std::vector<double> biases = std::vector<double>(),
          int activationFunctionType = 0);               // 构造函数，创建指定数量的神经元并初始化层属性
    void addNeuron(const Neuron& neuron);                // 向层中添加一个新的神经元，并建立必要的连接
    void deleteNeuron(int index);                        // 删除指定索引的神经元
    void printNeurons() const;                           // 打印当前层的所有神经元信息
    const std::vector<Neuron>& getNeurons() const;       // 获取当前层的神经元列表
    const Neuron& getNeuron(int index) const;            // 获取指定索引的神经元
    int getNeuronCount() const;                          // 获取当前层的神经元数量
    int getIndex() const;                                // 获取当前层的索引
    void setBias(int neuronIndex, double newBias);       // 设置指定神经元的偏置值
    bool isConnectedTo(const Layer& other) const;        // 判断当前层是否与另一层连接
    Layer* getPreviousLayer() const;                     // 获取前一层
    Layer* getNextLayer() const;                         // 获取下一层
    void setNetwork(Network* network);                   // 设置所属网络
    void setPreviousLayer(Layer* previousLayer);         // 设置前一层
    void setNextLayer(Layer* nextLayer);                 // 设置下一层
    void disconnect();                                   // 断开当前层与下一层的连接
    void disconnectFrom();                               // 断开当前层与前一层的连接
    void connectTo(Layer* nextLayer);                    // 将当前层连接到下一层
    void setWeights(const std::vector<std::vector<double>>& weights); // 设置当前层所有神经元的权重
    void setInput(const std::vector<double>& input);      // 设置当前层的输入值
    void updateOutputs();                                // 更新当前层所有神经元的输出值
    void removeAllConnections();                        // 移除当前层所有神经元的连接
    std::vector<double> getOutputs() const;             // 获取当前层所有神经元的输出值
private:
    Network* network;                                   // 所属网络的指针
    Layer* previousLayer;                                // 前一层的指针
    Layer* nextLayer;                                    // 下一层的指针
    std::vector<Neuron> neurons;                        // 当前层的神经元列表
};
#endif // LAYER_HPP