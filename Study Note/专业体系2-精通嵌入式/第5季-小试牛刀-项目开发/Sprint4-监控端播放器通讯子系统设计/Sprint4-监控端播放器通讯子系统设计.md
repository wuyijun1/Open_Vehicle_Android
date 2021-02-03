基于视频压缩的实时监控系统-sprint4监控端播放器通讯子系统设计 

分为两个子系统：
　　1、通讯子系统
　　2、图像显示子系统






struct wcam_cli
{
    int sock;
    pthread_t tid;  //线程id
    bool stop;
    __u8 req[FRAME_MAX_SZ];  //保存发送请求
    __u8 rsp[FRAME_MAX_SZ + VID_FRAME_MAX_SZ];  //接收数据    
}; 

struct wcam_cli* client;
void net_sys_init(struct wcam_win* c)
{
    char* ip;
    int port;

    pthread_t tid;
    client = calloc(1, sizeof(struct wcam_cli));
    client->stop = false;

    //1、网络初始化
    client->sock = tcp_init_net(ip, port);

    //2. 构建工作线程
    pthread_create(&tid, NULL, video_thread, NULL);
}

//网络初始化
int tcp_init_net(char* ip, int port)
{
    int sock;
    struct sockaddr_in addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (-1 == connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr)))
    {
        close(sock);
        return -1;
    }

    return sock;
}
void* video_thread(void* arg)
{
    int len;
    int size;
    __u8 rsp = client->rsp;

    while (!(client->stop))
    {
        //1 发送图像请求
        //1.1 构造图像请求
        len = make_request(client->req, VID_REQ_FRAME, NULL); //请求一帧

        //1.2 发送图像请求
        send(client->sock, client->req, len, 0);

        //2 接收图像
        len = FRAME_HDR_SZ;
        recv(client->sock, rsp, len, MSG_WAITALL);//接收报文的头

        rsp += FRAME_HDR_SZ;
        len = client->rsp[LEN_POS];//数据长度
        recv(client->sock, rsp, len, MSG_WAITALL);//接收dat长度 保存在请求第一位

        memcpy(&size, rsp, len);

        rsp += len;
        recv(client->sock, rsp, size, MSG_WAITALL);//接收实际数据dat

        //3 提交图像给显示子系统
        //draw_video_frame(rsp, size, client->arg);  //图像指针  大小  arg : struct wcam_win *c = arg;
        usleep(10000);//停顿等待接收

    }
}
int make_request(__u8* buf, enum request req, __u8* dat)
{//参数1保存请求、2：请求类型、3：数据位置
    __u32 hdr = req;
    __u8* p = buf;
    __u8 len;

    memcpy(p, &hdr, FRAME_HDR_SZ);//将长度_SZ的请求hdr复制到p

    p += FRAME_HDR_SZ;
    len = REQUEST_LEN(req);//用于判断请求是否附带数据
    if (len > 0 && dat)
    {
        memcpy(p, dat, len);
        return len + FRAME_HDR_SZ;
    }
}