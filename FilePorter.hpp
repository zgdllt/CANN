//-------------------------------------------------------------------------------------------------------------------
//【文件名】FilePorter.hpp
//【功能模块和目的】文件操作基类的声明，提供文件导入导出的通用功能
//【开发者及日期】李孟涵 2025年7月20日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

#ifndef FILEPORTER_HPP
#define FILEPORTER_HPP

#include <string>   // 字符串头文件
#include <fstream>  // 文件流头文件
#include <vector>   // 向量头文件
#include <stdexcept>// 标准异常头文件

enum class FilePorterType { IMPORTER, EXPORTER };// 文件操作类型枚举，区分导入器和导出器

//-------------------------------------------------------------------------------------------------------------------
//【类名】FilePorter
//【功能】提供文件操作的基础功能，支持文件类型检测、打开状态检测等
//【接口说明】模板类，根据FilePorterType枚举决定是导入器还是导出器
//【开发者及日期】李孟涵 2025年7月20日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
template<FilePorterType ENUM>// 定义模板，只能从FilePorterType枚举中选择是IMPORTER或EXPORTER
class FilePorter {
public:
    // 构造函数，初始化文件名和支持的扩展名列表
    // 如果是导入器类型，会在初始化时测试文件类型和打开状态
    FilePorter(const std::string& filename, const std::vector<std::string>& extensions = {})
        : filename(filename), supportedExts(extensions) {
        // 测试文件类型和打开状态
        FileTypeTest(filename, GetExtName(filename));
        if (PORT_TYPE == FilePorterType::IMPORTER) {
            FileOpenTest(filename);
        }
        // 对于导出器类型，不测试文件是否存在，因为可能是新建文件
    }

    // 获取文件名
    const std::string& getFilename() const {
        return filename;
    }

    // 设置文件名
    void setFilename(const std::string& newFilename) {
        filename = newFilename;
    }

    // 测试扩展名是否正确
    void FileTypeTest(const std::string& /* FileName */, const std::string& ExtName) {
        if (ExtName.empty()) {// 如果扩展名为空，抛出异常
            throw INVALID_FILE_TYPE(ExtName);
        }
        bool isSupported = false;
        for (const auto& ext : supportedExts) {
            if (ExtName == ext) {
                isSupported = true;
                break;
            }
        }
        if (!isSupported) {// 如果扩展名不在支持列表中，抛出异常
            throw INVALID_FILE_TYPE(ExtName);
        }
    }

    // 静态函数：测试文件是否可以打开
    static void FileOpenTest(const std::string& FileName) {
        std::ifstream file(FileName);
        if (!file.is_open()) {
            throw FILE_OPEN_FAIL(FileName);
        }
    }

    // 静态函数：获取文件扩展名
    static std::string GetExtName(const std::string& FileName) {
        size_t pos = FileName.find_last_of('.');
        if (pos == std::string::npos) {
            return "";
        }
        return FileName.substr(pos + 1);
    }

    // 异常类：无效的文件类型
    class INVALID_FILE_TYPE : public std::invalid_argument {
    public:
        INVALID_FILE_TYPE(const std::string& FileExtName)
            : std::invalid_argument(FileExtName + " is an invalid file extension name") {}
    };

    // 异常类：文件打开失败
    class FILE_OPEN_FAIL : public std::invalid_argument {
    public:
        FILE_OPEN_FAIL(const std::string& FileName)
            : std::invalid_argument(FileName + " failed to open") {}
    };

protected:
    // 受保护的构造函数、析构函数和赋值操作符，防止直接实例化
    FilePorter() = default;
    FilePorter(const FilePorter& Source) = delete;
    FilePorter& operator=(const FilePorter& Source) = delete;
    virtual ~FilePorter() = default;

    // 文件名，子类可以访问
    std::string filename;

    // 设置支持的扩展名列表
    void setSupportedExtensions(const std::vector<std::string>& extensions) {
        supportedExts = extensions;
    }

    // 获取支持的扩展名列表
    std::vector<std::string> getSupportedExtensions() const {
        return supportedExts;
    }

private:
    // 支持的文件扩展名列表
    std::vector<std::string> supportedExts = {};

    // 静态常量，表示当前类的类型（导入器或导出器）
    static constexpr FilePorterType PORT_TYPE = ENUM;
};

#endif // FILEPORTER_HPP
