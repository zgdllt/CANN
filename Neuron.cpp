//-------------------------------------------------------------------------------------------------------------------
//【文件名】Neuron.cpp
//【功能模块和目的】神经元类的实现，包含神经元的所有功能实现
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

#include "Neuron.hpp"
#include "Soma.hpp"
#include "Synapse.hpp"
#include <vector>
#include <algorithm>

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::Neuron
//【函数功能】Neuron类的构造函数，初始化神经元的基本属性和连接
//【参数】pre - 前驱突触指针向量，bias - 偏置值，activationFunctionType - 激活函数类型，layerIndex - 层索引，index - 神经元索引
//【返回值】无
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
Neuron::Neuron(std::vector<Synapse*> pre, double bias, int activationFunctionType, int layerIndex, int index)
    : Soma({},bias, activationFunctionType), layerIndex(layerIndex), index(index)
{
    for (auto& synapse : pre) {
        synapse->setNxt(this);
        Dendrites.push_back(synapse);
        Soma::addInput(synapse->getSignal());
    }
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::isConnectedTo
//【函数功能】判断当前神经元是否与另一个神经元连接
//【参数】other - 另一个神经元的引用
//【返回值】bool - 是否连接
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
bool Neuron::isConnectedTo(const Neuron& other) const {
    for (const auto& dendrite : Dendrites) {
        if (dendrite->getPre() == &other) {
            return true;
        }
    }
    for (const auto& dendrite : other.Dendrites) {
        if (dendrite->getPre() == this) {
            return true;
        }
    }
    return false;
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::connectTo
//【函数功能】将当前神经元连接到另一个神经元
//【参数】other - 目标神经元指针，weight - 突触权重
//【返回值】无
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Neuron::connectTo(Neuron* other, double weight) {
    if (other != nullptr && other->layerIndex - 1 == layerIndex && !isConnectedTo(*other)) {
        Synapse* synapse = new Synapse(Soma::getOutput(), weight, this, other);
        other->Dendrites.push_back(synapse);
        Axon.push_back(synapse);
    }
    else {
        std::cerr << "Cannot connect: invalid neuron or already connected. "
                  << "From Neuron(layer=" << layerIndex << ", index=" << index << ") "
                  << "To Neuron(layer=" << (other ? other->layerIndex : -1) << ", index=" << (other ? other->index : -1) << ")\n";
        throw std::runtime_error("Cannot connect: invalid neuron or already connected.");
    }
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::disconnectTo
//【函数功能】断开当前神经元与另一个神经元的连接
//【参数】other - 目标神经元指针
//【返回值】无
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Neuron::disconnectTo(Neuron* other) {
    if (other != nullptr && isConnectedTo(*other)) {
        for (auto it = other->Dendrites.begin(); it != other->Dendrites.end(); ++it) {
            if ((*it)->getPre() == this) {
                Axon.erase(std::remove(Axon.begin(), Axon.end(), *it), Axon.end());
                delete *it; // Free memory
                other->Dendrites.erase(it);
                return;
            }
        }
    } else {
        std::cerr << "Cannot disconnect: invalid neuron or not connected. "
                  << "From Neuron(layer=" << layerIndex << ", index=" << index << ") "
                  << "To Neuron(layer=" << (other ? other->layerIndex : -1) << ", index=" << (other ? other->index : -1) << ")\n";
        throw std::runtime_error("Cannot disconnect: invalid neuron or not connected.");
    }
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::getWeights
//【函数功能】获取当前神经元所有突触的权重
//【参数】无
//【返回值】std::vector<double> - 突触权重的向量
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
std::vector<double> Neuron::getWeights() const {
    std::vector<double> weights;
    for (const auto& dendrite : Dendrites) {
        weights.push_back(dendrite->getWeight());
    }
    return weights;
//【函数功能】设置神经元在层内的索引
//【参数】newIndex - 新的索引值
//【返回值】无
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Neuron::setIndex(int newIndex) {
    index = newIndex;
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::setLayerIndex
//【函数功能】设置神经元所在层的索引
//【参数】newLayerIndex - 新的层索引值
//【返回值】无
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Neuron::setLayerIndex(int newLayerIndex) {
    layerIndex = newLayerIndex;
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::setBias
//【函数功能】设置神经元的偏置值
//【参数】newBias - 新的偏置值
//【返回值】无
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Neuron::setBias(double newBias) {
    Soma::setBias(newBias);
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::remove
//【函数功能】移除当前神经元的所有连接
//【参数】无
//【返回值】无
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Neuron::remove() {
    // Remove all outgoing connections (Axon)
    for (auto* synapse : Axon) {
        if (synapse) {
            Neuron* postNeuron = synapse->getNxt();
            if (postNeuron) {
                auto& dendrites = postNeuron->Dendrites;
                dendrites.erase(std::remove(dendrites.begin(), dendrites.end(), synapse), dendrites.end());
            }
            delete synapse;
        }
    }
    Axon.clear();
    // Remove all incoming connections (Dendrites)
    for (auto* synapse : Dendrites) {
        if (synapse) {
            Neuron* preNeuron = synapse->getPre();
            if (preNeuron) {
                preNeuron->Axon.erase(std::remove(preNeuron->Axon.begin(), preNeuron->Axon.end(), synapse), preNeuron->Axon.end());
            }
            delete synapse;
        }
    }
    Dendrites.clear();
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::updateInput
//【函数功能】更新神经元的输入信号
//【参数】无
//【返回值】无
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Neuron::updateInput() {
    std::vector<double> inputs;
    for (const auto& dendrite : Dendrites) {
        dendrite->setInput(dendrite->getPre() != nullptr ? dendrite->getPre()->getOutput() : 0.0);
    }
    for (const auto& dendrite : Dendrites) {
        inputs.push_back(dendrite->getSignal());
    }
    Soma::setInputs(inputs); // Set the inputs to Soma
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::updateOutput
//【函数功能】更新神经元的输出信号
//【参数】无
//【返回值】无
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Neuron::updateOutput() {
    if (layerIndex != 0) {
        updateInput(); // Update inputs from Dendrites
    }
    Soma::updateOutput(); // Update the Soma output
    // Update Axon signals based on the Soma output
    for (auto& synapse : Axon) {
        synapse->setInput(Soma::getOutput());
    }
}