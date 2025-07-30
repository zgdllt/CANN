//-------------------------------------------------------------------------------------------------------------------
//【文件名】Controller.cpp
//【功能模块和目的】控制器类的实现，负责业务逻辑处理和模型与视图的协调
//【开发者及日期】李孟涵 2025年7月30日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

#include "Controller.hpp"     // 控制器类头文件
#include "ANNFilePorter.hpp"  // ANN文件导入导出类头文件
#include "Layer.hpp"          // 层类头文件
#include <stdexcept>          // 标准异常头文件
#include <iostream>           // 输入输出流头文件

//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::Controller
// 【函数功能】Controller类的构造函数，初始化视图和网络模型
// 【参数】view - 视图对象指针
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
Controller::Controller(View* view) : view_(view), network_(std::make_unique<Network>()) {
    if (view_ == nullptr) {
        throw std::invalid_argument("View pointer cannot be null");
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::~Controller
// 【函数功能】Controller类的析构函数，释放资源
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
Controller::~Controller() = default;
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::run
// 【函数功能】控制器的主循环，处理用户输入并调用相应的业务逻辑方法
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Controller::run() {
    view_->clearScreen();
    
    while (true) {
        view_->showMainMenu();
        
        try {
            int choice = view_->getMenuChoice();
            
            switch (choice) {
                case 1:
                    importNetwork();
                    break;
                case 2:
                    createNetwork();
                    break;
                case 3:
                    showNetworkInfo();
                    break;
                case 4:
                    executeForwardPropagation();
                    break;
                case 5:
                    exportNetwork();
                    break;
                case 6:
                    showNeuronConnections();
                    break;
                case 7:
                    addLayer();
                    break;
                case 8:
                    addNeuron();
                    break;
                case 9:
                    deleteLayer();
                    break;
                case 10:
                    deleteNeuron();
                    break;
                case 11:
                    modifyNetworkName();
                    break;
                case 0:
                    view_->showGoodbye();
                    return;
                default:
                    view_->showErrorMessage("无效选择，请输入 0-11 之间的数字");
                    break;
            }
        } catch (const std::exception& exception) {
            handleException(exception);
        }
        
        view_->waitForUser();
        view_->clearScreen();
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::importNetwork
// 【函数功能】导入神经网络文件
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Controller::importNetwork() {
    view_->showImportDialog();
    std::string filename = view_->getFilename();
    
    view_->showProgress("正在导入 " + filename + "...");
    
    try {
        ANNImporter importer(filename);
        Network importedNetwork = importer.import();
        *network_ = importedNetwork;
        view_->showSuccessMessage("导入成功!");
        view_->showNetworkInfo(*network_);
    } catch (const std::exception& exception) {
        view_->showErrorMessage("导入失败: " + std::string(exception.what()));
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::exportNetwork
// 【函数功能】导出神经网络到文件
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Controller::exportNetwork() {
    view_->showExportDialog();
    std::string filename = view_->getFilename();
    
    try {
        ANNExporter exporter(filename);
        exporter.exportNetwork(*network_);
        view_->showSuccessMessage("导出到 " + filename + " 成功!");
    } catch (const std::exception& exception) {
        view_->showErrorMessage("导出失败: " + std::string(exception.what()));
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::createNetwork
// 【函数功能】创建新的神经网络
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Controller::createNetwork() {
    view_->showCreateNetworkDialog();
    
    auto newNetwork = std::make_unique<Network>();
    int layerCount = view_->getLayerCount();
    
    for (int i = 0; i < layerCount; i++) {
        int neuronCount = view_->getNeuronCount(i);
        std::vector<double> inputs(i == 0 ? neuronCount : 0, 0.0);
        Layer* layer = new Layer(newNetwork.get(), neuronCount, inputs, 0);
        newNetwork->addLayer(layer);
    }
    
    network_ = std::move(newNetwork);
    view_->showSuccessMessage("网络创建成功!");
    view_->showNetworkInfo(*network_);
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::showNetworkInfo
// 【函数功能】显示当前神经网络的详细信息
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Controller::showNetworkInfo() {
    view_->showLayerInfoDialog();
    view_->showNetworkInfo(*network_);
    
    std::cout << "请输入要查看的层索引 (-1 查看所有层): ";
    int layerIndex = view_->getLayerIndex();
    
    if (layerIndex == -1) {
        view_->showAllLayersInfo(*network_);
    } else if (validateLayerIndex(layerIndex)) {
        view_->showLayerInfo(*network_, layerIndex);
    } else {
        view_->showErrorMessage("无效的层索引!");
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::executeForwardPropagation
// 【函数功能】执行神经网络的前向传播
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Controller::executeForwardPropagation() {
    view_->showForwardPropagationDialog();
    
    if (network_->getLayerCount() == 0) {
        view_->showErrorMessage("网络为空，无法执行前向传播!");
        return;
    }
    
    int inputSize = network_->getLayer(0)->getNeuronCount();
    auto input = view_->getInputValues(inputSize);
    
    view_->showProgress("正在计算...");
    
    try {
        auto output = network_->forward(input);
        view_->showForwardPropagationResult(output);
    } catch (const std::exception& exception) {
        view_->showErrorMessage("前向传播失败: " + std::string(exception.what()));
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::addLayer
// 【函数功能】添加新层
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Controller::addLayer() {
    view_->showAddLayerDialog();
    
    std::cout << "请输入新层的神经元数量: ";
    int neuronCount = view_->getNeuronCount(0);
    
    std::cout << "请输入插入位置 (0-" << network_->getLayerCount() << "): ";
    int position = view_->getPosition();
    
    if (!validatePosition(position)) {
        view_->showErrorMessage("无效的位置!");
        return;
    }
    
    std::vector<double> inputs(neuronCount, 0.0);
    Layer* newLayer = new Layer(network_.get(), neuronCount, inputs, 0);
    
    if (position == network_->getLayerCount()) {
        network_->addLayer(newLayer);
    } else {
        network_->addLayer(newLayer);
        // 插入到指定位置需要网络类支持插入操作
    }
    
    view_->showSuccessMessage("成功添加层!");
    view_->showNetworkInfo(*network_);
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::addNeuron
// 【函数功能】添加神经元到指定层
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Controller::addNeuron() {
    view_->showAddNeuronDialog();
    
    int layerIndex = view_->getLayerIndex();
    
    if (!validateLayerIndex(layerIndex)) {
        view_->showErrorMessage("无效的层索引!");
        return;
    }
    
    double bias = view_->getBias();
    int activationType = view_->getActivationType();
    
    network_->addNeuron(layerIndex, bias, activationType);
    
    view_->showSuccessMessage("成功添加神经元到第 " + std::to_string(layerIndex) + " 层!");
    view_->showLayerInfo(*network_, layerIndex);
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::deleteLayer
// 【函数功能】删除指定层
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Controller::deleteLayer() {
    view_->showDeleteLayerDialog();
    
    if (network_->getLayerCount() <= 1) {
        view_->showErrorMessage("网络至少需要保留一层!");
        return;
    }
    
    std::cout << "请输入要删除的层索引 (0-" << (network_->getLayerCount()-1) << "): ";
    int layerIndex = view_->getLayerIndex();
    
    if (!validateLayerIndex(layerIndex)) {
        view_->showErrorMessage("无效的层索引!");
        return;
    }
    
    view_->showWarningMessage("确认删除第 " + std::to_string(layerIndex) + " 层? (y/n): ");
    char confirm = view_->getConfirmation();
    
    if (confirm == 'y' || confirm == 'Y') {
        network_->deleteLayer(layerIndex);
        view_->showSuccessMessage("成功删除第 " + std::to_string(layerIndex) + " 层!");
        view_->showNetworkInfo(*network_);
    } else {
        view_->showInfoMessage("删除操作已取消");
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::deleteNeuron
// 【函数功能】删除指定层中的神经元
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Controller::deleteNeuron() {
    view_->showDeleteNeuronDialog();
    
    int layerIndex = view_->getLayerIndex();
    
    if (!validateLayerIndex(layerIndex)) {
        view_->showErrorMessage("无效的层索引!");
        return;
    }
    
    const Layer* layer = network_->getLayer(layerIndex);
    if (layer->getNeuronCount() <= 1) {
        view_->showErrorMessage("层至少需要保留一个神经元!");
        return;
    }
    
    std::cout << "请输入要删除的神经元索引 (0-" << (layer->getNeuronCount()-1) << "): ";
    int neuronIndex = view_->getNeuronIndex();
    
    if (!validateNeuronIndex(layerIndex, neuronIndex)) {
        view_->showErrorMessage("无效的神经元索引!");
        return;
    }
    
    view_->showWarningMessage("确认删除第 " + std::to_string(layerIndex) + " 层的第 " + 
                             std::to_string(neuronIndex) + " 个神经元? (y/n): ");
    char confirm = view_->getConfirmation();
    
    if (confirm == 'y' || confirm == 'Y') {
        network_->deleteNeuron(layerIndex, neuronIndex);
        view_->showSuccessMessage("成功删除神经元!");
        view_->showLayerInfo(*network_, layerIndex);
    } else {
        view_->showInfoMessage("删除操作已取消");
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::showNeuronConnections
// 【函数功能】显示指定神经元的连接信息
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Controller::showNeuronConnections() {
    view_->showConnectionsDialog();
    
    int layerIndex = view_->getLayerIndex();
    int neuronIndex = view_->getNeuronIndex();
    
    if (!validateLayerIndex(layerIndex)) {
        view_->showErrorMessage("无效的层索引!");
        return;
    }
    
    if (!validateNeuronIndex(layerIndex, neuronIndex)) {
        view_->showErrorMessage("无效的神经元索引!");
        return;
    }
    
    const Layer* layer = network_->getLayer(layerIndex);
    Neuron neuron = layer->getNeuron(neuronIndex);
    view_->showNeuronConnections(neuron);
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::modifyNetworkName
// 【函数功能】修改神经网络的名称
// 【参数】无
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Controller::modifyNetworkName() {
    view_->showModifyNameDialog();
    std::string newName = view_->getNetworkName();
    network_->setName(newName);
    view_->showSuccessMessage("网络名称已更新为: " + newName);
}

//-------------------------------------------------------------------------------------------------------------------
// 私有辅助方法
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::validateLayerIndex
// 【函数功能】验证层索引是否有效
// 【参数】layerIndex - 层索引
// 【返回值】bool - 如果索引有效返回true，否则返回false
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
bool Controller::validateLayerIndex(int layerIndex) const {
    return layerIndex >= 0 && layerIndex < network_->getLayerCount();
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::validateNeuronIndex
// 【函数功能】验证神经元索引是否有效
// 【参数】layerIndex - 层索引, neuronIndex - 神经元索引
// 【返回值】bool - 如果索引有效返回true，否则返回false
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

bool Controller::validateNeuronIndex(int layerIndex, int neuronIndex) const {
    if (!validateLayerIndex(layerIndex)) {
        return false;
    }
    const Layer* layer = network_->getLayer(layerIndex);
    return neuronIndex >= 0 && neuronIndex < layer->getNeuronCount();
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::validatePosition
// 【函数功能】验证插入位置是否有效
// 【参数】position - 插入位置
// 【返回值】bool - 如果位置有效返回true，否则返回false
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
bool Controller::validatePosition(int position) const {
    return position >= 0 && position <= network_->getLayerCount();
}
//-------------------------------------------------------------------------------------------------------------------
// 【函数名称】Controller::handleException
// 【函数功能】处理异常并显示错误信息
// 【参数】exception - 异常对象
// 【返回值】无
// 【开发者及日期】李孟涵 2025年7月30日
// 【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
void Controller::handleException(const std::exception& exception) {
    view_->showErrorMessage("错误: " + std::string(exception.what()));
}
