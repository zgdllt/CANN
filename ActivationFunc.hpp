#ifndef ACTIVATION_FUNC_HPP
#define ACTIVATION_FUNC_HPP

class ActivationFunc {
public:
    static double sigmoid(double x);
    static double tanh(double x);
    static double relu(double x);
    static double linear(double x);
};

#endif // ACTIVATION_FUNC_HPP
