#ifndef MEDIASRVQTPLUGIN_GLOBAL_H
#define MEDIASRVQTPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>

#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>
#include <QtCore/QHash>
#include <QtCore/QDateTime>

#ifdef _MSC_VER
#ifdef QT_NO_DEBUG
#pragma comment(lib, "adapi.lib")
#pragma comment(lib, "rde.lib")
#pragma comment(lib, "libcpputil.lib")
#else
#pragma comment(lib, "adapid.lib")
#pragma comment(lib, "rded.lib")
#pragma comment(lib, "libcpputild.lib")
#endif
#endif

#define _TAG_ "mediasrv_plugin"

#if defined(MEDIASRVQTPLUGIN_LIBRARY)
#  define MEDIASRVQTPLUGINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MEDIASRVQTPLUGINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MEDIASRVQTPLUGIN_GLOBAL_H
