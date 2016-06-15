#include "mediasrvqtplugin.h"




extern "C" Q_DECL_EXPORT  void * MYAPI MediaSessionNotifyOpen(const char *configFile)
{
    AdAPIInit_Mtx();
    RDE_Init();
    CMediasrvQtPlugin *ntf = new CMediasrvQtPlugin();
 
    if( !ntf->InitConfig(configFile) )
    {
        delete ntf ;
        return NULL ;
    }
    ntf->Open();
    return ntf ;
}



/////////////////////////////////////////////////////
#define kSectionQtSession "qtplugin-session"
#define kSectionQtCheckSession "qtplugin-check-session"
#define kSectionQtUser "qtplugin-user"

#define kTablename "table_name"
#define kVideoSession "field_video_session"

#define kUserId "field_user_id"
#define kUserPwd "field_user_pwd"
#define kVideoOnline "field_video_online"

#define kCheckPwd "check_pwd"
#define kMd5IsUppercase "md5_is_uppercase"
#define kPwdMd5Count "pwd_md5_count"
#define kDeleteSessionUserNotExist "user_not_exist_delete_session"

CMediasrvQtPlugin::CMediasrvQtPlugin()
{

	m_bMd5IsUppercase = FALSE;
    m_tmLastReconnect = 0 ;
    m_tmLastError = 0 ;

    m_bCheckpwd = FALSE ;
    m_bUserNotExistDeleteSession = FALSE ;
    m_nPwdMd5Count = 0;


    m_szTableName[0] = 0 ;
    m_szFieldId[0] = 0;
    m_szFieldPwd[0] = 0;
    m_szFieldSession[0] = 0;
	m_szFieldVideoOnline[0] = 0;

    memset(m_szFieldId_, 0, sizeof(m_szFieldId_));
    memset(m_szFieldPwd_, 0, sizeof(m_szFieldPwd_));
    memset(m_szFieldSession_, 0, sizeof(m_szFieldSession_));
	memset(m_szFieldVideoOnline_, 0, sizeof(m_szFieldVideoOnline_));



}
CMediasrvQtPlugin::~CMediasrvQtPlugin()
{

}
bool CMediasrvQtPlugin::InitConfigSessionMode(const char *configFile)
{
	char szSection[] = kSectionQtSession;
	IniReadStr(szSection, kTablename, m_szTableName, sizeof(m_szTableName), configFile);
	if (m_szTableName[0] == 0)
	{
		LogPrint(_LOG_LEVEL_ERROR, _TAG_, "[%s].%s is empty\n", szSection, kTablename);
		return false;
	}

	IniReadStr(szSection, kVideoSession, m_szFieldSession, sizeof(m_szFieldSession), configFile);
	if (m_szFieldSession[0] == 0)
	{
		LogPrint(_LOG_LEVEL_ERROR, _TAG_, "[%s].%s is empty\n", szSection, kVideoSession);
		return false;
	}
	m_szFieldSession_[0] = ':';
	strcatn(m_szFieldSession_, m_szFieldSession, sizeof(m_szFieldSession_));

	return true;
}
bool CMediasrvQtPlugin::InitConfigCheckSessionMode(const char *configFile)
{
	char szSection[] = kSectionQtCheckSession;
	IniReadStr(szSection, kTablename, m_szTableName, sizeof(m_szTableName), configFile);
	if (m_szTableName[0] == 0)
	{
		LogPrint(_LOG_LEVEL_ERROR, _TAG_, "[%s].%s is empty\n", szSection, kTablename);
		return false;
	}

	IniReadStr(szSection, kVideoSession, m_szFieldSession, sizeof(m_szFieldSession), configFile);
	if (m_szFieldSession[0] == 0)
	{
		LogPrint(_LOG_LEVEL_ERROR, _TAG_, "[%s].%s is empty\n", szSection, kTablename);
		return false;
	}
	m_szFieldSession_[0] = ':';
	strcatn(m_szFieldSession_, m_szFieldSession, sizeof(m_szFieldSession_));

	IniReadStr(szSection, kVideoOnline, m_szFieldVideoOnline, sizeof(m_szFieldVideoOnline), configFile);
	if (m_szFieldVideoOnline[0] == 0)
	{
		LogPrint(_LOG_LEVEL_ERROR, _TAG_, "[%s].%s is empty\n", szSection, kVideoOnline);
		return false;
	}
	m_szFieldVideoOnline_[0] = ':';
	strcatn(m_szFieldVideoOnline_, m_szFieldVideoOnline, sizeof(m_szFieldVideoOnline_));



	m_bCheckpwd = IniReadBool(szSection, kCheckPwd, configFile, 0);

	if (m_bCheckpwd)
	{
		IniReadStr(szSection, kUserPwd, m_szFieldPwd, sizeof(m_szFieldPwd), configFile);
		if (m_szFieldPwd[0] == 0)
		{
			LogPrint(_LOG_LEVEL_ERROR, _TAG_, "[%s].%s is empty\n", szSection, kUserPwd);
			return false;
		}
		m_szFieldPwd_[0] = ':';
		strcatn(m_szFieldPwd_, m_szFieldPwd, sizeof(m_szFieldPwd_));
	}


	m_bUserNotExistDeleteSession = IniReadBool(szSection, kDeleteSessionUserNotExist, configFile, TRUE);

	m_nPwdMd5Count = IniReadInt(szSection, kPwdMd5Count, configFile, 0);
	m_bMd5IsUppercase = IniReadInt(szSection, kMd5IsUppercase, configFile, 0);

	return true;
}
bool CMediasrvQtPlugin::InitConfigUserMode(const char *configFile)
{
	char szSection[] = kSectionQtUser;
	IniReadStr(szSection, kTablename, m_szTableName, sizeof(m_szTableName), configFile);
	if (m_szTableName[0] == 0)
	{
		LogPrint(_LOG_LEVEL_ERROR, _TAG_, "[%s].%s is empty\n", szSection, kTablename);
		return false;
	}

	m_bCheckpwd = IniReadBool(szSection, kCheckPwd, configFile, 0);

	IniReadStr(szSection, kUserId, m_szFieldId, sizeof(m_szFieldId), configFile);
	if (m_szFieldId[0] == 0)
	{
		LogPrint(_LOG_LEVEL_ERROR, _TAG_, "[%s].%s is empty\n", szSection, kUserId);
		return false;
	}
	m_szFieldId_[0] = ':';
	strcatn(m_szFieldId_, m_szFieldId, sizeof(m_szFieldId_));


	IniReadStr(szSection, kVideoOnline, m_szFieldVideoOnline, sizeof(m_szFieldVideoOnline), configFile);
	if (m_szFieldVideoOnline[0] == 0)
	{
		LogPrint(_LOG_LEVEL_ERROR, _TAG_, "[%s].%s is empty\n", szSection, kVideoOnline);
		return false;
	}
	m_szFieldVideoOnline_[0] = ':';
	strcatn(m_szFieldVideoOnline_, m_szFieldVideoOnline, sizeof(m_szFieldVideoOnline_));

	if (m_bCheckpwd)
	{
		IniReadStr(szSection, kUserPwd, m_szFieldPwd, sizeof(m_szFieldPwd), configFile);
		if (m_szFieldPwd[0] == 0)
		{
			LogPrint(_LOG_LEVEL_ERROR, _TAG_, "[%s].%s is empty\n", szSection, kUserPwd);
			return false;
		}
		m_szFieldPwd_[0] = ':';
		strcatn(m_szFieldPwd_, m_szFieldPwd, sizeof(m_szFieldPwd_));
	}


	IniReadStr(szSection, kVideoSession, m_szFieldSession, sizeof(m_szFieldSession), configFile);
	if (m_szFieldSession[0] == 0)
	{
		LogPrint(_LOG_LEVEL_ERROR, _TAG_, "[%s].%s is empty\n", szSection, kVideoSession);
		return false;
	}
	m_szFieldSession_[0] = ':';
	strcatn(m_szFieldSession_, m_szFieldSession, sizeof(m_szFieldSession_));




	m_bUserNotExistDeleteSession = IniReadBool(szSection, kDeleteSessionUserNotExist, configFile, TRUE);
	
	m_nPwdMd5Count = IniReadInt(szSection, kPwdMd5Count, configFile, 0);
	m_bMd5IsUppercase = IniReadInt(szSection, kMd5IsUppercase, configFile, 0);

	return true;
}
bool CMediasrvQtPlugin::InitConfig(const char *configFile)
{
  
    bool bRet = CSessionNotifyObj::InitConfig(configFile);
    char szBuf[100];
    IniReadStr("mediasrv","notify_addr",szBuf,sizeof(szBuf),configFile);
    char szAddr[80];
    RDE_IP_PORT( szBuf ,szAddr , sizeof(szAddr) , &m_destPort );
    if( m_destPort == 0 )
    {
        m_destPort = 9300 ;
    }
    if( szAddr[0] == 0 )
    {
        strcpy(szAddr ,"127.0.0.1");
    }
    m_destIp = RDE_GetIPByName( szAddr , NULL , 0 );
	bRet = false;
	switch (m_pluginType)
	{
		case PluginTypeSession:
		{
			bRet = InitConfigSessionMode(configFile);
			break;
		}
		case PluginTypeCheckSession:
		{
			bRet = InitConfigCheckSessionMode(configFile);
			break;
		}
		default:
		{
			bRet = InitConfigUserMode(configFile);
			break;
		}
	}

	if (!bRet)
	{
		return false;
	}
	bRet = Open();
	if (bRet && m_pluginType == PluginTypeSession)
	{
		static char szSQL_truncate[] = "TRUNCATE TABLE %s ";
		QString strSQL;

		strSQL.sprintf(szSQL_truncate, m_szTableName);
		db().exec(strSQL);
		db().commit();
	}
	return bRet;
}

bool CMediasrvQtPlugin::Open()
{

    return CSessionNotifyObj::Open() ;
}

void CMediasrvQtPlugin::Close()
{
    CSessionNotifyObj::Close();
}

void CMediasrvQtPlugin::Notify(MediaSessionNotifyType type, const MediaSessionPar *par)
{
    BOOL bRet = FALSE;
    BOOL bTry = FALSE;
LABEL_TRY:;





    switch (type)
    {
		case SessionNotifyTypeAdd:
		{
			BOOL bError = FALSE;

			switch (m_pluginType)
			{
				case PluginTypeSession:
				{
					bRet = SessionNotifyTypeAdd_Session(par, bError);
					break;
				}
				case PluginTypeCheckSession:
				{
					bRet = SessionNotifyTypeAdd_CheckSession(par, bError);
					break;
				}
				default:
				{
					bRet = SessionNotifyTypeAdd_User(par, bError);
					break;
				}
			}

			
			if (bError)
			{
				CloseSession(par );
			}
			break;
		}
		case SessionNotifyTypeRemove:
		{
			switch (m_pluginType)
			{
				case PluginTypeSession:
				{
					bRet = SessionNotifyTypeRemove_Session(par);
					break;
				}
				case PluginTypeCheckSession:
				{
					bRet = SessionNotifyTypeRemove_CheckSession(par);
					break;
				}
				default:
				{
					bRet = SessionNotifyTypeRemove_User(par);
					break;
				}
			}


			
			break;
		}
		case SessionNotifyUpdateConnections:
		{
			switch (m_pluginType)
			{
				case PluginTypeSession:
				{
					bRet = SessionUpdateConnections_Session(par);
					break;
				}
				case PluginTypeCheckSession:
				{
					bRet = SessionUpdateConnections_CheckSession(par);
					break;
				}
				default:
				{
					bRet = SessionUpdateConnections_User(par);
					break;
				}
			}


			
			break;
		}
    }


}
void CMediasrvQtPlugin::CloseSession(const MediaSessionPar *par)
{
    NotifyServer("close-session", par->session);
}

BOOL CMediasrvQtPlugin::OnError(QSqlQuery &query)
{
   
    BOOL bRet = FALSE;
    QString strErrorText;


    strErrorText = query.lastError().text();

    if (!strErrorText.isEmpty())
    {
#if defined(WIN32)
        LogPrint(_LOG_LEVEL_ERROR, _TAG_, "%s\n", (const char *)strErrorText.toLocal8Bit().constData());
#else
        LogPrint(_LOG_LEVEL_ERROR, _TAG_, "%s\n", (const char *)strErrorText.toUtf8().constData());
#endif
    }
    return bRet;

}
BOOL CMediasrvQtPlugin::SessionNotifyTypeAdd_User(const MediaSessionPar *par, BOOL &bError)
{
    
    if (par->usr[0] == 0)
    {
        if (m_bUserNotExistDeleteSession)
        {
            CloseSession(par);
        }
        return FALSE;
    }
    
    static char szSQL_update[] = "UPDATE %s set %s=:%s , %s=:%s where %s=:%s ";
    static char szSQL_updateWithPwd[] = "UPDATE %s set %s=:%s , %s=:%s where %s=:%s and %s=:%s ";

    char szPwd[80];
    BOOL bTryied = FALSE;
    szPwd[0] = 0 ;

LABEL_TRY:;

    {
        QSqlQuery update(db());
        QString strSQL;
        if (m_bCheckpwd)
        {
            strSQL.sprintf( szSQL_updateWithPwd, m_szTableName
                , m_szFieldSession, m_szFieldSession 
				, m_szFieldVideoOnline, m_szFieldVideoOnline
                , m_szFieldId, m_szFieldId
                , m_szFieldPwd, m_szFieldPwd
                );
       

        }
        else
        {
			strSQL.sprintf(szSQL_update, m_szTableName
                , m_szFieldSession, m_szFieldSession
				, m_szFieldVideoOnline, m_szFieldVideoOnline
                , m_szFieldId, m_szFieldId
                );
        }

        if (!update.prepare(strSQL))
        {
            if (!bTryied && IsGoneAway(update))
            {
                Close();
                if (Open())
                {
                    bTryied = TRUE;
                    goto LABEL_TRY;
                }

            }
            bError = TRUE;
            OnError(update);
            return  FALSE;
        }

        strcpyn( szPwd, par->pwd, sizeof(szPwd) );
        if (m_bCheckpwd && szPwd[0] != 0)
        {
            int i = 0;
            for (i = 0; i < m_nPwdMd5Count; i++)
            {
                GetMD5(szPwd, -1, szPwd, sizeof(szPwd));
            }

			if (m_nPwdMd5Count > 0 && m_bMd5IsUppercase)
			{
				StrUpper( szPwd );
			}
        }

        
		update.bindValue(m_szFieldVideoOnline, 1);
		update.bindValue(m_szFieldSession_, par->session);
        update.bindValue(m_szFieldId_, par->usr);

        if ( m_bCheckpwd)
        {
            update.bindValue(m_szFieldPwd_, szPwd );
        }


        if (!update.exec())
        {
            OnError(update);
            bError = TRUE;
            return  FALSE;
        }

        if ( update.numRowsAffected() < 1 )
        {
            if (m_bUserNotExistDeleteSession)
            {
                CloseSession(par);
            }
        }
        else
        {
            db( ).commit( );
        }







    }



    return TRUE ;
}

BOOL CMediasrvQtPlugin::SessionNotifyTypeRemove_User(const MediaSessionPar *par)
{
    if (par->usr[0] == 0)
    {
        return FALSE;
    }
    BOOL bTryied = FALSE;
    static char szSQL_update[] = "UPDATE %s set %s=:%s  where %s=:%s";
LABEL_TRY:;

    {
        QSqlQuery update( db( ) );
        QString strSQL;
        strSQL.sprintf( szSQL_update, m_szTableName
			, m_szFieldVideoOnline, m_szFieldVideoOnline
            , m_szFieldId, m_szFieldId
            );

        if (!update.prepare(strSQL))
        {
            if (!bTryied && IsGoneAway(update))
            {
                Close();
                if (Open())
                {
                    bTryied = TRUE;
                    goto LABEL_TRY;
                }

            }
            OnError(update);
            return  FALSE;
        }

		update.bindValue(m_szFieldVideoOnline_, 0);
        update.bindValue(m_szFieldId_, par->usr);

        if (!update.exec())
        {
            OnError(update);
            return  FALSE;
        }


        db( ).commit( );


    }

    return TRUE ;
}

BOOL CMediasrvQtPlugin::SessionUpdateConnections_User(const MediaSessionPar *par)
{
    return TRUE ;
}
BOOL CMediasrvQtPlugin::SessionNotifyTypeAdd_Session(const MediaSessionPar *par, BOOL &bError)
{
	static char szSQL_insert[] = "INSERT INTO %s(%s) VALUES(:%s) ";
	QString strSQL;

	strSQL.sprintf(szSQL_insert, m_szTableName, m_szFieldSession, m_szFieldSession);

	BOOL bTryied = FALSE;
LABEL_TRY:;
	{
		QSqlQuery update(db());
		if (!update.prepare(strSQL))
		{
			if (!bTryied && IsGoneAway(update))
			{
				Close();
				if (Open())
				{
					bTryied = TRUE;
					goto LABEL_TRY;
				}

			}
			bError = TRUE;
			OnError(update);
			return  FALSE;
		}

		update.bindValue(m_szFieldSession_, par->session);

		if (!update.exec())
		{
			OnError(update);
			return  FALSE;
		}


		db().commit();


	}

	return TRUE;
}
BOOL CMediasrvQtPlugin::SessionNotifyTypeRemove_Session(const MediaSessionPar *par)
{
	static char szSQL_delete[] = "DELETE FROM  %s WHERE %s=:%s ";
	QString strSQL;

	strSQL.sprintf(szSQL_delete, m_szTableName, m_szFieldSession, m_szFieldSession);

	BOOL bTryied = FALSE;
	LABEL_TRY:;
	{
		QSqlQuery update(db());
		if (!update.prepare(strSQL))
		{
			if (!bTryied && IsGoneAway(update))
			{
				Close();
				if (Open())
				{
					bTryied = TRUE;
					goto LABEL_TRY;
				}

			}
			OnError(update);
			return  FALSE;
		}

		update.bindValue(m_szFieldSession_, par->session);

		if (!update.exec())
		{
			OnError(update);
			return  FALSE;
		}


		db().commit();


	}

	return TRUE;
}
BOOL CMediasrvQtPlugin::SessionUpdateConnections_Session(const MediaSessionPar *par)
{
	return TRUE;
}

BOOL CMediasrvQtPlugin::SessionNotifyTypeAdd_CheckSession(const MediaSessionPar *par, BOOL &bError)
{


	static char szSQL_updateWithPwd[] = "UPDATE %s set %s=:%s where %s=:%s and %s=:%s ";
	static char szSQL_update[] = "UPDATE %s set  %s=:%s where %s=:%s ";
	

	char szPwd[80];
	BOOL bTryied = FALSE;
	szPwd[0] = 0;

LABEL_TRY:;

	{
		QSqlQuery update(db());
		QString strSQL;
		if (m_bCheckpwd)
		{
			strSQL.sprintf(szSQL_updateWithPwd, m_szTableName
				, m_szFieldVideoOnline, m_szFieldVideoOnline
				, m_szFieldSession, m_szFieldSession
				, m_szFieldPwd, m_szFieldPwd
				);


		}
		else
		{
			strSQL.sprintf(szSQL_update, m_szTableName
				, m_szFieldVideoOnline, m_szFieldVideoOnline
				, m_szFieldSession, m_szFieldSession
				);
		}

		if (!update.prepare(strSQL))
		{
			if (!bTryied && IsGoneAway(update))
			{
				Close();
				if (Open())
				{
					bTryied = TRUE;
					goto LABEL_TRY;
				}

			}
			bError = TRUE;
			OnError(update);
			return  FALSE;
		}

		strcpyn(szPwd, par->pwd, sizeof(szPwd));
		if (m_bCheckpwd && szPwd[0] != 0)
		{
			int i = 0;
			for (i = 0; i < m_nPwdMd5Count; i++)
			{
				GetMD5(szPwd, -1, szPwd, sizeof(szPwd));
			}

			if (m_nPwdMd5Count > 0 && m_bMd5IsUppercase)
			{
				StrUpper(szPwd);
			}
		}


		update.bindValue(m_szFieldVideoOnline_, 1);
		update.bindValue(m_szFieldSession_, par->session);


		if (m_bCheckpwd)
		{
			update.bindValue(m_szFieldPwd_, szPwd);
		}


		if (!update.exec())
		{
			OnError(update);
			bError = TRUE;
			return  FALSE;
		}

		if (update.numRowsAffected() < 1)
		{
			if (m_bUserNotExistDeleteSession)
			{
				CloseSession(par);
			}
		}
		else
		{
			db().commit();
		}







	}



	return TRUE;
}
BOOL CMediasrvQtPlugin::SessionNotifyTypeRemove_CheckSession(const MediaSessionPar *par)
{
	BOOL bTryied = FALSE;
	static char szSQL_update[] = "UPDATE %s set %s=:%s  where %s=:%s";
LABEL_TRY:;

	{
		QSqlQuery update(db());
		QString strSQL;
		strSQL.sprintf(szSQL_update, m_szTableName
			, m_szFieldVideoOnline, m_szFieldVideoOnline
			, m_szFieldSession, m_szFieldSession
			);

		if (!update.prepare(strSQL))
		{
			if (!bTryied && IsGoneAway(update))
			{
				Close();
				if (Open())
				{
					bTryied = TRUE;
					goto LABEL_TRY;
				}

			}
			OnError(update);
			return  FALSE;
		}

		update.bindValue(m_szFieldVideoOnline_, 0);
		update.bindValue(m_szFieldSession_, par->session);

		if (!update.exec())
		{
			OnError(update);
			return  FALSE;
		}


		db().commit();


	}

	return TRUE;
}
BOOL CMediasrvQtPlugin::SessionUpdateConnections_CheckSession(const MediaSessionPar *par)
{
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMediasrvQtPlugin::DeleteSession(const char *sessionName)
{

}

void CMediasrvQtPlugin::LogError()
{

}
