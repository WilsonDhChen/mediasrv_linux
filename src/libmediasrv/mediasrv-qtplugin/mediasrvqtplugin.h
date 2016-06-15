#ifndef MEDIASRVQTPLUGIN_H
#define MEDIASRVQTPLUGIN_H





#include "mediasrv-qtplugin_global.h"
#include "sessionnotifyobj.h"



class  CMediasrvQtPlugin:
    public CSessionNotifyObj
{

public:
    CMediasrvQtPlugin();
    ~CMediasrvQtPlugin();

    virtual bool InitConfig(const char *configFile);
    virtual bool Open();
    virtual void Close();
    virtual void Notify(MediaSessionNotifyType type, const MediaSessionPar *par);

    BOOL SessionNotifyTypeAdd_User(const MediaSessionPar *par , BOOL &bError);
    BOOL SessionNotifyTypeRemove_User(const MediaSessionPar *par);
    BOOL SessionUpdateConnections_User(const MediaSessionPar *par);

	BOOL SessionNotifyTypeAdd_Session(const MediaSessionPar *par, BOOL &bError);
	BOOL SessionNotifyTypeRemove_Session(const MediaSessionPar *par);
	BOOL SessionUpdateConnections_Session(const MediaSessionPar *par);

	BOOL SessionNotifyTypeAdd_CheckSession(const MediaSessionPar *par, BOOL &bError);
	BOOL SessionNotifyTypeRemove_CheckSession(const MediaSessionPar *par);
	BOOL SessionUpdateConnections_CheckSession(const MediaSessionPar *par);

    void DeleteSession(const char *sessionName);
    void LogError();

    void CloseSession(const MediaSessionPar *par);
    BOOL OnError(QSqlQuery &query);

	bool InitConfigUserMode(const char *configFile);
	bool InitConfigSessionMode(const char *configFile);
	bool InitConfigCheckSessionMode(const char *configFile);
    
public:
    INT_IP m_destIp;
    int  m_destPort;
    TIME_T m_tmLastReconnect;
    TIME_T m_tmLastError;
    
    char m_szTableName[80];
    char m_szFieldId[80];
    char m_szFieldPwd[80];
    char m_szFieldSession[80];
	char m_szFieldVideoOnline[80];

    char m_szFieldId_[80];
    char m_szFieldPwd_[80];
    char m_szFieldSession_[80];
	char m_szFieldVideoOnline_[80];

    BOOL m_bCheckpwd;
    BOOL m_bUserNotExistDeleteSession;
	BOOL m_bMd5IsUppercase;
    int  m_nPwdMd5Count;

};

#endif // MEDIASRVQTPLUGIN_H
