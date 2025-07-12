#include "Neuron.hpp"
#include "Soma.hpp"
#include "Synapse.hpp"
#include <vector>
Neuron::Neuron(std::vector<Dendrite*> pre, double bias, int activationFunctionType, int layerIndex, int index)
    : Soma(pre, bias, activationFunctionType), Axon(Soma::getOutput(),{},this), layerIndex(layerIndex), index(index) {
        for (auto& dendrite : pre) {
            Dendrites.push_back(dendrite);
        }
}
bool Neuron::isConnectedTo(const Neuron& other) const {
    for (const auto& dendrite : Dendrites) {
        if (other.Axon.isConnectedTo(dendrite)) {
            return true;
        }
    }
    for (const auto& dendrite : other.Dendrites) {
        if (Axon.isConnectedTo(dendrite)) {
            return true;
        }
    }
    return false;
}
void Neuron::connectTo(Neuron* other, double weight) {
    if (other != nullptr && other->layerIndex - 1 == layerIndex && !isConnectedTo(*other)) {
        Dendrite* synapse = new Dendrite(&Axon, weight, other);
        other->Soma::addInput(synapse->getSignal());
        other->Dendrites.push_back(synapse);
        Axon.connect(synapse);
    }
    else {
        std::cerr << "Cannot connect: invalid neuron or already connected. "
                  << "From Neuron(layer=" << layerIndex << ", index=" << index << ") "
                  << "To Neuron(layer=" << (other ? other->layerIndex : -1) << ", index=" << (other ? other->index : -1) << ")\n";
        throw std::runtime_error("Cannot connect: invalid neuron or already connected.");
    }
}
void Neuron::connectFrom(Neuron* other, double weight) {
    // Check if 'other' is not nullptr, and layer indices are correct, and not already connected
    bool alreadyConnected = false;
    if (other != nullptr && layerIndex - 1 == other->layerIndex) {
        // Check if any of 'other' neuron's Axon connections are to this neuron
        for (const auto& dendrite : Dendrites) {
            if (other->Axon.isConnectedTo(dendrite)) {
                alreadyConnected = true;
                break;
            }
        }
        if (!alreadyConnected) {
            Dendrite* synapse = new Dendrite(&(other->Axon), weight, this);
            Soma::addInput(synapse->getSignal());
            Axon.setInput(Soma::getOutput());
            Dendrites.push_back(synapse);
            other->Axon.connect(synapse); // Connect the synapse to the axon of the other neuron
        } else {
            std::cerr << "Cannot connect: already connected to this neuron. "
                      << "From Neuron(layer=" << layerIndex << ", index=" << index << ") "
                      << "To Neuron(layer=" << other->layerIndex << ", index=" << other->index << ")\n";
            throw std::runtime_error("Cannot connect: already connected to this neuron.");
        }
    } else {
        std::cerr << "Cannot connect: invalid neuron or already connected. "
                  << "From Neuron(layer=" << layerIndex << ", index=" << index << ") "
                  << "To Neuron(layer=" << (other ? other->layerIndex : -1) << ", index=" << (other ? other->index : -1) << ")\n";
        throw std::runtime_error("Cannot connect: invalid neuron or already connected.");
    }
}
void Neuron::disconnectFrom(Neuron* other) {
    if (other != nullptr) {
        for (auto it = Dendrites.begin(); it != Dendrites.end(); ++it) {
            if ((*it)->getNeuron() == other) {
                other->Axon.disconnect(*it);
                delete *it; // Free memory
                Dendrites.erase(it);
                return;
            }
        }
    }
}
void Neuron::disconnectTo(Neuron* other) {
    if (other != nullptr) {
        for (auto it = other->Dendrites.begin(); it != other->Dendrites.end(); ++it) {
            if ((*it)->getNeuron() == this) {
                Axon.disconnect(*it);
                delete *it; // Free memory
                other->Dendrites.erase(it);
                return;
            }
        }
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
    std::cout << "  Axon output: " << Axon.getSignal() << "\n";
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
    for (auto* synapse : Axon.getNext()) {
        if (synapse) {
            Neuron* postNeuron = synapse->getNeuron();
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
    Axon.disconnectAll();
    // Remove all incoming connections (Dendrites)
    for (auto* synapse : Dendrites) {
        if (synapse) {
            Neuron* preNeuron = synapse->getNeuron();
            if (preNeuron) {
                preNeuron->Axon.disconnect(synapse);
            }
            delete synapse;
        }
    }
    Dendrites.clear();
}
void Neuron::updateInput() {
    std::vector<double> inputs;
    for (const auto& dendrite : Dendrites) {
        dendrite->setInput(dendrite->getPrevious()->getSignal());
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
    Axon.setInput(Soma::getOutput()); // Update the Axon input based on Soma output
}