#ifndef HUDUN_SQLITE_ENTITY_H
#define HUDUN_SQLITE_ENTITY_H
#include <string>
#include <map>
using namespace std;

namespace hudun
{
namespace sqlite
{
/**
 * ʵ�壻
 * ���ڱ�ʾ�ָ�֮���ʵ�����
 * @author Wu Deyou
 * @date 2016-01-11
 * @note �̲߳���ȫ����Ҫ������auxiliary����ȫ��
 */
class Entity
{
public:
    /**
     * ��÷�ʽ��δ֪��
     */
    static const int16_t OBTAINWAY_UNKNOWN        = 0;

    /**
     * ��÷�ʽ����ʾ��Sqlite API��ȡ��ʵ�壻
     */
    static const int16_t OBTAINWAY_SQLITE_API     = 1;

    /**
     * ��÷�ʽ����ʾͨ��ʶ��SQLITE��¼������������ʵ�壻
     */
    static const int16_t OBTAINWAY_CELL_TYPE_SEEK = 2;

    /**
     * ʵ��ɾ��״̬��δ֪��
     */
    static const int16_t DELETEDSTATUS_UNKNOWN        = 0;

    /**
     * ʵ��ɾ��״̬��δɾ����
     */
    static const int16_t DELETEDSTATUS_UNDELETED      = 1;

    /**
     * ʵ��ɾ��״̬����ɾ����
     */
    static const int16_t DELETEDSTATUS_DELETED        = 9;

protected:
    map<string, string> auxiliaries;

    uint16_t obtainWay;
    uint64_t validFlags;
    uint64_t nullFlags;
    uint16_t deletedStatus;

public:

    /**
     * ���캯����
     */
    Entity(uint16_t obtainWay) noexcept;

    /**
     * ���ƹ��캯����
     */
    Entity(Entity const&) noexcept;

    /**
     * ��ֵ��
     */
    Entity& operator=(Entity const&) noexcept;

    /**
     * ����ĳ���ֶ�Ϊ��ֵ��
     * @param fieldFlag �ֶα�־λ��
     */
    void setNullValue(uint64_t fieldFlag) noexcept;

    /**
     * ����ʵ��ɾ��״̬��
     * @param deletedStatus ɾ��״̬��
     */
    void setDeletedStatus(uint16_t deletedStatus) noexcept;

    /**
     * ���Ӹ�����Ϣ��
     * @param name ������Ϣ���ƣ�
     * @param value ��ֵ��Ϣֵ��
     */
    void addAuxiliary(string const& name, string const& value) noexcept;

    /**
     * �ж�ĳЩ�����������ֶ��Ƿ���Ч��
     * @param fieldFlags �ֶα�־λ��
     * @return �Ƿ���Ч��
     */
    bool isValid(uint64_t fieldFlags) const noexcept;

    /**
     * �ж�ĳ���ֶ��Ƿ�Ϊ�գ�
     * @param fieldFlag �ֶα�־λ��
     * @return �Ƿ�Ϊ�ա�
     */
    bool isNull(uint64_t fieldFlag) const noexcept;

    /**
     * �ж�ĳ���ֶ��Ƿ���Ч�ҷǿգ�
     * @param fieldFlag �ֶα�־λ��
     * @return �Ƿ���Ч�ҷǿա�
     */
    bool isValidAndNotNull(uint64_t fieldFlag) const noexcept;

    /**
     * ��ȡʵ���÷�ʽ��
     * @return ��÷�ʽ��
     */
    uint16_t getObtainWay() const noexcept;

    /**
     * ��ȡʵ��ȫ���ֶε���Ч��־λ�����ܳ���64���ֶΣ���
     * @return ȫ���ֶε���Ч��־λ��
     */
    uint64_t getValidFlags() const noexcept;

    /**
     * ��ȡʵ��ɾ��״̬��
     * @return ɾ��״̬��
     */
    uint16_t getDeletedStatus() const noexcept;

     /**
     * ��ȡʵ�帨����Ϣ��
     * @param name ������Ϣ���ƣ�
     * @param exist (OUT)������Ϣ�Ƿ���ڣ�
     * @return ������Ϣֵ��
     */
    string getAuxiliary(string const& name, bool& exist) const noexcept;

     /**
     * ��ȡʵ��ȫ��������Ϣ��
     * @return ȫ��������Ϣ��
     */
    const map<string, string>& getAuxiliaries() const noexcept;


    /**
     * ����ʵ�塣
     */
    void reset() noexcept;
};
}
}
#endif // HUDUN_SQLITE_ENTITY_H
