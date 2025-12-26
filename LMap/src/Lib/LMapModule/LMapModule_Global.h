#ifndef LMapModule_GLOBAL_H
#define LMapModule_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LMAPMODULE_LIBRARY)
#define LMAPMODULE_EXPORT Q_DECL_EXPORT
#else
#define LMAPMODULE_EXPORT Q_DECL_IMPORT
#endif

#endif // LMapModule_GLOBAL_H
