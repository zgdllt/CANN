#include <iostream>
#include "Layer.hpp"
#include "Network.hpp"
#include "Neuron.hpp"
#include "Soma.hpp"
int main() {
    // 输入层有2个输入
    int inputSize = 2;
               // 
    // 创建层
    Layer layer1(0, 2, std::vector<double>(inputSize, 0.0), 0); // 两个使用Sigmoid激活函数的神经元
    Layer layer2(1, 2, std::vector<double>(2, 0.0), 0); // 一个使用ReLU激活函数的神经元，输入来自上一层2个输出

    std::cout<< "Layer 1: " << layer1.getIndex() << ", Neurons: " << layer1.getNeuronCount() << std::endl;
    // 创建网络
    Network network;
    network.addLayer(&layer1);
    network.addLayer(&layer2);
    network.showInfo();
    
    // 进行前向传播
    std::vector<double> input = { 0.5, 0.8 };
    network.setWeights(1, { { 0.2, 0.4 }, { 0.6, 0.8 } });
    auto output = network.forward(input);
    for (const auto& neuron : layer1.getNeurons()) {
        neuron.showConnections();
    }
    for (const auto& neuron : layer2.getNeurons()) {
        neuron.showConnections();
    }
    std::cout << "Output: "<< std::endl;
    for (size_t i = 0; i < output.size(); ++i) {
        std::cout << "Layer " << i << " output: ";
        for (const auto& val : output[i]) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
// int main()
// {
//     // Neuron neuron1({}, 0.1, 0, 0, 0);
//     // Neuron neuron2({}, 0.2, 0, 1, 1);
//     // // neuron1.showConnections();
//     // // neuron2.showConnections();
//     // neuron1.connectTo(&neuron2, 0.8);
//     // neuron1.addInput(1.0);
//     // neuron1.showConnections();
//     // neuron2.showConnections();
//     // std::cout<<neuron1.getOutput()<<std::endl;
//     // std::cout << neuron2.getOutput() << std::endl;
//     // neuron2.updateOutput();
//     // std::cout << "Neuron 2 Output: " << neuron2.getOutput() << std::endl;

//     Layer layer1(0, 2, std::vector<double>(2, 0.0), 0); // 两个使用Sigmoid激活函数的神经元
//     Layer layer2(1, 2, std::vector<double>(2, 0.0), 0); // 一个使用ReLU激活函数的神经元，输入来自上一层2个输出
//     layer1.connectTo(&layer2);
//     // for (const auto& neuron : layer1.getNeurons()) {
//     //     neuron.showConnections();
//     // }
//     // for (const auto& neuron : layer2.getNeurons()) {
//     //     neuron.showConnections();
//     // }
//     layer1.setInput({ 0.5, 0.5 });
//     std::cout << "successfully set input for layer1" << std::endl;
//     layer2.setWeights({ { 0.2, 0.4 }, { 0.6, 0.8 } });
//     layer1.updateOutputs();
//     layer2.updateOutputs();
//     for (const auto& neuron : layer1.getNeurons()) {
//         neuron.showConnections();
//     }
//     for (const auto& neuron : layer2.getNeurons()) {
//         neuron.showConnections();
//     }
//     for (const auto& neuron : layer2.getNeurons()) { 
//         std::cout << "Layer 2 Neuron " << neuron.getIndex() << " Output: " << neuron.getOutput() << std::endl;
//     }

// }