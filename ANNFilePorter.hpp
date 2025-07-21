//-------------------------------------------------------------------------------------------------------------------
//【文件名】ANNFilePorter.hpp
//【功能模块和目的】ANN文件操作类的声明，提供ANN格式文件的导入导出功能
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

#ifndef ANN_FILE_PORTER_HPP
#define ANN_FILE_PORTER_HPP

#include "FilePorter.hpp"
#include "Network.hpp"
#include <string>

//-------------------------------------------------------------------------------------------------------------------
//【类名】ANNImporter
//【功能】从ANN格式文件导入神经网络结构和参数
//【接口说明】继承自FilePorter，提供ANN文件的读取和网络构建功能
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
class ANNImporter : public FilePorter<FilePorterType::IMPORTER> {
public:
    explicit ANNImporter(const std::string& filename) 
        : FilePorter<FilePorterType::IMPORTER>(filename, { "ANN" }) {}
    Network import();

private:
    struct NeuronInfo {
        double bias;
        int activationType;
    };

    struct LayerInfo {
        int startNeuron;
        int endNeuron;
    };

    struct SynapseInfo {
        int fromNeuron;
        int toNeuron;
        double weight;
    };
};

//-------------------------------------------------------------------------------------------------------------------
//【类名】ANNExporter
//【功能】将神经网络结构和参数导出为ANN格式文件
//【接口说明】继承自FilePorter，提供ANN文件的写入功能
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
class ANNExporter : public FilePorter<FilePorterType::EXPORTER> {
public:
    explicit ANNExporter(const std::string& filename) 
        : FilePorter<FilePorterType::EXPORTER>(filename, { "ANN" }) {}
    void exportNetwork(const Network& network, const std::string& networkName = "MyNetwork");
};

#endif // ANN_FILE_PORTER_HPP
