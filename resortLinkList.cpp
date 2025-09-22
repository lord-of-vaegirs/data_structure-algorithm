#include<iostream>
using namespace std;

class LinkListNode
{
public:
    LinkListNode()
    {
        this->data=0;
        this->head=NULL;
        this->next=NULL;
    }
    ~LinkListNode()
    {
    }
    int data;
    LinkListNode *head;
    LinkListNode *next;

};

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
    void resort()
    {
        LinkListNode *startnode=this->p;
        LinkListNode *proc=this->p;
        int steps1=0;
        int steps2=0;
        while(startnode->next!=NULL&&steps1<this->size)
        {
            proc=this->p;
            steps2=0;
            while(steps2<this->size-steps1-1)
            {
                if(proc->data>proc->next->data)
                {
                    LinkListNode *tmp=proc->next;
                    proc->next=tmp->next;
                    if(tmp->next!=NULL)
                    {
                        tmp->next->head=proc;
                    }
                    tmp->next=proc;
                    if(proc->head!=NULL) 
                    {
                        proc->head->next=tmp;
                    }
                    tmp->head=proc->head;
                    proc->head=tmp;
                    if(tmp->head==NULL) this->p=tmp;
                    proc=tmp->next;
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

int main()
{
    int size;
    cin>>size;
    LinkList *list=new LinkList();
    list->size=size;
    list->initialList();
    list->resort();
    list->print();
    if(list!=NULL) delete list;
    return 0;
}

/*
for(int i=0;i<n;i++)
{
    for(int j=0;j<n-i-1;j++)
    {
        if(a[j]>a[j+1])
        {
            swap(a[j],a[j+1]);
        }
    }
}
*/