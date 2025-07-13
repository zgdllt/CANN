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
// void Network::deleteLayer(int index) {
//     if (index < 0 || index >= layerCount) {
//         std::cerr << "Error: Layer index out of range.\n";
//         throw std::out_of_range("Layer index out of range");
//     }
//     auto pre = layers.begin(),next = layers.begin(),it = layers.begin();
//     std::advance(pre, index-1);
//     std::advance(next, index+1);
//     std::advance(it, index);
//     it->connectTo(nullptr); // 断开当前层与下一层的连接
//     pre->connectTo(&(*next));
//     layers.erase(it);
//     for (auto iter = next; iter != layers.end(); ++iter) {
//         iter->setIndex(iter->getIndex() - 1);
//     }
//     layerCount--;
// }
// void Network::addLayer(int index) {
//     if (index < 0 || index > layerCount) {
//         std::cerr << "Error: Layer index out of range.\n";
//         throw std::out_of_range("Layer index out of range");
//     }
//     Layer newLayer(index, 0, std::vector<double>(), 0);
//     if (index == 0) {
//         newLayer.connectTo(layers.empty() ? nullptr : &layers.front());
//         for (auto& layer : layers) {
//             layer.setIndex(layer.getIndex() + 1);
//         }
//         layers.push_front(newLayer);
//     } else if (index == layerCount) {
//         layers.back().connectTo(&newLayer);
//         layers.push_back(newLayer);
//     } else {
//         auto it = layers.begin();
//         std::advance(it, index - 1);
//         it->connectTo(&newLayer);
//         newLayer.connectTo(&(*std::next(it)));
//         for (auto iter = std::next(it); iter != layers.end(); ++iter) {
//             iter->setIndex(iter->getIndex() + 1);
//         }
//         layers.insert(it, newLayer);
//     }
//     layerCount++;
// }
// void Network::showlayer(int index) const {
//     if (index < 0 || index >= layerCount) {
//         std::cerr << "Error: Layer index out of range.\n";
//         throw std::out_of_range("Layer index out of range");
//     }
//     auto it = layers.begin();
//     std::advance(it, index);
//     it->printNeurons();
// }
void Network::showInfo() const {
    std::cout << "Network has " << layerCount << " layers:\n";
    for (const auto& layer : layers) {
        std::cout << "Layer Index: " << layer->getIndex() << ", Neuron Count: " << layer->getNeuronCount() << "\n";
        layer->printNeurons();
    }
}
// bool Network::isValid() const {
//     if (layers.empty()) {
//         return false;
//     }
//     for (const auto& layer : layers) {
//         if (layer.getNeuronCount() == 0) {
//             return false;
//         }
//     }
//     return true;
// }
Network::~Network() {
    layers.clear();
    layerCount = 0;
}