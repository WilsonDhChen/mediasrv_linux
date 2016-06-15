

#ifndef _libmediasrv_h_
#define _libmediasrv_h_
	





#ifdef __cplusplus
extern "C"
{
#endif

#ifndef MYAPI

	#if defined(WIN32) || defined(_WIN32_WCE)

	#define MYAPI WINAPI

	#else

	#define MYAPI

	#endif

#endif




	typedef BOOL ( MYAPI *_MediaSrvAuthentication)(const char *szAddr,const char *szUser,const char *szPwd,const char *szURL,const char *szTimeBegin);
	BOOL MYAPI SetMediaSrvAuthenticationCallback( _MediaSrvAuthentication func);

	typedef BOOL ( MYAPI *_MediaSrvDisconnected)(const char *szAddr,const char *szUser,const char *szPwd,const char *szURL,const char *szTimeBegin,const char *szTimeEnd,int totalSeconds);
	BOOL MYAPI SetMediaSrvDisconnectedCallback( _MediaSrvDisconnected func);

	typedef BOOL ( MYAPI *_MediaSrvError)(int ErrorCode ,const char *szMsg);
	BOOL MYAPI SetMediaSrvErrorCallback( _MediaSrvError func);

	BOOL MYAPI MediaSrvStart();
	void MYAPI MediaSrvStop();

	typedef enum tagMediaSessionNotifyType
	{
		SessionNotifyTypeUnknown,
		SessionNotifyTypeAdd,
		SessionNotifyTypeRemove,
		ClientNotifyTypeAdd,
		ClientNotifyTypeRemove,
		SessionNotifyUpdateConnections,
        SystemNotifyIdle,
	}MediaSessionNotifyType;

	typedef struct tagMediaSessionPar
	{
		const char *usr ;
        const char *pwd ;
		const char *Id;
		const char *session ;
		const char *nameOfDisplay ;
		const char *timeBegin;
		const char *timeEnd;
		const char *urlOfJoin;
		const char *remoteAddr;
		const char *type;
		const char *os;
		INT64  bytesOfTransmit ;
		INT64  bytesOfSend2Clients ;
		int  sessionDurationOfSeconds; 
		int  connections;
	}MediaSessionPar;

    typedef void  (MYAPI *_MediasrvCallback)(const char *cmd, const char *par);

	typedef void * ( MYAPI *_MediaSessionNotifyOpen)(const char *configFile) ;
	typedef void   ( MYAPI *_MediaSessionNotifyClose)(void *handle) ;
	typedef void   ( MYAPI *_MediaSessionNotify)(void *handle, MediaSessionNotifyType type, const MediaSessionPar *par) ;
    typedef void   ( MYAPI *_MediaSessionNotifySetCallback)(void *handle, _MediasrvCallback callback);

//	void * MYAPI MediaSessionNotifyOpen(const char *configFile, _LogLevel log);
//	void   MYAPI MediaSessionNotifyClose(void *handle) ;
//	void   MYAPI MediaSessionNotify(void *handle, MediaSessionNotifyType type, const MediaSessionPar *par) ;

#ifdef __cplusplus
}
#endif	
	
	
	
	
	
	
	
	
	
#endif





