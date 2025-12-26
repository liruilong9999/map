#ifndef MAINWINDOWPLUGIN_H
#define MAINWINDOWPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <QAction>
#include <LCommon/IPlugin.h>

#include "MainWindow.h"

class MainWindowPlugin : public QObject,
                         public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "MainWindowPlugin" FILE "plugin_info.json") // QT5.0 ÒýÈë
    Q_INTERFACES(IPlugin)
public:
    explicit MainWindowPlugin(QObject * parent = 0);
    virtual QString getName() override;
    virtual bool    init() override;
    virtual bool    clean() override;
public slots:
    

private:
    QAction * m_actionTestClass{nullptr}; // °´Å¥

    MainWindow * m_pMainWindow{nullptr};
};

#endif // MAINWINDOWPLUGIN_H