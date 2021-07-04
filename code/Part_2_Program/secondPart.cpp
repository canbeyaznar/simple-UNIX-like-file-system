//Can BEYAZNAR
//161044038

#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <fstream>
#define MAX_FILE_NAME_SIZE 20
#define BYTE_SIZE 1024
#define M_BYTE_SIZE 1024*1024
#define TOTALBYTE_INODE 107
#define TOTALBYTE_SUPERBLOCK 217
#define TOTALBYTE_DIRECTORY 148

using namespace std;

int current_inode_id=0;
int directory_count = 0;

int return_numberOfBytes(int val,int max_size);
string appendZeroToStr(string input, int numberOfZero);
int getNumberOfEachInode(int blockSize);
int getMaxBigINode(int blockSize);

//this function will set the integer values to string
//and it will fill zeros for remaining bytes
// Example  intToStringWithZero(321, str, 7)
// str will be "0000321"
void intToStringWithZero(int value, string &myStr, int maxByteSize);

int getNumberOfMaxINode(int blockSize);

class superblock_init{

    public:
        superblock_init(){
            fileName = "";
            endOfTheFile = "";
            numberOfBlocks = "";
            sizeOfEachBlock = "";
            //startaddress_FreeSpace = "";
            startaddress_INodes = "";
            startaddress_RootDir = "";
            startaddress_DataBlock = "";
        }
        superblock_init(string _fileName, int blockSizeOfFile)
        {
            int blockSizeOfFile_Byte = BYTE_SIZE * blockSizeOfFile;
            int numberofblock = M_BYTE_SIZE / blockSizeOfFile_Byte;
            int remainingByteForFileName = MAX_FILE_NAME_SIZE - _fileName.length();
            int maxBigInode = getMaxBigINode(blockSizeOfFile);
            
            fileName = _fileName;
            int i;
            for(i=0; i<remainingByteForFileName; i++)
                fileName.append(" ");
            intToStringWithZero(M_BYTE_SIZE, endOfTheFile, 7);
            intToStringWithZero(numberofblock, numberOfBlocks, 4); 
            intToStringWithZero(blockSizeOfFile_Byte, sizeOfEachBlock, 5);

            //these values will change
            //intToStringWithZero(0, startaddress_FreeSpace, 7);
            intToStringWithZero(blockSizeOfFile_Byte , startaddress_INodes, 7); 
            intToStringWithZero( ( blockSizeOfFile_Byte + (blockSizeOfFile_Byte * maxBigInode)  ) , startaddress_RootDir, 7);
            intToStringWithZero(( blockSizeOfFile_Byte + (blockSizeOfFile_Byte * maxBigInode) + (blockSizeOfFile_Byte)  ), startaddress_DataBlock, 7);

        }
        void set_fileName(string x) {fileName = x;}
        void set_endOfTheFile(int x) {intToStringWithZero(x, endOfTheFile, 7); }
        void set_numberOfBlocks(int x)  {intToStringWithZero(x, numberOfBlocks, 4); }
        void set_sizeOfEachBlock(int x) {intToStringWithZero(x, sizeOfEachBlock, 5); }
        //void set_startaddress_FreeSpace(int x) {intToStringWithZero(x, startaddress_FreeSpace, 7); }
        void set_startaddress_INodes(int x) {intToStringWithZero(x, startaddress_INodes, 7); }
        void set_startaddress_RootDir(int x) {intToStringWithZero(x, startaddress_RootDir, 7); }
        void set_startaddress_DataBlock(int x) {intToStringWithZero(x, startaddress_INodes, 7); }

        string get_fileName() {return fileName;}
        string get_endOfTheFile() {return endOfTheFile;}
        string get_numberOfBlocks() {return numberOfBlocks;}
        string get_sizeOfEachBlock() {return sizeOfEachBlock;}
        //string get_startaddress_FreeSpace() {return startaddress_FreeSpace;}
        string get_startaddress_INodes() {return startaddress_INodes;}
        string get_startaddress_RootDir() {return startaddress_RootDir;}
        string get_startaddress_DataBlock() {return startaddress_DataBlock;}
        int get_totalByte() {return totalByte;}

        friend ostream& operator<<(ostream &os, superblock_init &input);
        void print_superblock();
    
    private:
        string fileName;                // 20 bytes
        string endOfTheFile;            // 7 bytes
        string numberOfBlocks;          // 4 bytes
        string sizeOfEachBlock;         // 5 bytes
        //string startaddress_FreeSpace;  // 7 bytes
        string startaddress_INodes;     // 7 bytes
        string startaddress_RootDir;    // 7 bytes
        string startaddress_DataBlock;  // 7 bytes
        // total size is : 217 bytes
        int totalByte = 217;

};

class INode_init{

    public:
        INode_init(){
            string x = to_string(current_inode_id);
            validBit = "0"; //empty
            intToStringWithZero(current_inode_id, id, 4);
            fileName = "UNDEFINED";
            fileName.append(x.c_str());

            fileName = appendZeroToStr(fileName,(20-fileName.length()));

            intToStringWithZero(0, fileSize, 2);
            intToStringWithZero(0, data0, 7);
            intToStringWithZero(0, data1, 7);
            time = "00.00.00 00:00:00";

            current_inode_id++;
        }

        INode_init(const INode_init& copyINode){
            validBit = copyINode.validBit;
            id = copyINode.id;
            fileName = copyINode.fileName;
            fileSize = copyINode.fileSize;
            data0 = copyINode.data0;
            data1 = copyINode.data1;
            time = copyINode.time;
            totalByte = copyINode.totalByte;
        }

        string get_validBit() {return validBit;}
        string get_id() {return id;}
        string get_fileName() {return fileName;}
        string get_fileSize() {return fileSize;}
        string get_data0() {return data0;}
        string get_data1() {return data1;}
        string get_time() {return time;}
        int get_totalByte() {return totalByte;}

        friend ostream& operator<<(ostream &os, INode_init &input);
        void print_inode();

    private:
        string validBit;
        string id;
        string fileName;
        string fileSize;
        string data0;
        string data1;
        string time;
        // total 107 byte
        int totalByte = 107;

};

class Directory_init{

    public:
        Directory_init(){
            Dname = "??????????";
            fileName0 = "????????????????????";
            fileName1 = "????????????????????";
            intToStringWithZero(0, DAdress0, 7);
            intToStringWithZero(0, DAdress1, 7);
            intToStringWithZero(0, DAdress2, 7);
            date = "00.00.00 00:00:00";
        }
        
        string get_DName() {return Dname; }
        string get_fileName0() {return fileName0; }
        string get_fileName1() {return fileName1; }
        string get_DAdress0() {return DAdress0; }
        string get_DAdress1() {return DAdress1; }
        string get_DAdress2() {return DAdress2; }
        string get_date() {return date; }

        void set_Dname(string x) {Dname = x;}

        friend ostream& operator<<(ostream &os, Directory_init &input);

    private:
        string Dname;
        string fileName0;
        string fileName1;
        string DAdress0;
        string DAdress1;
        string DAdress2;
        string date;

};

// Kullanıcının file system için verdiği dosya ismiyle dosyayı yaratır ve bilgileri yazar
void init_the_file(ofstream& file, superblock_init superBlock,
            INode_init inode_arr[], Directory_init Dir_Arr[],
            int totalSize, int blockSizeOfFile_KB, 
            int sizeOfEachBlock,int INodeCount, int directorySize,int remaining_directory_byte );


int main(int argc, char* argv[])
{   
    string fileName;
    int blockSizeOfFile_KB, numberOfFree_inode;
    int blockSizeOfFile;
    int maxEmptyINode = 0;

    if(argv[1] == NULL || argv[2] == NULL || argv[3] == NULL )
        return 1;   

    blockSizeOfFile_KB = atoi(argv[1]);
    numberOfFree_inode = atoi(argv[2]);
    fileName = argv[3]; 

    if(isdigit(fileName[0]))
    {
        cout<<"First character of the file name can not be integer!!"<<endl;
        return 1;
    }
    if(fileName.length() > MAX_FILE_NAME_SIZE)
    {
        cout<<"The size of the file name must be equal or less than "<<MAX_FILE_NAME_SIZE<<endl;
        return 1;
    }

    if(blockSizeOfFile_KB <= 0)
    {
        cout << "Block size of file can not be equal or less than 0!!"<<endl;
        exit(1); 
    }

    if( blockSizeOfFile_KB != 1 && blockSizeOfFile_KB != 2 && blockSizeOfFile_KB != 4 &&
        blockSizeOfFile_KB != 8 && blockSizeOfFile_KB != 16 )
    {
        cout<<"Your second input is wrong"<<endl;
        cout<<"Block size must be 1,2,4,8 or 16"<<endl;
        return 1;
    }

    if(numberOfFree_inode <= 0)
    {
        cout<<"Number of free can not be equal or less than "<<numberOfFree_inode<<endl;
        return 1;
    }

    maxEmptyINode = getNumberOfMaxINode(blockSizeOfFile_KB);
    if(maxEmptyINode == -1)
    {
        cout<<"Block size must be 1KB, 2KB, 4KB, 8KB or 16KB!!"<<endl;
        return 1;
    }

    if(numberOfFree_inode > maxEmptyINode)
    {
        cout<<endl<<endl<<"-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-"<<endl;
        cout<<endl<<"ATTENTION!!"<<endl<<endl;
        cout<<"The program can store maximum "<< maxEmptyINode <<" empty inodes"<<endl;
        cout<<"Your input is : "<<numberOfFree_inode<<endl;
        cout<<"The program will take your input as "<<maxEmptyINode<<endl;
        cout<<endl<<"-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-"<<endl;

        numberOfFree_inode = maxEmptyINode;
    }

    
    blockSizeOfFile = blockSizeOfFile_KB * pow(2,10);
    //cout<<"blockSizeOfFile (Byte): "<<blockSizeOfFile<<endl;

    superblock_init mySuperBlock(fileName, blockSizeOfFile_KB);
    INode_init INode_Arr[numberOfFree_inode];

    int directorySize = blockSizeOfFile / TOTALBYTE_DIRECTORY;
    int remaining_directory_Byte = blockSizeOfFile%TOTALBYTE_DIRECTORY;
    Directory_init Dir_Arr[directorySize];
    
    Dir_Arr[0].set_Dname("/         ");

    ofstream myFile(fileName);
    
    init_the_file(myFile,mySuperBlock,INode_Arr, Dir_Arr, M_BYTE_SIZE, blockSizeOfFile_KB, blockSizeOfFile, numberOfFree_inode, directorySize, remaining_directory_Byte );

    myFile.close();
    cout<<endl;
}

int return_numberOfBytes(int val,int max_size)
{
    int i,temp = 1;
    int res;
    for(i=0; i<=max_size; i++)
    {
        temp = temp*10;
        res =  val /temp;
        if(res <= 0 )
            return (i+1);
    }
    return max_size;
}

string appendZeroToStr(string input, int numberOfZero)
{
    int i;
    for(i=0; i<numberOfZero; i++)
        input.append("0");
    return input;
}
void intToStringWithZero(int value, string &myStr, int maxByteSize)
{   
    int bytesize;
    myStr = "";
    bytesize = return_numberOfBytes(value,maxByteSize);
    myStr = appendZeroToStr(myStr, (maxByteSize-bytesize));
    myStr.append(to_string(value));
}

void init_the_file(ofstream& file, superblock_init superBlock,
            INode_init inode_arr[], Directory_init Dir_Arr[],
            int totalSize, int blockSizeOfFile_KB, 
            int sizeOfEachBlock,int INodeCount, int directorySize,int remaining_directory_byte )
{
    int i,j,k;
    int numberOfBigINode=0;
    //superblock part
    //printing superblock taking 253 bytes
    int superblock_bytesize = superBlock.get_totalByte();
    int maxEmptyINode = getNumberOfMaxINode(blockSizeOfFile_KB);
    int numberOfMiniINode = getNumberOfEachInode(blockSizeOfFile_KB);
    file<<superBlock;

    for(i=superblock_bytesize; i<sizeOfEachBlock-1; i++)
        file<<'\0';
    file<<'\n';

    //each inode has 107 byte
    //INode part
    j=0;
    int zeroCount =0;
    for(i=0; i<INodeCount; i++)
    {
        file<<inode_arr[i];
        j++;
        if(j==numberOfMiniINode)
        {
            j=0;
            zeroCount = sizeOfEachBlock - numberOfMiniINode*107;
            for(k=0; k<zeroCount-1; k++)
                file<<'\0';
            file<<'\n';
            numberOfBigINode++;
        }
    }
    if(j>0)
        numberOfBigINode++;

    zeroCount = sizeOfEachBlock - j*TOTALBYTE_INODE;
    if(!(blockSizeOfFile_KB >= 8))
    {
        int notUsed_BigINodes = maxEmptyINode/numberOfMiniINode;
        if( (maxEmptyINode%numberOfMiniINode) > 0 )
            notUsed_BigINodes += 1;
        notUsed_BigINodes = notUsed_BigINodes - numberOfBigINode;
        zeroCount += notUsed_BigINodes*sizeOfEachBlock ;
    }

    if( !((blockSizeOfFile_KB >= 8) && (maxEmptyINode == INodeCount )))
    {
        for(i=0; i<zeroCount-1; i++)
            file<<'\0';
        file<<'\n';
    }

    //finished inode part

    //directory part
    
    for(i=0; i<directorySize; i++)
        file<<Dir_Arr[i];
    zeroCount = remaining_directory_byte;
    for(i=0; i<zeroCount-1; i++)
        file<<'\0';
    file<<'\n';

    //data blocks
    int totalDataBlock = maxEmptyINode*2;
    
    for(i =0; i<totalDataBlock; i++)
    {
        for(j=0; j<sizeOfEachBlock-1; j++)
            file<<'\0';
        file<<'\n';
    }

    

    cout<<"sizeOfEachBlock "<<sizeOfEachBlock;
    if( sizeOfEachBlock >= 8192  )
    {
        zeroCount = sizeOfEachBlock;
        for(i=0; i<zeroCount-1; i++)
            file<<'\0';
        file<<'\n';
    }
    

}

int getNumberOfMaxINode(int blockSize)
{
    switch (blockSize)
    {
    //1KB
    case 1:
        return 484;
    case 2:
        return 248;
    case 4:
        return 125;
    case 8:
        return 62;
    case 16:
        return 30;
    default:
        return -1;
    }


}


int getNumberOfEachInode(int blockSize)
{
    switch (blockSize)
    {
    //1KB
    case 1:
        return 9;
    case 2:
        return 19;
    case 4:
        return 38;
    case 8:
        return 62;
    case 16:
        return 30;
    default:
        return -1;
    }
}

int getMaxBigINode(int blockSize)
{
    switch (blockSize)
    {
    //1KB
    case 1:
        return 54;
    case 2:
        return 14;
    case 4:
        return 4;
    case 8:
        return 1;
    case 16:
        return 1;
    default:
        return -1;
    }
}


ostream& operator<<(ostream &os, superblock_init &input)
{
    os<<"File name: "<<input.get_fileName()<<endl;
    os<<"End of the file: "<<input.get_endOfTheFile()<<endl;
    os<<"Number of blocks: "<<input.get_numberOfBlocks()<<endl;
    os<<"Size of each block: "<<input.get_sizeOfEachBlock()<<endl;
    //os<<"Start adress of free space: "<<input.get_startaddress_FreeSpace()<<endl;
    os<<"Start address of inodes: "<<input.get_startaddress_INodes()<<endl;
    os<<"Start address of Root Directory: "<<input.get_startaddress_RootDir()<<endl;
    os<<"Start adress of data blocks: "<<input.get_startaddress_DataBlock()<<endl;
    return os;
}

ostream& operator<<(ostream &os, INode_init &input)
{
    os<<"validbit "<<input.get_validBit()<<endl;
    os<<"id "<<input.get_id()<<endl;
    os<<"filename "<<input.get_fileName()<<endl;
    os<<"filesize "<<input.get_fileSize()<<endl;
    os<<"data0 "<<input.get_data0()<<endl;
    os<<"data1 "<<input.get_data1()<<endl;
    os<<input.get_time()<<endl;
    return os;
}

ostream& operator<<(ostream &os, Directory_init &input)
{
    os<<"Dname "<<input.get_DName()<<endl;
    os<<"fileName0 "<<input.get_fileName0()<<endl;
    os<<"fileName1 "<<input.get_fileName1()<<endl;
    os<<"DAdress0 "<<input.get_DAdress0()<<endl;
    os<<"DAdress1 "<<input.get_DAdress1()<<endl;
    os<<"DAdress2 "<<input.get_DAdress2()<<endl;
    os<<input.get_date()<<endl;
    return os;
}


void superblock_init::print_superblock()
{
    cout<<endl<<"-o-o-o-o-o-o-o-o-o-o-o-o-o-o-"<<endl<<endl;
    cout<<"Super Block Informations"<<endl;
    cout<<"File name: "<<this->get_fileName()<<endl;
    cout<<"End of the file: "<<this->get_endOfTheFile()<<endl;
    cout<<"Number of blocks: "<<this->get_numberOfBlocks()<<endl;
    cout<<"Size of each block: "<<this->get_sizeOfEachBlock()<<endl;
    //cout<<"Start adress of free space: "<<this->get_startaddress_FreeSpace()<<endl;
    cout<<"Start address of inodes: "<<this->get_startaddress_INodes()<<endl;
    cout<<"Start address of Root Directory: "<<this->get_startaddress_RootDir()<<endl;
    cout<<"Start adress of data blocks: "<<this->get_startaddress_DataBlock()<<endl;
    cout<<endl<<"-o-o-o-o-o-o-o-o-o-o-o-o-o-o-"<<endl<<endl;
}

void INode_init::print_inode()
{
    cout<<endl<<"-o-o-o-o-o-o-o-o-o-o-o-o-o-o-"<<endl<<endl;
    cout<<"INode Informations"<<endl;
    cout<<"validbit "<<this->get_validBit()<<endl;
    cout<<"id "<<this->get_id()<<endl;
    cout<<"filename "<<this->get_fileName()<<endl;
    cout<<"filesize "<<this->get_fileSize()<<endl;
    cout<<"data0 "<<this->get_data0()<<endl;
    cout<<"data1 "<<this->get_data1()<<endl;
    cout<<this->get_time()<<endl;
    cout<<endl<<"-o-o-o-o-o-o-o-o-o-o-o-o-o-o-"<<endl<<endl;
}

