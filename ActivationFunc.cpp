#include "ActivationFunc.hpp"
#include <cmath>

double ActivationFunc::sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}

double ActivationFunc::tanh(double x) {
    return std::tanh(x);
}

double ActivationFunc::relu(double x) {
    return x > 0 ? x : 0;
}

double ActivationFunc::linear(double x) {
    return x;
}