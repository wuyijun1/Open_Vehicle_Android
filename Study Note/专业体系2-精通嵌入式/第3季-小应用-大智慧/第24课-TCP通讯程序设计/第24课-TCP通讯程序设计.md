第三季-第24课-TCP通讯程序设计 

第24课-TCP通讯程序设计
 
 
24.1 TCP编程模型函数化
这里面我们用到的框架就是上一节课的框架。即我们需要有服务器的搭建还要有客户机的搭建。执行的步骤如下：
服务器：创建socket（socket），绑定地址（bind），监听端口（listen），等待连接（accept），发/ 收数据（send/receive），结束连接（close）。
客户机：创建socket（socket），连接服务器（connet），发/收数据（send/receive），结束连接（close）。
以上一共有有8个函数，下面一一介绍：
1. 创建套接字
（1）函数名
socket()
（2）函数原型
int socket(int domain, int type, int protocol);
（3）函数功能
创建一个套接字
（4）包含的头文件
#include<sys/socket.h>
（5）返回值
成功：返回文件（套接字）描述符；
失败：-1
（6）参数说明
domain：确定通讯特性。
AF_INET（IPv4因特网域）；
AF_INET6（IPv6因特网域）；
AF_UNIX（UNIX域）；
AF_UNSPEC（未指定）。
type：确定套接字的类型。
SOCK_DGRAM（长度固定的、无连接的不可靠报文传递）；
SOCK_RAW（IP协议的数据报接口，POSIX.1中为可选）；
SOCK_SEQPACKET（长度固定、有序、可靠的面向连接报文传递）；
SOCK_STREAM（有序、可靠、双向的面向连接字节流）。
protocol：通常是0，表示按照参数domain和type确定通讯协议。
 
2. 绑定地址
（1）函数名
bind()
（2）函数原型
int bind(int sockfd, const struct sockaddr *addr, socklen_t len);
（3）函数功能
降地址绑定到一个套接字。
（4）包含的头文件
#include<sys/socket.h>
（5）返回值
成功：0；
失败：-1
（6）参数说明
sockfd：套接字的fd
addr：绑定的地址
len：绑定的地址长度
我们将结构体列出来：
struct sockaddr
{
sa_family_t   sa_family;
char         sa_data[14];
}
这是一个通用的地址类型（ipv4和ipv6），第一个成员表示协议族的地址类型，第二个成员表示具体的地址的值。但是我们平时用到的地址都是ipv4的，要在上述的结构上进行一定的演化：
struct sockaddr_in
{
short int            sin_family;
unsigned short int    sin_port;
struct in_addr        sin_addr;
unsigned char        sin_zero[8];
}
struct in_addr
{
unsigned long s_addr;
}
为了区分这两种方式的区别，我们用表格表示：
协议族
 
协议族（AF_INET）
 
地址（14字节）
端口号（2字节）
IP地址（4字节）
填充（8字节）
 
注：我们生活中用到的ip地址是类似于192.168.1.1这样的，但是我们我们程序中的ip地址却是整型的，所以需要一定的转换。我们提供下面两个函数:
l  in_addr_t inet_addr(const char *cp)
功能：将字符串形式的IP地址转化为整数型的IP地址（网络字节序）
范例：in_addr.saddr = inet_addr(“192.168.1.1”);
 
l  char *inet_ntoa(struct in_addr)
功能：将整数形式的Ip地址转化为字符串形式的IP地址。
 
3. 设置监听端口
（1）函数名
listen()
（2）函数原型
int listen(int sockfd, int backlog);
（3）函数功能
宣告可以接受连接请求。
（4）包含的头文件
#include<sys/socket.h>
（5）返回值
成功：0；
失败：-1
（6）参数说明
sockfd：套接字的fd
backlog：提供一个提示，用于表示该进程所需要入队的连接请求数量。其实际值由系统决定。
 
4. 等待连接
（1）函数名
accept()
（2）函数原型
int accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict len);
（3）函数功能
一旦服务器调用了listen，套接字就能就收连接请求。使用函数accept获得连接请求并建立连接。没有客户机连接会是服务器发生阻塞。
（4）包含的头文件
#include<sys/socket.h>
（5）返回值
成功：文件（连接字）描述符；一个新的文件描述符，以后操作用的都是它。
失败：-1
（6）参数说明
sockfd：文件（连接字）描述符；
addr：将客户机的地址记录在这里；
len：地址的长度
 
5. 发送数据
（1）函数名
send()
（2）函数原型
ssize_t send(int sockfd, const void *buf, size_t nbytes, int flags);
（3）函数功能
它和write很像，但是可以指定标志来改变处理传输数据的方式。使用send时套接字必须是已经连接的。
（4）包含的头文件
#include<sys/socket.h>
（5）返回值
成功：返回发送的字节数；
失败：-1
（6）参数说明
sockfd：套接字的fd，是使用了accept建立连接后返回的fd；
buf：发送的数据存放的位置
nbytes：发送数据的长度
flags：标志，若是不用设为0；包括如下的符号:
MSG_DONTROUTE：勿将数据路由出本地网络
MSG_DONTWAIT：允许非阻塞操作（等价于O_NONBLOCK）
MSG_EOR：如果协议支持，此为记录结束
MSG_OOB：如果协议支持，发送带外数据
6. 接收数据
（1）函数名
recv()
（2）函数原型
ssize_t recv(int sockfd, void *buf, size_t nbytes, int flags);
（3）函数功能
它和read很像，但是允许指定选项来控制如何接收数据。
（4）包含的头文件
#include<sys/socket.h>
（5）返回值
成功：以字节计数的消息长度，若无可用消息或对方已经按序结束则返回0；
失败：-1
（6）参数说明
sockfd：套接字的fd，是使用了accept建立连接后返回的fd；
buf：接收的数据存放的位置
nbytes：接收数据的长度
flags：标志；
 
7. 关闭连接
close(sockfd)，和文件的关闭函数是一样的。
 
8. 连接服务器
（1）函数名
connect()
（2）函数原型
int connect(int sockfd, const struct sockaddr *addr, socklen_t len);
（3）函数功能
如果处理的是面向连接的网络服务（SOCK_STREAM或SOCK_SEQPACKET），在开始交换数据以前，需要在请求服务的进程套接字（客户端）和提供服务的进程套接字（服务器）之间建立一个连接。这就是connect函数的作用。
（4）包含的头文件
#include<sys/socket.h>
（5）返回值
成功：0；
失败：-1
（6）参数说明、
sockfd：客户机上创建的套接字的fd；
addr：服务器的地址；
len：地址的长度。
 
24.2 网络字节序
大端字节序（big-endian）：不按照内存的增长方向，高位数据存储于低位内存中。
小端字节序（little-endian）：按照内存的增长方向，高位数据存储于高位内存中。
在两个主机A和B之间通讯是，若是两个主机一个是大端通讯一个是小端通讯，就会使得发送的字节顺序颠倒。这就需要网络字节序的使用，它实际就是一个在通讯过程中对主机大小端的一个规定。网络字节序对应的都是大端模式的，发送端若是小端模式的我们就要先将它转化成大端模式。
但是对于接收端，我们对它的存储就要看接收方到底是什么模式的。若是大端模式，就按大端模式存储；若是小端模式，就按小端模式存储。
在linux系统中，提供了四个函数进行网络字节序的转换，如下:
1. uint32_t htonl(uint32_t hostlong);
将32位的数据从主机字节序转换为网络字
in_addr.saddr = htonl(INADDR_AN
2. uint16_t htons(uint16_t hostshort);
将16位的数据从主机字节序转换为网络字节序
3. uint32_t ntohl(uint32_t netlong);
将32位的数据从网络字节序转换为主机字节序
4.  uint16_t ntohs(uint16_t netshort);
将16位的数据从网络字节序转换为主机字节序
 
 
24.3 实例编写
TCP通讯程序设计
tcp_server.c
#include<sys/socket.h>
#include<stdio.h>
#include<string.h>  //字符串头文件
#include<netinet/in.h>  //地址的头文件
#define portnum 3333
int main()
{
 
         int sockfd;
         int new_fd;
         char buffer[128];//定义存储数据的字节
         int nbyte;//接收到的字符串的长度
         int sin_size;
         struct sockaddr_in server_addr;
         struct sockaddr_in client_addr;
         //1.创建套接字
         if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
         {
                  printf("creat socket error!\n");
                  exit(1);
         }
         //2.1 设置要绑定的地址
         bzero(&server_addr, sizeof(struct sockaddr_in));//清零
         server_addr.sin_family = AF_INET;  //网络协议
         server_addr.sin_port = htons(portnum);  //端口，超过两个字节就要转换
         server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//表示任意地址，超过两个字节就要转换
         //2.2 绑定地址
         bind(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr));//表示指针的强制转换
         //3.监听端口
         listen(sockfd, 5);
         while(1)
         {
         //4.等待连接
         sin_size = sizeof(struct sockaddr);
         new_fd = accept(sockfd, (struct sockaddr *)(&client_addr), &sin_size);
         printf("server get connection from %s\n", inet_ntoa(client_addr.sin_addr));//取客户机的ip地址，并且将整型地址转换成字符串地址
         //5.接收数据
    nbyte = recv(new_fd,buffer,128,0);
         buffer[nbyte] = '\0';//字符串的结束符
         printf("server received: %s \n",buffer);
         //6.结束连接
         close(new_fd);
         }
         close(sockfd);
         return 0;
}
 
tcp_client.c
 
#include<sys/socket.h>
#include<stdio.h>
#include<string.h>  //字符串头文件
#include<netinet/in.h>  //地址的头文件
#define portnum 333  //不用分号
int main()
{
         int sockfd;
         char buffer[128];//保存我们输入的数据
         struct sockaddr_in server_addr;
         //1.创建套接字
         if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
         {
                  printf("creat socket error!\n");
                  exit(1);
         }
         //2.1 设置要连接的服务器的地址
         bzero(&server_addr, sizeof(struct sockaddr_in));//清零
         server_addr.sin_family = AF_INET;  //网络协议
         server_addr.sin_port = htons(portnum);  //端口，超过两个字节就要转换
         server_addr.sin_addr.s_addr = inet_addr("192.168.153.129");
         //服务其地址理论应该是通过main函数传进来的，但是这里为了方便将它写死。
         //2.2 连接服务器
         if(connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)
         {
                  printf("connect error!\n");
                  exit(1);
         }
         //3. 发送数据到服务器
         printf("please input char:\n");
         fgets(buffer,128,stdin);
         send(sockfd, buffer, strlen(buffer),0);
         //4. 关键套接字
         close(sockfd);
         return 0;
}
运行：
         用gcc编译好文件后，打开两个相同的终端，在一个中端里面运行./tcp_server，在另一个终端运行./tcp_client。在tcp_client终端输入的字符，在tcp_server能显示。