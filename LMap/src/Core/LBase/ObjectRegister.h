#ifndef ObjectRegistry_H__20241109
#define ObjectRegistry_H__20241109

#include <QString>
#include <QMap>
#include <memory>
#include <QObject>
#include "lcore_global.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	对象注册类，通过 字符串 和对象基类获取对象，注意，子类对象需要继承QObject. </summary>
///
/// <remarks>	Liruilong, 2024/11/14. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class LCORE_EXPORT ObjectRegistry
{
public:
    static ObjectRegistry & instance();

    void registerObject(const QString & name, QObject * object);

    QObject * getObject(const QString & name) const;

private:
    ObjectRegistry(); // 私有构造函数，单例模式
    QMap<QString, QObject *> registry;
};

#endif