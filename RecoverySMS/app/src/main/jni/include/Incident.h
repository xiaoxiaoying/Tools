#ifndef HUDUN_COMMON_INCIDENT_H
#define HUDUN_COMMON_INCIDENT_H

#include <map>
#include <string>
using namespace std;

namespace hudun
{
namespace common
{
/**
 * 事故
 * C++异常(exception)的替代品，其作为一个引用对象传给一个方法，待方法执行完毕后，带出方法的执行情况。
 * @author Wu Deyou
 * @date 2015-12-16
 * @note 线程不安全，主要是由于auxiliary不安全；
 */
class Incident
{
public:
    static const int64_t SUCCEEDED;
    static const int64_t FAIL_GENERAL;
    static const int64_t WARN_GENERAL;

protected:
    string fileName;
    string functionName;
    uint32_t lineNo = 0;
    int64_t code = Incident::SUCCEEDED;
    string message;
    map<string, string>* auxiliary = nullptr;

public:
    /**
     * 构造函数；
     */
    Incident() noexcept;

    /**
     * 构造函数；
     */
    explicit Incident(int64_t code) noexcept;

    /**
     * 构造函数；
     */
    Incident(int64_t code, string const& message) noexcept;

    /**
     * 构造函数；
     */
    Incident(int64_t code, string const& message, string const& filename, string const& functionName, uint32_t lineNo) noexcept;

    /**
     * 复制构造函数；
     */
    Incident(Incident const&) noexcept;

    /**
     * 移动构造函数；
     */
    Incident(Incident&&) noexcept;

    /**
     * 复制赋值函数；
     */
    Incident& operator=(Incident const&) noexcept;

    /**
     * 移动赋值函数；
     */
    Incident& operator=(Incident&&) noexcept;

    /**
     * 转换函数；
     */
    Incident& operator=(int64_t) noexcept;

    /**
     * 析构函数；
     */
    ~Incident();

    /**
     * 设置；
     * @param code 事故编码；
     * @param message 事故内容；
     * @param fileName 事故发生所在的文件名称；
     * @param functionName 事故发生所在的函数名称；
     * @param lineNo 事故发生所在的行号。
     */
    void set(int64_t code, string const& message, string const& fileName, string const& functionName, uint32_t lineNo) noexcept;

    /**
     * 抛锚，即重新设置文件名称、函数名称和行号信息，事故编码、事故内容以及辅助信息保留不变；
     * @param fileName 事故发生所在的文件名称；
     * @param functionName 事故发生所在的函数名称；
     * @param lineNo 事故发生所在的行号。
     */
    void anchor(string const& fileName, string const& functionName, uint32_t lineNo) noexcept;

    /**
     * 设置字符串类型的辅助信息；
     * @param name 辅助信息名称；
     * @param value 赋值信息值。
     */
    void set(string const& name, string value) noexcept;

    /**
     * 设置整数类型的辅助信息；
     * @param name 辅助信息名称；
     * @param value 赋值信息值。
     */
    void set(string const& name, int64_t value) noexcept;

    /**
     * 恢复初始状态；
     */
    void reset() noexcept;

    /**
     * 检查结果是否成功，注意：只要事故编码大于或等于0，都认为是成功；
     */
    bool succeeded() const noexcept;

    /**
     * 获取事故编码；
     * @return 事故编码。
     */
    int64_t getCode() const noexcept;

    /**
     * 获取事故内容；
     * @return 事故内容。
     */
    string getMessage() const noexcept;

    /**
     * 获取事故发生所在的文件名称；
     * @return 事故发生所在的文件名称。
     */
    string getFilename() const noexcept;

    /**
     * 获取事故发生所在的函数名称；
     * @return 事故发生所在的函数名称。
     */
    string getFunctionName() const noexcept;

    /**
     * 获取事故发生所在的行号；
     * @return 事故发生所在的行号。
     */
    uint32_t getLineNo() const noexcept;

    /**
     * 获取辅助信息值；
     * @param name 辅助信息名称；
     */
    string get(string const& name) const noexcept;

    /**
     * 输出字符串格式；
     * @param separator 分隔符；
     */
    string toString(string separator="; ") const noexcept;

};
}
}

#endif // HUDUN_COMMON_INCIDENT_H
