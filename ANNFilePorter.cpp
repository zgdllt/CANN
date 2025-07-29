//-------------------------------------------------------------------------------------------------------------------
// 【文件名】ANNFilePorter.cpp
// 【功能模块和目的】神经网络文件导入导出类的实现
// 【开发者及日期】李孟涵 2025年7月20日
// 【更改记录】2025年7月20日 对扩展名的检查方法进行了改进
// 【更改记录】2025年7月24日 增加了对网络有效性的检查, 增加导入网络名称的功能
// 【更改记录】2025年7月29日 针对索引进行相应修改
//-------------------------------------------------------------------------------------------------------------------
#include "ANNFilePorter.hpp" // 神经网络文件导入导出类的头文件
#include <iostream>          // 输入输出流头文件
#include <fstream>           // 文件流头文件
#include <sstream>           // 字符串流头文件
#include <stdexcept>         // 标准异常头文件

//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】ANNImporter::import
// 【函数功能】从文件中导入神经网络结构和数据
// 【参数】无
// 【返回值】Network - 导入的神经网络对象
// 【开发者及日期】李孟涵 2025年7月20日
// 【更改记录】2025年7月20日 对扩展名的检查方法进行了改进
// 【更改记录】2025年7月24日 增加了对网络有效性的检查, 增加导入网络名称的功能
//-------------------------------------------------------------------------------------------------------------------
Network ANNImporter::import() {
    std::ifstream file(filename);
    if (!file.is_open()) {// 打开文件失败
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    std::string line;               // 每一行的内容
    std::vector<NeuronInfo> neurons;// 神经元信息的数组
    std::vector<LayerInfo> layers;  // 层信息的数组
    std::vector<SynapseInfo> synapses;// 突触信息的数组
    std::string networkName;        // 网络名称

    //解析文件结构
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue; // 跳过注释和空行
        }
        
        std::istringstream iss(line);
        char type;
        iss >> type;
        
        switch (type) {
            case 'G': {// 网络名称
                iss >> networkName;
                break;
            }
            case 'N': {// 神经元信息
                NeuronInfo neuron;
                iss >> neuron.bias >> neuron.activationType;
                neurons.push_back(neuron);
                break;
            }
            case 'L': {// 层信息
                LayerInfo layer;
                iss >> layer.startNeuron >> layer.endNeuron;
                layers.push_back(layer);
                break;
            }
            case 'S': {// 突触信息
                SynapseInfo synapse;
                iss >> synapse.fromNeuron >> synapse.toNeuron >> synapse.weight;
                synapses.push_back(synapse);
                break;
            }
            default:
                // 忽略未知类型
                break;
        }
    }
    
    // 创建网络
    Network network;
    network.setName(networkName);
    // 创建层
    for (size_t layerIdx = 0; layerIdx < layers.size(); ++layerIdx) {
        const auto& layerInfo = layers[layerIdx];
        int neuronCount = layerInfo.endNeuron - layerInfo.startNeuron + 1;
        std::vector<double> biases;
        
        // 收集该层神经元的偏置值
        for (int i = layerInfo.startNeuron; i <= layerInfo.endNeuron; ++i) {
            if (i < static_cast<int>(neurons.size())) {
                biases.push_back(neurons[i].bias);
            } else {
                biases.push_back(0.0);
            }
        }
        
        // 使用第一个神经元的激活函数类型作为整层的激活函数
        int activationType = 0;
        if (layerInfo.startNeuron < static_cast<int>(neurons.size())) {
            activationType = neurons[layerInfo.startNeuron].activationType;
        }
        
        Layer* layer = new Layer(&network, neuronCount, biases, activationType);
        network.addLayer(layer);
    }
    
    // 根据突触信息设置层间权重
    // 为每一层（除了第一层）收集权重矩阵
    for (size_t layerIdx = 1; layerIdx < layers.size(); ++layerIdx) {
        const auto& currentLayerInfo = layers[layerIdx];
        const auto& prevLayerInfo = layers[layerIdx - 1];
        
        int currentLayerSize = currentLayerInfo.endNeuron - currentLayerInfo.startNeuron + 1;
        int prevLayerSize = prevLayerInfo.endNeuron - prevLayerInfo.startNeuron + 1;
        
        // 创建权重矩阵：weights[i][j] = 从前一层神经元j到当前层神经元i的权重
        std::vector<std::vector<double>> weights(currentLayerSize, std::vector<double>(prevLayerSize, 1.0));
        
        // 从突触信息中设置权重
        for (const auto& synapse : synapses) {
            // 跳过输入和输出连接
            if (synapse.fromNeuron == -1 || synapse.toNeuron == -1) {
                continue;
            }
            
            // 检查这个突触是否连接到当前层
            if (synapse.toNeuron >= currentLayerInfo.startNeuron && 
                synapse.toNeuron <= currentLayerInfo.endNeuron &&
                synapse.fromNeuron >= prevLayerInfo.startNeuron &&
                synapse.fromNeuron <= prevLayerInfo.endNeuron) {
                
                int toNeuronInLayer = synapse.toNeuron - currentLayerInfo.startNeuron;
                int fromNeuronInLayer = synapse.fromNeuron - prevLayerInfo.startNeuron;
                
                weights[toNeuronInLayer][fromNeuronInLayer] = synapse.weight;
            }
        }
        
        // 设置这一层的权重
        try {
            network.setWeights(static_cast<int>(layerIdx), weights);
        } catch (const std::exception& e) {
            std::cerr << "Warning: Failed to set weights for layer " << layerIdx 
                      << ": " << e.what() << std::endl;
        }
    }
    if(!network.isValid()) {//增加对网络有效性的检查
        std::cerr << "Warning: Imported network is not valid!" << std::endl;
    }
    file.close();
    return network;
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】ANNExporter::exportNetwork
//【函数功能】将神经网络结构和数据导出到文件
//【参数】network - 要导出的神经网络对象，networkName - 网络名称
//【返回值】无
//【开发者及日期】李孟涵 2025年7月20日
//【更改记录】2025年7月24日：增加了对网络名称的导出
//-------------------------------------------------------------------------------------------------------------------
void ANNExporter::exportNetwork(const Network& network) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to create file: " + filename);
    }
    
    // 写入文件头注释
    file << "# " << filename << "\n";
    file << "G " << network.getName() << "\n";

    // 收集所有神经元信息并写入神经元
    file << "# Neurons\n";
    const auto& layers = network.getLayers();
    
    // 导出神经元
    for (const auto& layer : layers) {
        const auto& neurons = layer->getNeurons();
        for (const auto& neuron : neurons) {
            double bias = neuron.getBias();
            int activationType = neuron.getActivationFunctionType();
            file << "N " << bias << " " << activationType << "\n";
        }
    }
    
    // 写入层信息
    file << "# Layers\n";
    int neuronIndex = 0;
    for (const auto& layer : layers) {
        int layerSize = layer->getNeuronCount();
        int startIndex = neuronIndex;
        int endIndex = neuronIndex + layerSize - 1;
        file << "L " << startIndex << " " << endIndex << "\n";
        neuronIndex += layerSize;
    }
    
    // 写入突触信息
    file << "# Synapses\n";
    
    // 导出输入突触（针对第一层）
    auto layerIt = layers.begin();
    if (layerIt != layers.end()) {
        const auto& firstLayer = *layerIt;
        for (int i = 0; i < firstLayer->getNeuronCount(); ++i) {
            file << "S -1 " << i << " 1.0\n";
        }
    }
    
    // 导出输出突触（针对最后一层）
    if (layers.size() > 1) {
        auto lastLayerIt = layers.end();
        --lastLayerIt;
        const auto& lastLayer = *lastLayerIt;
        int lastLayerStartIndex = neuronIndex - lastLayer->getNeuronCount();
        for (int i = 0; i < lastLayer->getNeuronCount(); ++i) {
            file << "S " << (lastLayerStartIndex + i) << " -1 1.0\n";
        }
    }
    
    // 导出层间连接权重
    neuronIndex = 0;
    for (auto it = layers.begin(); it != layers.end(); ++it) {
        auto nextIt = std::next(it);
        if (nextIt != layers.end()) {
            const auto& currentLayer = *it;
            const auto& nextLayer = *nextIt;
            
            int currentLayerStart = neuronIndex;
            int nextLayerStart = neuronIndex + currentLayer->getNeuronCount();
            
            // 获取下一层的神经元，读取它们的权重
            const auto& nextLayerNeurons = nextLayer->getNeurons();
            int nextNeuronIdx = 0;
            
            for (auto neuronIt = nextLayerNeurons.begin(); neuronIt != nextLayerNeurons.end(); ++neuronIt, ++nextNeuronIdx) {
                const auto& weights = neuronIt->getWeights();
                
                // 连接当前层的每个神经元到这个下一层神经元
                for (int i = 0; i < currentLayer->getNeuronCount() && i < static_cast<int>(weights.size()); ++i) {
                    double weight = weights[i];
                    file << "S " << (currentLayerStart + i) << " " << (nextLayerStart + nextNeuronIdx) << " " << weight << "\n";
                }
            }
        }
        neuronIndex += (*it)->getNeuronCount();
    }
    
    file.close();
}
