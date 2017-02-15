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
 * �¹�
 * C++�쳣(exception)�����Ʒ������Ϊһ�����ö��󴫸�һ��������������ִ����Ϻ󣬴���������ִ�������
 * @author Wu Deyou
 * @date 2015-12-16
 * @note �̲߳���ȫ����Ҫ������auxiliary����ȫ��
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
     * ���캯����
     */
    Incident() noexcept;

    /**
     * ���캯����
     */
    explicit Incident(int64_t code) noexcept;

    /**
     * ���캯����
     */
    Incident(int64_t code, string const& message) noexcept;

    /**
     * ���캯����
     */
    Incident(int64_t code, string const& message, string const& filename, string const& functionName, uint32_t lineNo) noexcept;

    /**
     * ���ƹ��캯����
     */
    Incident(Incident const&) noexcept;

    /**
     * �ƶ����캯����
     */
    Incident(Incident&&) noexcept;

    /**
     * ���Ƹ�ֵ������
     */
    Incident& operator=(Incident const&) noexcept;

    /**
     * �ƶ���ֵ������
     */
    Incident& operator=(Incident&&) noexcept;

    /**
     * ת��������
     */
    Incident& operator=(int64_t) noexcept;

    /**
     * ����������
     */
    ~Incident();

    /**
     * ���ã�
     * @param code �¹ʱ��룻
     * @param message �¹����ݣ�
     * @param fileName �¹ʷ������ڵ��ļ����ƣ�
     * @param functionName �¹ʷ������ڵĺ������ƣ�
     * @param lineNo �¹ʷ������ڵ��кš�
     */
    void set(int64_t code, string const& message, string const& fileName, string const& functionName, uint32_t lineNo) noexcept;

    /**
     * ��ê�������������ļ����ơ��������ƺ��к���Ϣ���¹ʱ��롢�¹������Լ�������Ϣ�������䣻
     * @param fileName �¹ʷ������ڵ��ļ����ƣ�
     * @param functionName �¹ʷ������ڵĺ������ƣ�
     * @param lineNo �¹ʷ������ڵ��кš�
     */
    void anchor(string const& fileName, string const& functionName, uint32_t lineNo) noexcept;

    /**
     * �����ַ������͵ĸ�����Ϣ��
     * @param name ������Ϣ���ƣ�
     * @param value ��ֵ��Ϣֵ��
     */
    void set(string const& name, string value) noexcept;

    /**
     * �����������͵ĸ�����Ϣ��
     * @param name ������Ϣ���ƣ�
     * @param value ��ֵ��Ϣֵ��
     */
    void set(string const& name, int64_t value) noexcept;

    /**
     * �ָ���ʼ״̬��
     */
    void reset() noexcept;

    /**
     * ������Ƿ�ɹ���ע�⣺ֻҪ�¹ʱ�����ڻ����0������Ϊ�ǳɹ���
     */
    bool succeeded() const noexcept;

    /**
     * ��ȡ�¹ʱ��룻
     * @return �¹ʱ��롣
     */
    int64_t getCode() const noexcept;

    /**
     * ��ȡ�¹����ݣ�
     * @return �¹����ݡ�
     */
    string getMessage() const noexcept;

    /**
     * ��ȡ�¹ʷ������ڵ��ļ����ƣ�
     * @return �¹ʷ������ڵ��ļ����ơ�
     */
    string getFilename() const noexcept;

    /**
     * ��ȡ�¹ʷ������ڵĺ������ƣ�
     * @return �¹ʷ������ڵĺ������ơ�
     */
    string getFunctionName() const noexcept;

    /**
     * ��ȡ�¹ʷ������ڵ��кţ�
     * @return �¹ʷ������ڵ��кš�
     */
    uint32_t getLineNo() const noexcept;

    /**
     * ��ȡ������Ϣֵ��
     * @param name ������Ϣ���ƣ�
     */
    string get(string const& name) const noexcept;

    /**
     * ����ַ�����ʽ��
     * @param separator �ָ�����
     */
    string toString(string separator="; ") const noexcept;

};
}
}

#endif // HUDUN_COMMON_INCIDENT_H
