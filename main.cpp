//-------------------------------------------------------------------------------------------------------------------
//【文件名】main.cpp
//【功能模块和目的】程序主入口，演示神经网络的创建、训练和使用
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】2025年7月24日 增加了对网络有效性的检查, 增加对网络名称的支持
//-------------------------------------------------------------------------------------------------------------------

#include <iostream>          // 输入输出流头文件
#include <iomanip>           // 输入输出格式化头文件
#include "ANNFilePorter.hpp" // 神经网络文件导入导出类的头文件
#include "Layer.hpp"         // 层类头文件
#include "Network.hpp"       // 网络类头文件
#include "Neuron.hpp"        // 神经元类头文件

//-------------------------------------------------------------------------------------------------------------------
//【函数名称】main
//【函数功能】程序主函数，演示神经网络的文件导入、前向传播和导出功能
//【参数】无
//【返回值】int - 程序退出状态码
//【开发者及日期】李孟涵 2025年7月21日
//【更改记录】2025年7月24日 增加了对网络有效性的检查, 增加对网络名称的支持
//-------------------------------------------------------------------------------------------------------------------

int main()
{
    std::cout << "\033[2J\033[H"; // 清屏
    Network network;
    while (true) {
        // 显示主菜单
        std::cout << "\033[36m" << std::endl;
        std::cout << "╔══════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║                 🧠 ANN 神经网络系统 v1.0                 ║" << std::endl;
        std::cout << "║                Artificial Neural Network                 ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════════════╝" << std::endl;
        std::cout << "\033[0m" << std::endl;

        std::cout << "\033[32m📋 主菜单:\033[0m" << std::endl;
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
        std::cout << "\033[33m请选择操作 (0-11): \033[0m";

        int choice;
        std::cin >> choice;

        try {
            switch (choice) {
                case 1: {
                    std::cout << "\n\033[34m📁 导入神经网络文件\033[0m" << std::endl;
                    std::cout << "请输入文件名: ";
                    std::string filename;
                    std::cin >> filename;

                    std::cout << "🔄 正在导入 " << filename << "..." << std::endl;
                    ANNImporter importer(filename);
                    try {
                        Network importedNetwork = importer.import();
                        network = importedNetwork; // 更新当前网络
                        std::cout << "\033[32m✅ 导入成功!\033[0m" << std::endl;
                        network.showInfo();
                    } catch (const std::exception& e) {
                        std::cout << "\033[31m❌ 导入失败: " << e.what() << "\033[0m" << std::endl;
                    }
                    break;
                }

                case 2: {
                    std::cout << "\n\033[34m🏗️  创建新的神经网络\033[0m" << std::endl;
                    Network* newNetwork = new Network();

                    std::cout << "请输入层数: ";
                    int layerCount;
                    std::cin >> layerCount;

                    for (int i = 0; i < layerCount; i++) {
                        std::cout << "第 " << (i + 1) << " 层神经元数量: ";
                        int neuronCount;
                        std::cin >> neuronCount;

                        std::vector<double> inputs(i == 0 ? neuronCount : 0, 0.0);
                        Layer* layer = new Layer(i, neuronCount, inputs, 0);
                        newNetwork->addLayer(layer);
                    }

                    std::cout << "\033[32m✅ 网络创建成功!\033[0m" << std::endl;
                    newNetwork->showInfo();
                    network=*newNetwork;
                    break;
                }

                case 3: {
                    std::cout << "\n\033[34m📊 网络信息\033[0m" << std::endl;
                    network.showInfo();
                    std::cout << "请输入要查看的层索引 (-1 查看所有层): ";
                    int layerIndex;
                    std::cin >> layerIndex;

                    if (layerIndex == -1) {
                        for (int i = 0; i < network.getLayerCount(); ++i) {
                            std::cout << "\n\033[36m🔍 第 " << i << " 层详细信息:\033[0m" << std::endl;
                            network.showLayer(i);
                        }
                    } else if (layerIndex >= 0 && layerIndex < network.getLayerCount()) {
                        std::cout << "\n\033[36m🔍 第 " << layerIndex << " 层详细信息:\033[0m" << std::endl;
                        network.showLayer(layerIndex);
                    } else {
                        std::cout << "\033[31m❌ 无效的层索引!\033[0m" << std::endl;
                    }
                    break;
                }

                case 4: {
                    std::cout << "\n\033[34m⚡ 执行前向传播\033[0m" << std::endl;
                    int inputSize = network.getLayer(0)->getNeuronCount();
                    std::vector<double> input(inputSize);
                    std::cout << "请输入 " << inputSize << " 个数值:" << std::endl;
                    for (int i = 0; i < inputSize; i++) {
                        std::cout << "  输入[" << i << "]: ";
                        std::cin >> input[i];
                    }

                    std::cout << "🔄 正在计算..." << std::endl;
                    auto output = network.forward(input);

                    std::cout << "\033[32m📈 计算结果:\033[0m" << std::endl;
                    for (size_t i = 0; i < output.size(); ++i) {
                        std::cout << "  层 " << i << " 输出: [";
                        for (size_t j = 0; j < output[i].size(); ++j) {
                            std::cout << std::fixed << std::setprecision(4) << output[i][j];
                            if (j < output[i].size() - 1) std::cout << ", ";
                        }
                        std::cout << "]" << std::endl;
                    }
                    break;
                }

                case 5: {
                    std::cout << "\n\033[34m💾 导出神经网络\033[0m" << std::endl;
                    std::cout << "请输入导出文件名: ";
                    std::string filename;
                    std::cin >> filename;
                    ANNExporter exporter(filename);
                    exporter.exportNetwork(network);
                    std::cout << "\033[32m✅ 导出到 " << filename << " 成功!\033[0m" << std::endl;
                    break;
                }

                case 6: {
                    std::cout << "\n\033[34m🔗 神经元连接状态\033[0m" << std::endl;
                    std::cout << "请输入层索引: ";
                    int layerIndex;
                    std::cin >> layerIndex;

                    std::cout << "请输入神经元索引: ";
                    int neuronIndex;
                    std::cin >> neuronIndex;

                    if (layerIndex >= 0 && layerIndex < network.getLayerCount()) {
                        const Layer* layer = network.getLayer(layerIndex);
                        if (neuronIndex >= 0 && neuronIndex < layer->getNeuronCount()) {
                            Neuron neuron = layer->getNeuron(neuronIndex);
                            neuron.showConnections();
                        } else {
                            std::cout << "\033[31m❌ 无效的神经元索引!\033[0m" << std::endl;
                        }
                    } else {
                        std::cout << "\033[31m❌ 无效的层索引!\033[0m" << std::endl;
                    }

                    break;
                }
                    case 7: {
                        std::cout << "\n\033[34m➕ 增加层\033[0m" << std::endl;
                        std::cout << "请输入新层的神经元数量: ";
                        int neuronCount;
                        std::cin >> neuronCount;

                        std::cout << "请输入插入位置 (0-" << network.getLayerCount() << "): ";
                        int position;
                        std::cin >> position;

                        if (position < 0 || position > network.getLayerCount()) {
                            std::cout << "\033[31m❌ 无效的位置!\033[0m" << std::endl;
                            break;
                        }

                        std::vector<double> inputs(neuronCount, 0.0);
                        Layer* newLayer = new Layer(position, neuronCount, inputs, 0);

                        if (position == network.getLayerCount()) {
                            network.addLayer(newLayer);
                        } else {
                            // 插入到指定位置需要重新调整层索引
                            network.addLayer(newLayer);
                            // 这里需要网络类支持插入操作
                        }

                        std::cout << "\033[32m✅ 成功添加层!\033[0m" << std::endl;
                        network.showInfo();
                        break;
                    }

                    case 8: {
                        std::cout << "\n\033[34m➕ 增加神经元\033[0m" << std::endl;
                        std::cout << "请输入层索引: ";
                        int layerIndex;
                        std::cin >> layerIndex;

                        if (layerIndex < 0 || layerIndex >= network.getLayerCount()) {
                            std::cout << "\033[31m❌ 无效的层索引!\033[0m" << std::endl;
                            break;
                        }

                        std::cout << "请输入偏置值: ";
                        double bias;
                        std::cin >> bias;

                        std::cout << "请输入激活函数类型 (0=sigmoid, 1=tanh, 2=relu, 3=linear): ";
                        int activationType;
                        std::cin >> activationType;

                        // 参数顺序: pre, bias, activationFunctionType, layerIndex, index
                        network.addNeuron(layerIndex, bias, activationType);

                        std::cout << "\033[32m✅ 成功添加神经元到第 " << layerIndex << " 层!\033[0m" << std::endl;
                        network.showLayer(layerIndex);
                        break;
                    }

                    case 9: {
                        std::cout << "\n\033[34m➖ 删除层\033[0m" << std::endl;
                        if (network.getLayerCount() <= 1) {
                            std::cout << "\033[31m❌ 网络至少需要保留一层!\033[0m" << std::endl;
                            break;
                        }

                        std::cout << "请输入要删除的层索引 (0-" << (network.getLayerCount()-1) << "): ";
                        int layerIndex;
                        std::cin >> layerIndex;

                        if (layerIndex < 0 || layerIndex >= network.getLayerCount()) {
                            std::cout << "\033[31m❌ 无效的层索引!\033[0m" << std::endl;
                            break;
                        }

                        std::cout << "\033[33m⚠️  确认删除第 " << layerIndex << " 层? (y/n): \033[0m";
                        char confirm;
                        std::cin >> confirm;

                        if (confirm == 'y' || confirm == 'Y') {
                            network.deleteLayer(layerIndex);
                            std::cout << "\033[32m✅ 成功删除第 " << layerIndex << " 层!\033[0m" << std::endl;
                            network.showInfo();
                        } else {
                            std::cout << "\033[36m🚫 删除操作已取消\033[0m" << std::endl;
                        }
                        break;
                    }

                    case 10: {
                        std::cout << "\n\033[34m➖ 删除神经元\033[0m" << std::endl;
                        std::cout << "请输入层索引: ";
                        int layerIndex;
                        std::cin >> layerIndex;

                        if (layerIndex < 0 || layerIndex >= network.getLayerCount()) {
                            std::cout << "\033[31m❌ 无效的层索引!\033[0m" << std::endl;
                            break;
                        }

                        const Layer* layer = network.getLayer(layerIndex);
                        if (layer->getNeuronCount() <= 1) {
                            std::cout << "\033[31m❌ 层至少需要保留一个神经元!\033[0m" << std::endl;
                            break;
                        }

                        std::cout << "请输入要删除的神经元索引 (0-" << (layer->getNeuronCount()-1) << "): ";
                        int neuronIndex;
                        std::cin >> neuronIndex;

                        if (neuronIndex < 0 || neuronIndex >= layer->getNeuronCount()) {
                            std::cout << "\033[31m❌ 无效的神经元索引!\033[0m" << std::endl;
                            break;
                        }

                        std::cout << "\033[33m⚠️  确认删除第 " << layerIndex << " 层的第 " << neuronIndex << " 个神经元? (y/n): \033[0m";
                        char confirm;
                        std::cin >> confirm;

                        if (confirm == 'y' || confirm == 'Y') {
                            network.deleteNeuron(layerIndex, neuronIndex);
                            std::cout << "\033[32m✅ 成功删除神经元!\033[0m" << std::endl;
                            network.showLayer(layerIndex);
                        } else {
                            std::cout << "\033[36m🚫 删除操作已取消\033[0m" << std::endl;
                        }
                        break;
                    }
                    case 11: {
                        std::cout << "\n\033[34m✏️ 修改网络名称\033[0m" << std::endl;
                        std::cout << "请输入新的网络名称: ";
                        std::string newName;
                        std::cin.ignore(); // 清除输入缓冲区
                        std::getline(std::cin, newName);
                        network.setName(newName);
                        std::cout << "\033[32m✅ 网络名称已更新为: " << newName << "\033[0m" << std::endl;
                        break;
                    }
                    case 0: {
                        std::cout << "\n\033[35m👋 感谢使用 ANN 神经网络系统!\033[0m" << std::endl;
                        std::cout << "\033[36m🚀 期待下次再见~\033[0m" << std::endl;
                        return 0;
                    }

                    default:
                        std::cout << "\n\033[31m❌ 无效选择，请输入 0-6 之间的数字\033[0m" << std::endl;
                        break;
            }

        } catch (const std::exception& e) {
            std::cout << "\n\033[31m❌ 错误: " << e.what() << "\033[0m" << std::endl;
        }

        std::cout << "\n\033[90m按回车键继续...\033[0m";
        std::cin.ignore();
        std::cin.get();
        std::cout << "\033[2J\033[H"; // 清屏
    }

    return 0;
}

/*
int main() //无交互式CLI的演示程序
{
    std::cout << "========================================" << std::endl;
    std::cout << "      神经网络演示程序 (CANN v1.0)      " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    try {
        std::cout << "[1] 正在导入神经网络文件..." << std::endl;
        std::cout << "    文件: simple.ANN" << std::endl;

        ANNImporter importer("simple.ANN");
        Network network = importer.import();

        std::cout << "    ✓ 导入成功!" << std::endl;
        std::cout << std::endl;

        std::cout << "[2] 网络结构信息" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        network.showInfo();

        std::cout << std::endl;

        std::cout << "[3] 第0层详细信息" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        network.showLayer(0);
        std::cout << std::endl;

        std::cout << "[4] 执行前向传播" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "    输入数据: [0.5, 0.8, 0.3]" << std::endl;

        auto output = network.forward({0.5, 0.8, 0.3}).back();

        std::cout << "    网络输出: [";
        for (size_t i = 0; i < output.size(); ++i) {
            std::cout << std::fixed << std::setprecision(4) << output[i];
            if (i < output.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
        std::cout << "    ✓ 前向传播完成!" << std::endl;
        std::cout << std::endl;
        std::cout << "[4.1] 删除第一层" << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        if (network.getLayerCount() > 1) {
            network.deleteLayer(0);
            std::cout << "    ✓ 第一层已删除!" << std::endl;
        } else {
            std::cout << "    ❌ 无法删除，网络至少需要保留一层!" << std::endl;
        }

        std::cout << std::endl;
        std::cout << "[4.2] 删除后的网络结构信息" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        network.showInfo();
        network.getLayer(0)->getNeuron(1).showConnections();
        std::cout << std::endl;
        std::cout << "[5] 导出网络到文件" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "    输出文件: output.ANN" << std::endl;

        ANNExporter exporter("output.ANN");
        exporter.exportNetwork(network);

        std::cout << "    ✓ 导出成功!" << std::endl;
        std::cout << std::endl;

        std::cout << "========================================" << std::endl;
        std::cout << "           程序执行完成!               " << std::endl;
        std::cout << "========================================" << std::endl;

    } catch (const std::exception& e) {
        std::cout << std::endl;
        std::cout << "❌ 错误: " << e.what() << std::endl;
        std::cout << "========================================" << std::endl;
        return 1;
    }

    return 0;
}
*/