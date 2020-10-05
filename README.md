
## 简介

```shell
          _____                    _____                    _____          
         /\    \                  /\    \                  /\    \         
        /::\    \                /::\    \                /::\____\        
        \:::\    \              /::::\    \              /:::/    /        
         \:::\    \            /::::::\    \            /:::/   _/___      
          \:::\    \          /:::/\:::\    \          /:::/   /\    \     
           \:::\    \        /:::/  \:::\    \        /:::/   /::\____\    
           /::::\    \      /:::/    \:::\    \      /:::/   /:::/    /    
          /::::::\    \    /:::/    / \:::\    \    /:::/   /:::/   _/___  
         /:::/\:::\    \  /:::/    /   \:::\    \  /:::/___/:::/   /\    \ 
        /:::/  \:::\____\/:::/____/     \:::\____\|:::|   /:::/   /::\____\
       /:::/    \::/    /\:::\    \      \::/    /|:::|__/:::/   /:::/    /
      /:::/    / \/____/  \:::\    \      \/____/  \:::\/:::/   /:::/    / 
     /:::/    /            \:::\    \               \::::::/   /:::/    /  
    /:::/    /              \:::\    \               \::::/___/:::/    /   
    \::/    /                \:::\    \               \:::\__/:::/    /    
     \/____/                  \:::\    \               \::::::::/    /     
                               \:::\    \               \::::::/    /      
                                \:::\____\               \::::/    /       
                                 \::/    /                \::/____/        
                                  \/____/                  ~~              
                                                                           
```


## 开源支持
- [tortellini](https://github.com/Qix-/tortellini)

## 说明

定奇蛙(TingChiWa)分布服务框架，不依赖其他库的轻服务框架。
基于 epoll 反应器，由框架负责底层通信，使用者只需要关注业务。
提供面向连接的服务通信。
使用者通过配置文件配置自己的服务，框架提供基于配置的服务注册、发现。
为使用者提供类似于 Windows 消息处理的回调接口，用户在此回调中编写自己的业务。
框架不提供序列化/反序列化，使用者可以自己灵活定义。
为所有的服务提供守护，包括进程的优雅退出和僵尸进程的清理。

[更多说明](doc/功能设计详述.md)

## 环境
- 项目管理工具支持
    + [x] cmake
- 最小编译配置支持
    + [x] GNU gcc/g++ 4.8.4
    + [x] cmake 2.8.0

## 使用说明
- 使用者只需要添加配置、根据回调接口定义自己的服务、编写自己的 main 函数即可。可参考测试文件。

## 测试
[测试说明](test/)

## 其他
[备忘](doc/备忘.md)

# TODO

## 近期功能
- setpid
- 编写测试

## 远期效率
- 增加可选异步log
- 守护进程采用多线程，添加自旋锁
- 环形队列
- 内存池


----
*穿越之海，跳下去就穿越的那种*
<h1 align="center">
    <img width="700" height="988" src="doc/chuanyuezhihai.jpg">
</h1>
