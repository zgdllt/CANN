//-------------------------------------------------------------------------------------------------------------------
//【文件名】Soma.hpp
//【功能模块和目的】神经元胞体类的声明，定义了神经元的基本计算功能，继承自激活函数类
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

#ifndef SOMA_HPP
#define SOMA_HPP

#include <vector>
#include "ActivationFunc.hpp"
#include "Synapse.hpp"

//-------------------------------------------------------------------------------------------------------------------
//【类名】Soma
//【功能】实现神经元胞体的基本功能，包括输入处理、偏置设置、激活函数选择和输出计算
//【接口说明】提供输入输出管理、偏置设置、激活函数配置等公有接口，继承激活函数功能
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
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
