
http = require "socket.http"
mime = require "mime"

_SLAVES = {"192.168.1.231:8580"}

function Push2Slaves(streamid)

	for i, host in pairs(_SLAVES) do  
		r, e = http.request( "http://"..host.."/pull?stream="..streamid )
	end  
end


function OnStreamPublished( ctx )
	local streamId = stream.getstream(ctx);
	Push2Slaves( streamId ) ;

	return 0 
end