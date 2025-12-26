#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <LCommon/TSingleton.hpp>
#include "lcore_global.h"
#include <QMap>
#include <QString>

// AppConfig 类负责管理配置文件
class LCORE_EXPORT AppConfig : public TSingleton<AppConfig>
{
    friend class TSingleton<AppConfig>;

public:
    // 禁用拷贝构造函数和赋值操作符
    AppConfig(const AppConfig &)             = delete;
    AppConfig & operator=(const AppConfig &) = delete;

    // 获取配置项
    QString getAppConfig(QString  mark, QString defaultValue = "") const;

    // 设置配置项
    void setAppConfig(QString  mark, QString  data);

private:
    // 私有构造函数
    AppConfig();

    // 加载配置文件
    void loadConfig(const QString & filePath);

    // 用于存储配置项的 QMap
    QMap<QString, QString> paramMap; // QMap<mark, data>
};

#endif // APPCONFIG_H