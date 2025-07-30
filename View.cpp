//-------------------------------------------------------------------------------------------------------------------
//【文件名】View.cpp
//【功能模块和目的】用户界面类的实现，负责所有的用户交互和显示功能
//【开发者及日期】李孟涵 2025年7月30日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

#include <iomanip>  // 格式化输出头文件
#include <iostream> // 输入输出流头文件
#include "View.hpp" // 视图类头文件

// 颜色常量定义
const std::string View::RESET = "\033[0m";
const std::string View::RED = "\033[31m";
const std::string View::GREEN = "\033[32m";
const std::string View::YELLOW = "\033[33m";
const std::string View::BLUE = "\033[34m";
const std::string View::MAGENTA = "\033[35m";
const std::string View::CYAN = "\033[36m";
const std::string View::WHITE = "\033[37m";
const std::string View::GRAY = "\033[90m";

//-------------------------------------------------------------------------------------------------------------------
// 构造函数和析构函数
//-------------------------------------------------------------------------------------------------------------------
View::View() {
    // 构造函数实现
}

View::~View() {
    // 析构函数实现
}

//-------------------------------------------------------------------------------------------------------------------
// 界面显示相关方法
//-------------------------------------------------------------------------------------------------------------------
void View::clearScreen() {
    std::cout << "\033[2J\033[H"; // 清屏并移动光标到左上角
}

void View::showHeader() {
    std::cout << CYAN << std::endl;
    std::cout << "╔══════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                 🧠 ANN 神经网络系统 v1.0                 ║" << std::endl;
    std::cout << "║                Artificial Neural Network                 ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << RESET << std::endl;
}

void View::showMainMenu() {
    showHeader();
    
    std::cout << GREEN << "📋 主菜单:" << RESET << std::endl;
    std::cout << "  1️⃣  导入神经网络文件 (.ANN)" << std::endl;
    std::cout << "  2️⃣  创建新的神经网络" << std::endl;
    std::cout << "  3️⃣  查看网络信息" << std::endl;
    std::cout << "  4️⃣  执行前向传播" << std::endl;
    std::cout << "  5️⃣  导出神经网络" << std::endl;
    std::cout << "  6️⃣  神经元连接状态" << std::endl;
    std::cout << "  7️⃣  增加层" << std::endl;
    std::cout << "  8️⃣  增加神经元" << std::endl;
    std::cout << "  9️⃣  删除层" << std::endl;
    std::cout << "  🔟 删除神经元" << std::endl;
    std::cout << "  ⏸️  修改网络名称" << std::endl;
    std::cout << "  0️⃣  退出程序" << std::endl;
    std::cout << std::endl;
    std::cout << YELLOW << "请选择操作 (0-11): " << RESET;
}

void View::waitForUser() {
    std::cout << std::endl << GRAY << "按回车键继续..." << RESET;
    std::cin.ignore();
    std::cin.get();
}

//-------------------------------------------------------------------------------------------------------------------
// 用户输入获取方法
//-------------------------------------------------------------------------------------------------------------------
int View::getMenuChoice() {
    int choice;
    std::cin >> choice;
    return choice;
}

std::string View::getFilename() {
    std::string filename;
    std::cin >> filename;
    return filename;
}

int View::getLayerCount() {
    std::cout << "请输入层数: ";
    int count;
    std::cin >> count;
    return count;
}

int View::getNeuronCount(int layerIndex) {
    std::cout << "第 " << (layerIndex + 1) << " 层神经元数量: ";
    int count;
    std::cin >> count;
    return count;
}

int View::getLayerIndex() {
    std::cout << "请输入层索引: ";
    int index;
    std::cin >> index;
    return index;
}

int View::getNeuronIndex() {
    std::cout << "请输入神经元索引: ";
    int index;
    std::cin >> index;
    return index;
}

int View::getPosition() {
    int position;
    std::cin >> position;
    return position;
}

double View::getBias() {
    std::cout << "请输入偏置值: ";
    double bias;
    std::cin >> bias;
    return bias;
}

int View::getActivationType() {
    std::cout << "请输入激活函数类型 (0=sigmoid, 1=tanh, 2=relu, 3=linear): ";
    int type;
    std::cin >> type;
    return type;
}

std::vector<double> View::getInputValues(int size) {
    std::vector<double> input(size);
    std::cout << "请输入 " << size << " 个数值:" << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << "  输入[" << i << "]: ";
        std::cin >> input[i];
    }
    return input;
}

std::string View::getNetworkName() {
    std::cout << "请输入新的网络名称: ";
    std::string name;
    std::cin.ignore(); // 清除输入缓冲区
    std::getline(std::cin, name);
    return name;
}

char View::getConfirmation() {
    char confirm;
    std::cin >> confirm;
    return confirm;
}

//-------------------------------------------------------------------------------------------------------------------
// 信息显示方法
//-------------------------------------------------------------------------------------------------------------------
void View::showSuccessMessage(const std::string& message) {
    std::cout << GREEN << "✅ " << message << RESET << std::endl;
}

void View::showErrorMessage(const std::string& message) {
    std::cout << RED << "❌ " << message << RESET << std::endl;
}

void View::showWarningMessage(const std::string& message) {
    std::cout << YELLOW << "⚠️ " << message << RESET << std::endl;
}

void View::showInfoMessage(const std::string& message) {
    std::cout << BLUE << "ℹ️ " << message << RESET << std::endl;
}

void View::showProgress(const std::string& message) {
    std::cout << "🔄 " << message << std::endl;
}

//-------------------------------------------------------------------------------------------------------------------
// 网络信息显示方法
//-------------------------------------------------------------------------------------------------------------------
void View::showNetworkInfo(const Network& network) {
    network.showInfo();
}

void View::showLayerInfo(const Network& network, int layerIndex) {
    std::cout << std::endl << CYAN << "🔍 第 " << layerIndex << " 层详细信息:" << RESET << std::endl;
    network.showLayer(layerIndex);
}

void View::showAllLayersInfo(const Network& network) {
    for (int i = 0; i < network.getLayerCount(); ++i) {
        std::cout << std::endl << CYAN << "🔍 第 " << i << " 层详细信息:" << RESET << std::endl;
        network.showLayer(i);
    }
}

void View::showForwardPropagationResult(const std::vector<std::vector<double>>& output) {
    std::cout << GREEN << "📈 计算结果:" << RESET << std::endl;
    for (size_t i = 0; i < output.size(); ++i) {
        std::cout << "  层 " << i << " 输出: [";
        for (size_t j = 0; j < output[i].size(); ++j) {
            std::cout << std::fixed << std::setprecision(4) << output[i][j];
            if (j < output[i].size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
}

void View::showNeuronConnections(const Neuron& neuron) {
    neuron.showConnections();
}

//-------------------------------------------------------------------------------------------------------------------
// 操作提示方法
//-------------------------------------------------------------------------------------------------------------------
void View::showImportDialog() {
    std::cout << std::endl << BLUE << "📁 导入神经网络文件" << RESET << std::endl;
    std::cout << "请输入文件名: ";
}

void View::showExportDialog() {
    std::cout << std::endl << BLUE << "💾 导出神经网络" << RESET << std::endl;
    std::cout << "请输入导出文件名: ";
}

void View::showCreateNetworkDialog() {
    std::cout << std::endl << BLUE << "🏗️ 创建新的神经网络" << RESET << std::endl;
}

void View::showForwardPropagationDialog() {
    std::cout << std::endl << BLUE << "⚡ 执行前向传播" << RESET << std::endl;
}

void View::showAddLayerDialog() {
    std::cout << std::endl << BLUE << "➕ 增加层" << RESET << std::endl;
}

void View::showAddNeuronDialog() {
    std::cout << std::endl << BLUE << "➕ 增加神经元" << RESET << std::endl;
}

void View::showDeleteLayerDialog() {
    std::cout << std::endl << BLUE << "➖ 删除层" << RESET << std::endl;
}

void View::showDeleteNeuronDialog() {
    std::cout << std::endl << BLUE << "➖ 删除神经元" << RESET << std::endl;
}

void View::showModifyNameDialog() {
    std::cout << std::endl << BLUE << "✏️ 修改网络名称" << RESET << std::endl;
}

void View::showConnectionsDialog() {
    std::cout << std::endl << BLUE << "🔗 神经元连接状态" << RESET << std::endl;
}

void View::showLayerInfoDialog() {
    std::cout << std::endl << BLUE << "📊 网络信息" << RESET << std::endl;
}

void View::showGoodbye() {
    std::cout << std::endl << MAGENTA << "👋 感谢使用 ANN 神经网络系统!" << RESET << std::endl;
    std::cout << CYAN << "🚀 期待下次再见~" << RESET << std::endl;
}

//-------------------------------------------------------------------------------------------------------------------
// 私有辅助方法
//-------------------------------------------------------------------------------------------------------------------
void View::printColorText(const std::string& text, const std::string& color) {
    std::cout << color << text << RESET;
}

void View::printSeparator() {
    std::cout << "----------------------------------------" << std::endl;
}
