//-------------------------------------------------------------------------------------------------------------------
// 【文件名】Network.cpp
// 【功能模块和目的】神经网络类的实现，包含网络的创建、前向传播、权重设置等功能
// 【开发者及日期】李孟涵 2025年7月13日
// 【更改记录】2025年7月20日 完善了网络层的添加、删除等功能
// 【更改记录】2025年7月23日 增加拷贝构造函数和赋值运算符重载，使用addLayer等函数从基础结构重新构建网络，防止浅拷贝导致的指针错误
// 【更改记录】2025年7月24日 增加网络名称的设置和获取功能, 在showInfo中增加对网络名称和有效性的显示
//-------------------------------------------------------------------------------------------------------------------

#include "Network.hpp"  // 网络类头文件
#include "Layer.hpp"    // 层类所在头文件
#include "Neuron.hpp"   // 神经元类所在头文件
#include <iostream>     // 输入输出流头文件
#include <stdexcept>    // 标准异常头文件
#include <exception>    // 异常处理头文件

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Network::Network
//【函数功能】Network类的默认构造函数，初始化网络层数为0
//【参数】无
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】2025年7月24日：增加对网络名称的初始化
//-------------------------------------------------------------------------------------------------------------------
Network::Network() {
    layerCount = 0;
    networkName = "Untitled";
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Network::Network
// 【函数功能】Network类的拷贝构造函数，使用addLayer等函数从基础结构重新构建网络
// 【参数】other - 另一个Network对象的引用
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月21日
// 【更改记录】2025年7月23日：从基础结构拷贝构造，解决指针传递问题
// 【更改记录】2025年7月24日：增加对网络名称的拷贝
//-------------------------------------------------------------------------------------------------------------------
Network::Network(const Network& other) : layerCount(0), networkName(other.networkName) {
    // 逐层重新构建网络
    for (const auto* layer : other.layers) {
        // 收集该层神经元的偏置值
        std::vector<double> biases;
        const auto& neurons = layer->getNeurons();
        for (const auto& neuron : neurons) {
            biases.push_back(neuron.getBias());
        }
        
        // 使用第一个神经元的激活函数类型作为整层的激活函数
        int activationType = 0;
        if (!neurons.empty()) {
            activationType = neurons[0].getActivationFunctionType();
        }
        
        // 创建新层并添加到网络中
        Layer* newLayer = new Layer(layerCount, layer->getNeuronCount(), biases, activationType);
        addLayer(newLayer);
    }
    
    // 设置权重（跳过第一层，因为它没有输入权重）
    for (int layerIdx = 1; layerIdx < layerCount; ++layerIdx) {
        const auto* sourceLayer = other.getLayer(layerIdx);
        const auto& sourceNeurons = sourceLayer->getNeurons();
        
        // 构建权重矩阵
        std::vector<std::vector<double>> weights;
        for (const auto& neuron : sourceNeurons) {
            weights.push_back(neuron.getWeights());
        }
        
        // 设置权重
        try {
            setWeights(layerIdx, weights);
        } catch (const std::exception& e) {//增加异常处理
            std::cerr << "Warning: Failed to set weights for layer " << layerIdx 
                      << " during copy construction: " << e.what() << std::endl;
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Network::operator=
// 【函数功能】Network类的赋值运算符，使用addLayer等函数从基础结构重新构建网络
// 【参数】other - 另一个Network对象的引用
// 【返回值】Network& - 返回当前对象的引用
// 【开发者及日期】李孟涵 2025年7月21日
// 【更改记录】2025年7月23日：从基础结构拷贝构造，解决指针传递问题
// 【更改记录】2025年7月24日：增加网络名称的传递
//-------------------------------------------------------------------------------------------------------------------
Network& Network::operator=(const Network& other) {
    if (this != &other) {
        // 清理当前对象的资源
        for (auto* layer : layers) {
            delete layer;
        }
        layers.clear();
        layerCount = 0;
        networkName = other.networkName;  // 复制网络名称
        // 逐层重新构建网络
        for (const auto* layer : other.layers) {
            // 收集该层神经元的偏置值
            std::vector<double> biases;
            const auto& neurons = layer->getNeurons();
            for (const auto& neuron : neurons) {
                biases.push_back(neuron.getBias());
            }
            
            // 使用第一个神经元的激活函数类型作为整层的激活函数
            int activationType = 0;
            if (!neurons.empty()) {
                activationType = neurons[0].getActivationFunctionType();
            }
            
            // 创建新层并添加到网络中
            Layer* newLayer = new Layer(layerCount, layer->getNeuronCount(), biases, activationType);
            addLayer(newLayer);
        }
        
        // 设置权重（跳过第一层，因为它没有输入权重）
        for (int layerIdx = 1; layerIdx < layerCount; ++layerIdx) {
            const auto* sourceLayer = other.getLayer(layerIdx);
            const auto& sourceNeurons = sourceLayer->getNeurons();
            
            // 构建权重矩阵
            std::vector<std::vector<double>> weights;
            for (const auto& neuron : sourceNeurons) {
                weights.push_back(neuron.getWeights());
            }
            
            // 设置权重
            try {
                setWeights(layerIdx, weights);
            } catch (const std::exception& e) {
                std::cerr << "Warning: Failed to set weights for layer " << layerIdx 
                          << " during assignment: " << e.what() << std::endl;
            }
        }
    }
    return *this;
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Network::setWeights
//【函数功能】设置指定层的权重值。
//【参数】layerIndex - 要设置权重的层的索引, weights - 包含权重值的二维向量。
//【返回值】void - 无返回值。
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Network::setWeights(int layerIndex, const std::vector<std::vector<double>>& weights) {
    if (layerIndex < 0 || layerIndex >= layerCount) {// 检查层索引是否有效
        std::cerr << "Error: Layer index out of range.\n";
        throw std::out_of_range("Layer index out of range");
    }
    if (layerIndex == 0) {// 检查是否为第一层, 第一层作为输入层没有权重矩阵
        std::cerr << "Error: Cannot set weights for the first layer in the network.\n";
        throw std::runtime_error("Cannot set weights: Layer is the first layer in the network.");
    }
    auto it = layers.begin();
    std::advance(it, layerIndex);
    (*it)->setWeights(weights);
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Network::addLayer
//【函数功能】向神经网络中添加一个新的层
//【参数】layer - 指向要添加的层的指针
//【返回值】void - 无返回值
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】2025年7月20日 完善功能
//-------------------------------------------------------------------------------------------------------------------
void Network::addLayer(Layer* layer) {
    if (layer->getIndex() < 0 || layer->getIndex() > layerCount) {// 检查层索引是否有效
        std::cerr << "Error: Layer index out of range.\n";
        throw std::out_of_range("Layer index out of range");
    }
    if (layer->getIndex() == 0) {               // 如果是第一层，直接添加到前面
        if (!layers.empty()) {
            layer->connectTo(*layers.begin());
        }
        for (auto& lyr : layers) {
            lyr->setIndex(lyr->getIndex() + 1);
        }
        layers.push_front(layer);
    } else if (layer->getIndex() == layerCount) {// 如果是最后一层，直接添加到末尾
        layers.back()->connectTo(layer);
        layers.push_back(layer);
    } else {                                    // 如果是中间层，插入到指定位置
        auto it = layers.begin();
        std::advance(it, layer->getIndex() - 1);
        (*it)->connectTo(layer);    // 连接前一层
        layer->connectTo(*std::next(it));// 连接后一层
        for (auto iter = std::next(it); iter != layers.end(); ++iter) {
            (*iter)->setIndex((*iter)->getIndex() + 1);
        }
        layers.insert(it, layer);
    }
    layerCount++;
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Network::addNeuron
//【函数功能】向指定层添加一个新的神经元
//【参数】layerIndex - 层索引，bias - 偏置值，activationType - 激活函数类型
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Network::addNeuron(int layerIndex,double bias, int activationType) {
    if (layerIndex < 0 || layerIndex >= layerCount) {// 检查层索引是否有效
        std::cerr << "Error: Layer index out of range.\n";
        throw std::out_of_range("Layer index out of range");
    }
    
    // 获取指定层
    auto it = layers.begin();
    std::advance(it, layerIndex);
    Layer* layer = *it;
    
    // 创建新神经元
    int neuronIndex = layer->getNeuronCount();
    Neuron newNeuron({}, bias, activationType, layerIndex, neuronIndex);
    layer->addNeuron(newNeuron);
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Network::deleteNeuron
//【函数功能】删除指定层的指定神经元
//【参数】layerIndex - 层索引，neuronIndex - 神经元索引
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Network::deleteNeuron(int layerIndex, int neuronIndex) {
    if (layerIndex < 0 || layerIndex >= layerCount) {// 检查层索引是否有效
        std::cerr << "Error: Layer index out of range.\n";
        throw std::out_of_range("Layer index out of range");
    }
    
    // 获取指定层
    auto it = layers.begin();
    std::advance(it, layerIndex);
    Layer* layer = *it;
    
    // 删除指定神经元
    layer->deleteNeuron(neuronIndex);
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Network::forward
//【函数功能】执行神经网络的前向传播，计算每一层的输出
//【参数】inputs - 输入数据向量
//【返回值】std::vector<std::vector<double>> - 每一层的输出结果
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】2025年7月24日：增加异常处理，检查网络有效性和输入大小
//-------------------------------------------------------------------------------------------------------------------
std::vector<std::vector<double>> Network::forward(const std::vector<double>& inputs) {
    if (layers.empty()) {// 检查网络是否为空
        std::cerr << "Error: Network is empty. Cannot perform forward propagation.\n";
        throw std::runtime_error("Network is empty. Cannot perform forward propagation.");
    }
    if (inputs.size() != layers.front()->getNeuronCount()) {// 检查输入大小是否与第一层神经元数量匹配
        std::cerr << "Error: Input size does not match the number of neurons in the first layer.\n";
        throw std::invalid_argument("Input size does not match the number of neurons in the first layer.");
    }
    if (!isValid()) {// 检查网络是否有效
        std::cerr << "Error: Network is not valid. Cannot perform forward propagation.\n";
        throw std::runtime_error("Network is not valid. Cannot perform forward propagation.");
    }
    std::vector<std::vector<double>> outputs;     // 存储每一层的输出
    std::vector<double> currentInputs = inputs;   // 当前输入，初始为网络输入
    layers.front()->setInput(currentInputs);
    for (auto& layer : layers) {
        layer->updateOutputs();
        std::vector<double> layerOutputs;
        for (const auto& neuron : layer->getNeurons()) {// 获取当前层每个神经元的输出
            layerOutputs.push_back(neuron.getOutput());
        }
        outputs.push_back(layerOutputs);
        currentInputs = layerOutputs;             // 下一层的输入是当前层的输出
    }

    return outputs;
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Network::deleteLayer
// 【函数功能】从神经网络中删除指定的层
// 【参数】layerIndex - 要删除的层的索引
// 【返回值】void - 无返回值
// 【开发者及日期】李孟涵 2025年7月13日
// 【更改记录】2025年7月21日 第一版
// 【更改记录】2025年7月23日 第二版，增加异常处理
//-------------------------------------------------------------------------------------------------------------------
void Network::deleteLayer(int index) {
    if (index < 0 || index >= layerCount) {
        std::cerr << "Error: Layer index out of range.\n";
        throw std::out_of_range("Layer index out of range");
    }
    
    if (layerCount == 1) {
        // 如果只有一层，直接清空
        layers.clear();
        layerCount = 0;
        return;
    }
    
    // 更新后续层的索引
    auto iter = layers.begin();
    std::advance(iter, index + 1);
    for (; iter != layers.end(); ++iter) {
        (*iter)->setIndex((*iter)->getIndex() - 1);
    }
    
    auto it = layers.begin();
    std::advance(it, index);
    
    // 断开当前层的连接
    (*it)->connectTo(nullptr);
    
    // 连接前一层和后一层
    if (index > 0 && index < layerCount - 1) {
        // 中间层：连接前一层和后一层
        auto pre = layers.begin();
        std::advance(pre, index - 1);
        auto next = layers.begin();
        std::advance(next, index + 1);
        (*pre)->connectTo(*next);
    }
    else if(index == 0 && layerCount > 1) {
        // 删除第一层：下一层成为新的第一层
        auto next = layers.begin();
        std::advance(next, 1);
        (*next)->setPreviousLayer(nullptr);
    }
    else if(index == layerCount - 1 && layerCount > 1) {
        // 删除最后一层：前一层成为新的最后一层
        auto pre = layers.begin();
        std::advance(pre, layerCount - 2);
        (*pre)->disconnect();
        (*pre)->setNextLayer(nullptr);
    }
    
    // 删除当前层
    layers.erase(it);
    layerCount--;
}
/* void Network::deleteLayer(int index) //初版deleteLayer函数
{
    if (index < 0 || index >= layerCount) {
        std::cerr << "Error: Layer index out of range.\n";
        throw std::out_of_range("Layer index out of range");
    }
    
    auto it = layers.begin();
    std::advance(it, index);
    Layer* layerToDelete = *it;
    
    if (layerCount == 1) {
        // 如果只有一层，先清理所有连接再删除
        layerToDelete->removeAllConnections();
        delete layerToDelete;
        layers.clear();
        layerCount = 0;
        return;
    }
    
    // === 第一步：收集要删除层中所有神经元的指针 ===
    std::vector<Neuron*> invalidNeurons;
    for (const auto& neuron : layerToDelete->getNeurons()) {
        invalidNeurons.push_back(const_cast<Neuron*>(&neuron));
    }
    
    // === 第二步：清理所有其他层中指向被删除层的突触 ===
    for (auto& layer : layers) {
        if (layer != layerToDelete) {
            for (auto& neuron : layer->getNeurons()) {
                const_cast<Neuron&>(neuron).cleanInvalidSynapses(invalidNeurons);
            }
        }
    }
    
    // === 第三步：清理当前层内部的所有突触和神经元连接 ===
    layerToDelete->removeAllConnections();
    
    // === 第四步：更新层级连接 ===
    if (layerToDelete->getPreviousLayer()) {
        layerToDelete->getPreviousLayer()->setNextLayer(nullptr);
    }
    if (layerToDelete->getNextLayer()) {
        layerToDelete->getNextLayer()->setPreviousLayer(nullptr);
    }
    
    // === 第五步：重新建立相邻层的连接 ===
    if (index > 0 && index < layerCount - 1) {
        // 中间层：连接前一层和后一层
        auto pre = layers.begin();
        std::advance(pre, index - 1);
        auto next = layers.begin();
        std::advance(next, index + 1);
        (*pre)->connectTo(*next);
    }
    else if(index == 0 && layerCount > 1) {
        // 删除第一层：下一层成为新的第一层
        auto next = layers.begin();
        std::advance(next, 1);
        (*next)->setPreviousLayer(nullptr);
    }
    else if(index == layerCount - 1 && layerCount > 1) {
        // 删除最后一层：前一层成为新的最后一层
        auto pre = layers.begin();
        std::advance(pre, layerCount - 2);
        (*pre)->setNextLayer(nullptr);
    }
    
    // === 第六步：更新后续层的索引 ===
    auto iter = layers.begin();
    std::advance(iter, index + 1);
    for (; iter != layers.end(); ++iter) {
        (*iter)->setIndex((*iter)->getIndex() - 1);
    }
    
    // === 第七步：从容器中移除并删除层对象 ===
    layers.erase(it);
    delete layerToDelete;  // 释放内存
    layerCount--;
}*/
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Network::addLayer
//【函数功能】在神经网络中添加一个新的层
//【参数】index - 新层的索引
//【返回值】void - 无返回值
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】2025年7月20日 完善功能
//-------------------------------------------------------------------------------------------------------------------
void Network::addLayer(int index) {
    if (index < 0 || index > layerCount) {
        std::cerr << "Error: Layer index out of range.\n";
        throw std::out_of_range("Layer index out of range");
    }
    
    Layer* newLayer = new Layer(index, 0, std::vector<double>(), 0);
    
    if (index == 0) {
        // 插入到开头
        newLayer->connectTo(layers.empty() ? nullptr : layers.front());
        for (auto& layer : layers) {
            layer->setIndex(layer->getIndex() + 1);
        }
        layers.push_front(newLayer);
    } else if (index == layerCount) {
        // 插入到末尾
        if (!layers.empty()) {
            layers.back()->connectTo(newLayer);
        }
        layers.push_back(newLayer);
    } else {
        // 插入到中间
        auto it = layers.begin();
        std::advance(it, index - 1);
        
        (*it)->connectTo(newLayer);
        newLayer->connectTo(*std::next(it));
        
        // 更新后续层的索引
        for (auto iter = std::next(it); iter != layers.end(); ++iter) {
            (*iter)->setIndex((*iter)->getIndex() + 1);
        }
        
        layers.insert(std::next(it), newLayer);
    }
    
    layerCount++;
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Network::getName
// 【函数功能】获取网络的名称
// 【参数】无
// 【返回值】std::string - 网络名称
//  开发者及日期】李孟涵 2025年7月24日
// 【更改记录】无
//------------------------------------------------------------------------------------------------------------------------
std::string Network::getName() const {
    return networkName;
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Network::setName
// 【函数功能】设置网络的名称
// 【参数】name - 网络名称
// 【返回值】void - 无返回值
// 【开发者及日期】李孟涵 2025年7月24日
// 【更改记录】无
//-----------------------------------------------------------------------------------------------------------------------
void Network::setName(const std::string& name) {
    networkName = name;
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Network::showInfo
//【函数功能】显示网络的基本信息，包括层数、总神经元数和总突触数
//【参数】无
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】2025年7月24日 增加显示网络名称的功能, 增加对网络有效性的检查
//-------------------------------------------------------------------------------------------------------------------
void Network::showInfo() const{
    int totalNeurons = 0;      // 总神经元数量
    int totalSynapses = 0;     // 总突触数量
    for (const auto& layer : layers) {
        totalNeurons += layer->getNeuronCount();
        for (const auto& neuron : layer->getNeurons()) {
            totalSynapses += neuron.getDendriteCount();
        }
    }
    std::cout << "Network Name: "   << networkName   << std::endl; // 显示网络名称
    std::cout << "Network has "     << layerCount    << " layers:\n";
    std::cout << "Total Neurons: "  << totalNeurons  << std::endl;
    std::cout << "Total Synapses: " << totalSynapses << std::endl;
    // 验证网络结构是否正确
    if (isValid()) {
        std::cout << "The network structure is valid." << std::endl;
    } else {
        std::cout << "The network structure is invalid." << std::endl;
    }
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Network::showLayer
//【函数功能】显示指定层的详细信息，包括索引、神经元数量
//【参数】index - 要显示的层的索引
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Network::showLayer(int index) const {
    if (index < 0 || index >= layerCount) {
        std::cerr << "Error: Layer index out of range.\n";
        throw std::out_of_range("Layer index out of range");
    }
    
    auto it = layers.begin();
    std::advance(it, index);
    
    std::cout << "=== Layer " << index << " Details ===" << std::endl;
    std::cout << "Layer Index: " << (*it)->getIndex() << std::endl;
    std::cout << "Neuron Count: " << (*it)->getNeuronCount() << std::endl;
    
    // 显示层的连接信息
    if ((*it)->getPreviousLayer()) {
        std::cout << "Previous Layer: " << (*it)->getPreviousLayer()->getIndex() << std::endl;
    } else {
        std::cout << "Previous Layer: None (Input Layer)" << std::endl;
    }
    
    if ((*it)->getNextLayer()) {
        std::cout << "Next Layer: " << (*it)->getNextLayer()->getIndex() << std::endl;
    } else {
        std::cout << "Next Layer: None (Output Layer)" << std::endl;
    }
    
    std::cout << "--- Neurons in this layer ---" << std::endl;
    (*it)->printNeurons();
    std::cout << "==============================" << std::endl;
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Network::showLayers
//【函数功能】显示网络中所有层的详细信息，包括索引和神经
//【参数】无
//【返回值】无
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Network::showLayers() const {
    std::cout << "Network Layers:\n";
    for (const auto& layer : layers) {
        std::cout << "Layer Index: " << layer->getIndex() << ", Neuron Count: " << layer->getNeuronCount() << "\n";
        layer->printNeurons();
    }
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Network::isValid
//【函数功能】验证神经网络的结构是否正确
//【参数】无
//【返回值】bool - 如果网络结构正确返回true，否则返回false
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
bool Network::isValid() const {
    // 检查网络是否为空
    if (layers.empty()) {
        std::cerr << "Network validation failed: No layers in network.\n";
        return false;
    }
    
    // 检查层数是否一致
    if (layerCount != static_cast<int>(layers.size())) {
        std::cerr << "Network validation failed: Layer count mismatch.\n";
        return false;
    }
    
    int expectedIndex = 0;
    Layer* previousLayer = nullptr;
    
    for (const auto& layer : layers) {
        // 检查层指针是否有效
        if (layer == nullptr) {
            std::cerr << "Network validation failed: Null layer pointer at index " << expectedIndex << ".\n";
            return false;
        }
        
        // 检查层索引是否正确
        if (layer->getIndex() != expectedIndex) {
            std::cerr << "Network validation failed: Layer index mismatch. Expected " 
                      << expectedIndex << ", got " << layer->getIndex() << ".\n";
            return false;
        }
        
        // 检查层是否有神经元
        if (layer->getNeuronCount() == 0) {
            std::cerr << "Network validation failed: Layer " << expectedIndex 
                      << " has no neurons.\n";
            return false;
        }
        
        // 检查层连接是否正确
        if (previousLayer != nullptr) {
            if (layer->getPreviousLayer() != previousLayer) {
                std::cerr << "Network validation failed: Layer " << expectedIndex 
                          << " previous layer connection is incorrect.\n";
                return false;
            }
            if (previousLayer->getNextLayer() != layer) {
                std::cerr << "Network validation failed: Layer " << (expectedIndex - 1) 
                          << " next layer connection is incorrect.\n";
                return false;
            }
        } else {
            // 第一层的前一层应该为空
            if (layer->getPreviousLayer() != nullptr) {
                std::cerr << "Network validation failed: First layer should not have previous layer.\n";
                return false;
            }
        }
        
        previousLayer = layer;
        expectedIndex++;
    }
    
    // 检查最后一层的下一层应该为空
    if (previousLayer && previousLayer->getNextLayer() != nullptr) {
        std::cerr << "Network validation failed: Last layer should not have next layer.\n";
        return false;
    }
    
    return true;
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Network::getLayer
//【函数功能】获取指定索引的层
//【参数】index - 层的索引
//【返回值】const Layer* - 指向指定层的指针，如果索引超出范围则抛出异常
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
const Layer* Network::getLayer(int index) const {
    if (index < 0 || index >= layerCount) {// 检查索引是否在有效范围内
        std::cerr << "Error: Layer index out of range.\n";
        throw std::out_of_range("Layer index out of range");
    }
    auto it = layers.begin();
    std::advance(it, index);
    return *it;
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Network::getLayers
//【函数功能】获取神经网络中的所有层
//【参数】无
//【返回值】const std::list<Layer*>& - 包含所有层的列表
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
const std::list<Layer*>& Network::getLayers() const {
    return layers;
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Network::getLayerCount
//【函数功能】获取神经网络的层数
//【参数】无
//【返回值】int - 神经网络的层数
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
int Network::getLayerCount() const {
    return layerCount;
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Network::~Network
//【函数功能】Network类的析构函数，释放所有动态分配的Layer
//【参数】无
//【返回值】无
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
Network::~Network() {
    // 释放所有动态分配的 Layer 对象
    for (auto& layer : layers) {
        delete layer;
    }
    layers.clear();
    layerCount = 0;
}