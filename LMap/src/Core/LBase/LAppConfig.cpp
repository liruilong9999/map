#include "LAppconfig.h"
#include <QDomDocument>
#include <QFile>
#include <QPair>
#include "LLog.h"

#define AppConfig_PATH "./config/config.xml"

// AppConfig 构造函数
AppConfig::AppConfig()
{
    QString filePath = AppConfig_PATH;
    if (!filePath.isEmpty())
    {
        loadConfig(filePath);
    }
    else
    {
        QString msg = QString("模块（AppConfig）加载配置文件（%1）失败").arg(filePath);
        LOG_WARN(msg);
    }
}

// 加载配置文件的方法
void AppConfig::loadConfig(const QString & filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QString msg = QString("模块（AppConfig）加载配置文件（%1）失败").arg(filePath);
        LOG_WARN(msg);
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        QString msg = QString("模块（AppConfig）打开配置文件（%1）失败").arg(filePath);
        LOG_WARN(msg);
        return;
    }
    file.close();

    QDomElement  root      = doc.documentElement();
    QDomNodeList paramList = root.elementsByTagName("paramList");
    if (paramList.isEmpty())
    {
        QString msg = QString("模块（AppConfig）加载配置文件（%1）无节点：paramList").arg(filePath);
        LOG_WARN(msg);
        return;
    }

    QDomNode paramNode = paramList.at(0).firstChild();
    while (!paramNode.isNull())
    {
        QDomElement paramElement = paramNode.toElement();
        if (!paramElement.isNull() && paramElement.tagName() == "param")
        {
            QString mark = paramElement.attribute("mark");
            QString data = paramElement.attribute("data");
            paramMap.insert(mark, data);
        }
        paramNode = paramNode.nextSibling();
    }
    QString msg = QString("模块（AppConfig）加载配置文件（%1）成功").arg(filePath);
    LOG_INFO(msg);
}

// 获取配置项的方法
QString AppConfig::getAppConfig(QString  mark, QString  defaultValue) const
{
    return paramMap.value(mark, defaultValue);
}

// 设置配置项的方法
void AppConfig::setAppConfig(QString  mark, QString  data)
{
    paramMap.insert(mark, data);
}