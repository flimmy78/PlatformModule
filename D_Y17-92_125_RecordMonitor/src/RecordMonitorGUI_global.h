#ifndef RECORDMONITOR_GLOBAL_H
#define RECORDMONITOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RECORDMONITORGUISHARED_LIBRARY)
#  define RECORDMONITORGUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define RECORDMONITORGUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // RECORDMONITOR_GLOBAL_H

