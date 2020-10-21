#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LEN sizeof(struct Student)
#define LEN1 sizeof(struct Inform)

struct Student
{
    char name[100];  //学生姓名
    char school[100];  //学校名称
    float east_lon;  //东经
    float north_lat;  //北纬
    float dist;  //距离
    struct Student *next;  //下一个结点地址
    struct Student *pre;  //上一个结点地址
};
struct Inform
{
    char school1[100];  //学校名称
    float east_lon1;  //东经
    float north_lat1;  //北纬
    struct Inform *next1;  //下一个结点位置
};

//建立链表(储存学校信息)
struct Inform *creat1(void)
{
    struct Inform *head = NULL, *p1 = NULL, *p2 = NULL;
    FILE *fp = NULL;
    fp = fopen("schools.txt", "r");
    p1=p2=(struct Inform*)malloc(LEN1);
    fscanf(fp, "%s %f %f",p1->school1, &p1->east_lon1, &p1->north_lat1);
    head=p1;
    while(!feof(fp))
    {
        p1 = (struct Inform*)malloc(LEN1);
        fscanf(fp, "%s %f %f",p1->school1, &p1->east_lon1, &p1->north_lat1);
        p2->next1 = p1;
        p2 = p1;
    }
    p1->next1 = NULL;
    fclose(fp);
    return (head);
}

//建立链表(储存学生信息)
struct Student *creat(void)
{
    struct Student *head=NULL;
    struct Student *p1=NULL,*p2=NULL;
    FILE *fp=NULL;  
    fp = fopen("informs.txt", "r");
    p1=p2=(struct Student*)malloc(LEN);
    fscanf(fp, "%s %s %f %f %f", p1->name, p1->school, &p1->east_lon, &p1->north_lat, &p1->dist);
    head=p1;
    p1->pre = NULL;
    while(!feof(fp))
    {
        p1 = (struct Student*)malloc(LEN);
        fscanf(fp, "%s %s %f %f %f", p1->name, p1->school, &p1->east_lon, &p1->north_lat, &p1->dist);
        p1->pre = p2;
        p2->next = p1;
        p2 = p1;
    }
    p1->next = NULL;
    fclose(fp);
    return (head);
}

//将角度转化为弧度
float deg2rad(float deg) 
{
    double PI = acos(-1.0);
    return deg * PI / 180.0;
}

//通过经纬度计算距离
float greatCircleDistance(float lon,float lat) 
{
    float us_long = 117.269603, us_lat = 31.836854;
    if (us_lat == lat && us_long == lon)
        return 0.0;
    static const int R = 6371;
    return R * acos(cos(deg2rad(us_lat)) * cos(deg2rad(lat)) *cos(deg2rad(us_long - lon)) +sin(deg2rad(us_lat)) * sin(deg2rad(lat)));
}

//帮助列表
void help()
{
    printf("输入“帮助列表”以获得帮助\n");
    printf("输入“列”即可显示所有学生信息\n");
    printf("输入“查”即可查询指定学生的信息\n");
    printf("输入“增”即可新增一位学生\n");
    printf("输入“删”即可删除一位学生的所有信息\n");
    printf("输入“撤销”即可撤销上一次的“增”或“删”操作\n");
    printf("**注意：输入“退出”后一切的更改才会生效！\n");
}

//“列”功能
void list(struct Student*head)
{
    struct Student *p=NULL;
    printf("学生信息如下：\n");
    printf("大学                       距离    学生\n");
    p=head;
    while(p!=NULL)
    {
        if(p==head||strcmp(p->school,p->pre->school)!=0)
            printf("%-16s      %7.0fkm    %-10s",p->school,p->dist,p->name);
        else if(strcmp(p->school,p->pre->school)==0)
            printf(" %s", p->name);
        if(p->next==NULL||strcmp(p->school,p->next->school)!=0)
            printf("\n");
        p = p->next;
    }
}

//“查”功能
void search(struct Student *head, char name[])
{
    struct Student *p=NULL;
    p = head;
    int tag = 0;
    while(p!=NULL)
    {
        if(!strcmp(name,p->name))
        {
            tag=1;
            printf("姓名：%s  学校：%s\n北纬：%.1f°  东经：%.1f°\nTA的学校与科大的距离：%.1fkm\n", p->name, p->school,p->north_lat,p->east_lon,p->dist);
            break;
        }
        p = p->next;
    }
    if(tag==0)
        printf("查无此人！\n");
}

//“增”功能
struct Student *add(struct Student *head,struct Inform *head1, char name[], char school[])
{
    struct Student *p1=NULL,*p=head;
    struct Inform *pt = head1;
    float east_lon=0, north_lat = 0;
    float dist = 0;
    int tag = 0;
    while(pt!=NULL)
    {
        if(strcmp(pt->school1,school)==0)
        {
            tag=1;
            break;
        }
        pt = pt->next1;
    }
    if(tag==0)
    {
        printf("文件中未含有该大学的经纬度信息，请自行输入经度与纬度：\n");
        scanf("%f%f", &east_lon, &north_lat);
    }
    else if(tag==1)
    {
        east_lon = pt->east_lon1;
        north_lat = pt->north_lat1;
    }
    dist = greatCircleDistance(east_lon, north_lat);
    while(p!=NULL)
    {
        if((p->next==NULL)&&dist>p->dist)
        {
            tag = 1;
            break;
        }
        else if((p->dist<=dist)&&(p->next->dist>dist))
        {
            tag=2;
            break;
        }
        p = p -> next;
    }
    if(tag==1)
    {
        p1=(struct Student *)malloc(LEN);
        strcpy(p1->name,name);
        strcpy(p1->school, school);
        p1->east_lon = east_lon;
        p1->north_lat = north_lat;
        p1->dist = dist;
        p1->next=NULL;
        p1->pre=p;
        p->next = p1;
        printf("添加成功！(注意：输入“退出”后一切的更改才会生效！)\n");
    }
    else if(tag==2)
    {
        p1=(struct Student *)malloc(LEN);
        strcpy(p1->name,name);
        strcpy(p1->school, school);
        p1->east_lon = east_lon;
        p1->north_lat = north_lat;
        p1->dist = dist;
        p1->next = p->next;
        p1->pre = p;
        p->next->pre=p1;
        p->next = p1;
        printf("添加成功！(注意：输入“退出”后一切的更改才会生效！)\n");
    }
    return (head);
}

//“删”功能
struct Student *delete(struct Student *head, char name[])
{
    struct Student *p=head;
    int tag = 0;
    while(p!=NULL)
    {
        if(strcmp(p->name,name)==0)
        {
            tag = 1;
            break;
        }
        p = p->next;
    }
    if(tag==0)
        printf("需要删除的人不存在!\n");
    else if(tag==1)
    {
        if(p==head)
        {
            head = head->next;
            free(p);
        }
        else if(p->next==NULL)
        {
            p->pre->next=NULL;
            free(p);
        }
        else 
        {
            p->pre->next=p->next;
            p->next->pre = p->pre;
            free(p);
        }
        printf("删除成功！(注意：输入“退出”后一切的更改才会生效！)\n");
    }
    return (head);
}

//“退出”保存功能
void save(struct Student *head)
{
    struct Student *p=head;
    FILE *fp=NULL;
    fp = fopen("informs.txt", "w");
    while(p!=NULL)
    {
        if(p->next!=NULL)
            fprintf(fp,"%s %s %f %f %f\n", p->name, p->school, p->east_lon, p->north_lat, p->dist);
        else if(p->next==NULL)
            fprintf(fp,"%s %s %f %f %f", p->name, p->school, p->east_lon, p->north_lat, p->dist);
        p = p->next;
    }
    printf("退出保存成功！\n");
    fclose(fp);
}

//链表复制(撤销功能)
struct Student *copy_node(struct Student *head,struct Student *copy)
{
    struct Student *p = head, *pt = copy,*temp=NULL;
    int tag ;
    while(p!=NULL)
    {
        if(p->next==NULL&&pt->next==NULL)
        {
            tag=1;
            break;
        }
        else if((p->next==NULL&&pt->next!=NULL)||(pt->next==NULL&&p->next!=NULL))
        {
            tag = 0;
            break;
        }
        p=p->next;
        pt = pt->next;
    }
    if(tag==0)
    {
        p=head;
        pt = copy;
        while(pt!=NULL)
        {
            temp=pt;
            pt=pt->next;
            free(temp);
        }
        temp=pt = (struct Student *)malloc(LEN);
        copy = pt;
        pt->pre = NULL;
        strcpy(pt->name,p->name);
        strcpy(pt->school, p->school);
        pt->east_lon = p->east_lon;
        pt->north_lat = p->north_lat;
        pt->dist = p->dist;
        p = p->next;
        while(p!=NULL)
        {
            pt=(struct Student *)malloc(LEN);
            if(p->next!=NULL)
            {
                temp->next = pt;
                pt->pre = temp;
                temp = pt;
            }
            else if(p->next==NULL)
            {
                temp->next = pt;
                pt->pre = temp;
                temp=pt;
                pt->next = NULL;
            }
            strcpy(pt->name,p->name);
            strcpy(pt->school, p->school);
            pt->east_lon = p->east_lon;
            pt->north_lat = p->north_lat;
            pt->dist = p->dist;
            p = p->next;
        }
    }
    return copy;
}

//主函数
int main()
{
    struct Student *head,*head_copy;
    struct Inform *head1;
    char ch1[100] = {'\0'};
    char ch2[100] = {'\0'};
    head = creat();
    head1 = creat1();
    head_copy = creat();
    printf("欢迎使用超级无敌简陋的高中同学通讯录系统\n");
    printf("该通讯录中的姓名均为化名！\n");
    help();
    printf("请输入:\n");
    scanf("%s",ch1);
    while(1)
    {
        if(strcmp(ch1,"帮助列表")==0)
            help();
        else if(strcmp(ch1,"列")==0)
            list(head);
        else if(strcmp(ch1,"查")==0)
        {
            printf("请输入要查找的对象:\n");
            scanf("%s", ch1);
            search(head, ch1);
        }
        else if(strcmp(ch1,"增")==0)
        {
            head_copy = copy_node(head, head_copy);
            printf("请输入要增加对象的姓名与学校:\n");
            scanf("%s %s", ch1, ch2);
            head = add(head,head1, ch1, ch2);
        }
        else if(strcmp(ch1,"删")==0)
        {
            head_copy = copy_node(head, head_copy);
            printf("请输入要删除对象的姓名:\n");
            scanf("%s",ch1);
            head = delete (head, ch1);
        }
        else if(strcmp(ch1,"退出")==0)
        {
            save(head);
            break;
        }
        else if(strcmp(ch1,"撤销")==0)
        {
            head = copy_node(head_copy,head);
            printf("撤销成功！\n");
        }
        else
            printf("输入不正确！\n");
        printf("请输入:\n");
        scanf("%s", ch1);
    }
    return 0;
}
