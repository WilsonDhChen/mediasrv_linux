--------------------hls录制到数据库------------------------------------------------------
http = require "socket.http"
mime = require "mime"

_DBNAME="hls"
_DBUSER="root"
_DBPWD="123456"
_DBADDR="127.0.0.1"
_DBPORT=3306

_RECORD_TYPE="nvr"
----_RECORD_DIR="/tmp/hlsrecord/"
_RECORD_DIR="D:/video/record/"
_RECORD_DUR=10
_URLCALLBACK="http://127.0.0.1/hls/callback?"



config.record.settype(_RECORD_TYPE)
config.record.setduration(_RECORD_DUR)
config.record.setformat("ts")
config.record.settspath(_RECORD_DIR)

config.record.settsenable(true)
config.record.setmp4enable(false)
config.record.setflvenable(false)



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
		if( i ~= nil and i >= 0 )
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



function create_hls_table(conn,app,streamid)
	local sql= [[ CREATE TABLE IF NOT EXISTS   hls_]]..app.."_"..streamid..[[
	(
		nID int auto_increment not null primary key ,
		sApp   varchar(120)  NOT NULL default '',
	    	sStream   varchar(120)  NOT NULL default '',
		sFileName varchar(255) NOT NULL default '',
		sTimeBegin   char(24)  NOT NULL default '',
	    	sTimeEnd   char(24)  NOT NULL default '',
	    	fDur double NOT NULL default 0
	) ENGINE=MyISAM  default charset utf8;

	]]

	exesql(conn,sql)
end



function NotifyRecord(msg)
r, e = http.request(_URLCALLBACK..msg)
end

-----------------URL 转义----------------------------------------------------
function encodeURI(s)
    s = string.gsub(s, "([^%w%.%- ])", function(c) return string.format("%%%02X", string.byte(c)) end)
    return string.gsub(s, " ", "+")
end
----------------------------------------------------------------------
function GetStreamFullPath(stream,date)
	local recdir = _RECORD_DIR..stream.."/"
	return app.mkdir(recdir)
end
---------------------------------------------------------------------------
function OnStreamWillPublish( ctx )
local app = stream.getapp(ctx)
local streamid = stream.getstream(ctx)
create_hls_table(conn,app,streamid)
end
---------------------------------------------------------------------------------------------------------
function OnStreamRecordFinished( ctx,fileName,fullPath , dur,beginTm,endTm,type )

local date = os.date("%Y-%m-%d")
local fullpathNew  = GetStreamFullPath(stream.getstream(ctx),date)..fileName 
local app = stream.getapp(ctx)
local streamid = stream.getstream(ctx)
ret,err=os.rename(fullPath,fullpathNew)


	local sql= [[ INSERT INTO   hls_]]..app.."_"..streamid..[[
	(
	sApp,
	sStream,
	sFileName,
	sTimeBegin,
	sTimeEnd,
	fDur)
	VALUES
	(
	]]..
	"'"..app.."',"..
	"'"..streamid.."',"..
	"'"..fileName.."',"..
	"'"..beginTm.."',"..
	"'"..endTm.."',"..
	tostring(dur)..
	[[ ); ]]


	exesql(conn,sql)


local qry = "app="..encodeURI(stream.getapp(ctx)).."&stream="..encodeURI(stream.getstream(ctx)).."&date="..date.."&file="..encodeURI(fileName)
NotifyRecord(qry)

end
----------------------------------------------------------------------------
function OnStreamRecordFlush( ctx)


end
