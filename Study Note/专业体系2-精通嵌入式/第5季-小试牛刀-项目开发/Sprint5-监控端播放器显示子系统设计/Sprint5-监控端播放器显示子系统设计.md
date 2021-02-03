Sprint5-监控端播放器显示子系统设计(未完成)
第6课-监控端播放器显示子系统设计
大纲：
GTK图形程序设计
GTK简介
GTK实例分析
移植GTK图形程序
1. GTK图形程序设计
在嵌入式Linux下有很多图形界面系统GUI包括Qt、GTK+、miniGUI等。其中，最优秀的应该是QT，但是GTK+去世最适合新手快速上手的，原因很简单：
GTK+采用C语言，而QT采用C++编程。其中QT是最优秀的图形系统，但是GTK+是相对简单上手的。
我们将纯图形化界面文件（有三个：wacam_win.c，Makefile，子文件夹icons用于存放图形）导进我们的linux系统，输入make编译一下。
 
生成了我们要的wcamclient执行文件，./wcamclient运行程序，出现我们要的连接界面，如下：
 
连接好网址后，显示如下的界面：
 
这里由于我们没有连接，所以显示的是一个纯图形的界面。下面我们看一下，我们的程序，wacam_win.c：
/**gtk编程参考
http://blog.csdn.net/lianghe_work/article/details/47087109
http://blog.csdn.net/exbob/article/details/6932864
http://blog.csdn.net/fykhlp/article/details/5985131
*/
 
#include <gtk/gtk.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
 
 
#define DEF_CONN_IP   "192.168.1.230"
#define DEF_PORT     "19868"
#define LOGO_IMG          "./icons/webcam_icon.png"
#define LOGO_IMG1         "./icons/wcamclient.png"
 
#define WIN_TITLE             "Web Camera"
#define WIN_ICON         "./icons/icon.png"
 
#define SNAP_BUTTON_IMG       "./icons/snap-icon-small.xpm"
#define SETTING_BUTTON_IMG "./icons/settings.png"
 
#define WCAM_VERSION     "Web Camera 2.0"
#define WCAM_HOMEPAGE     "http://www.enjoylinux.cn"
#define SNAPSHOT_PATH       IMGDIR
 
    #define FRAME_MAX_SZ    253
    #define VID_FRAME_MAX_SZ    (0xFFFFF - FRAME_MAX_SZ)
 
struct entry_win {
    GtkWidget   *win;
    GtkWidget   *ip_entry;
    GtkWidget   *port_entry;
    GtkWidget   *connect_button;
    gboolean    connected;
    int         sock;
};
 
typedef struct entry_win *entry_win_t;
 
typedef void (*wcc_img_proc_t)(const void *p, int size, void *arg);
 
struct wcam_cli {
    int                     sock;           /* 涓庢湇鍔″櫒閫氫俊鐨勫缁撳瓧 */
    pthread_t               tid;            /* 澶勭悊绾跨▼ */     
    bool                    need_stop;
   
    __u8                    req[FRAME_MAX_SZ];
    __u8                    rsp[FRAME_MAX_SZ + VID_FRAME_MAX_SZ];
 
    wcc_img_proc_t          proc;           /* 鍥惧儚澶勭悊鍑芥暟 */
    void                    *arg;           /* 鍥惧儚澶勭悊鍑芥暟浼犲叆鍙傛暟 */
};
 
typedef struct wcam_cli *wcc_t;
 
 
struct wcam_win {
    GtkWidget           *win;
    wcc_t               client;
    entry_win_t         entry_win;
 
    GtkWidget           *video_area;
 
    guint32             video_format;
    guint32             video_width;
    guint32             video_height;
    gboolean            video_fullscreen;
    gboolean            need_snapshot;
 
    //struct wcam_uctls   *uctls;  
 
    gchar               ipaddr[24];     /* format: "ip :port" */
 
    GtkWidget           *fps_label;
    GtkWidget           *frmsize_label;
    guint32             frm_cnt;
    guint64             last_twenty_frm_us;
 
    GtkWidget           *info_area;
    GtkWidget           *button_area;
    GtkWidget           *control_area_button;
    GtkWidget           *control_area;
 
};
 
__u64 clock_get_time_us()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (__u64)ts.tv_sec * 1000000LL + (__u64)ts.tv_nsec / 1000LL;
}
 
void draw_video_frame(const void *p, int size, void *arg)
{
    struct wcam_win *c      = arg;
    GdkPixbufLoader *pixbuf_loader;
    GdkPixbuf       *pixbuf;
    GdkPixbuf       *pixbuf4fullscreen = NULL;
    GdkScreen       *screen;
    cairo_t         *cr;
    gchar           outstr[100];
    gfloat          fps;
    guint64         now_us;
 
    gdk_threads_enter();
   
    /* update frame size label */
    sprintf(outstr, "%d 瀛楄妭", size);
    gtk_label_set_text(GTK_LABEL(c->frmsize_label), outstr);
 
    /* update fps label */
    c->frm_cnt++;
    if (c->frm_cnt == 1) {
        c->last_twenty_frm_us = clock_get_time_us();
    } else if (c->frm_cnt > 10) {
        now_us = clock_get_time_us();
        fps = 10.0 * 1000000.0 / (now_us - c->last_twenty_frm_us);
        c->frm_cnt = 0;
        sprintf(outstr, "%2.1f 甯?绉?, fps);
        gtk_label_set_text(GTK_LABEL(c->fps_label), outstr);
    }
 
    /* update video */
    pixbuf_loader = gdk_pixbuf_loader_new();       
    gdk_pixbuf_loader_write(pixbuf_loader, p, size, NULL);
    gdk_pixbuf_loader_close(pixbuf_loader, NULL);
    pixbuf = gdk_pixbuf_loader_get_pixbuf(pixbuf_loader);
 
    /*创建cairo*/
    cr = gdk_cairo_create(c->video_area->window);
   
    /*导入图像*/
    gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);
   
    /*绘制图像*/
    cairo_paint(cr);
    cairo_destroy(cr);
 
    g_object_unref(pixbuf_loader);
    if (pixbuf4fullscreen)
        g_object_unref(pixbuf4fullscreen);
       
    gdk_threads_leave();
 
    if (c->need_snapshot) {     /* Maybe we should open a file_save_dialog here */
        char img_path[100];     /* or maybe we should make the image_storage_path Configurable */
        FILE *fp;              
        time_t t;
        struct tm *tmp;
        t = time(NULL);
        tmp = localtime(&t);
 
        strftime(outstr, sizeof(outstr), "%F-%T", tmp);
        printf(SNAPSHOT_PATH"/webcam-%s.jpg\n", outstr);
        printf(SNAPSHOT_PATH"/webcam-%s.jpg", outstr);
        sprintf(img_path, SNAPSHOT_PATH"/webcam-%s.jpg", outstr);
        fp = fopen(img_path, "w");       
        fwrite(p, size, 1, fp);
        fclose(fp);
        c->need_snapshot = FALSE;
    }
}
 
static void connect_handler(GtkButton *button, gpointer data)
{
    struct entry_win *c = data;
    const gchar      *ip;
    gint16           port;
   
    /*读取IP和端口*/
    ip   = gtk_entry_get_text(GTK_ENTRY(c->ip_entry));
    port = atoi(gtk_entry_get_text(GTK_ENTRY(c->port_entry)));
   
        c->connected = TRUE;
        gtk_main_quit();  
}
 
static gboolean logo_draw(struct entry_win *c, GtkWidget* box)
{
    gtk_box_pack_start_defaults(GTK_BOX(box), gtk_image_new_from_file(LOGO_IMG));
    gtk_box_pack_start_defaults(GTK_BOX(box), gtk_image_new_from_file(LOGO_IMG1));
    return TRUE;
}
 
static gboolean entry_area_draw(struct entry_win *c, GtkWidget* box)
{
    GtkWidget *label;
    GtkWidget *entry;
 
        /*创建标签*/
    label = gtk_label_new("Server Ip:");
    gtk_box_pack_start_defaults(GTK_BOX(box), label);
   
    /*创建文本输入框*/
    entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry), DEF_CONN_IP);
    gtk_entry_set_max_length(GTK_ENTRY(entry), 15);
 
    gtk_box_pack_start_defaults(GTK_BOX(box), entry);
        c->ip_entry = entry; 
   
        /*创建标签*/
    label = gtk_label_new("Server Port:");
        gtk_box_pack_start_defaults(GTK_BOX(box), label);
       
        /*创建文本输入框*/
    entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry), DEF_PORT);
    gtk_entry_set_max_length(GTK_ENTRY(entry), 5);
 
    gtk_box_pack_start_defaults(GTK_BOX(box), entry);  
    c->port_entry = entry;
 
    return TRUE;
}
 
static gboolean button_area_draw(struct entry_win *c, GtkWidget* box)
{
    GtkWidget *button;
   
    /* 创建带文字的按钮 */
    button = gtk_button_new_with_label("OK"); 
    gtk_box_pack_start_defaults(GTK_BOX(box), button);
    c->connect_button = button;
   
    /*关联按钮点击事件*/
    g_signal_connect(button, "clicked", G_CALLBACK(connect_handler), c);     
 
    /* 创建带文字的按钮 */
    button = gtk_button_new_with_label("Cancel"); 
    gtk_box_pack_start_defaults(GTK_BOX(box), button);
   
    g_signal_connect(button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
 
    return TRUE;
}
 
 
static gboolean entry_win_draw_face(struct entry_win *c)
{
    GtkWidget *vbox;
    GtkWidget *hbox;
 
    /*创建垂直布局容器*/
    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(c->win), vbox);         
   
    /*创建水平布局容器*/
    hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox);
   
    /*绘制logo*/
    logo_draw(c, hbox);
 
    /*输入ip和端口*/
    hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox);
    entry_area_draw(c, hbox);
 
    /*创建水平按键布局容器*/
    hbox = gtk_hbutton_box_new();
    /*设置按钮间歇*/
    gtk_box_set_spacing(GTK_BOX(hbox), 5);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(hbox), GTK_BUTTONBOX_END);  
    gtk_box_pack_start_defaults(GTK_BOX(vbox), hbox);
    button_area_draw(c, hbox);
 
    return TRUE;
}
 
static gboolean
key_press_func(GtkWidget* widget, GdkEventKey* key, gpointer data)
{
#define KEY_ENTER 0xff0d
#define KEY_ESC   0xff1b
    struct entry_win *c = data;
    if (KEY_ENTER == key->keyval) {
        connect_handler(GTK_BUTTON(c->connect_button), c);
        return TRUE;
    } else if (KEY_ESC == key->keyval) {
        c->connected = FALSE;
        gtk_main_quit();
        return TRUE;
    }
    return FALSE;
#undef  KEY_ENTER
#undef  KEY_ESC
}
 
entry_win_t login_create()
{
    struct entry_win *c = calloc(1, sizeof(struct entry_win));
    if (!c) {
        perror("entry_win_create");
        return NULL;
    }
 
    /*---创建新窗口---*/
    c->win = gtk_window_new(GTK_WINDOW_TOPLEVEL);    
 
    /*---设置窗口标题---*/
    gtk_window_set_title(GTK_WINDOW(c->win), WIN_TITLE);
   
    /*---设置窗口图标---*/             
    gtk_window_set_icon(GTK_WINDOW(c->win), gdk_pixbuf_new_from_file(WIN_ICON, NULL));
   
    /*---设置窗口位置---*/ 
    gtk_window_set_position(GTK_WINDOW(c->win), GTK_WIN_POS_CENTER);
   
    /*---设置窗口是否可伸缩---*/
    gtk_window_set_resizable(GTK_WINDOW(c->win), FALSE);   
   
    /*---设置窗口边框宽度---*/
    gtk_container_set_border_width(GTK_CONTAINER(c->win), 0);
   
    /*---关联退出信号与处理函数---*/
    g_signal_connect(GTK_OBJECT(c->win), "destroy", G_CALLBACK(gtk_main_quit), NULL);
   
    /*---关联按键信号与处理函数---*/
    g_signal_connect(G_OBJECT(c->win), "key_press_event", G_CALLBACK(key_press_func), c);
   
    entry_win_draw_face(c);
   
    gtk_widget_show_all(c->win);
    return c;
}
 
int login_run(entry_win_t win)
{
    struct entry_win *c = win;
    /*启动该窗口*/
    gtk_main();
    return c->connected == TRUE ? 0 : -1;
}
 
void login_hide(entry_win_t win)
{
    struct entry_win *c = win;
    gtk_widget_hide_all(c->win);
}
 
 
 
/*------------主窗口-------------------*/
 
int entry_win_get_ip(entry_win_t win, char *ip)
{
    struct entry_win *c = win;
    if (c->connected == FALSE)
        return -1;
    strcpy(ip, gtk_entry_get_text(GTK_ENTRY(c->ip_entry)));
    return 0;
}
 
int entry_win_get_port(entry_win_t win, char *port)
{
    struct entry_win *c = win;
    if (c->connected == FALSE)
        return -1;
    strcpy(port, gtk_entry_get_text(GTK_ENTRY(c->port_entry)));
    return 0;
}
 
void main_quit(GtkWidget *Object, gpointer data)
{
    gtk_main_quit();
}
 
static gboolean
draw_area_draw(struct wcam_win *c, GtkWidget *box)
{
    /*创建图像显示区域*/
    c->video_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(c->video_area, c->video_width, c->video_height);
    gtk_box_pack_start(GTK_BOX(box), c->video_area, FALSE, FALSE, 0);
   
    gtk_widget_add_events(c->video_area, GDK_BUTTON_PRESS_MASK);
 
    return TRUE;
}
 
static gboolean
info_area_draw(struct wcam_win *c, GtkWidget *box)
{
    GtkWidget   *frame;
    GtkWidget   *table;
    GtkWidget   *label;
    GtkWidget   *align;
    GtkWidget   *separator;
    gchar       buf[256];
 
    frame = gtk_frame_new("淇℃伅鍖?);
    c->info_area = frame;
    gtk_box_pack_start_defaults(GTK_BOX(box), frame);
 
    table = gtk_table_new(9, 2, FALSE);
    gtk_container_add(GTK_CONTAINER(frame), table);
   
    /* VERSION & HOMEPAGE */
    label = gtk_label_new("涓婚〉:");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,
                     GTK_FILL, GTK_SHRINK, 1, 1);
 
    label = gtk_link_button_new_with_label(WCAM_HOMEPAGE, "www.enjoylinux.cn");
    align = gtk_alignment_new(0, 0, 0, 0);              /* 宸﹀榻?*/
    gtk_container_add(GTK_CONTAINER(align), label);
    gtk_table_attach(GTK_TABLE(table), align, 1, 2, 0, 1,
                     GTK_FILL, GTK_SHRINK, 1, 1);
 
    label = gtk_label_new("鐗堟湰:");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2,
                     GTK_FILL, GTK_SHRINK, 1, 1);
 
    label = gtk_label_new(WCAM_VERSION);
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_table_attach(GTK_TABLE(table), label, 1, 2, 1, 2,
                     GTK_FILL, GTK_SHRINK, 8, 1);
 
    /* IP & PORT */
    label = gtk_label_new("鏈嶅姟鍣?");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 2, 3,
                     GTK_FILL, GTK_SHRINK, 1, 1);
 
    label = gtk_label_new(c->ipaddr);
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_table_attach(GTK_TABLE(table), label, 1, 2, 2, 3,
                     GTK_FILL, GTK_SHRINK, 8, 1);
 
    separator = gtk_hseparator_new();
    gtk_table_attach(GTK_TABLE(table), separator, 0, 1, 3, 4,
                     GTK_FILL, GTK_SHRINK, 1, 1);
    separator = gtk_hseparator_new();
    gtk_table_attach(GTK_TABLE(table), separator, 1, 2, 3, 4,
                     GTK_FILL | GTK_EXPAND, GTK_SHRINK, 1, 1);
 
    /* Frame Format */
    label = gtk_label_new("甯ф牸寮?");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 4, 5,
                     GTK_FILL, GTK_SHRINK, 1, 1);
 
    sprintf(buf, "%c%c%c%c", (c->video_format)&0xFF,
                             (c->video_format>>8)&0xFF,
                             (c->video_format>>16)&0xFF,
                             (c->video_format>>24)&0xFF);
    label = gtk_label_new(buf);
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_table_attach(GTK_TABLE(table), label, 1, 2, 4, 5,
                     GTK_FILL, GTK_SHRINK, 8, 1);
 
    /* Frame width x height */
    label = gtk_label_new("甯у昂瀵?");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 5, 6,
                     GTK_FILL, GTK_SHRINK, 1, 1);
 
    sprintf(buf, "%d x %d", c->video_width, c->video_height);
    label = gtk_label_new(buf);
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_table_attach(GTK_TABLE(table), label, 1, 2, 5, 6,
                     GTK_FILL, GTK_SHRINK, 8, 1);
 
    /* Frame Size */
    label = gtk_label_new("甯уぇ灏?");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 6, 7,
                     GTK_FILL, GTK_SHRINK, 1, 1);
 
    label = gtk_label_new("0");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_table_attach(GTK_TABLE(table), label, 1, 2, 6, 7,
                     GTK_FILL, GTK_SHRINK, 8, 1);
    c->frmsize_label = label;
 
    /* FPS */
    label = gtk_label_new("甯ч€熺巼:");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 7, 8,
                     GTK_FILL, GTK_SHRINK, 1, 1);
 
    label = gtk_label_new("0");
    gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
    gtk_table_attach(GTK_TABLE(table), label, 1, 2, 7, 8,
                     GTK_FILL, GTK_SHRINK, 8, 1);
    c->fps_label = label;
 
    /* hseparator */
    separator = gtk_hseparator_new();
    gtk_table_attach(GTK_TABLE(table), separator, 0, 1, 8, 9,
                     GTK_FILL, GTK_SHRINK, 1, 1);
    separator = gtk_hseparator_new();
    gtk_table_attach(GTK_TABLE(table), separator, 1, 2, 8, 9,
                     GTK_FILL, GTK_SHRINK, 1, 1);
 
    return TRUE;
}
 
static gboolean main_button_area_draw(struct wcam_win *c, GtkWidget *box)
{
    GtkWidget *buttonbox;    
    GtkWidget *button;      
    GtkWidget *hbox;    
    GtkWidget *label;
    GtkWidget *image;
 
    buttonbox = gtk_hbutton_box_new();
    gtk_box_pack_start(GTK_BOX(box), buttonbox, FALSE, FALSE, 0);
    c->button_area = buttonbox;
    /* control button */
    image = gtk_image_new_from_file(SETTING_BUTTON_IMG);
    label = gtk_label_new("鏄剧ず鎺у埗椤?");
    hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start_defaults(GTK_BOX(hbox), image);
    gtk_box_pack_start_defaults(GTK_BOX(hbox), label);
 
    button = gtk_check_button_new();  
    gtk_container_add(GTK_CONTAINER(button), hbox);
    c->control_area_button = button;
   
    gtk_box_pack_start_defaults(GTK_BOX(buttonbox), button);
   
    /* snapshot button */
    image = gtk_image_new_from_file(SNAP_BUTTON_IMG);
    label = gtk_label_new("蹇収");
    hbox  = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start_defaults(GTK_BOX(hbox), image);
    gtk_box_pack_start_defaults(GTK_BOX(hbox), label);
 
    button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button), hbox);
   
    gtk_box_pack_start_defaults(GTK_BOX(buttonbox), button);
 
 
    return TRUE;
}
 
static gboolean main_win_draw_face(struct wcam_win *c)
{
    GtkWidget *box;
    GtkWidget *hbox;
    GtkWidget *hseparator;
 
    box = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(c->win), box);         
 
    /* draw_area & info_area */
    hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, TRUE, 0);
    draw_area_draw(c, hbox);
    info_area_draw(c, hbox); 
 
    /* hseparator */
    hseparator=gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(box), hseparator, FALSE, TRUE, 0);
   
    /* button_area */
    main_button_area_draw(c, box);
 
    /* control_area */
   // control_area_draw(c, box);
 
    return TRUE;
}
 
static gboolean main_create(struct wcam_win *c)
{
    int len;
    /*------------初始化----------------*/
    c->video_width = 640;
    c->video_height = 480;
 
    /* fullscreen */
    c->video_fullscreen = FALSE;
   
    /* ip & port */
    entry_win_get_ip(c->entry_win, c->ipaddr);
 
    len = strlen(c->ipaddr);
 
    c->ipaddr[len] = ':';
    entry_win_get_port(c->entry_win, &c->ipaddr[len+1]);
   
   
    c->win = gtk_window_new(GTK_WINDOW_TOPLEVEL); 
   
    /* 璁剧疆绐楀彛鏍囬 */
    gtk_window_set_title(GTK_WINDOW(c->win), WIN_TITLE);               
 
    gtk_window_set_icon(GTK_WINDOW(c->win),
                        gdk_pixbuf_new_from_file(WIN_ICON, NULL));
   
    /* 绐楀彛杈规澶у皬璁剧疆涓? */
    gtk_container_set_border_width(GTK_CONTAINER(c->win), 0);
   
    g_signal_connect(c->win, "destroy",
                     G_CALLBACK(main_quit), c);
    gtk_widget_set_app_paintable(c->win, TRUE);
 
    main_win_draw_face(c);
 
    gtk_widget_show_all(c->win);
    //toggle_control_area(GTK_TOGGLE_BUTTON(c->control_area_button), c);
   
    gtk_widget_hide(c->win);
    gtk_window_set_position(GTK_WINDOW(c->win), GTK_WIN_POS_CENTER);
    gtk_widget_show(c->win);
 
    return TRUE;
}
 
void main_run( )
{
    gtk_main();
}
 
gint main(gint argc,gchar* argv[])
{  
    int res;
   
    /*---GTK初始化----*/
    gtk_init(&argc, &argv);  
    g_thread_init(NULL);
    gdk_threads_init();
 
    struct wcam_win *c = calloc(1, sizeof(struct wcam_win));
 
    c->entry_win = login_create();
   
    res = login_run(c->entry_win);
    if (res == -1) {
        goto err_win;
    }
   
    login_hide(c->entry_win);
 
    main_create(c);
   
    main_run();
 
err_win:
    free(c->entry_win);
    free(c);
 
    return 0;
}
 
它的大致步骤是，先初始化GTK程序，再创建窗体。
 
2. QT移植以及程序
现在我们的界面已经编辑完成，我们要做的是使得各个部分的按键具有对应的功能。
（1）首先是登录的界面的“ok”按键，当我们按这个按键的时候，应该进行网络的连接；（2）再就是我们的主界面，没有视频的传输。
 
我们现在所缺少的内容在，我们之前编程的过程中都是实现了的。