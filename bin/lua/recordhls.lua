--------------------实现hls按天录制------------------------------------------------------
http = require "socket.http"
mime = require "mime"

_RECORD_TYPE="nvr"
----_RECORD_DIR="/tmp/hlsrecord/"
_RECORD_DIR="D:/video/record/"
_RECORD_DUR=10
_URLCALLBACK="http://127.0.0.1/hls/callback?"

_EXT_ENDLIST_LEN=string.len("#EXT-X-ENDLIST\n")

config.record.settype(_RECORD_TYPE)
config.record.setduration(_RECORD_DUR)
config.record.setformat("ts")
config.record.settspath(_RECORD_DIR)

config.record.settsenable(true)
config.record.setmp4enable(false)
config.record.setflvenable(false)

_mtxCtxs = mtx.create();
_hlsCtxs={}
_hlsduration=_RECORD_DUR



function NotifyRecord(msg)
r, e = http.request(_URLCALLBACK..msg)
end

-----------------判断文件是否存在---------------------------------------------------
function file_exists(path)
local file = io.open(path, "rb")
if file then file:close() end
return file ~= nil
end
-----------------URL 转义----------------------------------------------------
function encodeURI(s)
    s = string.gsub(s, "([^%w%.%- ])", function(c) return string.format("%%%02X", string.byte(c)) end)
    return string.gsub(s, " ", "+")
end
----------------------------------------------------------------------
function GetStreamFullPath(stream,date)
	local recdir = _RECORD_DIR..stream.."/"..date.."/"

	return app.mkdir(recdir)
end
-------------------------------------------------------------------------
function GetM3u8Name(ctx)
local name = stream.getstream(ctx).."_"..stream.geturiquery_var(ctx,"vdoid")..".m3u8"
return name
end
---------------------------------------------------------------------------
function OnStreamWillPublish( ctx )
end
---------------------------------------------------------------------------------------------------------
function OnStreamRecordFinished( ctx,fileName,fullPath , dur,beginTm,endTm,type )

local newFile = false
local dateChanged  = false

mtx.lock(_mtxCtxs)
local ltabel  = _hlsCtxs[app.ptr2string(ctx)]
if(   ltabel ==nil  )
then 
	dateChanged =true
else
	if( ltabel.date ~= os.date("%Y-%m-%d") )
	then
		dateChanged = true 
		ltabel.m3u8file:close()
		_hlsCtxs[app.ptr2string(ctx)] = nil
		ltabel =nil 
	end
end

if(   dateChanged ==true  )
then
	ltabel = {}
	ltabel.date = os.date("%Y-%m-%d")
	ltabel.fullpath = GetStreamFullPath(stream.getstream(ctx),ltabel.date)
	ltabel.m3u8=ltabel.fullpath..GetM3u8Name(ctx)
	local m3u8_exist = file_exists(ltabel.m3u8)

	if( m3u8_exist == true )
	then
		ltabel.m3u8file = io.open(ltabel.m3u8, "r+");		
	else
		ltabel.m3u8file = io.open(ltabel.m3u8, "w");
	end

	if( ltabel.m3u8file == nil )
	then
		mtx.unlock(_mtxCtxs)
		log.print(log.error,string.format("can't create m3u8 file <%s>\n",ltabel.m3u8))
		return 0
	end
	if  m3u8_exist == false then
		ltabel.m3u8file:write("#EXTM3U\n#EXT-X-VERSION:3\n");
		ltabel.m3u8file:write("#EXT-X-TARGETDURATION:")
		ltabel.m3u8file:write(tostring(_hlsduration))
		ltabel.m3u8file:write("\n")
		ltabel.m3u8file:flush()
		newFile = true
	else
		ltabel.m3u8file:seek("end",0)
	end

	_hlsCtxs[app.ptr2string(ctx)]=ltabel
end


mtx.unlock(_mtxCtxs)

local dstFile = ltabel.fullpath..fileName 
ret,err=os.rename(fullPath,dstFile)

local qry = "app="..encodeURI(stream.getapp(ctx)).."&stream="..encodeURI(stream.getstream(ctx)).."&date="..ltabel.date.."&file="..encodeURI(fileName)
NotifyRecord(qry)

local EXTINF = string.format("#EXTINF:%g,\n%s\n#EXT-X-ENDLIST\n",dur,fileName)
if newFile == true then
	ltabel.m3u8file:write(EXTINF)
	ltabel.m3u8file:flush()
else
	local seek = ltabel.m3u8file:seek("end",0-_EXT_ENDLIST_LEN)
	ltabel.m3u8file:write(EXTINF)
	ltabel.m3u8file:flush()
end


return 0 ;


end
----------------------------------------------------------------------------
function OnStreamRecordFlush( ctx)

mtx.lock(_mtxCtxs)
local ltabel  = _hlsCtxs[app.ptr2string(ctx)]
if(  ltabel ~=nil )
then 
-----	ltabel.m3u8file:write("#EXT-X-ENDLIST\n")
	ltabel.m3u8file:close()
	_hlsCtxs[app.ptr2string(ctx)] = nil
end
mtx.unlock(_mtxCtxs)

end
