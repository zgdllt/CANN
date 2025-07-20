#include <iostream>
#include "ANNFilePorter.hpp"
#include "Layer.hpp"
#include "Network.hpp"
#include "Neuron.hpp"
#include "Soma.hpp"
int main()
{

    try {
        ANNImporter importer("simple.ANN");
        Network network = importer.import();
        network.showInfo();
        for (const auto& layer : network.getLayers()) {
            for (const auto& neuron : layer->getNeurons()) {
                std::cout << "Neuron weights: ";
                neuron.showConnections();
                std::cout << std::endl;
            }
        }
        ANNExporter exporter("output.ANN");
        exporter.exportNetwork(network, "ExportedNetwork");
    } catch (const std::exception& e) {
        std::cerr << "Error importing ANN file: " << e.what() << std::endl;
    }
}
// int main() {
//     // 输入层有2个输入
//     int inputSize = 2;
//                // 
//     // 创建层
//     Layer layer1(0, 2, std::vector<double>(inputSize, 0.0), 0); // 两个使用Sigmoid激活函数的神经元
//     Layer layer2(1, 2, std::vector<double>(2, 0.0), 0); // 一个使用ReLU激活函数的神经元，输入来自上一层2个输出

//     std::cout<< "Layer 1: " << layer1.getIndex() << ", Neurons: " << layer1.getNeuronCount() << std::endl;
//     // 创建网络
//     Network network;
//     network.addLayer(&layer1);
//     network.addLayer(&layer2);
//     network.showInfo();
    
//     // 进行前向传播
//     std::vector<double> input = { 0.5, 0.8 };
//     network.setWeights(1, { { 0.2, 0.4 }, { 0.6, 0.8 } });
//     auto output = network.forward(input);
//     for (size_t i = 0; i < output.size(); ++i) {
//         std::cout << "Output " << i << ": ";
//         for (const auto& val : output[i]) {
//             std::cout << val << " ";
//         }
//         std::cout << std::endl;
//     }
    
//     // for (const auto& neuron : layer1.getNeurons()) {
//     //     neuron.showConnections();
//     // }
//     // for (const auto& neuron : layer2.getNeurons()) {
//     //     neuron.showConnections();
//     // }
//     // 删除层
//     Layer layer3(2, 3, std::vector<double>(3, 0.0), 0); // 新层
//     network.addLayer(&layer3);
//     std::cout << "After adding layer 3: " << std::endl;
//     network.showInfo();
//     output = network.forward(input);
//     // for (const auto& neuron : layer1.getNeurons()) {
//     //     neuron.showConnections();
//     // }
//     // for (const auto& neuron : layer2.getNeurons()) {
//     //     neuron.showConnections();
//     // }
//     // for (const auto& neuron : layer3.getNeurons()) {
//     //     neuron.showConnections();
//     // }
//     network.deleteLayer(1); // 删除第二层
//     std::cout << "After deleting layer 2: " << std::endl;
//     network.showInfo();
//     for (const auto& neuron : layer1.getNeurons()) {
//         neuron.showConnections();
//     }
//     for (const auto& neuron : layer2.getNeurons()) {
//         neuron.showConnections();
//     }
//     for (const auto& neuron : layer3.getNeurons()) {
//         neuron.showConnections();
//     }
//     std::cout<<"successfully set input for layer1" << std::endl;
//     return 0;
// }
// int main()
// {
    // Neuron neuron1({}, 0.1, 0, 0, 0);
    // Neuron neuron2({}, 0.2, 0, 1, 1);
    // Neuron neuron3({}, 0.3, 0, 2, 2);
    // // neuron1.showConnections();
    // // neuron2.showConnections();
    // neuron1.connectTo(&neuron2, 0.8);
    // neuron1.addInput(1.0);
    // neuron2.connectTo(&neuron3, 0.5);
    // neuron1.showConnections();
    // neuron2.showConnections();
    // neuron3.showConnections();
    // std::cout<<neuron1.getOutput()<<std::endl;
    // std::cout << neuron2.getOutput() << std::endl;
    // neuron2.updateOutput();
    // std::cout << "Neuron 2 Output: " << neuron2.getOutput() << std::endl;
    // neuron2.disconnectTo(&neuron3);
    // neuron1.showConnections();
    // neuron2.showConnections();
    // neuron3.showConnections();
    // Layer layer1(0, 2, std::vector<double>(2, 0.0), 0); // 两个使用Sigmoid激活函数的神经元
    // Layer layer2(1, 2, std::vector<double>(2, 0.0), 0); // 一个使用ReLU激活函数的神经元，输入来自上一层2个输出
    // Layer layer3(2, 3, std::vector<double>(3, 0.0), 0); // 新层
    // layer1.addNeuron(Neuron({}, 0.1, 0, 0, 0));
    // layer1.connectTo(&layer2);
    // layer2.connectTo(&layer3);
    // // for (const auto& neuron : layer1.getNeurons()) {
    // //     neuron.showConnections();
    // // }
    // // for (const auto& neuron : layer2.getNeurons()) {
    // //     neuron.showConnections();
    // // }
    // layer1.setInput({ 0.5, 0.5 , 0.5});
    // std::cout << "successfully set input for layer1" << std::endl;
    // layer2.setWeights({ { 0.2, 0.4 }, { 0.6, 0.8 } });
    // layer1.updateOutputs();
    // layer2.updateOutputs();
    // layer3.updateOutputs();
    // // for (const auto& neuron : layer1.getNeurons()) {
    // //     neuron.showConnections();
    // // }
    // // for (const auto& neuron : layer2.getNeurons()) {
    // //     neuron.showConnections();
    // // }
    // // for (const auto& neuron : layer3.getNeurons()) {
    // //     neuron.showConnections();
    // // }
    // std::cout<<layer2.getNextLayer()->getIndex()<<std::endl;
    // std::cout << layer2.getPreviousLayer()->getIndex() << std::endl;
    // // layer1.disconnect();
    // layer2.disconnect(); // 测试连接到nullptr
    // layer3.setIndex(1);
    // layer1.connectTo(&layer3);
    // for (const auto& neuron : layer1.getNeurons()) {
    //     neuron.showConnections();
    // }
    // for (const auto& neuron : layer2.getNeurons()) {
    //     neuron.showConnections();
    // }
    // for (const auto& neuron : layer3.getNeurons()) {
    //     neuron.showConnections();
    // }
// }