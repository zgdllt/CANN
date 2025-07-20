#include "Network.hpp"
#include "Soma.hpp"
#include <cstddef>

Network::Network() { layerCount = 0; }
// void Network::addLayer(Layer* layer) {
//     if (layer->getIndex() < 0 || layer->getIndex() > layerCount) {
//         std::cerr << "Error: Layer index out of range.\n";
//         throw std::out_of_range("Layer index out of range");
//     }
//     if (layer->getIndex() == 0) {
//         if (!layers.empty()) {
//             layer->connectTo(&(*layers.begin()));
//         }
//         for (auto& lyr : layers) {
//             lyr.setIndex(lyr.getIndex() + 1);
//         }
//         layers.push_front(*layer);
//     } else if (layer->getIndex() == layerCount) {
//         layers.back().connectTo(layer);
//         layers.push_back(*layer);
//     } else {
//         auto it = layers.begin();
//         std::advance(it, layer->getIndex() - 1);
//         it->connectTo(layer);
//         layer->connectTo(&(*std::next(it)));
//         for (auto iter = std::next(it); iter != layers.end(); ++iter) {
//             iter->setIndex(iter->getIndex() + 1);
//         }
//         layers.insert(it, *layer);
//     }
//     layerCount++;
// }
void Network::setWeights(int layerIndex, const std::vector<std::vector<double>>& weights) {
    if (layerIndex < 0 || layerIndex >= layerCount) {
        std::cerr << "Error: Layer index out of range.\n";
        throw std::out_of_range("Layer index out of range");
    }
    if (layerIndex == 0) {
        std::cerr << "Error: Cannot set weights for the first layer in the network.\n";
        throw std::runtime_error("Cannot set weights: Layer is the first layer in the network.");
    }
    auto it = layers.begin();
    std::advance(it, layerIndex);
    (*it)->setWeights(weights);
}
void Network::addLayer(Layer* layer) {
    if (layer->getIndex() < 0 || layer->getIndex() > layerCount) {
        std::cerr << "Error: Layer index out of range.\n";
        throw std::out_of_range("Layer index out of range");
    }
    if (layer->getIndex() == 0) {
        if (!layers.empty()) {
            layer->connectTo(*layers.begin());
        }
        for (auto& lyr : layers) {
            lyr->setIndex(lyr->getIndex() + 1);
        }
        layers.push_front(layer);
    } else if (layer->getIndex() == layerCount) {
        layers.back()->connectTo(layer);
        layers.push_back(layer);
    } else {
        auto it = layers.begin();
        std::advance(it, layer->getIndex() - 1);
        (*it)->connectTo(layer);
        layer->connectTo(*std::next(it));
        for (auto iter = std::next(it); iter != layers.end(); ++iter) {
            (*iter)->setIndex((*iter)->getIndex() + 1);
        }
        layers.insert(it, layer);
    }
    layerCount++;
}
std::vector<std::vector<double>> Network::forward(const std::vector<double>& inputs) {
    if (layers.empty()) {
        return {};
    }
    std::vector<std::vector<double>> outputs;
    std::vector<double> currentInputs = inputs;
    layers.front()->setInput(currentInputs);
    for (auto& layer : layers) {
        layer->updateOutputs();
        std::vector<double> layerOutputs;
        for (const auto& neuron : layer->getNeurons()) {
            layerOutputs.push_back(neuron.getOutput());
        }
        outputs.push_back(layerOutputs);
        currentInputs = layerOutputs;
    }

    return outputs;
}
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
    
    // 删除当前层
    layers.erase(it);
    layerCount--;
}
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
void Network::showInfo() const {
    std::cout << "Network has " << layerCount << " layers:\n";
    for (const auto& layer : layers) {
        std::cout << "Layer Index: " << layer->getIndex() << ", Neuron Count: " << layer->getNeuronCount() << "\n";
        layer->printNeurons();
    }
}
void Network::showlayer(int index) const {
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
const std::list<Layer*>& Network::getLayers() const {
    return layers;
}

int Network::getLayerCount() const {
    return layerCount;
}

Network::~Network() {
    // 释放所有动态分配的 Layer 对象
    for (auto& layer : layers) {
        delete layer;
    }
    layers.clear();
    layerCount = 0;
}