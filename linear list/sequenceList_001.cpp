#include<iostream>
using namespace std;

void myswap(int *a,int *b)
{
    int temp=*a;
    *a=*b;
    *b=temp;
}

void resortList(int *sq_list,int size)
{
    int i=0,j=size-1;
    while(i<=j)
    {   
        if(sq_list[j]%2==1)
        {
            if(sq_list[i]%2==0)
            {
                myswap(&sq_list[i],&sq_list[j]);
                i++;
                j--;
            }
            else
            {
                i++;
            }
        }
        else
        {
            j--;
        }
    }
}

int main()
{
    int size;
    int *sequenceList;
    cin>>size;
    sequenceList=new int[size];
    for(int i=0;i<size;i++) cin>>sequenceList[i];
    resortList(sequenceList,size);
    for(int i=0;i<size;i++) cout<<sequenceList[i]<<" ";
    cout<<endl;
    if(sequenceList!=NULL) delete[] sequenceList;
    return 0;
}