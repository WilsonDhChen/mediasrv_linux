print(_VERSION);
print("/************************************/");
print("The Lua plugin worked in multithread");
print("Lua 插件工作在多线程环境中请谨慎编程");
print("/************************************/");

print("threadid");
print(ets.threadid());
print("");

print("getappdir");
print(ets.getappdir());
print("");

print("getworkdir");
print(ets.getworkdir());
print("");

print("getinistring");
print(ets.getinistring("mediasrv","callback_plugin"))
print("");

print("getinibool");
print(ets.getinibool("mediasrv","log_debug"))
print("");

print("getostickcount");
print(ets.getostickcount());
print("");
-----------------------------------------------------------------
------- all function return 0 successful , else failed -----------
-----------------------------------------------------------------

function OnThreadExit()

end

function OnThreadInit()

end

function OnStreamWillPublish( ctx )
print("------OnStreamWillPublish------");

print("hold ctx");
ets.ctxaddref(ctx);
print("release ctx");
ets.ctxrelease(ctx);

--- ets.ctxclose(ctx);

print("threadid");
print(ets.threadid());
print("");

print("cid");
print(ets.getcid(ctx));
print("");

print("app");
print(ets.getapp(ctx));
print("");

print("stream");
print(ets.getstream(ctx));
print("");

print("getpeeraddr");
print(ets.getpeeraddr(ctx));
print("");


print("getbegintime");
print(ets.getbegintime(ctx));
print("");


print("getbegintickcount");
print(ets.getbegintickcount(ctx));
print("");


print("isfromcdn");
print(ets.isfromcdn(ctx));
print("");

print("issourcestream");
print(ets.issourcestream(ctx));
print("");

print("geturiquery");
print(ets.geturiquery(ctx));
print("");

print("geturiquery_var");
print(ets.geturiquery_var(ctx,"dd"));
print("");

print("getprotocol");
print(ets.getprotocol(ctx));
print("");

ets.setrecord_flv(ctx,1);
ets.setrecord_mp4(ctx,1);
ets.setrecord_ts(ctx,1);


return 0 ;

end
------------------------------------------------
function OnStreamPublished( ctx )
print("----------OnStreamPublished------------------");


return 0 ;


end
------------------------------------------------
function OnStreamPublishClosed( ctx )
print("----------OnStreamPublishClosed-------------");

print("threadid");
print(ets.threadid());
print("");

print("cid");
print(ets.getcid(ctx));
print("");

print("app");
print(ets.getapp(ctx));
print("");

print("stream");
print(ets.getstream(ctx));
print("");

print("getendtime");
print(ets.getendtime(ctx));
print("");


print("getendtickcount");
print(ets.getendtickcount(ctx));
print("");

local totolSeconds =  (ets.getendtickcount(ctx) -ets.getbegintickcount(ctx) ) /1000 ;

print("total");
print(totolSeconds );
print("");

print("read speed");
print( ets.getreadbytes(ctx)/totolSeconds/1024 );

print("write speed");
print( ets.getwritebytes(ctx)/totolSeconds/1024 );

return 0 ;


end

------------------------------------------------
function OnStreamWillPlay(  ctx )
print("--------------OnStreamWillPlay---------------------");


return 0 ;


end

------------------------------------------------
function OnStreamPlayClosed( ctx )
print("--------------OnStreamPlayClosed--------------");


return 0 ;


end

------------------------------------------------
function OnStreamRecordFinished( ctx,filePath , type )
print("--------------OnStreamRecordFinished--------------");
print(filePath);
print(type);
return 0 ;


end

------------------------------------------------
function OnGetSourceStreamURL( app , stream )
print("--------------OnGetSourceStreamURL--------------");

return "192.168.1.25" ;


end

------------------------------------------------
function OnAuthVerify( ctx )
print("--------------OnAuthVerify--------------");

return 0 ;


end









