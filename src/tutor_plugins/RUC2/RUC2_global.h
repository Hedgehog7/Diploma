#ifndef RUC2_GLOBAL_H
#define RUC2_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RUC2_LIBRARY)
#  define RUC2_EXPORT Q_DECL_EXPORT
#else
#  define RUC2_EXPORT Q_DECL_IMPORT
#endif

#endif // RUC2_GLOBAL_H
