#include "Neuron.hpp"
#include "Soma.hpp"
#include "Synapse.hpp"
#include <vector>
Neuron::Neuron(std::vector<Synapse*> pre, double bias, int activationFunctionType, int layerIndex, int index)
    : Soma({},bias, activationFunctionType), layerIndex(layerIndex), index(index)
{
    for (auto& synapse : pre) {
        synapse->setNxt(this);
        Dendrites.push_back(synapse);
        Soma::addInput(synapse->getSignal());
    }
}
bool Neuron::isConnectedTo(const Neuron& other) const {
    for (const auto& dendrite : Dendrites) {
        if (dendrite->getPre() == &other) {
            return true;
        }
    }
    for (const auto& dendrite : other.Dendrites) {
        if (dendrite->getPre() == this) {
            return true;
        }
    }
    return false;
}
void Neuron::connectTo(Neuron* other, double weight) {
    if (other != nullptr && other->layerIndex - 1 == layerIndex && !isConnectedTo(*other)) {
        Synapse* synapse = new Synapse(Soma::getOutput(), weight, this, other);
        other->Dendrites.push_back(synapse);
        Axon.push_back(synapse);
    }
    else {
        std::cerr << "Cannot connect: invalid neuron or already connected. "
                  << "From Neuron(layer=" << layerIndex << ", index=" << index << ") "
                  << "To Neuron(layer=" << (other ? other->layerIndex : -1) << ", index=" << (other ? other->index : -1) << ")\n";
        throw std::runtime_error("Cannot connect: invalid neuron or already connected.");
    }
}
void Neuron::disconnectTo(Neuron* other) {
    if (other != nullptr&&isConnectedTo(*other)) {
        for (auto it = other->Dendrites.begin(); it != other->Dendrites.end(); ++it) {
            if ((*it)->getPre() == this) {
                Axon.erase(std::remove(Axon.begin(), Axon.end(), *it), Axon.end());
                delete *it; // Free memory
                other->Dendrites.erase(it);
                return;
            }
        }
    } else {
        std::cerr << "Cannot disconnect: invalid neuron or not connected. "
                  << "From Neuron(layer=" << layerIndex << ", index=" << index << ") "
                  << "To Neuron(layer=" << (other ? other->layerIndex : -1) << ", index=" << (other ? other->index : -1) << ")\n";
        throw std::runtime_error("Cannot disconnect: invalid neuron or not connected.");
        
    }
}
std::vector<double> Neuron::getWeights() const {
    std::vector<double> weights;
    for (const auto& dendrite : Dendrites) {
        weights.push_back(dendrite->getWeight());
    }
    return weights;
}
void Neuron::setWeights(const std::vector<double>& weights) {
    for (size_t i = 0; i < weights.size() && i < Dendrites.size(); ++i) {
        Dendrites[i]->setWeight(weights[i]);
    }
}
void Neuron::setWeight(int index, double weight) {
    if (index >= 0 && index < Dendrites.size()) {
        Dendrites[index]->setWeight(weight);
    }
}
void Neuron::showConnections() const {
    std::cout << "Neuron at layer " << layerIndex << ", index " << index << " connections:\n";
    for (const auto& synapse : Dendrites) {
        std::cout << "  Dendrite connected to Synapse with input: " << synapse->getInput() 
                  << ", weight: " << synapse->getWeight() << "\n";
    }
    std::cout << "  Axon connections: " << Axon.size() << "\n";
    for (const auto& synapse : Axon) {
        std::cout << "    Axon synapse signal: " << synapse->getSignal() << "\n";
    }
}
int Neuron::getLayerIndex() const {
    return layerIndex;
}
int Neuron::getIndex() const {
    return index;
}
void Neuron::setLayerIndex(int newLayerIndex) {
    layerIndex = newLayerIndex;
}
void Neuron::setBias(double newBias) {
    Soma::setBias(newBias);
}
void Neuron::remove()
{
    // Remove all outgoing connections (Axon)
    for (auto* synapse : Axon) {
        if (synapse) {
            Neuron* postNeuron = synapse->getNxt();
            if (postNeuron) {
                auto& dendrites = postNeuron->Dendrites;
                dendrites.erase(
                    std::remove(dendrites.begin(), dendrites.end(), synapse),
                    dendrites.end()
                );
            }
            delete synapse;
        }
    }
    Axon.clear();
    // Remove all incoming connections (Dendrites)
    for (auto* synapse : Dendrites) {
        if (synapse) {
            Neuron* preNeuron = synapse->getPre();
            if (preNeuron) {
                preNeuron->Axon.erase(
                    std::remove(preNeuron->Axon.begin(), preNeuron->Axon.end(), synapse),
                    preNeuron->Axon.end()
                );
            }
            delete synapse;
        }
    }
    Dendrites.clear();
}
void Neuron::updateInput() {
    std::vector<double> inputs;
    for (const auto& dendrite : Dendrites) {
        dendrite->setInput(dendrite->getPre() != nullptr ? dendrite->getPre()->getOutput() : 0.0);
    }
    for (const auto& dendrite : Dendrites) {
        inputs.push_back(dendrite->getSignal());
    }
    
    Soma::setInputs(inputs); // Set the inputs to Soma
}
//!!to do!!
void Neuron::updateOutput()
{
    if(layerIndex != 0) {
        updateInput(); // Update inputs from Dendrites
    }
    Soma::updateOutput(); // Update the Soma output
    // Update Axon signals based on the Soma output
    for (auto& synapse : Axon) {
        synapse->setInput(Soma::getOutput());
    }
}