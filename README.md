 
# ETS Mediasrv    

ETS Mediasrv which is an excellent cross-platform Live Streaming Server, developed by f117cdh.

Support Linux Windows MacOSX and ARM-Linux platform.

Supported protocols:   
1)Input protocol, RTMP and ETS    
2)Output protocol, RTMP RTMPT HLS(Memory file support) RTSP HTTP-FLV(support video/audio only) HTTP-TS HTTP-AAC


For commercial support, please contact   f117cdh@qq.com.   QQ qun:144324691

**技术参数：**  
1）支持master/slave,自动请求master  
2）支持负载均衡/集群      
3）支持CDN部署(最大2W节点小型CDN)，部署简单      
4）支持3台控制服务器热备，任意两台出问题不影响服务      


**关于并发：**  
程序总并发没有任何限制，单路视频并发设计最大是15K，但是实际并发数取决于网卡的速率和CPU的性能以及视频码率。期待大家拿更NB的机器测试，性能强劲的机器需要优化mediasrv.ini发挥多CPU性能(这样的设计可以在不支持fork的windows上支持高并发)


**支持的协议：**  
1）输入协议，RTMP and ETS    
2）输出协议，RTMP  HLS(Memory file support)

**支持lua(5.3)：**  
1）lua脚本在mediasrv.lua中，里面对所有的API函数做了demo,请参考调用示例   
2）支持推送前鉴权  ,设置是否录制 TS/FLV/MP4    
3）支持推送成功通知   
4）支持推送关闭通知   
5）支持播放前鉴权  
6）支持播放关闭后通知   
7）支持录制结束通知    
8）支持CDN源站地址获取   
9）支持会话流量获取   



**RTMP 推送地址：**  
RTMP rtmp://127.0.0.1/live/chid   

**访问地址示例：**  

live是RTMP推送指定的App   
HLS http://127.0.0.1:280/live/chid/playlist.m3u8    
RTMP rtmp://127.0.0.1:1935/live/chid    

    

**运行**   
给权限    
chmod 777 ./mediasrv   
chmod 777 ./mediasrv_sh   
直接运行    
./mediasrv_sh   


**成功案例**   
已经广泛给各大集成商做OEM，行业涉及教育(IPTV 直播 电台)，武警（执法巡查 定制化产品 arm linux），石化（灾害监控）


**其他版本**   
Windows  https://github.com/WilsonDhChen/mediasrv_windows     
MacOSX   https://github.com/WilsonDhChen/mediasrv_macosx

**IPTV 监控行业组件**   
https://github.com/WilsonDhChen/transcodesrv_linux      

**关于软件更新**   
Linux 版本我们会实时更新   
Windows 和 MacOSX版本我们会在重大升级更新一次


**欢迎大家提出建议 QQ群:144324691**
