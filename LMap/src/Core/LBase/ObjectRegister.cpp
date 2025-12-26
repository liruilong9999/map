#include "ObjectRegister.h"

#include <QDebug>

ObjectRegistry & ObjectRegistry::instance()
{
    static ObjectRegistry instance;
    return instance;
}

void ObjectRegistry::registerObject(const QString & name, QObject * object)
{
    registry[name] = object;
}

QObject * ObjectRegistry::getObject(const QString & name) const
{
    auto it = registry.find(name);
    if (it != registry.end())
    {
        return it.value();
    }
    return nullptr;
}

ObjectRegistry::ObjectRegistry()
{
    
}
