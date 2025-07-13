#ifndef LAYER_HPP
#define LAYER_HPP

#include "Neuron.hpp"
#include <vector>

class Layer{
public:
    Layer(int index=0,int neuronCount=0, std::vector<double> biases=std::vector<double>(), int activationFunctionType=0);
    void addNeuron(const Neuron& neuron);
    void printNeurons() const;
    const std::vector<Neuron>& getNeurons() const;
    int getNeuronCount() const;
    void setIndex(int newIndex);
    void setBias(int neuronIndex, double newBias);
    int getIndex() const;
    bool isConnectedTo(const Layer& other) const;
    Layer* getPreviousLayer() const;
    Layer* getNextLayer() const;
    void connectTo(Layer* nextLayer);
    void setWeights(const std::vector<std::vector<double>>& weights);
    void setInput(const std::vector<double>& input);
    void updateOutputs();
    std::vector<double> getOutputs() const;
private:
    int index; // Index of the layer in the network
    Layer* previousLayer;
    Layer* nextLayer;
    std::vector<Neuron> neurons;
};
#endif // LAYER_HPP