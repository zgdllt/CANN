#include "Soma.hpp"
#include "Synapse.hpp"
Soma::Soma() : bias(0.0), activationFunctionType(0), output(0.0) {}
Soma::Soma(std::vector<double> inputs, double bias, int activationFunctionType)
    : inputs(inputs)
    , bias(bias)
    , activationFunctionType(activationFunctionType)
    , output(0.0)
{
    updateOutput();
}
Soma::Soma(std::vector<Dendrite*> inputs, double bias, int activationFunctionType)
    : bias(bias), activationFunctionType(activationFunctionType), output(0.0) {
    for (const auto& dendrite : inputs) {
        this->inputs.push_back(dendrite->getSignal());
    }
    updateOutput();
}
void Soma::addInput(double input) {
    inputs.push_back(input);
    updateOutput();
}
std::vector<double> Soma::getInputs() const {
    return inputs;
}
void Soma::setInputs(const std::vector<double>& inputs) {
    this->inputs = inputs;
    updateOutput();
}
void Soma::setBias(double bias) {
    this->bias = bias;
    updateOutput();
}
double Soma::getBias() const {
    return bias;
}
int Soma::getActivationFunctionType() const {
    return activationFunctionType;
}
void Soma::setActivationFunctionType(int type) {
    this->activationFunctionType = type;
    updateOutput();
}
double Soma::activate(double sum) const {
    switch (activationFunctionType) {
        case 1: return ActivationFunc::sigmoid(sum);
        case 2: return ActivationFunc::tanh(sum);
        case 3: return ActivationFunc::relu(sum);
        default: return ActivationFunc::linear(sum);
    }
}
void Soma::updateOutput() {
    double sum = bias;
    for (const auto& signal : inputs) {
        sum += signal;
    }
    output = activate(sum);
}
double Soma::getOutput() const {
    return output;
}