//-------------------------------------------------------------------------------------------------------------------
//【文件名】Neuron.hpp
//【功能模块和目的】神经元类的声明，定义了人工神经网络中神经元的基本功能和接口
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

#ifndef NEURON_HPP
#define NEURON_HPP
#include "Soma.hpp"
#include "Synapse.hpp"
#include <vector>
#include <iostream>
#include <cmath>

//-------------------------------------------------------------------------------------------------------------------
//【类名】Neuron
//【功能】实现人工神经网络中的神经元，继承自Soma类，提供神经元的连接、权重设置、信号传播等功能
//【接口说明】提供构造函数、连接管理、权重设置、信号处理、层级管理等公有接口
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
class Neuron:public Soma{
    friend class Layer;  // 允许Layer类访问私有成员
public:
    Neuron(const Neuron& other) = default; // Copy constructor
    Neuron(std::vector<Synapse*> pre = {}, double bias = 0.0, int activationFunctionType = 0,int layerIndex = 0, int idx = 0);
    bool isConnectedTo(const Neuron& other) const;
    void connectTo(Neuron* other, double weight = 1.0);
    void disconnectTo(Neuron* other);
    std::vector<double> getWeights() const;
    void setWeights(const std::vector<double>& weights);
    void setWeight(int index, double weight);
    void showConnections() const;
    int getDendriteCount() const;
    int getIndex() const;
    int getLayerIndex() const;
    void setLayerIndex(int newLayerIndex);
    void setIndex(int newIndex);
    void setBias(double newBias);
    void remove();
    void updateInput();
    void updateOutput() override;
    virtual ~Neuron() = default;
private:
    std::vector<Synapse*> Dendrites;
    std::vector<Synapse*> Axon; // Axon is a Synapse that sends signals to other neurons
    int layerIndex; // Index of the layer this neuron belongs to
    int index; // Index of the neuron in its layer
};

#endif // NEURON_HPP