#ifndef COREPLUGIN_GLOBAL_H
#define COREPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef COREPLUGIN_EXPORT
# define COREPLUGIN_API Q_DECL_EXPORT
#else
# define COREPLUGIN_API Q_DECL_IMPORT
#endif

#endif // COREPLUGIN_GLOBAL_H
