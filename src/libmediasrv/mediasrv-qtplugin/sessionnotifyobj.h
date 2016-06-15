

#ifndef _sessionnotifyobj_h_
#define _sessionnotifyobj_h_

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QCoreApplication>


#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>


#ifndef WIN32
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#if !defined(_FreeBSD_)
#include <alloca.h>
#endif


#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


#include <pthread.h>

#endif

#include "adtypes.h"
#include "adapi.h"
#include "rde.h"
#include "libcpputil.h"
#include "libmediasrv.h"

class CQSqlDatabase
{
public:
    QSqlDatabase m_Database;
};
class CSessionNotifyObj
{
public:
	typedef enum tagPluginType
	{
		PluginTypeUser,
		PluginTypeSession,
		PluginTypeCheckSession,

	}PluginType;
public:
    CSessionNotifyObj();
    virtual ~CSessionNotifyObj();

public:
    virtual bool InitConfig(const char *configFile);
    virtual bool Open();
    virtual void Close();
    virtual void Notify(MediaSessionNotifyType type, const MediaSessionPar *par);
    virtual BOOL IsGoneAway(QSqlQuery &query);

    void  NotifyServer(const char *cmd, const char *par);
    QString Str2DrvType(const char *szDrv);
    QSqlDatabase & db( );
public:
    DATABASE_INI m_DbIni;
    CQSqlDatabase *m_db;
    QString  m_strIniFile;
    _MediasrvCallback m_notifyServer;
	PluginType m_pluginType;
};

#endif
