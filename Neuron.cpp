//-------------------------------------------------------------------------------------------------------------------
// 【文件名】Neuron.cpp
// 【功能模块和目的】神经元类的实现，包含神经元的所有功能实现
// 【开发者及日期】李孟涵 2025年7月13日
// 【更改记录】2025年7月13日 不将树突轴突作为两个类 2025年7月21日 新增去除无效连接功能
// 【更改记录】2025年7月29日 删除索引变量，改为基于网络内位置的动态计算
//-------------------------------------------------------------------------------------------------------------------

#include "Neuron.hpp"     // 神经元类头文件
#include "Soma.hpp"       // 胞体类头文件
#include "Synapse.hpp"    // 突触类头文件
#include "Layer.hpp"      // 层类头文件
#include <vector>         // vector所在头文件
#include <algorithm>      // 算法库
#include <stdexcept>      // 异常处理头文件
#include <iostream>       // 输入输出流头文件

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::Neuron
//【函数功能】Neuron类的构造函数，初始化神经元的基本属性和连接
//【参数】pre - 前驱突触指针向量，bias - 偏置值，activationFunctionType - 激活函数类型，layerIndex - 层索引，index - 神经元索引
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】2025年7月29日 删除索引变量
//-------------------------------------------------------------------------------------------------------------------
Neuron::Neuron(std::vector<Synapse*> pre, double bias, int activationFunctionType, Layer* layer)
    : Soma({},bias, activationFunctionType), layer(layer)
{
    for (auto& synapse : pre) {
        synapse->setNxt(this);                      // 设置突触的下一个神经元为当前神经元
        Dendrites.push_back(synapse);          // 将突触添加到树突中
        Soma::addInput(synapse->getSignal());// 添加突触信号到胞体输入
    }
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::isConnectedTo
//【函数功能】判断当前神经元是否与另一个神经元连接
//【参数】other - 另一个神经元的引用
//【返回值】bool - 是否连接
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
bool Neuron::isConnectedTo(const Neuron& other) const{
    // 检查当前神经元的树突是否连接到另一个神经元
    for (const auto& dendrite : Dendrites) {
        if (dendrite->getPre() == &other) {
            return true;
        }
    }
    // 检查是否有其他神经元的树突连接到当前神经元
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
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】2025年7月29日 删除索引变量，改为基于层内位置的动态计算
//-------------------------------------------------------------------------------------------------------------------
void Neuron::connectTo(Neuron* other, double weight)
{
    if (layer == nullptr) {
        std::cerr << "Cannot connect: this neuron has not been attached to a layer. ";
        throw std::runtime_error("Cannot connect: layer is null.");
                return; // 如果神经元未附加到层，直接返回
    }
    try {
        if (other != nullptr && other != this && other->layer == layer->getNextLayer() && !isConnectedTo(*other)) {// 确保连接的神经元在前一层且未连接
            // 创建新的突触并连接
            Synapse* synapse = new Synapse(Soma::getOutput(), weight, this, other);// 突触的输出来自当前神经元的胞体
            other->Dendrites.push_back(synapse);                                              // 将突触添加到目标神经元的树突中
            Axon.push_back(synapse);                                                          // 将突触添加到当前神经元的轴突中
        }
        else {// 如果神经元无效或已连接，输出错误信息并抛出异常
            std::cerr << "Cannot connect: invalid neuron or already connected. "
                    << "From Neuron(layer=" << getPosition().first << ", index=" << getPosition().second << ") "
                    << "To Neuron(layer=" << (other ? other->getPosition().first : -1) << ", index=" << (other ? other->getPosition().second : -1) << ")\n";
            throw std::runtime_error("Cannot connect: invalid neuron or already connected.");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error occurred while connecting neurons: " << e.what() << std::endl;
        throw;
    }
    
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::disconnectTo
//【函数功能】断开当前神经元与另一个神经元的连接
//【参数】other - 目标神经元指针
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Neuron::disconnectTo(Neuron* other) {
    if (other != nullptr && isConnectedTo(*other)) { // 确保断开连接的神经元存在且已连接
        // 遍历目标神经元的树突，找到与当前神经元连接的突触
        for (auto it = other->Dendrites.begin(); it != other->Dendrites.end(); ++it) {
            if ((*it)->getPre() == this) {
                Axon.erase(std::remove(Axon.begin(), Axon.end(), *it), Axon.end());// 从当前神经元的轴突中移除突触
                delete *it; // 释放突触内存
                other->Dendrites.erase(it);// 从目标神经元的树突中移除突触
                return;
            }
        }
    } else {// 如果神经元无效或未连接，输出错误信息并抛出异常
        std::cerr << "Cannot disconnect: invalid neuron or not connected. "
                  << "From Neuron(layer=" << getPosition().first << ", index=" << getPosition().second << ") "
                  << "To Neuron(layer=" << (other ? other->getPosition().first : -1) << ", index=" << (other ? other->getPosition().second : -1) << ")\n";
        throw std::runtime_error("Cannot disconnect: invalid neuron or not connected.");
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Neuron::getPosition
// 【函数功能】获取当前神经元在网络内的位置（索引）
// 【参数】无
// 【返回值】std::pair<int, int> - 神经元所在层的索引和在层内的索引
// 【开发者及日期】李孟涵 2025年7月29日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
std::pair<int, int> Neuron::getPosition() const {
    // 获取当前神经元在网络中的位置
    if (layer == nullptr) {
        std::cerr << "Error: Neuron is not attached to a layer.\n";
        return {-1, -1}; // 如果神经元未附加到层，返回无效位置
    }
    // 获取当前神经元在层中的索引
    int indexInLayer = 0;
    for (const auto& neuron : layer->getNeurons()) {
        if (&neuron == this) {
            break; // 找到当前神经元在层中的索引
        }
        indexInLayer++;
    }
    return {layer->getIndex(), indexInLayer}; // 返回神经元在层中的索引
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Neuron::getDendriteCount
// 【函数功能】获取神经元的树突数量
// 【参数】无
// 【返回值】int - 神经元的树突数量
// 【开发者及日期】李孟涵 2025年7月13日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
int Neuron::getDendriteCount() const {
    return Dendrites.size();
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Neuron::getAxonCount
// 【函数功能】获取神经元的轴突数量
// 【参数】无
// 【返回值】int - 神经元的轴突数量
// 【开发者及日期】李孟涵 2025年7月13日
// 【更改记录】无
//---------------------------------------------------------------------------------------------------------
int Neuron::getAxonCount() const {
    return Axon.size();
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::getWeights
//【函数功能】获取当前神经元所有突触的权重
//【参数】无
//【返回值】std::vector<double> - 突触权重的向量
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
std::vector<double> Neuron::getWeights() const {
    std::vector<double> weights;
    for (const auto& dendrite : Dendrites) {
        weights.push_back(dendrite->getWeight());// 收集所有树突的权重
    }
    return weights;
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Neuron::setWeights
// 【函数功能】设置当前神经元的突触权重
// 【参数】weights - 突触权重的向量
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月13日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Neuron::setWeights(const std::vector<double>& weights) {
    if (weights.size() != Dendrites.size()) {// 确保权重向量的大小与树突数量匹配
        std::cerr << "Error: Weights size does not match the number of dendrites.\n";
        throw std::invalid_argument("Weights size does not match the number of dendrites.");
    }
    for (size_t i = 0; i < Dendrites.size(); ++i) {
        Dendrites[i]->setWeight(weights[i]);
    }
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::setBias
//【函数功能】设置神经元的偏置值
//【参数】newBias - 新的偏置值
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
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
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Neuron::remove() {
    // 删除所有输出连接（轴突）
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
    // 删除所有输入连接（树突）
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
//【函数名称】Neuron::cleanInvalidSynapses
//【函数功能】清理指向无效神经元的突触连接
//【参数】invalidNeurons - 无效神经元的指针列表
//【返回值】无
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Neuron::cleanInvalidSynapses(const std::vector<Neuron*>& invalidNeurons) {
    // 清理轴突中指向无效神经元的突触
    for (auto axonIt = Axon.begin(); axonIt != Axon.end();) {
        Synapse* synapse = *axonIt;
        if (synapse && synapse->getNxt()) {
            bool isInvalid = false;
            for (const auto* invalidNeuron : invalidNeurons) {
                if (synapse->getNxt() == invalidNeuron) {
                    isInvalid = true;
                    break;
                }
            }
            if (isInvalid) {
                delete synapse;
                axonIt = Axon.erase(axonIt);
            } else {
                ++axonIt;
            }
        } else {
            ++axonIt;
        }
    }
    // 清理树突中来自无效神经元的突触
    for (auto dendriteIt = Dendrites.begin(); dendriteIt != Dendrites.end();) {
        Synapse* synapse = *dendriteIt;
        if (synapse && synapse->getPre()) {
            bool isInvalid = false;
            for (const auto* invalidNeuron : invalidNeurons) {
                if (synapse->getPre() == invalidNeuron) {
                    isInvalid = true;
                    break;
                }
            }
            if (isInvalid) {
                delete synapse;
                dendriteIt = Dendrites.erase(dendriteIt);
            } else {
                ++dendriteIt;
            }
        } else {
            ++dendriteIt;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::updateInput
//【函数功能】更新神经元的输入信号
//【参数】无
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Neuron::updateInput() {
    std::vector<double> inputs;
    for (const auto& dendrite : Dendrites) {
        dendrite->setInput(dendrite->getPre() != nullptr ? dendrite->getPre()->getOutput() : 0.0);// 设置树突输入信号
    }
    for (const auto& dendrite : Dendrites) {
        inputs.push_back(dendrite->getSignal());// 收集所有树突的输入信号
    }
    Soma::setInputs(inputs); // 设置细胞体输入信号
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Neuron::updateOutput
//【函数功能】更新神经元的输出信号
//【参数】无
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Neuron::updateOutput() {
    if (getPosition().first != 0) {
        updateInput(); // 更新树突输入信号
    }
    Soma::updateOutput(); // 更新细胞体输出信号
    for (auto& synapse : Axon) {
        synapse->setInput(Soma::getOutput());
    }// 更新轴突信号，基于细胞体输出
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Neuron::showConnections
// 【函数功能】显示当前神经元的连接信息
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月13日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Neuron::showConnections() const {
    auto position = getPosition();
    std::cout << "Neuron at layer " << position.first << ", index " << position.second << ":\n";
    std::cout << "  Dendrites (incoming connections):\n";
    for (const auto& dendrite : Dendrites) {
        if (dendrite->getPre()) {
            auto prePosition = dendrite->getPre()->getPosition();
            std::cout << "    From Neuron(layer=" << prePosition.first
                      << ", index=" << prePosition.second << ") with weight "
                      << dendrite->getWeight() << "\n";
        }
    }// 输出当前神经元的树突连接信息
    std::cout << "  Axon (outgoing connections):\n";
    for (const auto& synapse : Axon) {
        if (synapse->getNxt()) {
            auto nxtPosition = synapse->getNxt()->getPosition();
            std::cout << "    To Neuron(layer=" << nxtPosition.first
                      << ", index=" << nxtPosition.second << ") with weight "
                      << synapse->getWeight() << "\n";
        }
    }// 输出当前神经元的轴突连接信息
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Neuron::setLayer
// 【函数功能】设置当前神经元所在的层
// 【参数】newLayer - 新的层指针
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月29日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Neuron::setLayer(Layer* newLayer) {
    layer = newLayer;
}