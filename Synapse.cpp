#include "Synapse.hpp"
#include <stdexcept>
#include <iostream>
Synapse::Synapse()
    : pre(nullptr)
    , nxt(nullptr)
    , weight(1.0)
    , input(0.0)
{}
Synapse::Synapse(double input, double weight, Neuron* pre, Neuron* nxt)
    : pre(pre)
    , nxt(nxt)
    , weight(weight)
    , input(input)
{}
// Dendrite::Dendrite(Axon* previous, double weight, Neuron* neuron)
//     : Synapse(previous ? previous->getInput() : 0.0, weight, neuron)
//     , previous(previous)
// {
//     if (previous) {
//         previous->connect(this);
//     }
// }
// Axon::Axon(double input, std::vector<Dendrite*> next, Neuron* neuron)
//     : Synapse(input, 1.0, neuron)
//     , next(next)
// {
//     for (auto& synapse : next) {
//         synapse->setPrevious(this);
//     }
// }
double Synapse::getWeight() const {
    return weight;
}
double Synapse::getInput() const {
    return input;
}
double Synapse::getSignal() const
{
    return input * weight; // Signal is calculated based on input and weight
}
void Synapse::setWeight(double weight) {
    this->weight = weight;
}
void Synapse::setInput(double input) {
    this->input = input;
}
Neuron* Synapse::getPre() const {
    return pre;
}
Neuron* Synapse::getNxt() const {
    return nxt;
}
void Synapse::setPre(Neuron* pre) {
    this->pre = pre;
}
void Synapse::setNxt(Neuron* nxt) {
    this->nxt = nxt;
}
// Axon* Dendrite::getPrevious() const {
//     return previous;
// }
// void Dendrite::setPrevious(Axon* previous) {
//     this->previous = previous;
// }
// const std::vector<Dendrite*>& Axon::getNext() const {
//     return next;
// }
// void Axon::setNext(const std::vector<Dendrite*>& nextSynapses) {
//     next = nextSynapses;
//     for (auto& synapse : next) {
//         synapse->setPrevious(this);
//     }
// }
// void Dendrite::connect(Axon* preAxon) {
//     if (preAxon) {
//         preAxon->connect(this);
//         this->previous = preAxon;
//     } else {
//         std::cerr << "Error: Cannot connect: previous Axon is null.\n";
//         throw std::runtime_error("Cannot connect: previous Axon is null.");
//     }
// }
// void Dendrite::disconnect() {
//     if (previous) {
//         previous->disconnect(this);
//         previous = nullptr;
//     } else {
//         std::cerr << "Error: Cannot disconnect: previous Axon is null.\n";
//         throw std::runtime_error("Cannot disconnect: previous Axon is null.");
//     }
// }
// bool Dendrite::isConnectedTo(const Axon* other) const {
//     return previous == other;
// }
// void Axon::connect(Dendrite* nextSynapse) {
//     if (nextSynapse) {
//         next.push_back(nextSynapse);
//         nextSynapse->setPrevious(this);
//     } else {
//         std::cerr << "Error: Cannot connect: next synapse is null.\n";
//         throw std::runtime_error("Cannot connect: next synapse is null.");
//     }
// }
// void Axon::disconnect(Dendrite* nextSynapse) {
//     auto it = std::remove(next.begin(), next.end(), nextSynapse);
//     if (it != next.end()) {
//         next.erase(it);
//         nextSynapse->setPrevious(nullptr);
//     }
// }
// void Axon::disconnectAll() {
//     for (auto* synapse : next) {
//         synapse->setPrevious(nullptr);
//     }
//     next.clear();
// }
// bool Axon::isConnectedTo(const Dendrite* other) const {
//     return std::find(next.begin(), next.end(), other) != next.end();
// }