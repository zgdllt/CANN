#ifndef SOMA_HPP
#define SOMA_HPP

#include <vector>
#include "ActivationFunc.hpp"
#include "Synapse.hpp"

class Soma : public ActivationFunc {
public:
    Soma();
    Soma(std::vector<double> inputs = {}, double bias = 0.0, int activationFunctionType = 0);
    void addInput(double input);
    std::vector<double> getInputs() const;
    void setInputs(const std::vector<double>& inputs);
    void setBias(double bias);
    double getBias() const;
    void setActivationFunctionType(int type);
    int getActivationFunctionType() const;
    virtual void updateOutput();
    double getOutput() const;

private:
    std::vector<double> inputs;
    double bias;
    int activationFunctionType;
    double activate(double sum) const;
    double output;
};

#endif // SOMA_HPP
