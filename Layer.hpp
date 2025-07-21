//-------------------------------------------------------------------------------------------------------------------
//【文件名】Layer.hpp
//【功能模块和目的】神经网络层类的声明，定义了人工神经网络中一层神经元的组织和管理功能
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

#ifndef LAYER_HPP
#define LAYER_HPP

#include "Neuron.hpp"
#include <vector>

//-------------------------------------------------------------------------------------------------------------------
//【类名】Layer
//【功能】管理神经网络中的一层神经元，提供层级连接、神经元管理、权重设置、信号传播等功能
//【接口说明】提供神经元添加、层连接管理、权重设置、输入输出处理等公有接口
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
class Layer{
public:
    Layer(int index=0,int neuronCount=0, std::vector<double> biases=std::vector<double>(), int activationFunctionType=0);
    void addNeuron(const Neuron& neuron);
    void deleteNeuron(int index);
    void printNeurons() const;
    const std::vector<Neuron>& getNeurons() const;
    const Neuron& getNeuron(int index) const;
    int getNeuronCount() const;
    void setIndex(int newIndex);
    void setBias(int neuronIndex, double newBias);
    int getIndex() const;
    bool isConnectedTo(const Layer& other) const;
    Layer* getPreviousLayer() const;
    Layer* getNextLayer() const;
    void disconnect();
    void connectTo(Layer* nextLayer);
    void setWeights(const std::vector<std::vector<double>>& weights);
    void setInput(const std::vector<double>& input);
    void updateOutputs();
    std::vector<double> getOutputs() const;
private:
    int index; // Index of the layer in the network
    Layer* previousLayer;
    Layer* nextLayer;
    std::vector<Neuron> neurons;
};
#endif // LAYER_HPP