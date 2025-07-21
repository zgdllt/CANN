//-------------------------------------------------------------------------------------------------------------------
//【文件名】Synapse.hpp
//【功能模块和目的】突触类的声明，定义了神经元之间的连接和信号传递功能
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

#ifndef SYNAPSE_HPP
#define SYNAPSE_HPP

// 可以直接用 Neuron，但如果 Neuron 需要包含 Synapse，建议用指针或引用避免循环包含。
// 例如：Synapse 里保存 Neuron* from, Neuron* to；Neuron 里保存 std::vector<Synapse*>。
// 这样可以避免头文件互相包含导致编译错误。

class Neuron;
class Axon;
#include <vector>

//-------------------------------------------------------------------------------------------------------------------
//【类名】Synapse
//【功能】实现神经元之间的突触连接，管理权重、输入信号和信号传递
//【接口说明】提供突触连接管理、权重设置、信号传递等公有接口
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
class Synapse {
public:
    Synapse();
    Synapse(double input = 0.0, double weight = 1.0, Neuron* pre = nullptr, Neuron* nxt = nullptr); // 基础构造
    Synapse& operator=(const Synapse& other) = default; // 复制赋值
    Neuron* getPre() const;
    Neuron* getNxt() const;
    void setPre(Neuron* pre);
    void setNxt(Neuron* nxt);
    double getWeight() const;
    void setWeight(double weight);
    double getInput() const;
    void setInput(double input);
    double getSignal() const; // 获取当前信号值
private:
    Neuron* pre,*nxt; // 连接的神经元
    double weight;
    double input;
};
// class Dendrite : public Synapse {
// public:
//     Dendrite(double input=0.0, double weight=1.0, Neuron* neuron=nullptr) : Synapse(input, weight, neuron) {previous = nullptr; }
//     Dendrite(Axon* previous = nullptr, double weight = 1.0, Neuron* neuron = nullptr);
//     Axon* getPrevious() const;
//     void setPrevious(Axon* previous);
//     void connect(Axon* preAxon);
//     void disconnect();
//     bool isConnectedTo(const Axon* other) const;
// private:
//     Axon* previous; // 如果是树突，指向前一个轴突
// };
// class Axon : public Synapse {
// public:
//     Axon(double input) : Synapse(input, 1.0) {}
//     Axon(double input, std::vector<Dendrite*> next = {}, Neuron* neuron = nullptr);
//     const std::vector<Dendrite*>& getNext() const;
//     void setNext(const std::vector<Dendrite*>& nextSynapses);
//     void connect(Dendrite* nextSynapse);
//     void disconnect(Dendrite* nextSynapse);
//     void disconnectAll();
//     bool isConnectedTo(const Dendrite* other) const;
// private:
//     std::vector<class Dendrite*> next; // 如果是轴突，指向所连的树突
// };

#endif // SYNAPSE_HPP
