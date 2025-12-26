#include "MainWindowPlugin.h"

MainWindowPlugin::MainWindowPlugin(QObject * parent)
{
}

QString MainWindowPlugin::getName()
{
    return QString();
}

bool MainWindowPlugin::init()
{
    m_pMainWindow = new MainWindow();

    m_pMainWindow->show();

    return true;
}

bool MainWindowPlugin::clean()
{
    if (m_pMainWindow)
    {
        delete m_pMainWindow;
        m_pMainWindow = nullptr;
    }
    return true;
}
