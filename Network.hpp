//-------------------------------------------------------------------------------------------------------------------
//【文件名】Network.hpp
//【功能模块和目的】神经网络类的声明，定义了完整的人工神经网络结构和管理功能
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "Layer.hpp"
#include <list>
#include <iostream>
#include<fstream>

//-------------------------------------------------------------------------------------------------------------------
//【类名】Network
//【功能】管理完整的神经网络，包括层的添加删除、前向传播、权重设置、网络验证、文件导入导出等功能
//【接口说明】提供网络构建、训练、推理、持久化等完整的神经网络操作接口
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
class Network {
public:
    Network();
    void addLayer(Layer* layer);
    std::vector<std::vector<double>> forward(const std::vector<double>& inputs);
    bool isValid() const;
    void setWeights(int layerIndex = 0, const std::vector<std::vector<double>>& weights = {});
    void deleteLayer(int index);
    void addLayer(int index);
    void addNeuron(int layerIndex,double bias = 0.0, int activationType = 0);
    void deleteNeuron(int layerIndex, int neuronIndex);
    void showLayer(int index) const;
    void showLayers() const;
    void showInfo() const;
    void exportToFile(const std::string& filename) const;
    void import(const std::string& filename);
    ~Network();
    // Getter methods for ANN export/import
    const Layer* getLayer(int index) const;
    const std::list<Layer*>& getLayers() const;
    int getLayerCount() const;
private:
    int layerCount = 0; // 记录网络层数
    std::list<Layer*> layers;
};

#endif // NETWORK_HPP
