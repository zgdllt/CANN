#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "Layer.hpp"
#include <list>
#include <iostream>
#include<fstream>


class Network {
public:
    Network();
    void addLayer(Layer* layer);
    std::vector<std::vector<double>> forward(const std::vector<double>& inputs);
    bool isValid() const;
    void printLayers() const;
    void setWeights(int layerIndex = 0, const std::vector<std::vector<double>>& weights = {});
    void deleteLayer(int index);
    void addLayer(int index);
    void showlayer(int index) const;
    void showInfo() const;
    void exportToFile(const std::string& filename) const;
    void import(const std::string& filename);
    ~Network();
    // Getter methods for ANN export/import
    const std::list<Layer*>& getLayers() const;
    int getLayerCount() const;
private:
    int layerCount = 0; // 记录网络层数
    std::list<Layer*> layers;
};

#endif // NETWORK_HPP
