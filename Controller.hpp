//-------------------------------------------------------------------------------------------------------------------
//【文件名】Controller.hpp
//【功能模块和目的】控制器类的声明，负责业务逻辑处理和模型与视图的协调
//【开发者及日期】李孟涵 2025年7月30日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "Network.hpp"
#include "View.hpp"
#include <memory>

//-------------------------------------------------------------------------------------------------------------------
//【类名】Controller
//【功能】控制器类，负责处理用户操作的业务逻辑，协调模型（Network）和视图（View）
//【接口说明】包含所有业务操作的处理方法，不直接处理用户界面
//【开发者及日期】李孟涵 2025年7月30日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
class Controller {
public:
    // 构造函数和析构函数
    explicit Controller(View* view);
    ~Controller();

    // 主控制方法
    void run();

    // 文件操作相关方法
    void importNetwork();
    void exportNetwork();

    // 网络操作相关方法
    void createNetwork();
    void showNetworkInfo();
    void executeForwardPropagation();

    // 网络结构修改方法
    void addLayer();
    void addNeuron();
    void deleteLayer();
    void deleteNeuron();

    // 其他功能方法
    void showNeuronConnections();
    void modifyNetworkName();

private:
    // 成员变量
    View* view_;                        // 视图对象指针
    std::unique_ptr<Network> network_;  // 神经网络模型

    // 私有辅助方法
    bool validateLayerIndex(int layerIndex) const;                  // 验证层索引是否合法
    bool validateNeuronIndex(int layerIndex, int neuronIndex) const;// 验证神经元索引是否合法
    bool validatePosition(int position) const;                      // 验证位置是否合法
    void handleException(const std::exception& e);                  // 处理异常情况
    
    // 禁止拷贝构造和赋值
    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;
};

#endif // CONTROLLER_HPP
