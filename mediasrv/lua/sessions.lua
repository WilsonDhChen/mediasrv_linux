
require("string")

_DBNAME="livestream"
_DBUSER="root"
_DBPWD="123456"
_DBADDR="127.0.0.1"
_DBPORT=3306


_mtxDb = mtx.create()


---MySQL
envMysql = luasql.mysql.mysql();
conn , errMysql= envMysql:connect(_DBNAME,_DBUSER,_DBPWD,_DBADDR,_DBPORT);


if( errMysql ~=nil)
then 
	print(errMysql);
	app.initfailed(-1)
	return 
end


function exesql(con,sql)
	mtx.lock(_mtxDb)

	if ( con == nil )
	then
		mtx.unlock(_mtxDb)
		return nil, "mysql not connected"
	end
	
	local cursor,errorString = con:execute(   sql      )

	if( errorString ~= nil )
	then
		local i,j = string.find(errorString,"gone away")
		if( i >= 0 )
		then
			conn:close()
			log.print(log.warning,"mysql is gone away,now reconnect......\n");
			conn , errMysql= envMysql:connect(_DBNAME,_DBUSER,_DBPWD,_DBADDR,_DBPORT);

			if( errMysql ~=nil)
			then 
				errorString = errMysql
			else
				cursor,errorString  = conn:execute(   sql      )
			end

		end

	end

	mtx.unlock(_mtxDb)
	return cursor,errorString
end




---启动更新所有设备为离线状态
local sql = [[TRUNCATE TABLE sessions ]]
exesql(conn,sql)










------------------------------------------------------
-----获取客户端设备id,有可能与视频流id不一样
------------------------------------------------------
function GetDevId(ctx)
	return string.upper(stream.getstream(ctx))
---- return stream.getdevid(ctx)
end


------------------------------------------------------
-----视频连接前鉴权
------------------------------------------------------
function OnStreamWillPublish( ctx )
	
return 0 ;
end

------------------------------------------------------
-----视频在线通知
------------------------------------------------------
function OnStreamPublished( ctx )

	local  devid=GetDevId(ctx)

	---更新数据库在线列表
	local sql = string.format("INSERT INTO sessions(nID,sID,sNameSession) VALUES(%d, '%s','%s') ",stream.getcid(ctx),devid, stream.getstream(ctx))

	local cursor,errorString = exesql( conn,   sql      )
	if( errorString ~=nil)
	then 
		log.print(log.error,errorString.."\n"..sql.."\n");
	return -1
	end

	return 0 ;
end

------------------------------------------------------
-----视频离线通知
------------------------------------------------------
function OnStreamPublishClosed( ctx )
	local  devid=GetDevId(ctx)

	
	---更新数据库在线列表
	local sql = string.format("delete  from sessions where nID=%d and sID='%s'", stream.getcid(ctx),devid );

	local cursor,errorString = exesql( conn,   sql      )
	if( errorString ~=nil)
	then 
		log.print(log.error,errorString.."\n"..sql.."\n");
	return -1
	end

	return 0 ;
end

------------------------------------------------------
-----播放前通知
------------------------------------------------------
function OnStreamWillPlay(  ctx )
	return 0 ;
end

------------------------------------------------------
-----播放后通知
------------------------------------------------------
function OnStreamPlayStarted(  ctx )
	return 0 ;
end
------------------------------------------------------
-----播放关闭通知
------------------------------------------------------
function OnStreamPlayClosed( ctx )
	return 0 ;
end













