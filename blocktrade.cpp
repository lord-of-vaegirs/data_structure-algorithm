#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
using namespace std;

vector<vector<string>> readCSV(const string &filename) {
    vector<vector<string>> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        return data;
    }

    string line;
    while (getline(file, line)) {  
        vector<string> row;
        string cell;
        stringstream lineStream(line);

        while (getline(lineStream, cell, ',')) {  
            row.push_back(cell);
        }
        data.push_back(row);
    }

    file.close();
    return data;
}


int main()
{
    string block_name=''
    return 0;
}