#ifndef ANN_FILE_PORTER_HPP
#define ANN_FILE_PORTER_HPP

#include "FilePorter.hpp"
#include "Network.hpp"

// ANN 文件导入器
class ANNImporter : public FilePorter<FilePorterType::IMPORTER> {
public:
    explicit ANNImporter(const std::string& filename) 
        : FilePorter<FilePorterType::IMPORTER>(filename, { "ANN" }) {}
    // 导入 ANN 文件并创建网络
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

// ANN 文件导出器
class ANNExporter : public FilePorter<FilePorterType::EXPORTER> {
public:
    explicit ANNExporter(const std::string& filename) 
        : FilePorter<FilePorterType::EXPORTER>(filename, { "ANN" }) {}
    // 导出网络到 ANN 文件
    void exportNetwork(const Network& network, const std::string& networkName = "MyNetwork");
};

#endif // ANN_FILE_PORTER_HPP
