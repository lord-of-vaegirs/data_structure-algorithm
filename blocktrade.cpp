#include <iostream>
#include <string>
#include "csv.hpp"
using namespace std;
using namespace csv;

#define TRANS_MAX 30000
#define IO_MAX_NUM 30000

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
    bool is_used;
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
    bool is_legal;
    output *used_ops_ptr[IO_MAX_NUM];
    int ptr_count;
};

struct block
{
    int height;
    string hash;
    transaction trans_arr[TRANS_MAX];
    int trans_size;
    block *next;
    block *prev;
};

struct blockList
{
    block *p;
    int size;
};

input InputArr[IO_MAX_NUM];
output OutputArr[IO_MAX_NUM];
transaction T_arr[TRANS_MAX];
blockList bl;

int main()
{
    try
    {
        /* code */
        CSVReader blockreader("./demo/blocks.csv");
        CSVReader transreader("./demo/transactions.csv");
        CSVReader inputreader("./demo/inputs.csv");
        CSVReader outputreader("./demo/outputs.csv");
        bl.p = new block();
        bl.p->height = -1;
        bl.p->next = NULL;
        bl.p->prev = NULL;
        bl.size = 0;
        block *cur;
        cur = bl.p;
        for (CSVRow &row : blockreader)
        {
            block *b = new block();
            b->height = row["height"].get<int>();
            b->hash = row["hash"].get<string>();
            b->trans_size = 0;
            b->next = NULL;
            b->prev = cur;
            cur->next = b;
            cur = b;
            bl.size++;
        }
        bl.p->prev = cur;
        cur->next = bl.p;
        cur = bl.p;

        int I_id = 0, O_id = 0, T_id = 0;
        for (CSVRow &row : inputreader)
        {
            InputArr[I_id].height = row["height"].get<int>();
            InputArr[I_id].txid = row["txid"].get<string>();
            InputArr[I_id].prev_block = row["prev_block"].get<int>();
            InputArr[I_id].prevTxID = row["prevTxID"].get<string>();
            InputArr[I_id].prevTxOutIndex = row["prevTxOutIndex"].get<int>();
            I_id++;
        }
        for (CSVRow &row : outputreader)
        {
            OutputArr[O_id].height = row["height"].get<int>();
            OutputArr[O_id].txid = row["txid"].get<string>();
            OutputArr[O_id].index = row["index"].get<int>();
            OutputArr[O_id].value = row["value"].get<long long>();
            OutputArr[O_id].is_used = false;
            O_id++;
        }
        int i_id = 0, o_id = 0;
        for (CSVRow &row : transreader)
        {
            int i = 0, j = 0;
            T_arr[T_id].height = row["height"].get<int>();
            T_arr[T_id].txid = row["txid"].get<string>();
            T_arr[T_id].is_coinbase = row["is_coinbase"].get<int>();
            T_arr[T_id].input_count = row["input_count"].get<int>();
            T_arr[T_id].output_count = row["output_count"].get<int>();
            T_arr[T_id].is_legal = true;
            while (i < T_arr[T_id].input_count && InputArr[i_id].height == T_arr[T_id].height && InputArr[i_id].txid == T_arr[T_id].txid)
            {
                T_arr[T_id].ip[i] = InputArr[i_id];
                i++;
                i_id++;
            }
            while (j < T_arr[T_id].output_count && OutputArr[o_id].height == T_arr[T_id].height && OutputArr[o_id].txid == T_arr[T_id].txid)
            {
                T_arr[T_id].op[j] = OutputArr[o_id];
                j++;
                o_id++;
            }
            T_id++;
        }
        int t_id = 0;
        cur = cur->next;
        while (cur != bl.p)
        {
            while (T_arr[t_id].height == cur->height)
            {
                cur->trans_arr[cur->trans_size] = T_arr[t_id];
                t_id++;
                cur->trans_size++;
            }
            cur = cur->next;
        }
        cout << "链表长度为：" << bl.size << endl;
        cout << "区块总数：" << bl.size << endl;
        cur = bl.p->next;
        int legal_count = 0;
        int illegal_count = 0;
        while (cur != bl.p)
        {
            long long input_value = 0;
            long long output_value = 0;
            for (int i = 0; i < T_id; i++)
            {
                input_value = 0;
                output_value = 0;
                cur->trans_arr[i].ptr_count = 0;
                if (cur->trans_arr[i].is_coinbase == 1)
                {
                    legal_count++;
                }
                else
                {
                    for (int j = 0; j < cur->trans_arr[i].input_count; j++)
                    {
                        block *it = bl.p->next;
                        while (it != bl.p && it->height != cur->trans_arr[i].ip[j].prev_block)
                        {
                            it = it->next;
                        }
                        if (it == bl.p)
                        {
                            cur->trans_arr[i].is_legal = false;
                            break;
                        }
                        int pid = 0;
                        while (pid < it->trans_size && it->trans_arr[pid].txid != cur->trans_arr[i].ip[j].prevTxID)
                        {
                            pid++;
                        }
                        if (pid == it->trans_size)
                        {
                            cur->trans_arr[i].is_legal = false;
                            break;
                        }
                        if (it->trans_arr[pid].is_legal == false)
                        {
                            cur->trans_arr[i].is_legal = false;
                            break;
                        }
                        if (it->trans_arr[pid].op[cur->trans_arr[i].ip[j].prevTxOutIndex].is_used == true)
                        {
                            cur->trans_arr[i].is_legal = false;
                            break;
                        }
                        else
                        {
                            input_value += it->trans_arr[pid].op[cur->trans_arr[i].ip[j].prevTxOutIndex].value;
                            cur->trans_arr[i].used_ops_ptr[cur->trans_arr[i].ptr_count++] = &it->trans_arr[pid].op[cur->trans_arr[i].ip[j].prevTxOutIndex];
                        }
                    }
                    if (cur->trans_arr[i].is_legal == false)
                    {
                        illegal_count++;
                        continue;
                    }
                    for (int j = 0; j < cur->trans_arr[i].output_count; j++)
                    {
                        output_value += cur->trans_arr[i].op[j].value;
                    }
                    if (input_value < output_value)
                    {
                        cur->trans_arr[i].is_legal = false;
                        illegal_count++;
                        continue;
                    }
                    legal_count++;
                    for (int j = 0; j < cur->trans_arr[i].ptr_count; j++)
                    {
                        cur->trans_arr[i].used_ops_ptr[j]->is_used = true;
                    }
                }
            }
            cur = cur->next;
        }
        cout << "合法交易数: " << legal_count << endl;
        cout << "非法交易数: " << illegal_count << endl;

        cout << "\n请输入区块高度: " << endl;
        int myheight;
        cin >> myheight;
        cur = bl.p->next;
        while (cur != bl.p)
        {
            if (cur->height == myheight)
            {
                break;
            }
            cur = cur->next;
        }
        if (cur == bl.p)
            cout << "错误的区块高度" << endl;
        else
        {
            cout << "hash : " << cur->hash << endl;
            cout << "current block transactions' size" << cur->trans_size << endl;
            for (int i = 0; i < cur->trans_size; i++)
            {
                cout << "\ttransaction txid: " << cur->trans_arr[i].txid << endl;
                cout << "\tinput count: " << cur->trans_arr[i].input_count << endl;
                cout << "\t\theight txid prev_block prevTxID prevTxOutIndex " << endl;
                for (int j = 0; j < cur->trans_arr[i].input_count; j++)
                {
                    cout << "\t\t" << cur->trans_arr[i].ip[j].height << " " << cur->trans_arr[i].ip[j].txid << " " << cur->trans_arr[i].ip[j].prev_block << " " << cur->trans_arr[i].ip[j].prevTxID << " " << cur->trans_arr[i].ip[j].prevTxOutIndex << " " << endl;
                }
                cout << "\toutput count: " << cur->trans_arr[i].output_count << endl;
                cout << "\t\theight txid index value " << endl;
                for (int j = 0; j < cur->trans_arr[i].output_count; j++)
                {
                    cout << "\t\t" << cur->trans_arr[i].op[j].height << " " << cur->trans_arr[i].op[j].txid << " " << cur->trans_arr[i].op[j].index << " " << cur->trans_arr[i].op[j].value << " " << endl;
                }
            }
        }
        cout << "\n 请输入想查询的交易TxID: " << endl;
        string mytxid;
        cin >> mytxid;
        for (int i = 0; i < T_id; i++)
        {
            if (T_arr[i].txid == mytxid)
            {
                cout << "height: " << T_arr[i].height << endl;
                cout << "is_coinbase: " << T_arr[i].is_coinbase << endl;
                cout << "input count: " << T_arr[i].input_count << endl;
                for (int j = 0; j < T_arr[i].input_count; j++)
                {
                    cout << "\t" << T_arr[i].ip[j].height << " " << T_arr[i].ip[j].txid << " " << T_arr[i].ip[j].prev_block << " " << T_arr[i].ip[j].prevTxID << " " << T_arr[i].ip[j].prevTxOutIndex << endl;
                }
                cout << "output count: " << T_arr[i].output_count << endl;
                for (int j = 0; j < T_arr[i].output_count; j++)
                {
                    cout << "\t" << T_arr[i].op[j].height << " " << T_arr[i].op[j].txid << " " << T_arr[i].op[j].index << " " << T_arr[i].op[j].value << endl;
                }
                break;
            }
        }
        cur = bl.p->next;
        while (cur != bl.p)
        {
            block *tmp = cur;
            cur = cur->next;
            delete tmp;
        }
        delete bl.p;
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}