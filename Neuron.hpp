//-------------------------------------------------------------------------------------------------------------------
//【文件名】Neuron.hpp
//【功能模块和目的】神经元类的声明，定义了人工神经网络中神经元的基本功能和接口
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】2025年7月13日 不将树突轴突作为两个类
//-------------------------------------------------------------------------------------------------------------------

#ifndef NEURON_HPP
#define NEURON_HPP
#include "Soma.hpp" //细胞体类的头文件
#include "Synapse.hpp" // 突触类的头文件
#include <vector> //vector所属头文件

//-------------------------------------------------------------------------------------------------------------------
//【类名】Neuron
//【功能】实现人工神经网络中的神经元，继承自Soma类，提供神经元的连接、权重设置、信号传播等功能
//【接口说明】提供构造函数、连接管理、权重设置、信号处理、层级管理等公有接口
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】2025年7月13日 不将树突轴突作为两个类
//-------------------------------------------------------------------------------------------------------------------
class Neuron:public Soma{ // 继承自Soma类，提供神经元的基本功能
    friend class Layer;                                      // 允许Layer类访问私有成员
public:
    Neuron(const Neuron& other) = default;                  // 复制构造函数
    Neuron(std::vector<Synapse*> pre = {}, 
           double bias = 0.0, 
           int activationFunctionType = 0,
           int layerIndex = 0, 
           int idx = 0);                                    // 构造函数，初始化神经元的基本属性和连接
    bool isConnectedTo(const Neuron& other) const;          // 判断当前神经元是否与另一个神经元连接
    void connectTo(Neuron* other, double weight = 1.0);     // 将当前神经元连接到另一个神经元
    void disconnectTo(Neuron* other);                      // 断开与另一个神经元的连接
    std::vector<double> getWeights() const;                // 获取当前神经元的树突权重
    void setWeights(const std::vector<double>& weights);   // 设置当前神经元的树突权重
    void setWeight(int index, double weight);              // 设置特定树突的权重
    void showConnections() const;                          // 显示当前神经元的连接信息
    int getDendriteCount() const;                           // 获取当前神经元的树突数量
    int getAxonCount() const;                               // 获取当前神经元的轴突数量
    int getIndex() const;                                  // 获取当前神经元的索引
    int getLayerIndex() const;                             // 获取当前神经元的层索引
    void setLayerIndex(int newLayerIndex);                 // 设置当前神经元的层索引
    void setIndex(int newIndex);                           // 设置当前神经元的索引
    void setBias(double newBias);                          // 设置当前神经元的偏置
    void remove();                                         // 移除当前神经元
    void cleanInvalidSynapses(const std::vector<Neuron*>& invalidNeurons); // 清理无效的突触连接
    void updateInput();                                    // 更新当前神经元的输入
    void updateOutput() override;                         // 更新当前神经元的输出
    virtual ~Neuron() = default;                          // 默认析构函数
private:
    std::vector<Synapse*> Dendrites;                      // 树突是突触的一种，用于接收其他神经元的信号
    std::vector<Synapse*> Axon;                           // 轴突是突触的一种，用于向其他神经元发送信号
    int layerIndex;                                       // 当前神经元的层索引
    int index;                                            // 当前神经元在其层中的索引
};

#endif // NEURON_HPP