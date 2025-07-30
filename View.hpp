//-------------------------------------------------------------------------------------------------------------------
//【文件名】View.hpp
//【功能模块和目的】用户界面类的声明，负责所有的用户交互和显示功能
//【开发者及日期】李孟涵 2025年7月30日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

#ifndef VIEW_HPP
#define VIEW_HPP

#include <string>// 字符串所在头文件
#include <vector>// vector所在头文件

#include "Network.hpp" // 网络类头文件

//-------------------------------------------------------------------------------------------------------------------
//【类名】View
//【功能】提供用户界面相关的所有功能，包括菜单显示、信息输出、用户输入等
//【接口说明】纯界面类，不包含业务逻辑，只负责与用户的交互
//【开发者及日期】李孟涵 2025年7月30日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
class View {
public:
    // 构造函数和析构函数
    View();
    ~View();

    // 界面显示相关方法
    void clearScreen();
    void showMainMenu();
    void showHeader();
    void waitForUser();
    
    // 用户输入获取方法
    int getMenuChoice();
    std::string getFilename();
    int getLayerCount();
    int getNeuronCount(int layerIndex);
    int getLayerIndex();
    int getNeuronIndex();
    int getPosition();
    double getBias();
    int getActivationType();
    std::vector<double> getInputValues(int size);
    std::string getNetworkName();
    char getConfirmation();

    // 信息显示方法
    void showSuccessMessage(const std::string& message);
    void showErrorMessage(const std::string& message);
    void showWarningMessage(const std::string& message);
    void showInfoMessage(const std::string& message);
    void showProgress(const std::string& message);
    
    // 网络信息显示方法
    void showNetworkInfo(const Network& network);
    void showLayerInfo(const Network& network, int layerIndex);
    void showAllLayersInfo(const Network& network);
    void showForwardPropagationResult(const std::vector<std::vector<double>>& output);
    void showNeuronConnections(const Neuron& neuron);

    // 操作提示方法
    void showImportDialog();
    void showExportDialog();
    void showCreateNetworkDialog();
    void showForwardPropagationDialog();
    void showAddLayerDialog();
    void showAddNeuronDialog();
    void showDeleteLayerDialog();
    void showDeleteNeuronDialog();
    void showModifyNameDialog();
    void showConnectionsDialog();
    void showLayerInfoDialog();

    // 退出和感谢信息
    void showGoodbye();

private:
    // 颜色和格式化常量
    static const std::string RESET;
    static const std::string RED;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string BLUE;
    static const std::string MAGENTA;
    static const std::string CYAN;
    static const std::string WHITE;
    static const std::string GRAY;

    // 私有辅助方法
    void printColorText(const std::string& text, const std::string& color);
    void printSeparator();
};

#endif // VIEW_HPP
