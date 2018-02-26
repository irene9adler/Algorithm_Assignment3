#include <iostream>
#include <stdio.h>
#include <string.h>
#include <limits>
#include <stdlib.h>

using namespace std;

class HuffmanNode {//Huffman�ڵ���
public:
    char info; //�ַ�
    double weight; //�ַ���Ȩֵ
    int parent, lchild, rchild; //���׽�㣬���Һ��ӽ��
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

class HuffmanTree {//HuffmanTree�࣬��Huffman�ڵ�����
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
    HuffmanNode *arrayTree; //�������������
    int maxSize; //�������ֵ
    int currentSize; //��ǰ�����С
    void insert(const char&, const double&);
    void createHuffmanTree();
    void createHuffmanCode();
    int findPosition(const char &) const;
    int getLongestCodeLength() const;
    int isEqual(const char *s) const;
    void write(int pos,FILE * fileOut);
private:
    class Code { //HuffmanTree������
    public:
        Code():length(20) { ptr=new char[length]; }
        ~Code() { delete[] ptr; }
        char *ptr;
        const int length;
    };
    Code *codeArray; //Huffman�������飬��СΪcurrentSize
    void reverse(char arr[]);
};

void HuffmanTree::insert(const char &data, const double &wt) { //HuffmanTree������
    if (2*currentSize-1 >= maxSize) //Ҷ�ӽ��Ϊn��HuffmanTree��2n-1�����
        return;
    arrayTree[currentSize].info=data;
    arrayTree[currentSize].weight=wt;
    currentSize++;
}

void HuffmanTree::reverse(char arr[]) { //��ת�ַ���
    const int len=strlen(arr);
    char *p;//�м�����
    p=new char[len+1];
    strcpy(p, arr);
    p[len]='\0';
    int k=0;
    for (int i=len-1; i>=0; i--)
        arr[i]=p[k++];
    arr[len]='\0';
    delete[] p;
}

int HuffmanTree::findPosition(const char &ch) const { //�ַ�ch��arrayTree�е�λ��
    for (int i=0; i<currentSize; i++)
        if (arrayTree[i].info == ch)
            return i;
    return -1;
}

int HuffmanTree::getLongestCodeLength() const { //codeArray������ı����λ��
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

int HuffmanTree::isEqual(const char *s) const { //�ж�s�ı����Ƿ���ڣ������ڷ��ر���������codeArray�е�λ��
    for (int i=0; i<currentSize; i++)
        if (strlen(s) == strlen(codeArray[i].ptr))
            if (strcmp(s, codeArray[i].ptr) == 0)
                return i;
    return -1;
}

void HuffmanTree::createHuffmanTree() { //����arrayTree����weight����huffmanTree
    int i=currentSize;
    int k;
    double wt1, wt2;
    int lnode, rnode;
    while (i < 2*currentSize-1) {
        wt1 = wt2 = std::numeric_limits<double>::max();//��ֵ��Ϊdouble�������ֵ
        k=0;
        while (k < i) {//��ǰi-1���������Һ��ӽڵ� СȨֵ�����
            if (arrayTree[k].parent==-1) {
                if (arrayTree[k].weight<wt1) {//weight<wt1 ������
                    wt2=wt1;
                    rnode=lnode;
                    wt1=arrayTree[k].weight;
                    lnode=k;
                }
                else if (arrayTree[k].weight<wt2) {//w1<weight<wt2 ���Һ���
                    wt2=arrayTree[k].weight;
                    rnode=k;
                }
            }
            k++;//weight>wt2 skip
        }
		//���½ڵ�Ȩ�غͺ�����Ϣ�����ӽڵ�parent��Ϣ
        arrayTree[i].weight = arrayTree[lnode].weight+arrayTree[rnode].weight; 
        arrayTree[i].lchild=lnode;
        arrayTree[i].rchild=rnode;
        arrayTree[i].parent = -1;
        arrayTree[lnode].parent=arrayTree[rnode].parent=i;
        i++;
    }
}

void HuffmanTree::createHuffmanCode() { //����arrayTree���鹹��huffmanCode
    codeArray=new Code[currentSize];
    int i=0;
    int k, n, m;
    while (i < currentSize) {
        k = arrayTree[i].parent;
        n=0;
        m=i;
        while (k!=-1 && k<currentSize*2-1) {//���֧��Ϊ0�ҷ�֧��Ϊ1
            if (arrayTree[k].lchild==m)
                codeArray[i].ptr[n++]='0';
            else if (arrayTree[k].rchild==m)
                codeArray[i].ptr[n++]='1';
            m=k;
            k=arrayTree[m].parent;
        }
        codeArray[i].ptr[n]='\0';
        //cout << codeArray[i].ptr << endl;
        reverse(codeArray[i].ptr); //��ת�ַ�����Ϊ��ȷ�ı���
        i++;
    }
}
int num = 0;
int a[100];
int aid = 0;
void HuffmanTree::write(int pos,FILE * fileOut){//������������뵽�ļ�,ÿ8��0/1ֵ��ʾΪһ���ַ�
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

void HuffmanTree::run(const char *inFilename, const char *outFilename, const char *secondOutName) { //run������ʵ��
    const string add_info ="SongQige2017E8018661044";//������Ϣ
    //const string add_info ="";//������Ϣ

    FILE * fileOut, *fileIn;
    unsigned char buf[1024];


    fileIn = fopen(inFilename, "r");

    unsigned char ch;
    int pos;

    //���ļ��ж����ַ��������ַ�pos����ͳ�Ƹ����ַ���������weightдarrayTree����
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

    //��Ӵ�����Ϣ
    for(int i=0;i < add_info.length();i++)
    {
        pos = findPosition(add_info[i]);
        if (pos != -1)
            arrayTree[pos].weight++;
        else
            insert(add_info[i], 1);
    }

    createHuffmanTree(); //arrayTree������ɣ�����huffman��
    createHuffmanCode(); //��ͳ���ַ����б���



    fileIn = fopen(inFilename, "r");
    fileOut = fopen(outFilename, "wb");

    //�Ѵ�inFilename�ļ��е��ַ����б��룬���ö����Ʒ�ʽд��outFilename�ļ�
    while(!feof(fileIn)){
        fread(&ch, sizeof(unsigned char), 1, fileIn);
        pos = findPosition(ch);//�ҵ����ַ�pos
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
    for(int i = 0;i < 7;i++)//�����˸�ʱ���ո񣬱��ⶪʧ�ַ���Ϣ
    {
       pos = findPosition(' ');
       write(pos,fileOut);
    }

    fclose(fileIn);
    fclose(fileOut);



    //��outFilename, secondOutName���ֱ��ṩ���������ע���ö����Ʒ�ʽ��ȡ�����ļ��������޷����ַ���ȡȫ������
    fileIn = fopen(outFilename, "rb");
    fileOut = fopen(secondOutName,"w");
    //��outFileName�ļ��еı�����н��룬���д��secondOutName�ļ�
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
        for(int i = 0 ; i < 8 ; i++){//��ȡ1�ַ���ԭԭ8λ01����
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
                fwrite(&arrayTree[pos].info, sizeof(unsigned char), 1, fileOut);//ԭ�ַ�д������ļ�
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
