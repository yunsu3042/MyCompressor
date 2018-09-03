#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include "Tree.cpp"
#include <map>

#define f first
#define vpii vector<pair<int, int>>
#define pii pair<int, int>
#define s second
#define notUsed 255
using namespace std;

int oneHotBin(int k);
void printBinary(int x);
bool compare(Tree* &a, Tree* &b);
int oneHotBin(int k);
void printBinary(int x);
void writeBin(string str, string outPath);

bool compare(Tree* &a, Tree* &b){
    if(a->getRoot()->getItem() > b->getRoot()->getItem())
    {
        return true;
    }
    return false;
}

// return int that one hot in k-th bit
int oneHotBin(int k)
{
    return pow(2, k);
}

// print int Type into Binary Type
void printBinary(int x)
{
    for(int i = 0; i < 8; i++)
    {
        int mask = oneHotBin(i);
        if((x & mask) != 0)
        {
            cout << 1;
        }
        else
            cout << 0;
    }
    cout << endl;
}

void writeBin(string str, string outPath)
{
    ofstream out;
    out.open(outPath);
    int length = str.size();
    int n = length / 8;
    int left = length % 8;

    // write in front of file that how many bits are not divided by 8bits.
    char c = left;
    out.write((char*)&c, sizeof(char));
    cout << endl;
    printBinary(c);

    for(int i = 0; i < length - 7; i += 8)
    {
        char bin = 0b00000000;
        for(int j = 0; j < 8; j++)
        {
            if(str[i + j] == '1')
            {
                bin = bin | oneHotBin(j);

            }
        }
        // write byte by byte;
        out.write((char*)&bin, sizeof(char));
        printBinary(bin);
    }

    // encode left bits and fill 8 - left bits as 0s
    char bin = 0b00000000;
    for(int j = 0; j < left; j++)
    {
        if(str[n * 8 + j] == '1')
        {
            bin = bin | oneHotBin(j);
        }
    }
    if(left > 0){
        out.write(&bin, sizeof(char));
        printBinary(bin);
    }
    out.close();

    cout << "==============================\n";
}

class Huffman
{
public:
    vpii frequency;
    map<char, string> dict;
    string text;

    Huffman(string &inputPath, string &compressPath, string &decompressPath)
    {
        this->inputPath = inputPath;
        this->readFile = ifstream(inputPath);
        this->compressPath = compressPath;
        this->decompressPath = decompressPath;
    }
    void countText()
    {
        if(readFile.is_open())
        {
            string line;
            while(getline(readFile, line))
            {
                countLine(line);
                text += (line + "\n");
            }
            //remove last "\n"
            text = text.substr(0, text.size() - 1);
        }
        readFile.close();
    }

    void countLine(string line)
    {
        for(char c : line)
        {
            int idx = indexOf(c);
            if(idx != -1)
                frequency.at(idx).s += 1;
            else
                frequency.push_back(pii(c, 1));
        }
    }

    int indexOf(char c)
    {
        for(int i = 0; i < frequency.size(); i++)
        {
            if(frequency.at(i).f == c)
            {
                return i;
            }
        }
        return -1;
    }

    void makeTree()
    {
        for(int i = 0; i < frequency.size(); i++)
        {
            Tree* tree = new Tree(new Node(frequency[i].s, frequency[i].f));
            v.push_back(tree);
        }

        merge();
    }

    void merge()
    {
        while(v.size() >= 2)
        {
            sort(v.begin(), v.end(), compare);

            Tree* x = v.at(v.size() - 1);
            Tree* y = v.at(v.size() - 2);
            v.pop_back();
            v.pop_back();
            Tree* merged = x->merge(y);
            v.push_back(merged);
        }
    }

    void showDict()
    {
        map<char, string> :: iterator iter;
        for(iter = dict.begin(); iter != dict.end(); ++iter)
        {
            cout << (*iter).first << " : " << (*iter).second << endl;
        }
    }


    void recursiveDict(Node* node, string code){
        if(node == nullptr)
            return ;
        char key = node->getData();
        if(key != (char)255)
        {
            dict[key] = code;
            return ;
        }
        string codeL = code + "0";
        string codeR = code + "1";
        recursiveDict(node->getLeft(), codeL);
        recursiveDict(node->getRight(), codeR);
    }

    void makeDictionary()
    {
        Tree* tree = v.at(0);
        recursiveDict(tree->getRoot(), "");
    }

    void compress()
    {
        string rawLong;
        for(char c : text)
        {
            string code = dict[c];
            rawLong += code;
        }
        writeBin(rawLong, compressPath);

        ifstream ifs(inputPath, ios::binary|ios::ate);
        cout << inputPath << " 파일은 " << ifs.tellg() << " Bytes 입니다." << endl; ifs.close();
        cout << compressPath << "경로에 " << (rawLong.length() + 8 + 7)/8  << "Bytes로 압축되었습니다." << endl;
    }

    void detail()
    {
        int count = 0;
        vpii::iterator iterV;
        for(iterV = frequency.begin(); iterV != frequency.end(); ++iterV)
        {
            count += iterV->second;
        }
        cout << "총 char 개수 : " << count << endl;
    }

    void deCompress()
    {
        Node* root = v.at(0)->getRoot();
        ifstream in;
        in.open(compressPath, ios::binary|ios::ate);
        ifstream::pos_type pos = in.tellg();
        string str;
        vector<char> chrBuffer(pos);
        vector<char> cleanBuffer;
        in.seekg(0, ios::beg);
        in.read(&chrBuffer[0], pos);
        char left = chrBuffer[0];
        for(int i = 1; i < chrBuffer.size(); i++)
        {
            char c = chrBuffer[i];
            for(int j = 0; j < 8; j++)
            {
                if(c & oneHotBin(j))
                    cleanBuffer.push_back('1');
                else
                    cleanBuffer.push_back('0');
            }

        }

        in.close();
        if(left > 0)
        {
            for(int i = 0; i < 8 - left; i++)
            {
                cleanBuffer.pop_back();
            }
        }

        print8Bit(cleanBuffer);
        writeBuffer(cleanBuffer, root);
    }

    void print8Bit(vector<char> cleanBuffer)
    {
        // print Buffer
        int cnt = 0;
        for(char c: cleanBuffer)
        {
            cnt++;
            cout << c;
            if(cnt == 8)
            {
                cout << endl;
                cnt = 0;
            }
        }
        cout << "\n=================\n";
    }

    void writeBuffer(vector<char> &cleanBuffer, Node* root)
    {
        ofstream restore(decompressPath);
        vector<char> :: iterator iter;
        iter = cleanBuffer.begin();
        char c = 0;
        while(1)
        {
            c = searchTree(iter, cleanBuffer.end(), root);
            if(c != (char)notUsed)
            {
                restore << c;
                cout << c ;
            }
            else break;
        }
        cout << endl;
    }

    char searchTree(vector<char>:: iterator &iter, vector<char>:: iterator end, Node* root)
    {
        if(root->getData() != (char)notUsed)
            return root->getData();
        if(iter == end | (root->getRight() == nullptr & root->getLeft() == nullptr))
            return (char)notUsed;
        if(*iter == '1')
            root = root->getRight();
        else
            root = root->getLeft();
        iter++;
        return searchTree(iter, end, root);
    }

private:
    ifstream readFile;
    vector<Tree*> v;
    string inputPath;
    string compressPath;
    string decompressPath;
};
