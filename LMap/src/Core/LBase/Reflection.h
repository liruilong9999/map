#ifndef REFLECTION_H__20241114
#define REFLECTION_H__20241114

#include "lcore_global.h"

#include <Core/LCommon/TSingleton.hpp>
#include <QMap>
#include <QString>
#include <QObject>
#include <QPointer>

typedef QObject * (*PTRCreateObject)(); // 创建对象的函数指针

enum class ObjectCreationMode
{
    Prototype, // 原型模式，每次创建新对象
    Singleton  // 单例模式，第一次创建后缓存对象
};

// 工厂类的定义
class LCORE_EXPORT ClassFactory : public QObject
{
    Q_OBJECT
private:
    QMap<QString, PTRCreateObject> m_classMap;      // 使用 QMap 和 QString 替代 map 和 string
    QMap<QString, QObject *>       m_singletonsMap; // 存储单例对象
    ClassFactory(){};                          // 构造函数私有化

public:
    QObject* getClassByName(const QString& className, ObjectCreationMode mode = ObjectCreationMode::Prototype);
    void      registClass(const QString & name, PTRCreateObject method);

    static ClassFactory & getInstance();
};

// 注册动作类
class LCORE_EXPORT RegisterAction
{
public:
    RegisterAction(const QString & className, PTRCreateObject ptrCreateFn);
};

// 宏定义注册函数
#define REGISTER(className, String)                     \
    static className * objectCreator##className()       \
    {                                                   \
        return new className;                           \
    }                                                   \
    static RegisterAction g_creatorRegister##className( \
        String, (PTRCreateObject)objectCreator##className)

#endif
