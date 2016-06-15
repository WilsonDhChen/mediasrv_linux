#include "sessionnotifyobj.h"


static MTX_OBJ _mtxDb;

static void init()
{
	static BOOL _bInit = FALSE;
	if (_bInit)
	{
		return;
	}
	if (!_bInit)
	{
		_bInit = TRUE;
		MtxInit(&_mtxDb, 0);
	}

#ifdef WIN32
	char szAppDir[500];
	memset(szAppDir, 0, sizeof(szAppDir));
	GetModuleFileNameA(NULL, szAppDir, sizeof(szAppDir));
	ChopPathFileName(szAppDir);
#else
	char szAppDir[500] = "/opt/webmsgsrv/";
#endif

	QCoreApplication::addLibraryPath(szAppDir);




}

CSessionNotifyObj::CSessionNotifyObj()
{
	init();
    m_notifyServer = NULL;
    memset(&m_DbIni, 0, sizeof(m_DbIni));
	m_pluginType = PluginTypeUser;

    m_db = new CQSqlDatabase( );
}


CSessionNotifyObj::~CSessionNotifyObj()
{
    QString dbName = m_db->m_Database.connectionName( );
    delete m_db;

    if( !dbName.isEmpty( ) )
    {
        QSqlDatabase::removeDatabase( dbName );
    }
}





bool CSessionNotifyObj::InitConfig(const char *configFile)
{
    CMtxAutoLock lock(&_mtxDb);
	char szSection[] = "database";
	char szMode[40];
	IniReadDatabase(configFile, szSection, &m_DbIni);
	IniReadStr(szSection, "mode", szMode, sizeof(szMode), configFile);

	if (strcmpix(szMode ,"session") == 0
		|| strcmpix(szMode, "sess") == 0
		|| strcmpix(szMode, "ss") == 0
		|| strcmpix(szMode, "s") == 0
		)
	{
		m_pluginType = PluginTypeSession;
	}
	else if (strcmpix(szMode, "checksession") == 0
		|| strcmpix(szMode, "check-session") == 0
		|| strcmpix(szMode, "check_session") == 0
		|| strcmpix(szMode, "chk") == 0
		|| strcmpix(szMode, "cs") == 0
		|| strcmpix(szMode, "chkss") == 0
		|| strcmpix(szMode, "chks") == 0
		)
	{
		m_pluginType = PluginTypeCheckSession;
	}
	else
	{
		m_pluginType = PluginTypeUser;
	}

    m_strIniFile = QString::fromUtf8(configFile);

    BOOL bNormal = TRUE;

    char szDbName[100];
    memset(szDbName, 0, sizeof(szDbName));
    snprintf(szDbName, sizeof(szDbName)-2, "%s-%x", m_DbIni.szDatabase, this);
    QSqlDatabase::removeDatabase(szDbName);
    m_db->m_Database = QSqlDatabase::addDatabase(Str2DrvType(m_DbIni.szExtra), szDbName);

    if( !db( ).isValid( ) )
    {
        return false;
    }

    if( db( ).driverName( ).contains( "mysql", Qt::CaseInsensitive ) )
    {
        db( ).setConnectOptions( "CLIENT_FOUND_ROWS=1" );
    }
    else if( db( ).driverName( ).contains( "ODBC", Qt::CaseInsensitive ) )
    {


        char szConnStr[1024];
        memset(szConnStr, 0, sizeof(szConnStr));
        if (strcmpifind(m_DbIni.szExtra, "mssql") >= 0)
        {
            bNormal = FALSE;

            snprintf(szConnStr, sizeof(szConnStr)-1, "Driver={sql server};server=%s;database=%s;uid=%s;pwd=%s;SQL_ATTR_CONNECTION_TIMEOUT=10",
                m_DbIni.szHost, m_DbIni.szDatabase, m_DbIni.szUser, m_DbIni.szPass);

            //  QString dsn = QString("Driver={sql server};server=%1;database=%2;uid=%3;pwd=%4;").arg(m_DbIni.szHost,m_DbIni.szDatabase,m_DbIni.szUser,m_DbIni.szPass);
            db() .setDatabaseName( szConnStr );
            db( ).setUserName( m_DbIni.szUser );
            db( ).setPassword( m_DbIni.szPass );
            //   m_Database.setConnectOptions(""); 


        }
    }



    if (bNormal)
    {
        db( ).setDatabaseName( m_DbIni.szDatabase );
        db( ).setUserName( m_DbIni.szUser );
        db( ).setPassword( m_DbIni.szPass );
        db( ).setHostName( m_DbIni.szHost );

    }


    return true;
}
void  CSessionNotifyObj::NotifyServer(const char *cmd, const char *par)
{

    if (m_notifyServer != NULL)
    {
        m_notifyServer(cmd, par);
    }

}
BOOL CSessionNotifyObj::IsGoneAway(QSqlQuery &query)
{

    QString strErrorText;



    strErrorText = query.lastError().text();
    if (strErrorText.indexOf("gone away", 0, Qt::CaseInsensitive) >= 0
     //   || query.lastError().type() == QSqlError::ConnectionError
        )
    {
        return TRUE;
    }
    else
    {
        return FALSE;

    }

}
void CSessionNotifyObj::Close()
{
    db( ).close( );
}
bool CSessionNotifyObj::Open()
{
    if( !db( ).open( ) )
    {

        QString strErrorText;
        strErrorText = db( ).lastError( ).text( );
        if (!strErrorText.isEmpty())
        {
#if defined(WIN32)
            LogPrint(_LOG_LEVEL_ERROR, "CSessionNotifyObj", "%s\n", (const char *)strErrorText.toLocal8Bit().constData());
#else
            LogPrint(_LOG_LEVEL_ERROR, "CSessionNotifyObj", "%s\n", (const char *)strErrorText.toUtf8().constData());
#endif
        }

        return false;

        
    }
    else
    {
        return true;
    }

    
}
void CSessionNotifyObj::Notify(MediaSessionNotifyType type, const MediaSessionPar *par)
{

}
QString CSessionNotifyObj::Str2DrvType(const char *szDrv)
{
    if (!szDrv || !szDrv[0])
    {
        return QString("QMYSQL");
    }

    if (strcmpifind(szDrv, "mysql") >= 0)
    {
        return QString("QMYSQL");
    }
    else if (strcmpifind(szDrv, "odbc") >= 0)
    {
        return QString("QODBC");
    }
    else if (strcmpifind(szDrv, "mssql") >= 0)
    {
        return QString("QODBC");
    }
    else
    {
        return QString(szDrv);
    }
}
QSqlDatabase & CSessionNotifyObj::db( )
{
    return  m_db->m_Database;
}



extern "C" Q_DECL_EXPORT   void   MYAPI MediaSessionNotifyClose(void *handle)
{
    CSessionNotifyObj *ntf = (CSessionNotifyObj *)handle;
    if (ntf == NULL)
    {
        return;
    }
    ntf->Close();
    delete ntf;

}
extern "C" Q_DECL_EXPORT    void  MYAPI MediaSessionNotifySetCallback(void *handle, _MediasrvCallback callback)
{
    CSessionNotifyObj *ntf = (CSessionNotifyObj *)handle;
    if (ntf == NULL)
    {
        return;
    }
    ntf->m_notifyServer = callback;
}
extern "C" Q_DECL_EXPORT   void   MYAPI MediaSessionNotify(void *handle, MediaSessionNotifyType type, const MediaSessionPar *par)
{
    CSessionNotifyObj *ntf = (CSessionNotifyObj *)handle;
    if (ntf == NULL)
    {
        return;
    }
    ntf->Notify(type, par);
}







