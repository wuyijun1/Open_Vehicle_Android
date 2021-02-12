第三季-第25课-UDP通讯程序设计 

第25课-UDP通讯程序设计
 
25.1 UDP编程模型函数化
UDP编程同样是有服务器和客户机的。在服务器中，用到的函数分别是：socket，bind，recvfrom，sendto，close。在客户机上的函数分别是：socket，sendto，recvfrom，close。针对不同的函数，我们只要学习新的函数sendto和recvfrom就好。
 
1. 发送数据
（1） 函数名
sendto
（2） 函数原型
ssize_t sendto(int sockfd, const void *buf, size_t nbytes, int flags, const struct sockaddr *destaddr, socklen_t destlen);
（3） 函数功能
函数sendto和send很类似。区别在于sendto允许在无连接的套接字上指定一个目标地址。
（4） 所包含的头文件
#include<sys/socket.h>
（5） 返回值
成功：返回发送给的字节数
失败：-1
（6） 参数说明
sockfd：创建套接字的描述符
buf：发送数据所存的地址
bytes：要发送的数据量
flags：发送的标识符
destaddr：数据要发送到的目的的地址
destlen：地址长度
 
2. 接收数据
（1）函数名
recvfrom
（2）函数原型
ssize_t recvfrom(int sockfd, void *restrict buf, size_t len, int flags, struct sockaddr *restrict addr, socklen_t *restrict addrlen);
（3）函数功能
可以得到数据发送者的源地址。
（7） 所包含的头文件
#include<sys/socket.h>
（8） 返回值
成功：已字节计数的消息长度，若无可用消息或对方已经按序结束则返回0
失败：-1
（9） 参数说明
sockfd：创建套接字的描述符
buf：接收到的数据所存的位置
len：接收数据的长度
flags：接收的标志
addr：发送方的地址，读取
addrlen：地址长度的指针
 
25.2 实例编写       
UDP通讯程序设计
udp_server.c
#include<sys/socket.h>
#include<stdio.h>
#include<string.h>  //字符串头文件
#include<netinet/in.h>  //地址的头文件
#define PORT 8888
#define MSG_SIZE 128
int main()
{
         int sockfd;
         char buffer[128];//定义存储数据的字节
         struct sockaddr_in server_addr;
         struct sockaddr_in client_addr;
         int addrlen;
         int n;
         //创建套件字
         sockfd = socket(AF_INET, SOCK_DGRAM, 0);
         //设置要绑定的地址
         bzero(&server_addr, sizeof(struct sockaddr_in));//清零
         server_addr.sin_family = AF_INET;  //网络协议
         server_addr.sin_port = htons(PORT);  //端口，超过两个字节就要转换
         server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//表示任意地址，超过两个字节就要转换
         //绑定地址
         bind(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr));//表示指针的强制转换
         //接收数据
         while(1)
         {
                  addrlen = sizeof(struct sockaddr);
                  bzero(buffer, sizeof(buffer));//清零
                  n = recvfrom(sockfd, buffer, MSG_SIZE, 0, (struct sockaddr *)(&client_addr), &addrlen);
                  buffer[n] = ‘\0’;
                  printf("server received: %s\n",buffer);
         }
         //结束连接
         close(sockfd);
         return 0;
}
 
udp_client.c
#include<sys/socket.h>
#include<stdio.h>
#include<string.h>  //字符串头文件
#include<netinet/in.h>  //地址的头文件
#define PORT 8888
#define BUF_SIZE 128
int main(int argc, char **argv)//区分TCP，这节将地址外传进来，也可以在内部完成固定地址
{
         int sockfd;
         char buffer[BUF_SIZE];//定义存储数据的字节
         struct sockaddr_in server_addr;
         if(argc !=2)//我们希望的输入是，程序名+IP地址，如果不对的话，输入提示信息
         {
                  printf("usage: %s server_ip\n ",argv[0]);
                  exit(1);
         }
         //创建套件字
         sockfd = socket(AF_INET, SOCK_DGRAM, 0);//和服务器的定义一样
         //初始化服务器地址
         bzero(&server_addr, sizeof(struct sockaddr_in));//清零
         server_addr.sin_family = AF_INET;  //网络协议
         server_addr.sin_port = htons(PORT);  //端口，超过两个字节就要转换
inet_aton(argv[1],server_addr.sin_addr);                /*将argv[1]的内容转换后传给server_addr.sin_addr并且直接转换成网络字节序*/
         //发送数据
         while(1)
         {
                  printf("please input char!\n");
                  fgets(buffer, BUF_SIZE,stdin);
                  sendto(sockfd, buffer, strlen(buffer),0, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr));
                  bzero(buffer, BUF_SIZE);//清零
         }
         //结束连接
         close(sockfd);
         return 0;
}
运行：
在复制的终端中分别运行两个编译好的文件。注意格式分别是：./udp_server和./udp_client 192.168.153.128（网络地址根据实际的情况而定）。在udp_client终端输入字符，在udp_server终端可以打印传输过来的字符。