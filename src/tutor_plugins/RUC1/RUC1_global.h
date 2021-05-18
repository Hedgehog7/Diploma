#ifndef RUC1_GLOBAL_H
#define RUC1_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RUC1_LIBRARY)
#  define RUC1_EXPORT Q_DECL_EXPORT
#else
#  define RUC1_EXPORT Q_DECL_IMPORT
#endif

#endif // RUC1_GLOBAL_H
