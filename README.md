# ETS Mediasrv

ETS Mediasrv which is an excellent cross-platform Live Streaming Server, developed by f117cdh.

Support Linux Windows MacOSX and ARM-Linux platform.

Supported protocols:   
1)Input protocol, RTMP and ETS    
2)Output protocol, RTMP RTMPT HLS(Memory file support) RTSP HTTP-FLV(support video/audio only) HTTP-TS HTTP-AAC


For commercial support, please contact   f117cdh@qq.com.   QQ qun:144324691

**性能：**  
1） 7x24 全天候稳定运行。   
2）Intel E3 CPU ，10Gb网卡,RTMP稳定输出 8.8Gb/s，HLS稳定输出9Gb/s，稳定并发8K连接   
3）支持视频秒开,出画面速度0.2-0.3s

**关于并发：**  
程序本身没有任何并发限制，理论单进程并发在60K左右，但是实际并发数取决于网卡的速率和CPU的性能以及视频码率。期待大家拿更NB的机器测试，性能强劲的机器需要优化mediasrv.ini发挥多CPU性能(这样的设计可以在不支持fork的windows上支持高并发)


**支持的协议：**  
1）输入协议，RTMP and ETS    
2）输出协议，RTMP RTMPT HLS(Memory file support) RTSP HTTP-FLV(support video/audio only) HTTP-TS(支持混合一路GPS数据) HTTP-AAC


**额外功能：**  
1）支持插件管理推流权限，推流状态可实时更新到数据库或者redis。   
2）流媒体服务器直接支持后台录像,录制格式MP4/TS，也可两种格式同时录制。   
3）录制可以是NVR(会根据设定的时间分割文件)类型录制，也可以是直播就录制，停直播关闭录制 。  

**访问地址示例：**  
HLS http://127.0.0.1:280/chid/playlist.m3u8    
RTMP rtmp://127.0.0.1:1935/live/chid    
RTSP rtsp://127.0.0.1:554/chid     
HTTP-TS http://127.0.0.1:281/chid.ts     
HTTP-FLV http://127.0.0.1:281/chid.flv    
HTTP-AAC http://127.0.0.1:281/chid.aac     

**成功案例**   
已经广泛给各大集成商做OEM，行业涉及教育(IPTV 直播 电台)，武警（执法巡查 定制化产品 arm linux），石化（灾害监控）


**其他版本**   
Windows  https://github.com/WilsonDhChen/mediasrv_windows     
MacOSX   https://github.com/WilsonDhChen/mediasrv_macosx

**关于软件更新**   
Linux 版本我们会实时更新   
Windows 和 MacOSX版本我们会在重大升级更新一次


**欢迎大家提出建议 QQ群:144324691**
