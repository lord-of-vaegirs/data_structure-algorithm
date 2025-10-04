## 作业一 面向线性表的设计

### 第一部分 设计顺序表使得奇数在偶数前

代码：sequenceList_001.cpp

核心部分：

```
void resortList(int *sq_list,int size)
{
    int i=0,j=size-1;
    while(i<=j)
    {   
        if(sq_list[j]%2==1) //右指针指向奇数
        {
            if(sq_list[i]%2==0) //左指针为偶数
            {
                myswap(&sq_list[i],&sq_list[j]); //交换函数
                i++;
                j--;
            }
            else // 否则就让左指针到偶数位置再交换
            {
                i++;
            }
        }
        else // 右指针不指向奇数就--
        {
            j--;
        }
    }
}
```

利用双指针从两端不断按条件交换实现功能


### 第二部分 实现链表的排序

代码：resortLinkList.cpp

核心部分：

```
class LinkList 
{
public:
    LinkList()
    {
        this->p=NULL;
        this->size=0;
    }
    void initialList()
    {
        LinkListNode *tmp;
        for(int i=0;i<this->size;i++)
        {
            LinkListNode *node=new LinkListNode();
            cin>>node->data;
            if(i==0) 
            {
                this->p=node;
                tmp=p;
                tmp->head=NULL;
            }
            else if(i<this->size)
            {
                tmp->next=node;
                node->head=tmp;
                tmp=node;
            }
            if(i==this->size-1)
            {
                tmp->next=NULL;
            }
        }
    }
    void resort() // 核心排序算法实现
    {
        LinkListNode *startnode=this->p; // 实现外层循环
        LinkListNode *proc=this->p; // 内层遍历指针
        int steps1=0; 
        int steps2=0; // 均用于内层循环计算循环到哪里
        while(startnode->next!=NULL&&steps1<this->size)
        {
            proc=this->p; // 每次内层都从头开始遍历
            steps2=0;
            while(steps2<this->size-steps1-1)
            {
                if(proc->data>proc->next->data) // 如果前大于后
                {
                    LinkListNode *tmp=proc->next; // 把下一个元素指针复制一份
                    proc->next=tmp->next; // proc的next指针指向原来下一个元素的下一个元素
                    if(tmp->next!=NULL) // 下一个元素的下一个如果有实体元素，那就还要改变它的head指针
                    {
                        tmp->next->head=proc;
                    }
                    tmp->next=proc; // 原先proc的下一个元素现在next指向proc
                    if(proc->head!=NULL) // proc如果本来不是第一个元素
                    {
                        proc->head->next=tmp; // 也要让原proc前一个元素的next指针指向原proc的下一个元素
                    }
                    tmp->head=proc->head; // 原proc下一个元素的head指向原proc的head
                    proc->head=tmp; // 现在proc的head指向原来proc的下一个元素
                    if(tmp->head==NULL) this->p=tmp; // 如果这时原proc的下一个元素在交换后成了整个链表的第一个元素，就更新p
                    proc=tmp->next; // 原proc成为原next的next 实现交换
                    steps2++;
                    continue;
                }
                steps2++;
                proc=proc->next;
            }
            steps1++;
            startnode=startnode->next;
        }
    }
    void print()
    {
        LinkListNode * cur=this->p;
        for(int i=0;i<size;i++)
        {
            cout<<cur->data<<" ";
            cur=cur->next;
        }
        cout<<endl;
    }
    ~LinkList()
    {
        LinkListNode* cur=this->p;
        while(cur)
        {
            LinkListNode * tmp=cur->next;
            delete cur;
            cur=tmp;
        }
    }
    LinkListNode *p;
    int size;
};
```

先维护一个双向链表，通过改变链表内元素的指针指向来实现交换，从而实现冒泡排序
