#include<iostream>
#include<string>
#include "./include/csv.hpp"
using namespace std;
using namespace csv;

#define TRANS_MAX 100000
#define IO_MAX_NUM 50000

struct input
{
    int height;
    string txid;
    int prev_block;
    string prevTxID;
    int prevTxOutIndex;
};

struct output
{
    int height;
    string txid;
    int index;
    long long value;
};

struct transaction
{
    int height;
    string txid;
    int is_coinbase;
    input ip[IO_MAX_NUM];
    output op[IO_MAX_NUM];
    int input_count;
    int output_count;
};

struct block
{
    int height;
    string hash;
    transaction trans_arr[TRANS_MAX];
    int trans_size;
    block * next;
};

struct blockList
{
    block * p;
    int size;
};

int main()
{
    CSVReader blockreader("./demo/blocks.csv");
    CSVReader transreader("./demo/transactions.csv");
    CSVReader inputreader("./demo/inputs.csv");
    CSVReader outputreader("./demo/outputs.csv");
    blockList bl;
    bl.p=NULL;
    bl.size=0;
    block * cur;
    for(CSVRow& row : blockreader)
    {
        block *b;
        b->height=row["height"].get<int>();
        b->hash=row["hash"].get<string>();
        b->trans_size=0;
        b->next=NULL;
        if(bl.size==0)
        {
            bl.p=b;
            cur=bl.p;
            bl.size++;
        }
        else
        {
            cur->next=b;
            cur=b;
            bl.size++;
        } 
    }
    cur=bl.p;
    input InputArr[IO_MAX_NUM];
    output OutputArr[IO_MAX_NUM];
    transaction T_arr[TRANS_MAX];
    int I_id=0,O_id=0,T_id=0;
    for(CSVRow& row : inputreader)
    {
        InputArr[I_id].height=row["height"].get<int>();
        InputArr[I_id].txid=row["txid"].get<string>();
        InputArr[I_id].prev_block=row["prev_block"].get<int>();
        InputArr[I_id].prevTxID=row["prevTxID"].get<string>();
        InputArr[I_id].prevTxOutIndex=row["prevTxOutIndex"].get<int>();
        I_id++;
    }
    for(CSVRow& row : outputreader)
    {
        OutputArr[O_id].height=row["height"].get<int>();
        OutputArr[O_id].txid=row["txid"].get<string>();
        OutputArr[O_id].index=row["index"].get<int>();
        OutputArr[O_id].value=row["value"].get<long long>();
        O_id++;
    }
    int i_id=0,o_id=0;
    for(CSVRow& row : transreader)
    {
        int i=0,j=0;
        T_arr[T_id].height=row["height"].get<int>();
        T_arr[T_id].txid=row["txid"].get<string>();
        T_arr[T_id].is_coinbase=row["is_coinbase"].get<int>();
        T_arr[T_id].input_count=row["input_count"].get<int>();
        T_arr[T_id].output_count=row["output_count"].get<int>();
        while(i<T_arr[T_id].input_count&&InputArr[i_id].height==T_arr[T_id].height&&InputArr[i_id].txid==T_arr[T_id].txid)
        {
            T_arr[T_id].ip[i]=InputArr[i_id];
            i++;
            i_id++;
        }
        while(j<T_arr[T_id].output_count&&OutputArr[o_id].height==T_arr[T_id].height&&OutputArr[o_id].txid==T_arr[T_id].txid)
        {
            T_arr[T_id].op[j]=OutputArr[o_id];
            j++;
            o_id++;
        }
        T_id++;
    }
    int t_id=0;
    while(cur!=NULL)
    {
        while(T_arr[t_id].height==cur->height)
        {
            cur->trans_arr[cur->trans_size]=T_arr[t_id];
            t_id++;
            cur->trans_size++;         
        }
        cur=cur->next;
    }
    return 0;
}