基于视频压缩的实时监控系统-sprint3采集端传输子系统设计 


由于jpg本来就是编码压缩后的格式，所有无需重复编码
 传输子系统步骤：（1）初始化：a.socket（初始化tcp连接）；b.将事件添加到epoll中
　　　　　　　　 （2）事件处理：接收到网络包、发送完网络包
struct tcp_srv* net_sys_init()
{
    struct tcp_srv* s;
    struct sockaddr_in addr;
    struct sockaddr_in sin;
    struct tcp_cli* c;
    int len;

    s = calloc(1, sizeof(struct tcp_srv));
    c = calloc(1, sizeof(struct tcp_cli));

    //1. 初始化传输子系统(socket初始化)
    //1.1 创建socket
    s->sock = socket(AF_INET, SOCK_STREAM, 0);

    //1.2 初始化地址
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(DEF_TCP_SRV_PORT);

    //1.3 bind地址
    bind(s->sock, (struct sockaddr*)&addr, sizeof(struct sockaddr));

    //1.4 listen
    listen(s->sock, 5);

    //1.5 accept
    c->sock = accept(s->sock, (struct sockaddr*)&sin, &len);
    memcpy(&(c->addr), &sin, len);

    //2. 将传输子系统中的事件加入epoll池
    c->ev_rx = epoll_event_create(c->sock, EPOLLIN, rx_app_handler, c);
    c->ev_tx = epoll_event_create(c->sock, EPOLLOUT, tx_app_handler, c);

    epoll_add_event(srv_main->epfd, c->ev_rx);

    return s;
}
void tx_app_handler(int sock, void* arg)
{   //发送事件
    struct tcp_cli* c = arg;
    send(sock, c->buf, c->len, 0);

    epoll_del_event(srv_main->epfd, c->ev_tx);
    epoll_add_event(srv_main->epfd, c->ev_rx);
}

void rx_app_handler(int sock, void* arg)
{   //接收事件
    unsigned char* pbuf;//保存读取的数据
    struct tcp_cli* c = arg;

    pbuf = &(c->req[0]);
    read(c->sock, pbuf, FRAME_HDR_SZ);
    process_incoming(c);//数据处理子函数
}
int process_incoming(struct tcp_cli* c)
{
    //针对不同的命令做处理
    __u8 id;
    __u8 data[FRAME_DAT_MAX];//数据最大长度
    struct cam* v = srv_main->cam;
    __u8* req = c->req;
    __u8* rsp = c->rsp;
    __u8 status = ERR_SUCCESS;
    __u32  pos, len, size;

    id = req[CMD1_POS];//请求的ID

    switch (id)
    {
    case REQUEST_ID(VID_GET_FMT)://VID_GET_FMT
        //1. 获取图像格式
        cam_get_fmt(v, data);

        //2. 构造返回数据
        build_ack(rsp, (TYPE_SREQ << TYPE_BIT_POS) | SUBS_VID, id, 4, data);

        //3. 发送返回数据
        net_send(c, rsp, FRAME_HDR_SZ + 4);

        break;
    case REQUEST_ID(VID_REQ_FRAME)://VID_REQ_FRAME
        //1. 获取一帧图像
        pos = FRAME_HDR_SZ + 4;
        size = cam_get_trans_frame(v, &rsp[pos]);//获取到图像的长度；

        //2. 构造返回数据，按照要求的格式数发送
        build_ack(rsp, (TYPE_SREQ << TYPE_BIT_POS) | SUBS_VID, id, 4, (__u8*)&size);

        //3. 发送返回数据
        net_send(c, rsp, FRAME_HDR_SZ + 4 + size);
        break;
    default:
        break;
    }
    return status;
}
int build_ack(unsigned char* rsp, unsigned char type, unsigned char id, unsigned char len, unsigned char* data)
{   //cam.c构造返回数据
    rsp[LEN_POS] = len;
    rsp[CMD0_POS] = type;
    rsp[CMD1_POS] = id;
    memcpy(&rsp[DAT_POS], data, len);

    return len + FRAME_HDR_SZ;
}
void net_send(struct tcp_cli* tc, void* buf, int len)
{   // 发送返回数据
    struct tcp_cli* c = (struct tcp_cli*)tc;
    struct tcp_srv* s = c->srv;

    epoll_del_event(s->epfd, c->ev_rx);
    c->buf = buf;
    c->len = len;
    epoll_add_event(srv_main->epfd, c->ev_tx);
}