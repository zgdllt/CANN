//-------------------------------------------------------------------------------------------------------------------
//【文件名】Layer.cpp
//【功能模块和目的】神经网络层类的实现，包含神经网络层的所有功能实现
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】2025年7月21日 新增移除所有连接的功能
//-------------------------------------------------------------------------------------------------------------------

#include "Layer.hpp"      // 包含层类头文件
#include "Network.hpp"
#include "Synapse.hpp"    // 包含突触类头文件
#include <iostream>       // 输入输出流头文件
#include <vector>         // vector所在头文件
#include <stdexcept>      // 标准异常头文件

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Layer::Layer
//【函数功能】Layer类的构造函数，创建指定数量的神经元并初始化层属性
//【参数】index - 层索引，neuronCount - 神经元数量，biases - 偏置值向量，activationFunctionType - 激活函数类型
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
Layer::Layer(Network* network, int neuronCount, std::vector<double> biases, int activationFunctionType)
{
    this->network = network; // 设置所属网络
    this->previousLayer = nullptr;// 设置前一层为空
    this->nextLayer = nullptr;// 设置下一层为空
    if (biases.size() < neuronCount) {
        biases.resize(neuronCount, 0.0); // 确保偏置向量有足够的元素, 如果不足默认设置为0.0
    }
    for (size_t i = 0; i < neuronCount; ++i) {
        neurons.emplace_back(std::vector<Synapse*>(), biases[i], activationFunctionType, this);// 创建神经元并添加到当前层
    }
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Layer::addNeuron
//【函数功能】向层中添加一个新的神经元，并建立必要的连接
//【参数】neuron - 要添加的神经元对象
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Layer::addNeuron(const Neuron& neuron) {
    neurons.push_back(neuron);
    
    // 如果当前层不是第一层，需要与前一层的所有神经元建立连接
    if (previousLayer != nullptr) {
        Neuron& newNeuron = neurons.back(); // 获取刚添加的神经元的引用
        for (auto& prevNeuron : previousLayer->neurons) {
            prevNeuron.connectTo(&newNeuron, 1.0); // 默认权重为1.0
        }
    }
    
    // 如果当前层不是最后一层，需要与下一层的所有神经元建立连接
    if (nextLayer != nullptr) {
        Neuron& newNeuron = neurons.back(); // 获取刚添加的神经元的引用
        for (auto& nextNeuron : nextLayer->neurons) {
            newNeuron.connectTo(&nextNeuron, 1.0); // 默认权重为1.0
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Layer::deleteNeuron
//【函数功能】删除指定索引的神经元，先断开其所有连接再从容器中移除，并更新其他神经元的索引
//【参数】index - 要删除的神经元索引
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Layer::deleteNeuron(int index)
{
    if (index < 0 || index >= static_cast<int>(neurons.size())) {// 检查神经元索引是否在范围内
        std::cerr << "Error: Neuron index out of range.\n";
        throw std::out_of_range("Neuron index out of range");
    }
    
    // 获取要删除的神经元的指针（在删除前保存）
    Neuron* neuronToDelete = &neurons[index];
    
    // 清理所有指向该神经元的连接
    // 遍历前一层的所有神经元，移除指向要删除神经元的连接
    if (previousLayer != nullptr) {
        for (auto& prevNeuron : previousLayer->neurons) {
            // 清理轴突中指向该神经元的连接
            for (auto it = prevNeuron.Axon.begin(); it != prevNeuron.Axon.end();) {
                if ((*it)->getNxt() == neuronToDelete) {
                    delete *it;
                    it = prevNeuron.Axon.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }
    
    // 遍历后一层的所有神经元，移除来自该神经元的连接
    if (nextLayer != nullptr) {
        for (auto& nextNeuron : nextLayer->neurons) {
            // 清理树突中来自该神经元的连接
            for (auto it = nextNeuron.Dendrites.begin(); it != nextNeuron.Dendrites.end();) {
                if ((*it)->getPre() == neuronToDelete) {
                    delete *it;
                    it = nextNeuron.Dendrites.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }
    
    // 清理该神经元自身的连接
    for (auto* synapse : neurons[index].Axon) {
        delete synapse;
    }
    for (auto* synapse : neurons[index].Dendrites) {
        delete synapse;
    }
    neurons[index].Axon.clear();
    neurons[index].Dendrites.clear();
    
    // 删除神经元
    neurons.erase(neurons.begin() + index);
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Layer::getNeurons
//【函数功能】获取当前层的所有神经元
//【参数】无
//【返回值】const std::vector<Neuron>& - 当前层的神经元向量
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
const std::vector<Neuron>& Layer::getNeurons() const {
    return neurons;
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Layer::getNeuron
//【函数功能】获取指定索引的神经元
//【参数】index - 神经元的索引
//【返回值】const Neuron& - 指定索引的神经元
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
const Neuron& Layer::getNeuron(int index) const {
    if (index < 0 || index >= neurons.size()) {// 检查神经元索引是否在范围内
        std::cerr << "Error: Neuron index out of range.\n";
        throw std::out_of_range("Neuron index out of range");
    }
    return neurons[index];
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Layer::setWeights
//【函数功能】设置当前层神经元的权重
//【参数】weights - 权重矩阵
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Layer::setWeights(const std::vector<std::vector<double>>& weights) {
    if (weights.size() != neurons.size()) {// 检查权重矩阵的大小是否与神经元数量匹配
        std::cerr << "Error: Weights size does not match the number of neurons in the layer.\n";
        throw std::invalid_argument("Weights size does not match the number of neurons in the layer");
    }
    if (previousLayer == nullptr) {// 检查当前层是否为网络的第一层
        std::cerr << "Error: Cannot set weights for the first layer in the network.\n";
        throw std::runtime_error("Cannot set weights: Layer is the first layer in the network");
    }
    for (size_t i = 0; i < neurons.size(); ++i) {
        neurons[i].setWeights(weights[i]);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Layer::getNeuronCount
//【函数功能】获取当前层的神经元数量
//【参数】无
//【返回值】int - 神经元数量
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
int Layer::getNeuronCount() const {
    return neurons.size();
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Layer::setBias
//【函数功能】设置指定神经元的偏置值
//【参数】neuronIndex - 神经元索引，newBias - 新的偏置值
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Layer::setBias(int neuronIndex, double newBias) {
    if (neuronIndex < 0 || neuronIndex >= neurons.size()) {// 检查神经元索引是否在范围内
        std::cerr << "Error: Neuron index out of range.\n";
        throw std::out_of_range("Neuron index out of range");
    }
    neurons[neuronIndex].setBias(newBias);
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Layer::setInput
//【函数功能】设置当前层的输入值
//【参数】input - 输入值向量
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Layer::setInput(const std::vector<double>& input) {
    if (input.size() != neurons.size()) {// 检查输入大小是否与神经元数量匹配
        std::cerr << "Error: Input size does not match the number of neurons in the layer.\n";
        throw std::invalid_argument("Input size does not match the number of neurons in the layer");
    }
    if (previousLayer) {// 检查当前层是否为网络的第一层
        std::cerr << "Error: Cannot set input for a layer that is not the first layer in the network.\n";
        throw std::runtime_error("Cannot set input: Layer is not the first layer in the network");
    }
    for (size_t i = 0; i < neurons.size(); ++i) {
        neurons[i].addInput(input[i]);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Layer::isConnectedTo
//【函数功能】检查当前层是否与另一层连接
//【参数】other - 另一层对象
//【返回值】bool - 是否连接
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】2025年7月24日 简化if语句为表达式
//-------------------------------------------------------------------------------------------------------------------
bool Layer::isConnectedTo(const Layer& other) const {
    return this->nextLayer == &other || this->previousLayer == &other;
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Layer::printNeurons
//【函数功能】打印当前层所有神经元的信息
//【参数】无
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Layer::printNeurons() const {
    for (size_t i = 0; i < neurons.size(); ++i) {
        std::cout << "Neuron Index: " << i << ", bias: " << neurons[i].getBias() << "\n";
    }
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Layer::getPreviousLayer
//【函数功能】获取当前层的前一层
//【参数】无
//【返回值】Layer* - 前一层的指针
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
Layer* Layer::getPreviousLayer() const {
    return previousLayer;
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Layer::getNextLayer
//【函数功能】获取当前层的下一层
//【参数】无
//【返回值】Layer* - 下一层的指针
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
Layer* Layer::getNextLayer() const {
    return nextLayer;
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Layer::setPreviousLayer
// 【函数功能】设置当前层的前一层
// 【参数】previousLayer - 前一层的指针
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月13日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Layer::setPreviousLayer(Layer* previousLayer) {
    this->previousLayer = previousLayer;
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Layer::setNextLayer
// 【函数功能】设置当前层的下一层
// 【参数】nextLayer - 下一层的指针
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月13日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Layer::setNextLayer(Layer* nextLayer) {
    this->nextLayer = nextLayer;
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Layer::disconnect
//【函数功能】断开当前层与下一层的连接
//【参数】无
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Layer::disconnect()
{
    if (this->nextLayer != nullptr) {
        // 先断开神经元级别的连接
        for (auto& neuron : neurons) {
            for (auto& nextNeuron : this->nextLayer->getNeurons()) {
                neuron.disconnectTo(const_cast<Neuron*>(&nextNeuron));
            }
            neuron.Axon.clear(); // 清除当前神经元的轴突连接
        }
        // 然后断开层级别的连接
        this->nextLayer->previousLayer = nullptr;
        this->nextLayer = nullptr;
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Layer::disconnectFrom
// 【函数功能】断开当前层与前一层的连接
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月13日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Layer::disconnectFrom()
{
    if (this->previousLayer != nullptr) {
        // 先断开神经元级别的连接
        for (auto& neuron : neurons) {
            for (auto& prevNeuron : this->previousLayer->getNeurons()) {
                neuron.disconnectTo(const_cast<Neuron*>(&prevNeuron));
            }
            neuron.Dendrites.clear(); // 清除当前神经元的树突连接
        }
        // 然后断开层级别的连接
        this->previousLayer->nextLayer = nullptr;
        this->previousLayer = nullptr;
    }
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Layer::connectTo
//【函数功能】连接当前层到指定的下一层
//【参数】newNextLayer - 指定的下一层指针
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Layer::connectTo(Layer* newNextLayer) {
    this->disconnect();// 先断开当前层与下一层的连接
    this->nextLayer = newNextLayer;
    if (newNextLayer != nullptr) {
        newNextLayer->previousLayer = this;
        for (auto& neuron : neurons) {
            for (auto& nextNeuron : newNextLayer->getNeurons()) {
                neuron.connectTo(const_cast<Neuron*>(&nextNeuron));//需要修改下一层神经元的树突，所以使用const_cast修饰符
            }
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Layer::updateOutputs
// 【函数功能】更新当前层所有神经元的输出值
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月13日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Layer::updateOutputs() {
    for (auto& neuron : neurons) {
        neuron.updateOutput();
    }
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Layer::getOutputs
//【函数功能】获取当前层所有神经元的输出值
//【参数】无
//【返回值】std::vector<double> - 当前层的输出值向量
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
std::vector<double> Layer::getOutputs() const {
    std::vector<double> outputs;// 当前层的输出值向量
    for (const auto& neuron : neurons) {
        outputs.push_back(neuron.getOutput());// 获取每个神经元的输出值并添加到向量中
    }
    return outputs;
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Layer::removeAllConnections
// 【函数功能】清除当前层所有神经元的连接
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月21日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Layer::removeAllConnections() {
    // 清除当前层所有神经元的连接
    for (auto& neuron : neurons) {
        neuron.remove();
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Layer::setNetwork
// 【函数功能】设置当前层所属的网络
// 【参数】newNetwork - 新的网络指针
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月29日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Layer::setNetwork(Network* newNetwork) {
    network = newNetwork;
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Layer::getIndex
// 【函数功能】获取当前层在网络中的索引
// 【参数】无
// 【返回值】int - 当前层的索引
// 【开发者及日期】李孟涵 2025年7月29日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
int Layer::getIndex() const {
    if (network == nullptr) {
        std::cerr << "Error: Network is not set for this layer.\n";
        throw std::runtime_error("Network is not set for this layer");
    }
    int index = 0;
    for (auto it = network->getLayers().begin(); it != network->getLayers().end(); ++it, ++index) {
        if (*it == this) {
            return index; // 返回当前层在网络中的索引
        }
    }
    return -1; // 如果未找到，返回-1表示错误
}