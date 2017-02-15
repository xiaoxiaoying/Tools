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
 * 实体；
 * 用于表示恢复之后的实体抽象。
 * @author Wu Deyou
 * @date 2016-01-11
 * @note 线程不安全，主要是由于auxiliary不安全；
 */
class Entity
{
public:
    /**
     * 获得方式：未知；
     */
    static const int16_t OBTAINWAY_UNKNOWN        = 0;

    /**
     * 获得方式：表示用Sqlite API读取的实体；
     */
    static const int16_t OBTAINWAY_SQLITE_API     = 1;

    /**
     * 获得方式：表示通过识别SQLITE记录类型搜索到的实体；
     */
    static const int16_t OBTAINWAY_CELL_TYPE_SEEK = 2;

    /**
     * 实体删除状态：未知；
     */
    static const int16_t DELETEDSTATUS_UNKNOWN        = 0;

    /**
     * 实体删除状态：未删除；
     */
    static const int16_t DELETEDSTATUS_UNDELETED      = 1;

    /**
     * 实体删除状态：已删除；
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
     * 构造函数；
     */
    Entity(uint16_t obtainWay) noexcept;

    /**
     * 复制构造函数；
     */
    Entity(Entity const&) noexcept;

    /**
     * 赋值；
     */
    Entity& operator=(Entity const&) noexcept;

    /**
     * 设置某个字段为空值；
     * @param fieldFlag 字段标志位；
     */
    void setNullValue(uint64_t fieldFlag) noexcept;

    /**
     * 设置实体删除状态；
     * @param deletedStatus 删除状态；
     */
    void setDeletedStatus(uint16_t deletedStatus) noexcept;

    /**
     * 增加辅助信息；
     * @param name 辅助信息名称；
     * @param value 赋值信息值。
     */
    void addAuxiliary(string const& name, string const& value) noexcept;

    /**
     * 判断某些（允许多个）字段是否有效；
     * @param fieldFlags 字段标志位；
     * @return 是否有效。
     */
    bool isValid(uint64_t fieldFlags) const noexcept;

    /**
     * 判断某个字段是否为空；
     * @param fieldFlag 字段标志位；
     * @return 是否为空。
     */
    bool isNull(uint64_t fieldFlag) const noexcept;

    /**
     * 判断某个字段是否有效且非空；
     * @param fieldFlag 字段标志位；
     * @return 是否有效且非空。
     */
    bool isValidAndNotNull(uint64_t fieldFlag) const noexcept;

    /**
     * 获取实体获得方式；
     * @return 获得方式。
     */
    uint16_t getObtainWay() const noexcept;

    /**
     * 获取实体全部字段的有效标志位（不能超过64个字段）；
     * @return 全部字段的有效标志位。
     */
    uint64_t getValidFlags() const noexcept;

    /**
     * 获取实体删除状态；
     * @return 删除状态。
     */
    uint16_t getDeletedStatus() const noexcept;

     /**
     * 获取实体辅助信息；
     * @param name 辅助信息名称；
     * @param exist (OUT)辅助信息是否存在；
     * @return 辅助信息值。
     */
    string getAuxiliary(string const& name, bool& exist) const noexcept;

     /**
     * 获取实体全部辅助信息；
     * @return 全部辅助信息。
     */
    const map<string, string>& getAuxiliaries() const noexcept;


    /**
     * 重置实体。
     */
    void reset() noexcept;
};
}
}
#endif // HUDUN_SQLITE_ENTITY_H
