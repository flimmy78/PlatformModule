#ifndef TCBBE_TCPARASETGUI_GLOBAL_H
#define TCBBE_TCPARASETGUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TCBBE_TCPARASETGUI_LIBRARY)
#  define TCBBE_TCPARASETGUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define TCBBE_TCPARASETGUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TCBBE_TCPARASETGUI_GLOBAL_H

