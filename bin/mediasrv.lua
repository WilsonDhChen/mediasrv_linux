
print("/************************************/");
print("The Lua plugin worked in multithread");
print("Lua 插件工作在多线程环境中请谨慎编程");
print("/************************************/");

require("socket")

---MySQL测试代码
envMysql = luasql.mysql.mysql();
connMysql , errMysql= envMysql:connect("test1234","root","123456","127.0.0.1",3306);
print(errMysql);

---Odbc测试代码
envOdbc = luasql.odbc.odbc();
connOdbc , errOdbc= envOdbc:connect("test","root","123456");
print(errOdbc);

---sqlite3测试代码
envSqlite3 = luasql.sqlite3.sqlite3();
connSqlite3 , errSqlite3= envSqlite3:connect("test.db");
print(connSqlite3);


print("threadid");
print(app.threadid());
print("");

print("getappdir");
print(app.getappdir());
print("");

print("getworkdir");
print(app.getworkdir());
print("");

print("getinistring");
print(app.getinistring("mediasrv","callback_plugin"))
print("");

print("getinibool");
print(app.getinibool("mediasrv","log_debug"))
print("");

print("getostickcount");
print(app.getostickcount());
print("");


mtxUdp = mtx.create();
mtx.lock(mtxUdp);
mtx.unlock(mtxUdp);
---mtx.destroy(mtxUdp);

udpSoc = rdeu.open("",0);
rdeu.close(udpSoc);

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
stream.ctxaddref(ctx);
print("release ctx");
stream.ctxrelease(ctx);

--- stream.ctxclose(ctx);
--- stream.ctxinvalid(ctx);

print("threadid");
print(app.threadid());
print("");

print("cid");
print(stream.getcid(ctx));
print("");

print("app");
print(stream.getapp(ctx));
print("");

print("stream");
print(stream.getstream(ctx));
print("");

print("getpeeraddr");
print(stream.getpeeraddr(ctx));
print("");


print("getbegintime");
print(stream.getbegintime(ctx));
print("");


print("getbegintickcount");
print(stream.getbegintickcount(ctx));
print("");


print("isfromcdn");
print(stream.isfromcdn(ctx));
print("");

print("issourcestream");
print(stream.issourcestream(ctx));
print("");

print("geturiquery");
print(stream.geturiquery(ctx));
print("");

print("geturiquery_var");
print(stream.geturiquery_var(ctx,"dd"));
print("");

print("getprotocol");
print(stream.getprotocol(ctx));
print("");

stream.setrecord_flv(ctx,1);
stream.setrecord_mp4(ctx,1);
stream.setrecord_ts(ctx,1);


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
print(app.threadid());
print("");

print("cid");
print(stream.getcid(ctx));
print("");

print("app");
print(stream.getapp(ctx));
print("");

print("stream");
print(stream.getstream(ctx));
print("");

print("getendtime");
print(stream.getendtime(ctx));
print("");


print("getendtickcount");
print(stream.getendtickcount(ctx));
print("");

local totolSeconds =  (stream.getendtickcount(ctx) -stream.getbegintickcount(ctx) ) /1000 ;

print("total");
print(totolSeconds );
print("");

print("read speed");
print( stream.getreadbytes(ctx)/totolSeconds/1024 );

print("write speed");
print( stream.getwritebytes(ctx)/totolSeconds/1024 );

return 0 ;


end

------------------------------------------------
function OnStreamWillPlay(  ctx )
print("--------------OnStreamWillPlay---------------------");

print("threadid");
print(app.threadid());
print("");

print("cid");
print(stream.getcid(ctx));
print("");

print("app");
print(stream.getapp(ctx));
print("");

print("stream");
print(stream.getstream(ctx));
print("");

print("getpeeraddr");
print(stream.getpeeraddr(ctx));
print("");


print("getbegintime");
print(stream.getbegintime(ctx));
print("");


print("getbegintickcount");
print(stream.getbegintickcount(ctx));
print("");


print("isfromcdn");
print(stream.isfromcdn(ctx));
print("");

print("issourcestream");
print(stream.issourcestream(ctx));
print("");

print("geturiquery");
print(stream.geturiquery(ctx));
print("");

print("geturiquery_var");
print(stream.geturiquery_var(ctx,"dd"));
print("");

print("getprotocol");
print(stream.getprotocol(ctx));
print("");


return 0 ;


end

------------------------------------------------
function OnStreamPlayClosed( ctx )
print("--------------OnStreamPlayClosed--------------");

print("threadid");
print(app.threadid());
print("");

print("cid");
print(stream.getcid(ctx));
print("");

print("app");
print(stream.getapp(ctx));
print("");

print("stream");
print(stream.getstream(ctx));
print("");

print("getendtime");
print(stream.getendtime(ctx));
print("");


print("getendtickcount");
print(stream.getendtickcount(ctx));
print("");

local totolSeconds =  (stream.getendtickcount(ctx) -stream.getbegintickcount(ctx) ) /1000 ;

print("total");
print(totolSeconds );
print("");

print("read speed");
print( stream.getreadbytes(ctx)/totolSeconds/1024 );

print("write speed");
print( stream.getwritebytes(ctx)/totolSeconds/1024 );


return 0 ;


end
------------------------------------------------
function OnInvalidRequestClosed( ctx )
print("--------------OnInvalidRequestClosed--------------");
print("invalid request from");
print(stream.getpeeraddr(ctx));
print("read");
print(stream.getreadbytes(ctx));
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
function OnStreamHLSRecordFinished( ctx,fileName,fullPath , dur,beginTm,endTm )
print("--------------OnStreamHLSRecordFinished--------------");
print(fileName);
print(fullPath);
print(dur);
print(beginTm);
print(endTm);
print("")
return 0 ;


end
------------------------------------------------
function OnStreamHLSFlush( ctx)
print("--------------OnStreamHLSFlush--------------");
return 0 ;


end
------------------------------------------------
function OnStreamHLSReOpen( ctx)
print("--------------OnStreamHLSReOpen--------------");
return 0 ;


end
------------------------------------------------
function OnGetSourceStreamURL( app , stream )
print("--------------OnGetSourceStreamURL--------------");

return "192.168.1.25" ;


end





