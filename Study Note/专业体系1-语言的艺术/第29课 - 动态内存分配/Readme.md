第29课 - 动态内存分配

 

先看一段例程，超市模拟程序，记录卖出去东西的价格以及名称

#include <stdio.h>

#include <string.h>

struct Product

{

    char name[128];

    int price;

};

struct Product pro[1000];

struct Product sale;

void waitForSale()

{

    printf("Record Product: ");

    scanf("%s %d", sale.name, &sale.price);

}

int main()

{

    int i = 0;    

    for(i=0; i<1000; i++)

    {

        waitForSale();

        strcpy(pro[i].name, sale.name);

        pro[i].price = sale.price;  

        printf("%s, %d\n", pro[i].name, pro[i].price);

    }

    return 0;

}

这里面最大的问题就是，我们设定物品最大的数量就是1000，但是物品超过1000的时候，就会使程序出现问题。这时，我们就可以使用动态内存分配了。

 

为什么使用动态内存分配？
(1) C语言中的一切操作都是基于内存的。

(2) 变量和数组都是内存而定别名，如何分配这些内存由编译器在编译期间决定

(3) 定义数组的时候必须指定数组的长度，而数组的长度是在编译期就必须决定的。我们面的需求是，程序运行的过程中，可能需要使用一些额外的内存空间。

 

针对例程的问题，我们有两种解决方法：

（1）定义1000个数组，当超过1000的时候，我们定义指针重新申请

（2）直接定义指针，不需要数组了。我们直接用指针指向malloc（1000），当超过的时候，重新指。

无论哪种方法都需要动态内存分配。

 

malloc和free
（1）`malloc和free用于执行动态内存分配和释放`

 

（3）`malloc分配的是一块连续的内存，以字节为单位，而且不带任何的类型信息`

（4）`free用于将动态内存归还系统`

void* malloc(size_t size)

void free(void* pointer)

系统里面有一个区域，这个区域有一些闲置的内存，它的作用就是完成动态内存的申请和归还的。

注意：malloc实际分配的内可能会比请求的稍微多一些，但是不能依赖于编译器的这个行为

当请求的动态内存无法满足malloc返回NULL

当free的参数为NULL时，函数直接返回

 

 

calloc函数:需要用到的头文件stdlib.h 
void *colloc(size_t num_elements,size_t element_size); 
功能:功能同malloc是一样的，但是作初始化 
参数:num_elements是所需的元素的数量，element_size是每个元素的字节数 
返回:同malloc函数一样 
也是需要与free(p)进行对称使用
#include<stdio.h> 

#include<stdlib.h>

int main(void)

{

printf("please input one number:");

int n;

scanf("%d",&n);

int *p = (int *)calloc(n,sizeof(int));

if(p!=NULL)

{

int i;

for(i=0;i<n;i++)

{

printf("%d ",*(p+i)); }

printf("\n"); free(p); }

else

{

printf("calloc error\n");

 }

return 0;

}

 

realloc函数:需要用到的头文件(stdlib.h),动态扩大缩小申请的内存 
void *realloc(void *ptr,size_t new_size); 
功能:在指针ptr指向的内存基础上扩大或者缩小内存 
参数:ptr是指向先前通过malloc,calloc和realloc函数后分配的内存块的指针，new_size是内存块的新尺寸，可能大于或者小于原有内存尺寸；这个是追加到new_size的新的内存 
realloc在C语言中也被称为动态数组； 
realloc函数使用的注意点: 
（1）当扩展内存的时候，不会对添加进内存块的字节进行初始化 
（2）若不能调整内存则返回NULL，但原有内存中的数据是不会发生改变的 
（3）若第一个参数为NULL那么功能 等同与malloc函数，若第二个参数为0,那么会释放调用内存块
realloc(NULL,10*size(int)) 等同malloc(10*sizeof(int));

realloc(p,0); 等同于free

例程分析：

#include <stdio.h>

#include <malloc.h>

int main()

{

    int i = 0;

    int* pI = (int*)malloc(5 * sizeof(int)); //申请了20个空间

    short* pS = (short*)calloc(5, sizeof(short)); //申请了10个空间

    for(i=0; i<5; i++)

    {

        printf("pI[%d] = %d, pS[%d] = %d\n", i, pI[i], i, pS[i]);

    }

     pI = (int*)realloc(pI, 10 * sizeof(int));   //重新分配

    for(i=0; i<10; i++)

    {

        printf("pI[%d] = %d\n", i, pI[i]);

    }  

    free(pI);

free(pS);

 return 0;

}

运行结果:

pI[0] = -842159451, pS[0] =0

pI[1] = -842159451, pS[1] =0

pI[2] = -842159451, pS[2] =0

pI[3] = -842159451, pS[3] =0

pI[4] = -842159451, pS[4] =0

pI[0] = -842159451

pI[1] = -842159451

pI[2] = -842159451

pI[3] = -842159451

pI[4] = -842159451

pI[5] = -842159451

pI[6] = -842159451

pI[7] = -842159451

pI[8] = -842159451

pI[9] = -842159451

 

 

小结：

l 动态内存分配是C语言中强大功能。

l 程序能够在需要的时候有机会试用更多的内存。

l malloc单纯的从系统中申请固定字节大小的内存。

l calloc能以类型大小为单位申请内存并且初始化。

l realloc用于重置内存大小。

 

 

 

 

 

常见的内存错误：
（1）段错误 :
使用未分配成功的内存 
避免方式：在使用内存之前检查指针是否为NULL； 
引用分配成功但尚未初始化的内存 
避免方式：赋予初值，即便是赋予零值也不可省略 
内存分配成功并且已经初始化，但操作越过了内存的边界 
避免：注意下表的使用不能超出边界 
忘记释放内存，造成内存泄露 
避免方式：申请内存的方式和释放内存的方式需要成双成对 
释放内存之后却继续去使用这一块内存 
避免方式：使用free内存之后，把指针置为NULL；

（2）内存错误的注意点： 
指针消亡了，并不表示它所指向的内存会被自动释放,(在free之前，直接将指针设为NULL)； 
内存释放了，并不代表指针会消亡或者成了NULL指针；(在free之后，指针并没有进行NULL设置)

（3）野指针： 
野指针的形成是指针变量没有被初始化，任何指针变量刚被创建的时候不会自动成为NULL指针，它的缺省值是最忌的，它会乱指一气

指针变量在创建的同时应当被初始化，要么将指针设置为NULL，要么让它指向合法内存

free内存块之后，需要将指针设置为NULL，如果没有设置为NULL，也会出现“野指针”，它是指向“垃圾”内存的指针；

多次free内存块，是会导致程序崩溃的

 