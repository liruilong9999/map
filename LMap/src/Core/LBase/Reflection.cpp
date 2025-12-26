#include "Reflection.h"

QObject * ClassFactory::getClassByName(const QString & className, ObjectCreationMode mode)
{
    if (mode == ObjectCreationMode::Singleton)
    {
        // 单例模式：检查是否已有对象
        if (!m_singletonsMap.contains(className))
        {
            // 如果没有，创建并缓存对象
            auto createFunc = m_classMap.value(className);
            if (createFunc)
            {
                m_singletonsMap[className] = createFunc();
            }
        }
        return m_singletonsMap.value(className, nullptr); // 如果没有，则返回 nullptr
    }
    else
    {
        // 原型模式：每次创建新对象
        auto createFunc = m_classMap.value(className);
        if (createFunc)
        {
            return createFunc();
        }
    }
    return nullptr; // 如果没有找到相应的创建函数，返回 nullptr
}

void ClassFactory::registClass(const QString & name, PTRCreateObject method)
{
    m_classMap.insert(name, method);
}

ClassFactory & ClassFactory::getInstance()
{
    static ClassFactory sLo_factory;
    return sLo_factory;
}

RegisterAction::RegisterAction(const QString & className, PTRCreateObject ptrCreateFn)
{
    ClassFactory::getInstance().registClass(className, ptrCreateFn);
}