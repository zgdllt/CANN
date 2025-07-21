//-------------------------------------------------------------------------------------------------------------------
//【文件名】FilePorter.hpp
//【功能模块和目的】文件操作基类的声明，提供文件导入导出的通用功能
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------

#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

enum class FilePorterType { IMPORTER,
    EXPORTER };

//-------------------------------------------------------------------------------------------------------------------
//【类名】FilePorter
//【功能】提供文件操作的基础功能，支持文件类型检测、打开状态检测等
//【接口说明】模板类，根据FilePorterType枚举决定是导入器还是导出器
//【开发者及日期】开发者姓名 2025年7月21日
//【更改记录】无
//-------------------------------------------------------------------------------------------------------------------
template<FilePorterType ENUM>
class FilePorter {
public:
    // Constructor
    FilePorter(const std::string& filename,const std::vector<std::string>& extensions = {})
        : filename(filename), supportedExts(extensions) {
        // Test file type and open status during initialization
        FileTypeTest(filename, GetExtName(filename));
        if (PORT_TYPE == FilePorterType::IMPORTER) {
            FileOpenTest(filename);
        }
        // For EXPORTER, we don't test if file exists since we may be creating it
    }
    // Getter for filename
    const std::string& getFilename() const {
        return filename;
    }

    // Setter for filename
    void setFilename(const std::string& newFilename) {
        filename = newFilename;
    }
    void FileTypeTest(const std::string& /* FileName */, const std::string& ExtName) {
        if (ExtName.empty()) {
            throw INVALID_FILE_TYPE(ExtName);
        }
        bool isSupported = false;
        for (const auto& ext : supportedExts) {
            if (ExtName == ext) {
                isSupported = true;
                break;
            }
        }
        
        if (!isSupported) {
            throw INVALID_FILE_TYPE(ExtName);
        }
    }

    // Static function to test if file can be opened
    static void FileOpenTest(const std::string& FileName) {
        std::ifstream file(FileName);
        if (!file.is_open()) {
            throw FILE_OPEN_FAIL(FileName);
        }
    }

    // Static function to get file extension
    static std::string GetExtName(const std::string& FileName) {
        size_t pos = FileName.find_last_of('.');
        if (pos == std::string::npos) {
            return "";
        }
        return FileName.substr(pos + 1);
    }

    // Exception classes
    class INVALID_FILE_TYPE : public std::invalid_argument {
    public:
        INVALID_FILE_TYPE(const std::string& FileExtName)
            : std::invalid_argument(FileExtName + " is an invalid file extension name") {}
    };

    class FILE_OPEN_FAIL : public std::invalid_argument {
    public:
        FILE_OPEN_FAIL(const std::string& FileName)
            : std::invalid_argument(FileName + " failed to open") {}
    };
protected:
    // Protected necessary constructors, destructors, and assignment operators
    FilePorter() = default;
    FilePorter(const FilePorter& Source) = delete;
    FilePorter& operator=(const FilePorter& Source) = delete;
    virtual ~FilePorter() = default;
    std::string filename;  // 移到 protected，让子类可以访问
    void setSupportedExtensions(const std::vector<std::string>& extensions) {
        supportedExts = extensions;
    }
    std::vector<std::string> getSupportedExtensions() const {
        return supportedExts;
    }
private:
    std::vector<std::string> supportedExts = {}; // Supported file extensions for this porter
    static constexpr FilePorterType PORT_TYPE = ENUM;
    // Static function to test file type
};

#endif // FILE_HPP
