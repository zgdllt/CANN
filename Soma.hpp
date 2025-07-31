//-------------------------------------------------------------------------------------------------------------------
//【文件名】Soma.hpp
//【功能模块和目的】神经元胞体类的声明，定义了神经元的基本计算功能，继承自激活函数类
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

#ifndef SOMA_HPP
#define SOMA_HPP

#include "ActivationFunc.hpp" // 激活函数所属头文件
#include <vector> // vector所属头文件

//-------------------------------------------------------------------------------------------------------------------
//【类名】Soma
//【功能】实现神经元胞体的基本功能，包括输入处理、偏置设置、激活函数选择和输出计算
//【接口说明】提供输入输出管理、偏置设置、激活函数配置等公有接口，继承激活函数功能
//  - Soma(): 默认构造函数，初始化空输入向量和默认偏置值
//  - Soma(std::vector<double> inputs, double bias, int activationFunctionType): 构造函数
//  - void addInput(double input): 添加输入信号到输入向量
//  - std::vector<double> getInputs() const: 获取当前输入向量
//  - void setInputs(const std::vector<double>& inputs): 设置输入向量
//  - void setBias(double bias): 设置偏置值
//  - double getBias() const: 获取当前偏置值
//  - void setActivationFunctionType(int type): 设置激活函数类型
//  - int getActivationFunctionType() const: 获取当前激活函数类型
//  - virtual void updateOutput(): 更新输出值，计算输入和偏置的激活函数结果
//  - double getOutput() const: 获取当前输出值
//  - virtual ~Soma(): 虚析构函数
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
class Soma : public ActivationFunc { // 继承自激活函数类，从激活函数功能出发
public:
    Soma();                                       // 默认构造函数，初始化空输入向量和默认偏置值
    Soma(std::vector<double> inputs = {}, 
         double bias = 0.0, 
         int activationFunctionType = 0);        // 构造函数，初始化输入向量、偏置和激活函数类型
    void addInput(double input);                 // 添加输入信号到输入向量
    std::vector<double> getInputs() const;       // 获取当前输入向量
    void setInputs(const std::vector<double>& inputs); // 设置输入向量
    void setBias(double bias);                   // 设置偏置值
    double getBias() const;                      // 获取当前偏置值
    void setActivationFunctionType(int type);    // 设置激活函数类型
    int getActivationFunctionType() const;       // 获取当前激活函数类型
    virtual void updateOutput();                 // 更新输出值，计算当前输入和偏置的激活函数结果
    double getOutput() const;                    // 获取当前输出值
    virtual ~Soma() = default;                   // 默认析构函数

private:
    std::vector<double> inputs;                  // 输入信号
    double bias;                                  // 偏置值
    int activationFunctionType;                   // 激活函数类型
    double activate(double sum) const;           // 激活操作
    double output;                                // 输出值
};

#endif // SOMA_HPP
