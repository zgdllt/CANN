#include "Layer.hpp"
#include "Synapse.hpp"
#include <vector>
Layer::Layer(int index,int neuronCount, std::vector<double> biases, int activationFunctionType)
{
    this->index = index;
    this->previousLayer = nullptr;
    this->nextLayer = nullptr;
    if (biases.size() < neuronCount) {
        biases.resize(neuronCount, 0.0); // Ensure biases has enough elements
    }
    for (size_t i = 0; i < neuronCount; ++i) {
        neurons.emplace_back(std::vector<Synapse*>(), biases[i], activationFunctionType, index, i);
    }
}

void Layer::addNeuron(const Neuron& neuron) {
    neurons.push_back(neuron);
}

const std::vector<Neuron>& Layer::getNeurons() const {
    return neurons;
}
void Layer::setWeights(const std::vector<std::vector<double>>& weights) {
    if (weights.size() != neurons.size()) {
        std::cerr << "Error: Weights size does not match the number of neurons in the layer.\n";
        throw std::invalid_argument("Weights size does not match the number of neurons in the layer");
    }
    if (previousLayer == nullptr) {
        std::cerr << "Error: Cannot set weights for the first layer in the network.\n";
        throw std::runtime_error("Cannot set weights: Layer is the first layer in the network");
    }
    for (size_t i = 0; i < neurons.size(); ++i) {
        neurons[i].setWeights(weights[i]);
    }
}
int Layer::getNeuronCount() const {
    return neurons.size();
}

int Layer::getIndex() const {
    return index;
}
void Layer::setIndex(int newIndex) {
    index = newIndex;
    for (auto& neuron : neurons) {
        neuron.setLayerIndex(newIndex);
    }
}
void Layer::setBias(int neuronIndex, double newBias) {
    if (neuronIndex < 0 || neuronIndex >= neurons.size()) {
        std::cerr << "Error: Neuron index out of range.\n";
        throw std::out_of_range("Neuron index out of range");
    }
    neurons[neuronIndex].setBias(newBias);
}
void Layer::setInput(const std::vector<double>& input) {
    if (input.size() != neurons.size()) {
        std::cerr << "Error: Input size does not match the number of neurons in the layer.\n";
        throw std::invalid_argument("Input size does not match the number of neurons in the layer");
    }
    if (previousLayer) {
        std::cerr << "Error: Cannot set input for a layer that is not the first layer in the network.\n";
        throw std::runtime_error("Cannot set input: Layer is not the first layer in the network");
    }
    for (size_t i = 0; i < neurons.size(); ++i) {
        neurons[i].addInput(input[i]);
    }
}
bool Layer::isConnectedTo(const Layer& other) const {
    if(this->nextLayer == &other||this->previousLayer == &other) {
        return true;
    }
    return false;
}
void Layer::printNeurons() const {
    for (const auto& neuron : neurons) {
        std::cout << "Neuron Index: " << neuron.getIndex() << ", Layer Index: " << neuron.getLayerIndex() << "\n";
    }
}
Layer* Layer::getPreviousLayer() const {
    return previousLayer;
}
Layer* Layer::getNextLayer() const {
    return nextLayer;
}
void Layer::connectTo(Layer* nextLayer)
{
    // Disconnect existing connections to the next layer
    for (auto& neuron : neurons) {
        for (const auto& nextNeuron : nextLayer->getNeurons()) {
            if (neuron.isConnectedTo(nextNeuron)) {
                neuron.disconnectTo(const_cast<Neuron*>(&nextNeuron));
            }
        }
    }
    this->nextLayer = nextLayer;
    if (nextLayer) {
        nextLayer->previousLayer = this;
    }
    for (auto& neuron : neurons) {
        for (const auto& nextNeuron : nextLayer->getNeurons()) {
            neuron.connectTo(const_cast<Neuron*>(&nextNeuron));
        }
    }
}
void Layer::updateOutputs() {
    for (auto& neuron : neurons) {
        neuron.updateOutput();
    }
}