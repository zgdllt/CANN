#ifndef NEURON_HPP
#define NEURON_HPP
#include "Soma.hpp"
#include "Synapse.hpp"
#include <vector>
#include <iostream>
#include <cmath>
class Neuron:public Soma{
public:
    Neuron(const Neuron& other) = default; // Copy constructor
    Neuron(std::vector<Dendrite*> pre = {}, double bias = 0.0, int activationFunctionType = 0,int layerIndex = 0, int idx = 0);
    bool isConnectedTo(const Neuron& other) const;
    void connectTo(Neuron* other, double weight = 1.0);
    void connectFrom(Neuron* other, double weight = 1.0);
    void disconnectFrom(Neuron* other);
    void disconnectTo(Neuron* other);
    std::vector<double> getWeights() const;
    void setWeights(const std::vector<double>& weights);
    void setWeight(int index, double weight);
    void showConnections() const;
    int getIndex() const;
    int getLayerIndex() const;
    void setLayerIndex(int newLayerIndex);
    void setBias(double newBias);
    void remove();
    void updateInput();
    void updateOutput() override;
    virtual ~Neuron() = default;
private:
    std::vector<Dendrite*> Dendrites;
    Axon Axon;
    int layerIndex; // Index of the layer this neuron belongs to
    int index; // Index of the neuron in its layer
};

#endif // NEURON_HPP