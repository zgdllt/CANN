//-------------------------------------------------------------------------------------------------------------------
//【文件名】Synapse.cpp
//【功能模块和目的】突触类的实现，包含突触连接和信号传递的所有功能实现
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

#include "Synapse.hpp"
#include <stdexcept>
#include <iostream>

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Synapse::Synapse
//【函数功能】Synapse类的默认构造函数，初始化突触的基本属性
//【参数】无
//【返回值】无
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
Synapse::Synapse()
    : pre(nullptr)
    , nxt(nullptr)
    , weight(1.0)
    , input(0.0)
{}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Synapse::Synapse
//【函数功能】Synapse类的带参构造函数，初始化突触的连接和权重
//【参数】input - 输入信号，weight - 权重，pre - 前置神经元指针，nxt - 后置神经元指针
//【返回值】无
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Synapse::getWeight
//【函数功能】获取突触的权重值
//【参数】无
//【返回值】double - 突触的权重值
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
    return weight;
}
double Synapse::getInput() const {
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Synapse::getInput
//【函数功能】获取突触的输入信号值
//【参数】无
//【返回值】double - 输入信号值
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
    return input;
}
double Synapse::getSignal() const
{
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Synapse::getSignal
//【函数功能】计算并返回突触的信号值
//【参数】无
//【返回值】double - 突触的信号值
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
    return input * weight; // Signal is calculated based on input and weight
}
//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Synapse::setWeight
//【函数功能】设置突触的权重值
//【参数】weight - 突触的权重值
//【返回值】无
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Synapse::setWeight(double weight) {
    this->weight = weight;
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Synapse::setInput
//【函数功能】设置突触的输入信号值
//【参数】input - 输入信号值
//【返回值】无
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Synapse::setInput(double input) {
    this->input = input;
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Synapse::getPre
//【函数功能】获取前置神经元指针
//【参数】无
//【返回值】Neuron* - 前置神经元指针
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
Neuron* Synapse::getPre() const {
    return pre;
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Synapse::getNxt
//【函数功能】获取后置神经元指针
//【参数】无
//【返回值】Neuron* - 后置神经元指针
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
Neuron* Synapse::getNxt() const {
    return nxt;
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Synapse::setPre
//【函数功能】设置前置神经元指针
//【参数】pre - 前置神经元指针
//【返回值】无
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Synapse::setPre(Neuron* pre) {
    this->pre = pre;
}

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】Synapse::setNxt
//【函数功能】设置后置神经元指针
//【参数】nxt - 后置神经元指针
//【返回值】无
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
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