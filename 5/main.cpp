#include <iostream>
#include <stdio.h>
#include <string.h>
#include <limits>
#include <stdlib.h>

using namespace std;

class HuffmanNode {//Huffman节点类
public:
    char info; //字符
    double weight; //字符的权值
    int parent, lchild, rchild; //父亲结点，左右孩子结点
    HuffmanNode() {
        parent=lchild=rchild=-1;
    }
    HuffmanNode(const char &data, const double &wt, const int &pa=-1, const int &lch=-1, const int &rch=-1) {
        info=data;
        weight=wt;
        parent=pa;
        lchild=lch;
        rchild=rch;
    }
};

class HuffmanTree {//HuffmanTree类，即Huffman节点数组
public:
    HuffmanTree(const int &s=10000) {
        maxSize=(s>10000?s:10000);
        arrayTree=new HuffmanNode[maxSize];
        currentSize=0;
        codeArray=0;
    }
    ~HuffmanTree() {
        delete[] arrayTree;
        if (codeArray!=0)
            delete[] codeArray;
    }
    void run(const char*, const char*, const char*);

private:
    HuffmanNode *arrayTree; //哈夫曼结点数组
    int maxSize; //数组最大值
    int currentSize; //当前数组大小
    void insert(const char&, const double&);
    void createHuffmanTree();
    void createHuffmanCode();
    int findPosition(const char &) const;
    int getLongestCodeLength() const;
    int isEqual(const char *s) const;
    void write(int pos,FILE * fileOut);
private:
    class Code { //HuffmanTree编码类
    public:
        Code():length(20) { ptr=new char[length]; }
        ~Code() { delete[] ptr; }
        char *ptr;
        const int length;
    };
    Code *codeArray; //Huffman编码数组，大小为currentSize
    void reverse(char arr[]);
};

void HuffmanTree::insert(const char &data, const double &wt) { //HuffmanTree插入结点
    if (2*currentSize-1 >= maxSize) //叶子结点为n的HuffmanTree有2n-1个结点
        return;
    arrayTree[currentSize].info=data;
    arrayTree[currentSize].weight=wt;
    currentSize++;
}

void HuffmanTree::reverse(char arr[]) { //反转字符串
    const int len=strlen(arr);
    char *p;//中间数组
    p=new char[len+1];
    strcpy(p, arr);
    p[len]='\0';
    int k=0;
    for (int i=len-1; i>=0; i--)
        arr[i]=p[k++];
    arr[len]='\0';
    delete[] p;
}

int HuffmanTree::findPosition(const char &ch) const { //字符ch在arrayTree中的位置
    for (int i=0; i<currentSize; i++)
        if (arrayTree[i].info == ch)
            return i;
    return -1;
}

int HuffmanTree::getLongestCodeLength() const { //codeArray长度最长的编码的位置
    if (currentSize == 0)
        return -1;
    int len=strlen(codeArray[0].ptr);
    int i=1;
    while (i < currentSize) {
        int tmp=strlen(codeArray[i].ptr);
        if (tmp > len)
            len=tmp;
        i++;
    }
    return len;
}

int HuffmanTree::isEqual(const char *s) const { //判断s的编码是否存在，若存在返回编码在数组codeArray中的位置
    for (int i=0; i<currentSize; i++)
        if (strlen(s) == strlen(codeArray[i].ptr))
            if (strcmp(s, codeArray[i].ptr) == 0)
                return i;
    return -1;
}

void HuffmanTree::createHuffmanTree() { //根据arrayTree数组weight构造huffmanTree
    int i=currentSize;
    int k;
    double wt1, wt2;
    int lnode, rnode;
    while (i < 2*currentSize-1) {
        wt1 = wt2 = std::numeric_limits<double>::max();//初值设为double类型最大值
        k=0;
        while (k < i) {//在前i-1项中找左右孩子节点 小权值往左放
            if (arrayTree[k].parent==-1) {
                if (arrayTree[k].weight<wt1) {//weight<wt1 做左孩子
                    wt2=wt1;
                    rnode=lnode;
                    wt1=arrayTree[k].weight;
                    lnode=k;
                }
                else if (arrayTree[k].weight<wt2) {//w1<weight<wt2 做右孩子
                    wt2=arrayTree[k].weight;
                    rnode=k;
                }
            }
            k++;//weight>wt2 skip
        }
		//更新节点权重和孩子信息，孩子节点parent信息
        arrayTree[i].weight = arrayTree[lnode].weight+arrayTree[rnode].weight; 
        arrayTree[i].lchild=lnode;
        arrayTree[i].rchild=rnode;
        arrayTree[i].parent = -1;
        arrayTree[lnode].parent=arrayTree[rnode].parent=i;
        i++;
    }
}

void HuffmanTree::createHuffmanCode() { //根据arrayTree数组构造huffmanCode
    codeArray=new Code[currentSize];
    int i=0;
    int k, n, m;
    while (i < currentSize) {
        k = arrayTree[i].parent;
        n=0;
        m=i;
        while (k!=-1 && k<currentSize*2-1) {//左分支编为0右分支编为1
            if (arrayTree[k].lchild==m)
                codeArray[i].ptr[n++]='0';
            else if (arrayTree[k].rchild==m)
                codeArray[i].ptr[n++]='1';
            m=k;
            k=arrayTree[m].parent;
        }
        codeArray[i].ptr[n]='\0';
        //cout << codeArray[i].ptr << endl;
        reverse(codeArray[i].ptr); //反转字符串成为正确的编码
        i++;
    }
}
int num = 0;
int a[100];
int aid = 0;
void HuffmanTree::write(int pos,FILE * fileOut){//输出哈夫曼编码到文件,每8个0/1值表示为一个字符
    int i = 0;
    while(codeArray[pos].ptr[i] != '\0'){
        a[aid++] = codeArray[pos].ptr[i]-'0';
        i++;
    }
    while(aid >= 8){
        unsigned char sum = 0;
        for(int i = 0 ; i < 8 ; i++)
            sum = sum*2+a[i];
        //fwrite(sum, sizeof(unsigned char), 1, fileOut);
        fputc(sum,fileOut);
        num++;
        for(int i = 8 ; i < aid ; i++)
            a[i-8] = a[i];
        //a[aid-8] = -1;
        aid -= 8;
    }
}

void HuffmanTree::run(const char *inFilename, const char *outFilename, const char *secondOutName) { //run函数的实现
    const string add_info ="SongQige2017E8018661044";//代码信息
    //const string add_info ="";//代码信息

    FILE * fileOut, *fileIn;
    unsigned char buf[1024];


    fileIn = fopen(inFilename, "r");

    unsigned char ch;
    int pos;

    //从文件中读入字符，查找字符pos，并统计各个字符个数，将weight写arrayTree数组
   while(!feof(fileIn)){
        fread(&ch, sizeof(unsigned char), 1, fileIn);
       //cout << ch;
        pos = findPosition(ch);
        if (pos != -1)
            arrayTree[pos].weight++;
        else
            insert(ch, 1);
    }
    fclose(fileIn);

    //添加代码信息
    for(int i=0;i < add_info.length();i++)
    {
        pos = findPosition(add_info[i]);
        if (pos != -1)
            arrayTree[pos].weight++;
        else
            insert(add_info[i], 1);
    }

    createHuffmanTree(); //arrayTree数组完成，构造huffman树
    createHuffmanCode(); //对统计字符进行编码



    fileIn = fopen(inFilename, "r");
    fileOut = fopen(outFilename, "wb");

    //把从inFilename文件中的字符进行编码，并用二进制方式写入outFilename文件
    while(!feof(fileIn)){
        fread(&ch, sizeof(unsigned char), 1, fileIn);
        pos = findPosition(ch);//找到该字符pos
        if (pos != -1)
            {
                write(pos,fileOut);
                //fwrite(codeArray[pos].ptr, sizeof(unsigned char), 1, fileOut);
               // cout << codeArray[pos].ptr << " " <<ch << endl;
            }
    }
    for(int i=0;i < add_info.length();i++)
    {
       pos = findPosition(add_info[i]);
        if (pos != -1)
           {
               write(pos,fileOut);
               //fwrite(codeArray[pos].ptr, sizeof(unsigned char), 1, fileOut);
               //cout << codeArray[pos].ptr <<endl;
           }
    }
    for(int i = 0;i < 7;i++)//最后不足八个时补空格，避免丢失字符信息
    {
       pos = findPosition(' ');
       write(pos,fileOut);
    }

    fclose(fileIn);
    fclose(fileOut);



    //打开outFilename, secondOutName，分别提供输入输出，注意用二进制方式读取编码文件，否则无法逐字符读取全部内容
    fileIn = fopen(outFilename, "rb");
    fileOut = fopen(secondOutName,"w");
    //把outFileName文件中的编码进行解码，结果写入secondOutName文件
    const int longestLen = getLongestCodeLength();
    //cout << "longestLen:" << longestLen << endl;
    char *p = new char[longestLen+1];
    int k=0;

    while(num){
    //while(k < longestLen){
        unsigned char cc = fgetc(fileIn);
        if(num<0)
            break;
        num--;
        //cout <<"cc = " << cc << endl;
        int pp = 128;
        for(int i = 0 ; i < 8 ; i++){//读取1字符后还原原8位01编码
        int qq = cc/pp;
        ch = qq+'0';
        cc -= pp*qq;
        pp/=2;
        //fread(&ch, sizeof(unsigned char), 1, fileIn);
        //cout << ch;
        if (k < longestLen) {
            p[k++]=ch;
            p[k]='\0';
            pos = isEqual(p);
            if (pos != -1) {
                fwrite(&arrayTree[pos].info, sizeof(unsigned char), 1, fileOut);//原字符写入解码文件
                //cout << arrayTree[pos].info ;
                k=0;
            }
        }
        }
//        else {
//            cerr<<"The code is wrong."<<endl;
//            exit(1);
//        }

    }
    fclose(fileIn);
    fclose(fileOut);
}


int main() {
    HuffmanTree tree;
    const char *inFileName="Aesop_Fables.txt";
    //const char *inFileName="graph.txt";
    const char *outFileName="outfile1.dat";
    const char *secondOutName="outfile2.txt";
    tree.run(inFileName, outFileName, secondOutName);

    return 0;
}
