//-------------------------------------------------------------------------------------------------------------------
//【文件名】Synapse.hpp
//【功能模块和目的】突触类的声明，定义了神经元之间的连接和信号传递功能
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】2025年7月13日 去除对树突和轴突的区分
//-------------------------------------------------------------------------------------------------------------------

#ifndef SYNAPSE_HPP
#define SYNAPSE_HPP

class Neuron; // 预先定义Neuron类，避免头文件互相包含导致编译错误。

//-------------------------------------------------------------------------------------------------------------------
//【类名】Synapse
//【功能】实现神经元之间的突触连接，管理权重、输入信号和信号传递
//【接口说明】提供突触连接管理、权重设置、信号传递等公有接口
//【开发者及日期】李孟涵 2025年7月13日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
class Synapse {
public:
    Synapse();                                // 默认构造函数，初始化为空突触
    Synapse(double input = 0.0, 
            double weight = 1.0, 
            Neuron* pre = nullptr, 
            Neuron* nxt = nullptr);          // 基础构造
    Synapse& operator=(const Synapse& other) = default; // 复制赋值
    Neuron* getPre() const;                  // 获取前驱神经元指针
    Neuron* getNxt() const;                  // 获取后继神经元指针
    void setPre(Neuron* pre);                // 设置前驱神经元指针
    void setNxt(Neuron* nxt);                // 设置后继神经元指针
    double getWeight() const;                // 获取当前突触权重
    void setWeight(double weight);           // 设置突触权重
    double getInput() const;                 // 获取当前输入信号
    void setInput(double input);             // 设置输入信号
    double getSignal() const;                // 获取当前信号值
private:
    Neuron* pre, *nxt;                       // 连接的神经元
    double weight;                           // 突触权重
    double input;                            // 输入信号
};
//-------------------------------------------------------------------------------------------------------------------
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
