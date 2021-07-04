//Can BEYAZNAR
//161044038

#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <fstream>
#include <string.h>
#include <vector>
#include <sys/stat.h>
#define MAX_DIRECTORY_NAME_SIZE 10
#define MAX_FILE_NAME_SIZE 20
#define MAX_FILE_NAME_SIZE_DIR 16
#define BYTE_SIZE 1024
#define M_BYTE_SIZE 1024 * 1024
#define TOTALBYTE_INODE 107
#define TOTALBYTE_SUPERBLOCK 217
#define TOTALBYTE_DIRECTORY 148

using namespace std;

int getNumberOfMaxINode(int blockSize);

//it returns time informations as string
string returnTime()
{
    time_t t = time(NULL);
    tm *timePtr = localtime(&t);
    string date = "";

    if ((timePtr->tm_mon + 1) < 10)
        date.append("0");
    date.append(to_string(timePtr->tm_mon + 1));
    date.append(".");

    if ((timePtr->tm_mday) < 10)
        date.append("0");
    date.append(to_string(timePtr->tm_mday));
    date.append(".");

    int year = timePtr->tm_year + 1900;

    date.append(to_string((((year % 100) - (year % 10)) / 10)));
    date.append(to_string(year % 10));
    date.append(" ");

    if ((timePtr->tm_hour) < 10)
        date.append("0");
    date.append(to_string(timePtr->tm_hour));
    date.append(":");

    if ((timePtr->tm_min) < 10)
        date.append("0");
    date.append(to_string(timePtr->tm_min));
    date.append(":");

    if ((timePtr->tm_sec) < 10)
        date.append("0");
    date.append(to_string(timePtr->tm_sec));

    return date;
}

//returns the number of digits
int return_numberOfBytes(int val, int max_size)
{
    int i, temp = 1;
    int res;
    for (i = 0; i <= max_size; i++)
    {
        temp = temp * 10;
        res = val / temp;
        if (res <= 0)
            return (i + 1);
    }
    return max_size;
}

//String'e numberOfZero kadar '\0' atar
string appendZeroToStr(string input, int numberOfZero)
{
    int i;
    for (i = 0; i < numberOfZero; i++)
        input.append("0");
    return input;
}

//Değeri bir stringe atar ve geriye kalan karakterleri 0 ile doldurur
void intToStringWithZero(int value, string &myStr, int maxByteSize)
{
    int bytesize;
    myStr = "";
    bytesize = return_numberOfBytes(value, maxByteSize);
    myStr = appendZeroToStr(myStr, (maxByteSize - bytesize));
    myStr.append(to_string(value));
}

//Bir stringe belirtilen sayi kadar '?' karakterini atar
string appendMarkToStr(string input, int numberOfNull)
{
    int i;
    for (i = 0; i < numberOfNull; i++)
        input.append("?");
    return input;
}

class my_superblock
{

public:
    my_superblock()
    {
        fileName = "";
        endOfFile = 0;
        numberOfBlocks = 0;
        sizeOfEachBlock = 0;
        startaddress_inode = 0;
        startaddress_directory = 0;
        startaddress_dataBlock = 0;
    }

    void set_fileName(string x) { fileName = x; }
    void set_endOfFile(int x) { endOfFile = x; }
    void set_numberOfBlocks(int x) { numberOfBlocks = x; }
    void set_sizeOfEachBlock(int x) { sizeOfEachBlock = x; }
    void set_startaddress_inode(int x) { startaddress_inode = x; }
    void set_startaddress_directory(int x) { startaddress_directory = x; }
    void set_startaddress_dataBlock(int x) { startaddress_dataBlock = x; }

    string get_fileName() { return fileName; }
    int get_endOfFile() { return endOfFile; }
    int get_numberOfBlocks() { return numberOfBlocks; }
    int get_sizeOfEachBlock() { return sizeOfEachBlock; }
    int get_startaddress_inode() { return startaddress_inode; }
    int get_startaddress_directory() { return startaddress_directory; }
    int get_startaddress_dataBlock() { return startaddress_dataBlock; }

    friend ostream &operator<<(ostream &os, my_superblock &input);
    void print_superblock();

private:
    string fileName;
    int endOfFile;
    int numberOfBlocks;
    int sizeOfEachBlock;
    int startaddress_inode;
    int startaddress_directory;
    int startaddress_dataBlock;
};

class my_INode
{

public:
    /*
    int validBit;
    int id;
    string fileName;
    int fileSize;
    int data0;
    int data1;
    string time;
    */

    my_INode()
    {
        validBit = 2;
        id = 999;
        fileName = "";
        fileSize = 0;
        data0 = 0;
        data1 = 0;
        time = "00.00.00 00:00:00";
    }

    my_INode(const my_INode &copyINode)
    {
        validBit = copyINode.validBit;
        id = copyINode.id;
        fileName = copyINode.fileName;
        fileSize = copyINode.fileSize;
        data0 = copyINode.data0;
        data1 = copyINode.data1;
        time = copyINode.time;
        totalByte = copyINode.totalByte;
    }

    int get_validBit() { return validBit; }
    int get_id() { return id; }
    string get_fileName() { return fileName; }
    int get_fileSize() { return fileSize; }
    int get_data0() { return data0; }
    int get_data1() { return data1; }
    string get_time() { return time; }
    int get_totalByte() { return totalByte; }

    void set_validBit(int x) { validBit = x; }
    void set_id(int x) { id = x; }
    void set_fileName(string x) { fileName = x; }
    void set_fileSize(int x) { fileSize = x; }
    void set_data0(int x) { data0 = x; }
    void set_data1(int x) { data1 = x; }
    void set_time(string x) { time = x; }

    friend ostream &operator<<(ostream &os, my_INode &input);
    string return_str();
    void print_inode();

private:
    int validBit;
    int id;
    string fileName;
    int fileSize;
    int data0;
    int data1;
    string time;
    // total 107 byte
    int totalByte = 107;
};

class my_Directory
{

public:
    my_Directory()
    {
        Dname = "??????????";
        fileName0 = "????????????????????";
        fileName1 = "????????????????????";
        DAdress0 = 0;
        DAdress1 = 0;
        DAdress2 = 0;
        date = "00.00.00 00:00:00";
    }

    my_Directory(const my_Directory &copyDir)
    {
        Dname = copyDir.Dname;
        fileName0 = copyDir.fileName0;
        fileName1 = copyDir.fileName1;
        DAdress0 = copyDir.DAdress0;
        DAdress1 = copyDir.DAdress1;
        DAdress2 = copyDir.DAdress2;
        date = copyDir.date;
    }

    string get_DName()
    {
        return Dname;
    }
    string get_fileName0() { return fileName0; }
    string get_fileName1() { return fileName1; }
    int get_DAdress0() { return DAdress0; }
    int get_DAdress1() { return DAdress1; }
    int get_DAdress2() { return DAdress2; }
    string get_date() { return date; }

    void set_DName(string x) { Dname = x; }
    void set_fileName0(string x) { fileName0 = x; }
    void set_fileName1(string x) { fileName1 = x; }
    void set_DAdress0(int x) { DAdress0 = x; }
    void set_DAdress1(int x) { DAdress1 = x; }
    void set_DAdress2(int x) { DAdress2 = x; }
    void set_date(string x) { date = x; }

    friend ostream &operator<<(ostream &os, my_Directory &input);
    string return_str();

private:
    string Dname;
    string fileName0;
    string fileName1;
    int DAdress0;
    int DAdress1;
    int DAdress2;
    string date;
};

int getNumberOfEachInode(int blockSize);
int getMaxBigINode(int blockSize);

//Dosyadan superblock kismini okur ve superblock classina değerlerini atar
int read_superblock(FILE *file_read, my_superblock &superblock)
{

    char fileName_superBlock[MAX_FILE_NAME_SIZE];
    int endOfFile_SB, numberOfBlocks_SB, sizeOfEachBlock_SB;
    int s_addr_inode, s_addr_dir, s_addr_datab;

    fscanf(file_read, "File name: %s\nEnd of the file: %d\nNumber of blocks: %d\n",
           fileName_superBlock, &endOfFile_SB, &numberOfBlocks_SB);

    fscanf(file_read, "Size of each block: %d\nStart address of inodes: %d\n",
           &sizeOfEachBlock_SB, &s_addr_inode);

    fscanf(file_read, "Start address of Root Directory: %d\nStart adress of data blocks: %d ",
           &s_addr_dir, &s_addr_datab);

    superblock.set_fileName(fileName_superBlock);
    superblock.set_endOfFile(endOfFile_SB);
    superblock.set_numberOfBlocks(numberOfBlocks_SB);
    superblock.set_sizeOfEachBlock(sizeOfEachBlock_SB);
    superblock.set_startaddress_inode(s_addr_inode);
    superblock.set_startaddress_directory(s_addr_dir);
    superblock.set_startaddress_dataBlock(s_addr_datab);

    return 0;
}

//Dosyadan inode kismini okur ve INode_Arr arrayine değerlerini atar
void read_INodes(FILE *file_read, my_superblock superblock, my_INode INode_Arr[], int maxInode)
{
    int blockSize_KB = superblock.get_sizeOfEachBlock() / BYTE_SIZE;
    int eachMiniINode = getNumberOfEachInode(blockSize_KB);

    fseek(file_read, superblock.get_startaddress_inode(), SEEK_SET);

    int i, readed_MiniINode = 0, readed_BigINode = 0;
    int fseek_shift = superblock.get_startaddress_inode();
    int totalMiniINode = 0;

    int validbit_IN, id_IN, filesize_IN;

    /*
    char filename_IN[MAX_FILE_NAME_SIZE+1];
    char date1[MAX_FILE_NAME_SIZE+1];
    char date2[MAX_FILE_NAME_SIZE+1];
    */

    char *filename_IN = (char *)malloc(sizeof(char) * (MAX_FILE_NAME_SIZE + 1));
    char *date1 = (char *)malloc(sizeof(char) * (MAX_FILE_NAME_SIZE + 1));
    char *date2 = (char *)malloc(sizeof(char) * (MAX_FILE_NAME_SIZE + 1));

    string lastDate;
    int data0, data1;
    for (i = 0; i < maxInode; i++)
    {
        fscanf(file_read, "validbit %d\nid %d\nfilename %s\nfilesize %d\n",
               &validbit_IN, &id_IN, filename_IN, &filesize_IN);
        fscanf(file_read, "data0 %d\ndata1 %d\n%s %s\n", &data0, &data1, date1, date2);

        INode_Arr[i].set_validBit(validbit_IN);
        INode_Arr[i].set_id(id_IN);
        INode_Arr[i].set_fileName(filename_IN);
        INode_Arr[i].set_fileSize(filesize_IN);
        INode_Arr[i].set_data0(data0);
        INode_Arr[i].set_data1(data1);

        lastDate.append(date1);
        lastDate.append(" ");
        lastDate.append(date2);

        INode_Arr[i].set_time(lastDate);
        lastDate = "";

        readed_MiniINode++;
        if (readed_MiniINode == eachMiniINode)
        {
            int numberOfZeroByte = (blockSize_KB * 1024) - (eachMiniINode * TOTALBYTE_INODE);
            fseek_shift += numberOfZeroByte;
            readed_MiniINode = 0;
            readed_BigINode++;
        }

        if ((INode_Arr[i].get_id() == INode_Arr[i - 1].get_id()) && (i != 0))
        {
            INode_Arr[i] = my_INode();
            break;
        }

        totalMiniINode++;

        fseek_shift += TOTALBYTE_INODE;
        fseek(file_read, fseek_shift, SEEK_SET);
    }

    free(filename_IN);
    free(date1);
    free(date2);

}

//Dosyadan inode kismini okur ve Directory_Arr arrayine değerlerini atar
void read_Directories(FILE *file_read, my_superblock superblock, my_Directory Directory_Arr[], int maxDirectory)
{

    fseek(file_read, superblock.get_startaddress_directory(), SEEK_SET);
    int fseek_shift = superblock.get_startaddress_directory();
    int i = 0;
    char date1[MAX_FILE_NAME_SIZE + 1], date2[MAX_FILE_NAME_SIZE + 1];

    string lastDate = "";
    /*
    char directoryName[MAX_DIRECTORY_NAME_SIZE+1];
    char fileName0[MAX_FILE_NAME_SIZE+1];
    char fileName1[MAX_FILE_NAME_SIZE+1]; */

    char *directoryName = (char *)malloc(sizeof(char) * (MAX_DIRECTORY_NAME_SIZE + 1));
    char *fileName0 = (char *)malloc(sizeof(char) * (MAX_FILE_NAME_SIZE + 1));
    char *fileName1 = (char *)malloc(sizeof(char) * (MAX_FILE_NAME_SIZE + 1));

    for (i = 0; i < maxDirectory; i++)
    {

        int DAddress0 = 0, DAddress1 = 0, DAddress2 = 0;
        fscanf(file_read, "Dname %s\nfileName0 %s\nfileName1 %s\n", directoryName,
               fileName0, fileName1);
        fscanf(file_read, "DAdress0 %d\nDAdress1 %d\nDAdress2 %d\n%s %s\n",
               &DAddress0, &DAddress1, &DAddress2, date1, date2);
        string temp = " ";

        lastDate.append(date1);
        lastDate.append(" ");
        lastDate.append(date2);

        Directory_Arr[i].set_DName(directoryName);
        Directory_Arr[i].set_fileName0(fileName0);
        Directory_Arr[i].set_fileName1(fileName1);
        Directory_Arr[i].set_DAdress0(DAddress0);
        Directory_Arr[i].set_DAdress1(DAddress1);
        Directory_Arr[i].set_DAdress2(DAddress2);
        Directory_Arr[i].set_date(lastDate);
        lastDate = "";

        fseek_shift += TOTALBYTE_DIRECTORY;
        fseek(file_read, fseek_shift, SEEK_SET);
    }
    free(directoryName);
    free(fileName0);
    free(fileName1);
}


void write_Datas_To_SystemFile(string filesystem_name, string user_fileinput, int user_fileSize, int fseek_start)
{
    FILE *myfileSystem = fopen(filesystem_name.c_str(), "r+");
    if (myfileSystem == NULL)
    {
        cout << filesystem_name << " file can not open!!" << endl;
        exit(1);
    }

    FILE *user_file = fopen(user_fileinput.c_str(), "rb");
    fseek(user_file, 0, SEEK_SET);

    if (user_fileSize <= 0)
    {
        fclose(user_file);
        fclose(myfileSystem);
        return;
    }

    char *buffer = (char *)malloc(sizeof(char) * user_fileSize);
    fread(buffer, sizeof(char), user_fileSize, user_file);

    fseek(myfileSystem, fseek_start, SEEK_SET);
    fprintf(myfileSystem, "%s", buffer);

    free(buffer);
    fclose(user_file);
    fclose(myfileSystem);
    return;
}

//Dosya sisteminin içinde bulunan data blocklarindaki verileri user_fileinput dosyasina yazar
// fseek_start  parametresini alarak filesystemde oraya gider ve byte_size_write kadar byte i user_fileinput dosyasina yazar
void write_Datas_To_InputFile(string filesystem_name, string user_fileinput, int byte_size_write, int fseek_start)
{
    FILE *filesystem = fopen(filesystem_name.c_str(), "r");
    FILE *user_file = fopen(user_fileinput.c_str(), "w+");

    if (byte_size_write <= 0)
    {
        fclose(user_file);
        fclose(filesystem);
    }

    char *writeToUserFile = (char *)malloc(sizeof(char) * (byte_size_write + 1));
    fseek(filesystem, fseek_start, SEEK_SET);
    fread(writeToUserFile, sizeof(char), byte_size_write, filesystem);

    fseek(user_file, 0, SEEK_SET);
    fprintf(user_file, "%s", writeToUserFile);

    free(writeToUserFile);
    fclose(user_file);
    fclose(filesystem);
}

// Dosya sisteminde belirtilen kismi '\0' karakteri ile doldurur
// Bu fonksiyon del operasyonu için yardimci fonksiyondur.
// Data blocklarin içindeki verileri silerken '\0' karakteri yazar
void fill_zero_filesystem(string filesystem_name, int eachTime, int totalByte, int fseek_start)
{
    fstream filesystem;
    filesystem.open(filesystem_name);
    filesystem.seekp(fseek_start);
    int i, j;
    for (i = 0; i < eachTime; i++)
    {
        for (j = 0; j < totalByte - 1; j++)
            filesystem.put('\0');
        filesystem.put('\n');
    }
    filesystem.close();
}

// INode entrylerinin guncellenmesinde kullanilir
// Dosya sisteminde belirtilen indexe gidilir ve belirtilen inode entrysi guncellenir 
void update_inode(string fileName, my_superblock superblock, my_INode INode_arr[], int index)
{
    FILE *file_write = fopen(fileName.c_str(), "r+");
    int inode_start_adress = superblock.get_startaddress_inode();
    int fseek_shift = index * TOTALBYTE_INODE + inode_start_adress;

    fseek(file_write, fseek_shift, SEEK_SET);

    string printToFile = INode_arr[index].return_str();
    fprintf(file_write, "%s", printToFile.c_str());

    fclose(file_write);
}

// directory entrylerinin guncellenmesinde kullanilir
// Dosya sisteminde belirtilen indexe gidilir ve belirtilen directory entrysi guncellenir 
void update_directory(string fileName, my_superblock superblock, my_Directory Directory_arr[], int index)
{
    FILE *file_write = fopen(fileName.c_str(), "r+");
    int directory_start_address = superblock.get_startaddress_directory();
    int fseek_shift = index * TOTALBYTE_DIRECTORY + directory_start_address;

    fseek(file_write, fseek_shift, SEEK_SET);

    string printToFile = Directory_arr[index].return_str();
    fprintf(file_write, "%s", printToFile.c_str());

    fclose(file_write);
}

// Kullanicinin verdiği path'in sonuna recursive yolu ile gider ve yeni bir klasor ekler
// Eğer bu islem basarili sonuçlanirsa geri giderek her bir bağlantili klasorun date'i guncellenir.
int add_directory_recursive(string fileName, my_superblock superblock, int currentDirectory,
                            my_Directory directoryArr[], int directorySize,
                            vector<string> dirNames, int pathCount, int maxDirectory)
{

    if (pathCount != (int)dirNames.size() - 1)
    {
        int dadress0 = directoryArr[currentDirectory].get_DAdress0();
        int dadress1 = directoryArr[currentDirectory].get_DAdress1();
        int dadress2 = directoryArr[currentDirectory].get_DAdress2();

        string dname0, dname1, dname2;

        dname0 = directoryArr[dadress0].get_DName();
        dname0 = dname0.substr(0, dname0.find("?"));

        dname1 = directoryArr[dadress1].get_DName();
        dname1 = dname1.substr(0, dname1.find("?"));

        dname2 = directoryArr[dadress2].get_DName();
        dname2 = dname2.substr(0, dname2.find("?"));
        int control = -1;
        // /usr/can/bin
        if (dirNames.at(pathCount) == dname0)
        {
            control = add_directory_recursive(fileName, superblock, dadress0, directoryArr,
                                              directorySize, dirNames, (pathCount + 1), maxDirectory);

            if (control > 0)
            {
                //directoryArr[dadress0].set_DAdress0(control);
                directoryArr[dadress0].set_date(returnTime());
                update_directory(fileName, superblock, directoryArr, dadress0);

                return 1;
            }
            return -1;
        }
        else if (dirNames.at(pathCount) == dname1)
        {
            control = add_directory_recursive(fileName, superblock, dadress1, directoryArr,
                                              directorySize, dirNames, (pathCount + 1), maxDirectory);
            if (control > 0)
            {
                //directoryArr[dadress1].set_DAdress0(control);
                directoryArr[dadress1].set_date(returnTime());
                update_directory(fileName, superblock, directoryArr, dadress1);

                return 1;
            }
            return -1;
        }
        else if (dirNames.at(pathCount) == dname2)
        {
            control = add_directory_recursive(fileName, superblock, dadress2, directoryArr,
                                              directorySize, dirNames, (pathCount + 1), maxDirectory);
            if (control > 0)
            {
                //directoryArr[dadress2].set_DAdress0(control);
                directoryArr[dadress2].set_date(returnTime());
                update_directory(fileName, superblock, directoryArr, dadress2);

                return 1;
            }
            return -1;
        }

        else
        {
            cout << dirNames.at(pathCount) << " directory is not exist!!" << endl;
            return -1;
        }
    }
    else
    {
        string directoryName_create = dirNames.at(directorySize - 1);

        int j = 0;
        for (j = currentDirectory; j < maxDirectory; j++)
        {
            if (directoryArr[j].get_DName() == "??????????")
                break;
        }
        if (directoryArr[j].get_DName() != "??????????")
        {
            cout << "I can not create your directory. There is no empty directory!!" << endl;
            return -1;
        }

        int dadress0 = directoryArr[currentDirectory].get_DAdress0();
        int dadress1 = directoryArr[currentDirectory].get_DAdress1();
        int dadress2 = directoryArr[currentDirectory].get_DAdress2();

        string dname0, dname1, dname2;

        dname0 = directoryArr[dadress0].get_DName();
        dname0 = dname0.substr(0, dname0.find("?"));

        dname1 = directoryArr[dadress1].get_DName();
        dname1 = dname1.substr(0, dname1.find("?"));

        dname2 = directoryArr[dadress2].get_DName();
        dname2 = dname2.substr(0, dname2.find("?"));

        if (dname0 == directoryName_create || dname1 == directoryName_create || dname2 == directoryName_create)
        {
            cout << dname0 << " is already exist in this directory!!" << endl;
            exit(1);
        }

        if (dadress0 == 0)
        {
            directoryArr[j].set_DName(directoryName_create);
            directoryArr[j].set_date(returnTime());
            update_directory(fileName, superblock, directoryArr, j);

            directoryArr[currentDirectory].set_DAdress0(j);
            update_directory(fileName, superblock, directoryArr, currentDirectory);
            return 1;
        }
        else if (dadress1 == 0)
        {
            directoryArr[j].set_DName(directoryName_create);
            directoryArr[j].set_date(returnTime());
            update_directory(fileName, superblock, directoryArr, j);

            directoryArr[currentDirectory].set_DAdress1(j);
            update_directory(fileName, superblock, directoryArr, currentDirectory);
            return 1;
        }
        else if (dadress2 == 0)
        {
            directoryArr[j].set_DName(directoryName_create);
            directoryArr[j].set_date(returnTime());
            update_directory(fileName, superblock, directoryArr, j);

            directoryArr[currentDirectory].set_DAdress2(j);
            update_directory(fileName, superblock, directoryArr, currentDirectory);
            return 1;
        }
        else
        {
            string directoryTemp = directoryArr[currentDirectory].get_DName();
            directoryTemp = directoryTemp.substr(0, directoryTemp.find("?"));
            cout << directoryTemp << " do not have enough space!!" << endl;
            return -1;
        }
    }
    return -1;
}

// Kullanicinin verdiği path'in sonuna recursive yolu ile gider ve klasor silinir
// Eğer bu islem basarili sonuçlanirsa geri giderek her bir bağlantili klasorun date'i guncellenir.
int remove_directory_recursive(string fileName, my_superblock superblock, int currentDirectory,
                               my_Directory directoryArr[], int directorySize,
                               vector<string> dirNames, int pathCount, int maxDirectory)
{
    if (pathCount != (int)dirNames.size() - 1)
    {
        int dadress0 = directoryArr[currentDirectory].get_DAdress0();
        int dadress1 = directoryArr[currentDirectory].get_DAdress1();
        int dadress2 = directoryArr[currentDirectory].get_DAdress2();

        string dname0, dname1, dname2;

        dname0 = directoryArr[dadress0].get_DName();
        dname0 = dname0.substr(0, dname0.find("?"));

        dname1 = directoryArr[dadress1].get_DName();
        dname1 = dname1.substr(0, dname1.find("?"));

        dname2 = directoryArr[dadress2].get_DName();
        dname2 = dname2.substr(0, dname2.find("?"));
        int control = -1;
        // /usr/can/bin
        if (dirNames.at(pathCount) == dname0)
        {
            control = remove_directory_recursive(fileName, superblock, dadress0, directoryArr,
                                                 directorySize, dirNames, (pathCount + 1), maxDirectory);

            if (control > 0)
            {
                directoryArr[currentDirectory].set_date(returnTime());
                update_directory(fileName, superblock, directoryArr, dadress0);
                return 1;
            }
            return -1;
        }
        else if (dirNames.at(pathCount) == dname1)
        {
            control = remove_directory_recursive(fileName, superblock, dadress1, directoryArr,
                                                 directorySize, dirNames, (pathCount + 1), maxDirectory);
            if (control > 0)
            {
                directoryArr[currentDirectory].set_date(returnTime());
                update_directory(fileName, superblock, directoryArr, dadress1);
                return 1;
            }
            return -1;
        }
        else if (dirNames.at(pathCount) == dname2)
        {
            control = remove_directory_recursive(fileName, superblock, dadress2, directoryArr,
                                                 directorySize, dirNames, (pathCount + 1), maxDirectory);
            if (control > 0)
            {
                directoryArr[currentDirectory].set_date(returnTime());
                update_directory(fileName, superblock, directoryArr, dadress2);
                return 1;
            }
            return -1;
        }

        else
        {
            cout << dirNames.at(pathCount) << " directory is not exist!!" << endl;
            return -1;
        }
    }
    else
    {

        string currentDirectory_Name = directoryArr[currentDirectory].get_DName();
        currentDirectory_Name = currentDirectory_Name.substr(0, currentDirectory_Name.find("?"));

        string userDirectoryName = dirNames.at(pathCount);

        int dadress0 = directoryArr[currentDirectory].get_DAdress0();
        int dadress1 = directoryArr[currentDirectory].get_DAdress1();
        int dadress2 = directoryArr[currentDirectory].get_DAdress2();

        string dname0, dname1, dname2;

        dname0 = directoryArr[dadress0].get_DName();
        dname0 = dname0.substr(0, dname0.find("?"));

        dname1 = directoryArr[dadress1].get_DName();
        dname1 = dname1.substr(0, dname1.find("?"));

        dname2 = directoryArr[dadress2].get_DName();
        dname2 = dname2.substr(0, dname2.find("?"));

        if (dname0 == userDirectoryName)
        {
            int new_dadress0 = directoryArr[dadress0].get_DAdress0();
            int new_dadress1 = directoryArr[dadress0].get_DAdress1();
            int new_dadress2 = directoryArr[dadress0].get_DAdress2();

            string fileName0 = directoryArr[dadress0].get_fileName0();
            string fileName1 = directoryArr[dadress0].get_fileName0();

            if (new_dadress0 != 0 || new_dadress1 != 0 || new_dadress2 != 0)
            {
                cout << userDirectoryName << " directory is not empty. You can not delete it" << endl;
                return -1;
            }
            if (fileName0 != "????????????????????" || fileName1 != "????????????????????")
            {
                cout << userDirectoryName << " directory is not empty. You can not delete it" << endl;
                return -1;
            }
            directoryArr[dadress0] = my_Directory();
            update_directory(fileName, superblock, directoryArr, dadress0);

            directoryArr[currentDirectory].set_DAdress0(0);
            directoryArr[currentDirectory].set_date(returnTime());
            update_directory(fileName, superblock, directoryArr, currentDirectory);

            return 1;
        }
        else if (dname1 == userDirectoryName)
        {
            int new_dadress0 = directoryArr[dadress1].get_DAdress0();
            int new_dadress1 = directoryArr[dadress1].get_DAdress1();
            int new_dadress2 = directoryArr[dadress1].get_DAdress2();

            string fileName0 = directoryArr[dadress1].get_fileName0();
            string fileName1 = directoryArr[dadress1].get_fileName0();

            if (new_dadress0 != 0 || new_dadress1 != 0 || new_dadress2 != 0)
            {
                cout << userDirectoryName << " directory is not empty. You can not delete it" << endl;
                return -1;
            }
            if (fileName0 != "????????????????????" || fileName1 != "????????????????????")
            {
                cout << userDirectoryName << " directory is not empty. You can not delete it" << endl;
                return -1;
            }

            directoryArr[dadress1] = my_Directory();
            update_directory(fileName, superblock, directoryArr, dadress1);

            directoryArr[currentDirectory].set_DAdress1(0);
            directoryArr[currentDirectory].set_date(returnTime());
            update_directory(fileName, superblock, directoryArr, currentDirectory);
            return 1;
        }

        else if (dname2 == userDirectoryName)
        {
            int new_dadress0 = directoryArr[dadress2].get_DAdress0();
            int new_dadress1 = directoryArr[dadress2].get_DAdress1();
            int new_dadress2 = directoryArr[dadress2].get_DAdress2();

            string fileName0 = directoryArr[dadress2].get_fileName0();
            string fileName1 = directoryArr[dadress2].get_fileName0();

            if (new_dadress0 != 0 || new_dadress1 != 0 || new_dadress2 != 0)
            {
                cout << userDirectoryName << " directory is not empty. You can not delete it" << endl;
                return -1;
            }
            if (fileName0 != "????????????????????" || fileName1 != "????????????????????")
            {
                cout << userDirectoryName << " directory is not empty. You can not delete it" << endl;
                return -1;
            }

            directoryArr[dadress2] = my_Directory();
            update_directory(fileName, superblock, directoryArr, dadress2);

            directoryArr[currentDirectory].set_DAdress2(0);
            directoryArr[currentDirectory].set_date(returnTime());
            update_directory(fileName, superblock, directoryArr, currentDirectory);

            return 1;
        }
        else
        {
            cout << userDirectoryName << " directory is not exist" << endl;
            return -1;
        }
    }
    return -1;
}


// Kullanicinin verdiği path'te gezerek bu directorylerin hangi indexlerde olduğunu dondurur.
// ve bu vector'u return eder.
vector<int> returnDirIndexes(vector<string> dirNames, my_superblock superblock, my_Directory directoryArr[])
{
    vector<int> result;
    int i;
    int dadress0 = 0, dadress1 = 0, dadress2 = 0;
    string dname0, dname1, dname2;

    int directorySize = dirNames.size();

    int pathCount = 0;
    int currentDirectory = 0;
    for (i = 0; i < directorySize - 1; i++)
    {
        dadress0 = directoryArr[currentDirectory].get_DAdress0();
        dadress1 = directoryArr[currentDirectory].get_DAdress1();
        dadress2 = directoryArr[currentDirectory].get_DAdress2();
        // /usr/ysa
        dname0 = directoryArr[dadress0].get_DName();
        dname0 = dname0.substr(0, dname0.find("?"));
        dname1 = directoryArr[dadress1].get_DName();
        dname1 = dname1.substr(0, dname1.find("?"));
        dname2 = directoryArr[dadress2].get_DName();
        dname2 = dname2.substr(0, dname2.find("?"));

        if (dname0 == dirNames.at(pathCount))
        {
            pathCount++;
            result.push_back(dadress0);
            currentDirectory = dadress0;
        }
        else if (dname1 == dirNames.at(pathCount))
        {
            pathCount++;
            result.push_back(dadress1);
            currentDirectory = dadress1;
        }
        else if (dname2 == dirNames.at(pathCount))
        {
            pathCount++;
            result.push_back(dadress2);
            currentDirectory = dadress2;
        }
        else
        {
            return result;
        }
            
    }
    return result;
}

// Bu fonksiyon returnDirIndexes fonksiyonu ile ayni islevi gorur.
// sadece list operasyonu için ufak bir değisiklik yapilmistir.
vector<int> returnDirIndexes_listop(vector<string> dirNames, my_superblock superblock, my_Directory directoryArr[])
{
    vector<int> result;
    int i;
    int dadress0 = 0, dadress1 = 0, dadress2 = 0;
    string dname0, dname1, dname2;

    int directorySize = dirNames.size();

    int pathCount = 0;
    int currentDirectory = 0;
    for (i = 0; i < directorySize ; i++)
    {
        dadress0 = directoryArr[currentDirectory].get_DAdress0();
        dadress1 = directoryArr[currentDirectory].get_DAdress1();
        dadress2 = directoryArr[currentDirectory].get_DAdress2();
        // /usr/ysa
        dname0 = directoryArr[dadress0].get_DName();
        dname0 = dname0.substr(0, dname0.find("?"));
        dname1 = directoryArr[dadress1].get_DName();
        dname1 = dname1.substr(0, dname1.find("?"));
        dname2 = directoryArr[dadress2].get_DName();
        dname2 = dname2.substr(0, dname2.find("?"));

        if (dname0 == dirNames.at(pathCount))
        {
            pathCount++;
            result.push_back(dadress0);
            currentDirectory = dadress0;
        }
        else if (dname1 == dirNames.at(pathCount))
        {
            pathCount++;
            result.push_back(dadress1);
            currentDirectory = dadress1;
        }
        else if (dname2 == dirNames.at(pathCount))
        {
            pathCount++;
            result.push_back(dadress2);
            currentDirectory = dadress2;
        }
        else
        {
            return result;
        }
            
    }
    return result;
}

//it returns first index of found char
//returns -1 if this charecter does not exit in str
int findCharecterinStr(string _str, char character )
{
    int i, strlength = _str.length();
    for(i=0; i<strlength; i++)
    {
        if(_str[i] == character)
            return i;
    }
    return -1;
}

// stringi verilmis olan karaktere gore parse eder
vector<string> parse_string(char *char_Arr, const char *parseCharacter)
{
    vector<string> vect_parse;
    char *eachName = strtok(char_Arr, parseCharacter);
    while (eachName != NULL)
    {
        vect_parse.push_back(eachName);
        eachName = strtok(NULL, parseCharacter);
    }
    return vect_parse;
}

// Ilk bos inode un index'ini return eder
int getEmptyINode(my_INode INode_Arr[], int inode_size)
{
    int i;
    for (i = 0; i < inode_size; i++)
    {
        if (INode_Arr[i].get_validBit() == 0)
            return i;
    }
    return -1;
}

// 
int getFileSizeOfDatablock(string filesystem_name, int start, int end)
{
    fstream filesystem;
    filesystem.open(filesystem_name);
    filesystem.seekp(start);

    int counter = 0;
    int i;
    char ch;
    for (i = start; i < end; i++)
    {
        filesystem.get(ch);
        if (ch == '\0')
            return counter;
        counter++;
    }
    return counter++;
}

string get_inode_newfileName_in_directory(my_Directory directory[],int maxDirectory ,int INode_index)
{
    int i;
    for(i=0; i<maxDirectory; i++)
    {
        string tempFileName = "";
        string tempINodeIndex_str;
        int tempINodeIndex;
        tempFileName = directory[i].get_fileName0();
        tempINodeIndex_str = tempFileName.substr(tempFileName.length()-3,tempFileName.length());
        tempINodeIndex = atoi(tempINodeIndex_str.c_str());
        if(tempINodeIndex == INode_index)
        {
            tempFileName = tempFileName.substr(0,tempFileName.find("?"));
            return tempFileName;
        }

        tempFileName = directory[i].get_fileName1();
        tempINodeIndex_str = tempFileName.substr(tempFileName.length()-3,tempFileName.length());
        tempINodeIndex = atoi(tempINodeIndex_str.c_str());
        if(tempINodeIndex == INode_index)
        {
            tempFileName = tempFileName.substr(0,tempFileName.find("?"));
            return tempFileName;
        }

    }
    return NULL;
}

// list operation
void list_op(string fileName, vector<string> dirNames, my_superblock superblock,
             my_Directory Directory_arr[], my_INode INode_arr[], int maxINode)
{
    int dirNames_size = dirNames.size();
    string read_FileName_input = dirNames.at(dirNames_size - 1);

    vector<int> pathIndex = returnDirIndexes_listop(dirNames, superblock, Directory_arr);
    int pathSize = pathIndex.size();

    if((int)(dirNames.size() ) != pathSize)
    {
        cout<<"Your path information is wrong"<<endl;
        exit(1);
    }

    if(pathSize == 0)
        return;
    int lastDirIndex = pathIndex[pathSize - 1];

    string filename0 = Directory_arr[lastDirIndex].get_fileName0();
    string inode_index_str0 = filename0.substr(filename0.length() - 3, filename0.length());

    filename0 = filename0.substr(0, filename0.find("?"));

    string filename1 = Directory_arr[lastDirIndex].get_fileName1();
    string inode_index_str1 = filename1.substr(filename1.length() - 3, filename1.length());

    filename1 = filename1.substr(0, filename1.find("?"));
    int totalByte = 0;
    string empty = "";
    printf("%12sName\t%sByte Size\t%sDate\n", empty.c_str(), empty.c_str(), empty.c_str());

    if (filename0 != "" && inode_index_str0 != "")
    {
        int inode_index_0 = atoi(inode_index_str0.c_str());
        string fileTime = INode_arr[inode_index_0].get_time();
        int data0 = INode_arr[inode_index_0].get_data0();
        int data1 = INode_arr[inode_index_0].get_data1();
        int sizeOfEachBlock = superblock.get_sizeOfEachBlock();
        int inode_datablock_byte_size = getFileSizeOfDatablock(fileName, data0, data1 + sizeOfEachBlock);
        totalByte += inode_datablock_byte_size;
        printf("%16s\t%7d\t%20s\n", filename0.c_str(), inode_datablock_byte_size, fileTime.c_str());
    }

    if (filename1 != "" && inode_index_str1 != "")
    {
        int inode_index_1 = atoi(inode_index_str1.c_str());
        string fileTime = INode_arr[inode_index_1].get_time();
        int data0 = INode_arr[inode_index_1].get_data0();
        int data1 = INode_arr[inode_index_1].get_data1();
        int sizeOfEachBlock = superblock.get_sizeOfEachBlock();
        int inode_datablock_byte_size = getFileSizeOfDatablock(fileName, data0, data1 + sizeOfEachBlock);
        totalByte += inode_datablock_byte_size;
        printf("%16s\t%7d\t%20s\n", filename1.c_str(), inode_datablock_byte_size, fileTime.c_str());
    }

    int Dadress0 = Directory_arr[lastDirIndex].get_DAdress0();
    int Dadress1 = Directory_arr[lastDirIndex].get_DAdress1();
    int Dadress2 = Directory_arr[lastDirIndex].get_DAdress2();

    string dname0 = Directory_arr[Dadress0].get_DName();
    dname0 = dname0.substr(0, dname0.find("?"));
    string dname1 = Directory_arr[Dadress1].get_DName();
    dname1 = dname1.substr(0, dname1.find("?"));
    string dname2 = Directory_arr[Dadress2].get_DName();
    dname2 = dname2.substr(0, dname2.find("?"));

    int sizeOfEachBlock = superblock.get_sizeOfEachBlock();
    if (Dadress0 != 0 && dname0 != "")
    {
        printf("%16s\t%7d\t%20s\n", dname0.c_str(), sizeOfEachBlock, Directory_arr[Dadress0].get_date().c_str());
        totalByte += sizeOfEachBlock;
    }
    if (Dadress1 != 0 && dname1 != "")
    {
        printf("%16s\t%7d\t%20s\n", dname1.c_str(), sizeOfEachBlock, Directory_arr[Dadress1].get_date().c_str());
        totalByte += sizeOfEachBlock;
    }
    if (Dadress2 != 0 && dname2 != "")
    {
        printf("%16s\t%7d\t%20s\n", dname2.c_str(), sizeOfEachBlock, Directory_arr[Dadress2].get_date().c_str());
        totalByte += sizeOfEachBlock;
    }
    int totalByte_KB = totalByte / BYTE_SIZE;
    cout << "Total " << totalByte_KB << " KB" << endl;
    return;
}

// mkdir operation
void mkdir_op(string fileName, vector<string> dirNames, my_superblock superblock,
              my_Directory Directory_arr[], int maxDirectory)
{
    int i;
    int directorySize = (int)dirNames.size();
    if (dirNames.at(directorySize - 1).size() > MAX_DIRECTORY_NAME_SIZE)
    {
        cout << "Your directory name must at most " << MAX_DIRECTORY_NAME_SIZE << " bytes ";
        cout << "at least 1 byte" << endl;
        return;
    }

    if (directorySize == 0)
    {
        cout << "Please give a path " << endl;
        exit(1);
    }

    if (directorySize == 1)
    {
        int DirAdress0 = Directory_arr[0].get_DAdress0();
        int DirAdress1 = Directory_arr[0].get_DAdress1();
        int DirAdress2 = Directory_arr[0].get_DAdress2();
        string directoryName = dirNames.at(0);
        if (DirAdress0 > 0 && DirAdress1 > 0 && DirAdress2 > 0)
        {
            string tempdirName = Directory_arr[0].get_DName();
            string tempDir = tempdirName.substr(0, tempdirName.find("?"));

            cout << tempDir << " do not have enough directory space" << endl;
            cout << "Maximum directory space is 3" << endl;

            exit(1);
        }

        string dname0 = Directory_arr[DirAdress0].get_DName();
        dname0 = dname0.substr(0, dname0.find("?"));

        string dname1 = Directory_arr[DirAdress1].get_DName();
        dname1 = dname1.substr(0, dname1.find("?"));

        string dname2 = Directory_arr[DirAdress1].get_DName();
        dname2 = dname2.substr(0, dname2.find("?"));

        if (dname0 == directoryName || dname1 == directoryName || dname2 == directoryName)
        {
            string tempdirName = Directory_arr[0].get_DName();
            string tempDir = tempdirName.substr(0, tempdirName.find("?"));
            cout << "This directory is already exist in " << tempDir << " directory!!" << endl;
            exit(1);
        }

        for (i = 1; i < maxDirectory; i++)
        {
            if (Directory_arr[i].get_DName() == "??????????")
                break;
        }
        if (Directory_arr[i].get_DName() != "??????????")
        {
            cout << "I can not create your directory. There is no empty directory!!" << endl;
            exit(1);
        }

        Directory_arr[i].set_DName(directoryName);
        Directory_arr[i].set_date(returnTime());

        update_directory(fileName, superblock, Directory_arr, i);

        if (Directory_arr[0].get_DAdress0() == 0)
            Directory_arr[0].set_DAdress0(i);

        else if (Directory_arr[0].get_DAdress1() == 0)
            Directory_arr[0].set_DAdress1(i);

        else if (Directory_arr[0].get_DAdress2() == 0)
            Directory_arr[0].set_DAdress2(i);

        else
        {
            cout << "UNEXPECTED ERROR!!!" << endl;
            exit(1);
        }
        update_directory(fileName, superblock, Directory_arr, 0);

        Directory_arr[i].return_str();
    }

    else
    {
        int currentDirectory = 0;
        add_directory_recursive(fileName, superblock, currentDirectory, Directory_arr,
                                directorySize, dirNames, 0, maxDirectory);
    }
}

// rmdir operation
void rmdir_op(string fileName, vector<string> dirNames, my_superblock superblock,
              my_Directory Directory_arr[], int maxDirectory)
{
    int directorySize = (int)dirNames.size();

    if (directorySize == 0)
    {
        cout << "Please give a path " << endl;
        cout << "You can not remove root directory!!" << endl;
        exit(1);
    }

    if (directorySize == 1)
    {

        int DirAdress0 = Directory_arr[0].get_DAdress0();
        int DirAdress1 = Directory_arr[0].get_DAdress1();
        int DirAdress2 = Directory_arr[0].get_DAdress2();

        if (DirAdress0 == 0 && DirAdress1 == 0 && DirAdress2 == 0)
        {
            cout << "This directory does not exist!!" << endl;
            return;
        }

        string user_Dname = dirNames.at(0);

        string dname0 = Directory_arr[DirAdress0].get_DName();
        dname0 = dname0.substr(0, dname0.find("?"));

        string dname1 = Directory_arr[DirAdress1].get_DName();
        dname1 = dname1.substr(0, dname1.find("?"));

        string dname2 = Directory_arr[DirAdress1].get_DName();
        dname2 = dname2.substr(0, dname2.find("?"));

        if (user_Dname == dname0)
        {
            int new_DirAdress0 = Directory_arr[DirAdress0].get_DAdress0();
            int new_DirAdress1 = Directory_arr[DirAdress0].get_DAdress1();
            int new_DirAdress2 = Directory_arr[DirAdress0].get_DAdress2();

            string fileName0 = Directory_arr[DirAdress0].get_fileName0();
            string fileName1 = Directory_arr[DirAdress0].get_fileName1();

            if (new_DirAdress0 > 0 || new_DirAdress1 > 0 || new_DirAdress2 > 0)
            {
                cout << "This directory is not empty. You can not remove it!!" << endl;
                return;
            }
            if (fileName0 != "????????????????????" || fileName1 != "????????????????????")
            {
                cout << "This directory is not empty. You can not remove it!!" << endl;
                return;
            }

            Directory_arr[DirAdress0] = my_Directory();
            update_directory(fileName, superblock, Directory_arr, DirAdress0);

            Directory_arr[0].set_DAdress0(0);
            update_directory(fileName, superblock, Directory_arr, 0);
        }
        else if (user_Dname == dname1)
        {
            int new_DirAdress0 = Directory_arr[DirAdress1].get_DAdress0();
            int new_DirAdress1 = Directory_arr[DirAdress1].get_DAdress1();
            int new_DirAdress2 = Directory_arr[DirAdress1].get_DAdress2();

            string fileName0 = Directory_arr[DirAdress1].get_fileName0();
            string fileName1 = Directory_arr[DirAdress1].get_fileName1();

            if (new_DirAdress0 > 0 || new_DirAdress1 > 0 || new_DirAdress2 > 0)
            {
                cout << "This directory is not empty. You can not remove it!!" << endl;
                return;
            }
            if (fileName0 != "????????????????????" || fileName1 != "????????????????????")
            {
                cout << "This directory is not empty. You can not remove it!!" << endl;
                return;
            }

            Directory_arr[DirAdress1] = my_Directory();
            update_directory(fileName, superblock, Directory_arr, DirAdress1);

            Directory_arr[0].set_DAdress1(0);
            update_directory(fileName, superblock, Directory_arr, 0);
        }
        else if (user_Dname == dname2)
        {
            int new_DirAdress0 = Directory_arr[DirAdress2].get_DAdress0();
            int new_DirAdress1 = Directory_arr[DirAdress2].get_DAdress1();
            int new_DirAdress2 = Directory_arr[DirAdress2].get_DAdress2();

            string fileName0 = Directory_arr[DirAdress2].get_fileName0();
            string fileName1 = Directory_arr[DirAdress2].get_fileName1();

            if (new_DirAdress0 > 0 || new_DirAdress1 > 0 || new_DirAdress2 > 0)
            {
                cout << "This directory is not empty. You can not remove it!!" << endl;
                return;
            }
            if (fileName0 != "????????????????????" || fileName1 != "????????????????????")
            {
                cout << "This directory is not empty. You can not remove it!!" << endl;
                return;
            }

            Directory_arr[DirAdress2] = my_Directory();
            update_directory(fileName, superblock, Directory_arr, DirAdress2);

            Directory_arr[0].set_DAdress2(0);
            update_directory(fileName, superblock, Directory_arr, 0);
        }

        else
        {
            cout << "This directory does not exist!!" << endl;
            return;
        }
    }

    else
    {
        int currentDirectory = 0;
        remove_directory_recursive(fileName, superblock, currentDirectory, Directory_arr,
                                   directorySize, dirNames, 0, maxDirectory);
    }
}

// dumpe2fs operation
void dumpe2fs_op(my_superblock superblock,my_INode INode_arr[],
    my_Directory Directory_arr[],int maxINode, int maxDirectory)
{
    int blockCount = M_BYTE_SIZE/superblock.get_sizeOfEachBlock();
    int filled_inode_count = 0, free_inode_count = 0;
    int i;
    int number_of_files = 0, number_of_directories=0;
    for(i=0; i<maxINode; i++)
    {
        if(INode_arr[i].get_validBit() == 0)
            free_inode_count++;
        else
        {  
            filled_inode_count++;
            number_of_files++;
        }
    }

    for(i=0; i<maxDirectory; i++)
    {
        if(Directory_arr[i].get_DName() != "??????????")
            number_of_directories++;
    }

    cout<<"Number of blocks \t"<<blockCount<<endl;
    cout<<"Number of i-nodes \t"<<(filled_inode_count + free_inode_count)<<endl;
    cout<<"Number of free blocks \t"<<(free_inode_count*2)<<endl;
    cout<<"Number of free i-nodes \t"<<free_inode_count<<endl;
    cout<<"Number of files \t"<<filled_inode_count<<endl;
    cout<<"Number of directories \t"<<number_of_directories<<endl;
    cout<<"Block size \t\t"<<superblock.get_sizeOfEachBlock()<<endl;

    //int occupied_blocks = superblock.get_sizeOfEachBlock()*2;
    int eachBlockSize_KB = superblock.get_sizeOfEachBlock() / BYTE_SIZE;
    string empty = "";
    printf("INode id\toccupied block\t%11sfile name\n",empty.c_str());
    for(i=0; i<maxINode; i++)
    {
        if(INode_arr[i].get_validBit() != 0)
        {
            int id = INode_arr[i].get_id();
            int occupied_block;
            if(INode_arr[i].get_fileSize() > eachBlockSize_KB)
                occupied_block = 2;
            else
                occupied_block = 1;
            
            string filename = INode_arr[i].get_fileName();
            filename = filename.substr(0,filename.find("?"));

            printf("%8d\t%14d\t%20s\n",id,occupied_block,filename.c_str());

        }

    }

}

// write operation
void write_op(char *argv[], string fileName, vector<string> dirNames, my_superblock superblock, my_Directory Directory_arr[],
              my_INode INode_arr[], int maxINode)
{
    
    int dirNames_size = dirNames.size();
    if (dirNames.at(dirNames_size - 1).size() > MAX_FILE_NAME_SIZE_DIR)
    {
        cout << "Your file name must have at most " << MAX_FILE_NAME_SIZE_DIR << " bytes, ";
        cout << "at least 1 byte" << endl;
        return;
    }
    string set_FileName_input = dirNames.at(dirNames_size - 1);

    vector<int> pathIndex = returnDirIndexes(dirNames, superblock, Directory_arr);

    int pathSize = pathIndex.size();
    if ((int)(dirNames_size - 1) != pathSize)
    {
        cout << "Your path information is wrong!!" << endl;
        exit(1);
    }

    int lastDirIndex;
    if (pathSize == 0)
        lastDirIndex = 0;
    else
        lastDirIndex = pathIndex[pathSize - 1];

    string dir_fileName0 = Directory_arr[lastDirIndex].get_fileName0();
    string dir_fileName1 = Directory_arr[lastDirIndex].get_fileName1();

    if (dir_fileName0 != "????????????????????" && dir_fileName1 != "????????????????????")
    {
        string currentDirectoryName = Directory_arr[lastDirIndex].get_DName();
        currentDirectoryName = currentDirectoryName.substr(0, currentDirectoryName.find("?"));
        cout << endl
             << currentDirectoryName << " do not have enough space" << endl;
        cout << "Directory file limit is 2" << endl;
        exit(1);
    }

    int empty_INodeIndex = getEmptyINode(INode_arr, maxINode);
    if (empty_INodeIndex == -1)
    {
        cout << "There is no empty INode in file system" << endl;
        exit(1);
    }
    
    
    char *user_fileName = argv[4];
    if (user_fileName == NULL)
    {
        cout << "Please give your file name" << endl;
        exit(1);
    }

    dir_fileName0 = dir_fileName0.substr(0, dir_fileName0.find("?"));
    dir_fileName1 = dir_fileName1.substr(0, dir_fileName1.find("?"));

    if (dir_fileName0 == set_FileName_input || dir_fileName1 == set_FileName_input)
    {
        cout << set_FileName_input << " named file is already exist!!" << endl;

        exit(1);
    }

    //int fileNamelen = strlen(user_fileName);
    FILE* controlFile;
    controlFile = fopen(user_fileName,"r");
    if(controlFile == NULL)
    {
        cout<<"Your file is not exist"<<endl;
        exit(1);
    }
    fclose(controlFile);
    struct stat user_FileInformation;
    stat(user_fileName, &user_FileInformation);

    int user_fileSize = user_FileInformation.st_size;
    if (user_fileSize <= 0)
    {
        cout << "Your " << user_fileName << " file is not exist or its size is 0" << endl;
        exit(1);
    }

    if (user_fileSize > superblock.get_sizeOfEachBlock() * 2)
    {
        cout << "Your file input is too large." << endl;
        cout << " Your file input must be at most " << (superblock.get_sizeOfEachBlock() * 2) << " bytes" << endl;

        exit(1);
    }
    int eachBlockSize = superblock.get_sizeOfEachBlock();
    int dataStartAddress = superblock.get_startaddress_dataBlock();

    INode_arr[empty_INodeIndex].set_validBit(1);
    string temp = appendMarkToStr(set_FileName_input, MAX_FILE_NAME_SIZE - set_FileName_input.length());
    INode_arr[empty_INodeIndex].set_fileName(temp);

    if (user_fileSize / BYTE_SIZE == 0)
        INode_arr[empty_INodeIndex].set_fileSize(superblock.get_sizeOfEachBlock() / BYTE_SIZE);
    else
        INode_arr[empty_INodeIndex].set_fileSize(user_fileSize / BYTE_SIZE);
    INode_arr[empty_INodeIndex].set_data0(dataStartAddress + (2 * eachBlockSize * empty_INodeIndex));
    INode_arr[empty_INodeIndex].set_data1(dataStartAddress + (2 * eachBlockSize * empty_INodeIndex) + eachBlockSize);
    INode_arr[empty_INodeIndex].set_time(returnTime());

    update_inode(fileName, superblock, INode_arr, empty_INodeIndex);

    write_Datas_To_SystemFile(fileName, user_fileName, user_fileSize, INode_arr[empty_INodeIndex].get_data0());

    dir_fileName0 = Directory_arr[lastDirIndex].get_fileName0();
    dir_fileName1 = Directory_arr[lastDirIndex].get_fileName1();
    if (dir_fileName0 == "????????????????????")
    {
        set_FileName_input = appendMarkToStr(set_FileName_input, MAX_FILE_NAME_SIZE_DIR - set_FileName_input.length());
        set_FileName_input.append("?");
        string inodeNumber = "";
        intToStringWithZero(empty_INodeIndex, inodeNumber, 3);
        set_FileName_input.append(inodeNumber);

        Directory_arr[lastDirIndex].set_fileName0(set_FileName_input);
    }

    else if (dir_fileName1 == "????????????????????")
    {
        set_FileName_input = appendMarkToStr(set_FileName_input, MAX_FILE_NAME_SIZE_DIR - set_FileName_input.length());
        set_FileName_input.append("?");
        string inodeNumber = "";
        intToStringWithZero(empty_INodeIndex, inodeNumber, 3);
        set_FileName_input.append(inodeNumber);
        Directory_arr[lastDirIndex].set_fileName1(set_FileName_input);
    }

    Directory_arr[lastDirIndex].set_date(returnTime());

    update_directory(fileName, superblock, Directory_arr, lastDirIndex);

    int i;
    for (i = 0; i < pathSize - 1; i++)
    {
        int tempIndex = pathIndex.at(i);
        Directory_arr[tempIndex].set_date(returnTime());
        update_directory(fileName, superblock, Directory_arr, tempIndex);
    }
}

// read operation
void read_op(char *argv[], string fileName, vector<string> dirNames, my_superblock superblock, my_Directory Directory_arr[],
             my_INode INode_arr[], int maxINode)
{
    int dirNames_size = dirNames.size();

    string read_FileName_input = dirNames.at(dirNames_size - 1);

    vector<int> pathIndex = returnDirIndexes(dirNames, superblock, Directory_arr);
    int pathSize = pathIndex.size();

    if ((int)(dirNames.size() - 1) != pathSize)
    {
        cout << "Your path information is wrong!!" << endl;
        exit(1);
    }
    int lastDirIndex;
    if (pathSize == 0)
        lastDirIndex = 0;
    else
        lastDirIndex = pathIndex[pathSize - 1];

    string fileName0 = Directory_arr[lastDirIndex].get_fileName0();
    string fileName0_inode_index = fileName0.substr(fileName0.length() - 3, fileName0.length());
    fileName0 = fileName0.substr(0, fileName0.find("?"));

    string fileName1 = Directory_arr[lastDirIndex].get_fileName1();
    string fileName1_inode_index = fileName1.substr(fileName1.length() - 3, fileName1.length());
    fileName1 = fileName1.substr(0, fileName1.find("?"));

    char *user_fileName = argv[4];
    if (user_fileName == NULL)
    {
        cout << "Please give your file name" << endl;
        exit(1);
    }

    if (fileName0 == read_FileName_input)
    {
        int inode_index = atoi(fileName0_inode_index.c_str());
        int byte_size_write = superblock.get_sizeOfEachBlock() * 2;
        int fseek_shift = INode_arr[inode_index].get_data0();
        write_Datas_To_InputFile(fileName, user_fileName, byte_size_write, fseek_shift);
    }
    else if (fileName1 == read_FileName_input)
    {
        int inode_index = atoi(fileName1_inode_index.c_str());
        int byte_size_write = superblock.get_sizeOfEachBlock() * 2;
        int fseek_shift = INode_arr[inode_index].get_data0();
        write_Datas_To_InputFile(fileName, user_fileName, byte_size_write, fseek_shift);
    }
    else
    {
        cout << read_FileName_input << " file is not exist in this directory" << endl;
        return;
    }
}

// del operation
void del_op(char *argv[], string fileName, vector<string> dirNames, my_superblock superblock, my_Directory Directory_arr[],
            my_INode INode_arr[], int maxINode,int maxDirectory)
{
    int dirNames_size = dirNames.size();

    string read_FileName_input = dirNames.at(dirNames_size - 1);

    vector<int> pathIndex = returnDirIndexes(dirNames, superblock, Directory_arr);
    int pathSize = pathIndex.size();

    if ((int)(dirNames.size() - 1) != pathSize)
    {
        cout << "Your path information is wrong!!" << endl;
        exit(1);
    }

    int lastDirIndex;
    if (pathSize == 0)
        lastDirIndex = 0;
    else
        lastDirIndex = pathIndex[pathSize - 1];
    string user_delete_filename = dirNames.at(dirNames_size - 1);

    string fileName0 = Directory_arr[lastDirIndex].get_fileName0();
    string inode_index_fileName0 = fileName0.substr(fileName0.length() - 3, fileName0.length());
    int inode_index_f0 = atoi(inode_index_fileName0.c_str());

    fileName0 = fileName0.substr(0, fileName0.find("?"));

    string fileName1 = Directory_arr[lastDirIndex].get_fileName1();
    string inode_index_fileName1 = fileName1.substr(fileName1.length() - 3, fileName1.length());
    int inode_index_f1 = atoi(inode_index_fileName1.c_str());

    fileName1 = fileName1.substr(0, fileName1.find("?"));

    if (user_delete_filename == fileName0)
    {
        if(!(INode_arr[inode_index_f0].get_validBit() >= 2))
        {
            int fseek_start = INode_arr[inode_index_f0].get_data0();
            int eachBlockSize = superblock.get_sizeOfEachBlock();
            fill_zero_filesystem(fileName, 2, eachBlockSize, fseek_start);
        }
        

        Directory_arr[lastDirIndex].set_fileName0("????????????????????");
        Directory_arr[lastDirIndex].set_date(returnTime());
        update_directory(fileName, superblock, Directory_arr, lastDirIndex);

        int i;
        int currentindex = 0;
        for (i = 0; i < pathSize - 1; i++)
        {
            currentindex = pathIndex.at(i);
            Directory_arr[currentindex].set_date(returnTime());
            update_directory(fileName, superblock, Directory_arr, currentindex);
        }

        if(INode_arr[inode_index_f0].get_validBit() >= 2)
        {

            string currentFileName_inode = INode_arr[inode_index_f0].get_fileName();
            currentFileName_inode = currentFileName_inode.substr(0,currentFileName_inode.find("?"));
            
            if(currentFileName_inode == user_delete_filename)
            {
                string newFileName;
                newFileName = get_inode_newfileName_in_directory(Directory_arr,maxDirectory, inode_index_f0);
                
                newFileName = appendMarkToStr(newFileName,MAX_FILE_NAME_SIZE-newFileName.length());
                INode_arr[inode_index_f0].set_fileName(newFileName);
            }
            
            INode_arr[inode_index_f0].set_validBit(INode_arr[inode_index_f0].get_validBit() - 1);
            INode_arr[inode_index_f0].set_time(returnTime());
            update_inode(fileName, superblock,INode_arr,inode_index_f0);
      
        }
        else
        {
            INode_arr[inode_index_f0] = my_INode();
            INode_arr[inode_index_f0].set_id(inode_index_f0);
            INode_arr[inode_index_f0].set_validBit(0);
            string inode_setname = "UNDEFINED";
            string temp;
            intToStringWithZero(inode_index_f0, temp, 3);
            inode_setname += temp + "00000000";
            INode_arr[inode_index_f0].set_fileName(inode_setname);

            update_inode(fileName, superblock, INode_arr, inode_index_f0);
        }   
    }
    else if (user_delete_filename == fileName1)
    {
        if(!(INode_arr[inode_index_f1].get_validBit() >= 2))
        {
            int fseek_start = INode_arr[inode_index_f1].get_data0();
            int eachBlockSize = superblock.get_sizeOfEachBlock();
            fill_zero_filesystem(fileName, 2, eachBlockSize, fseek_start);
        }
        

        Directory_arr[lastDirIndex].set_fileName1("????????????????????");
        Directory_arr[lastDirIndex].set_date(returnTime());
        update_directory(fileName, superblock, Directory_arr, lastDirIndex);

        int i;
        int currentindex = 0;
        for (i = 0; i < pathSize - 1; i++)
        {
            currentindex = pathIndex.at(i);
            Directory_arr[currentindex].set_date(returnTime());
            update_directory(fileName, superblock, Directory_arr, currentindex);
        }
        if(INode_arr[inode_index_f1].get_validBit() >= 2)
        {
            string currentFileName_inode = INode_arr[inode_index_f1].get_fileName();
            currentFileName_inode = currentFileName_inode.substr(0,currentFileName_inode.find("?"));
            
            if(currentFileName_inode == user_delete_filename)
            {
                string newFileName;
                newFileName = get_inode_newfileName_in_directory(Directory_arr,maxDirectory, inode_index_f1);
                
                newFileName = appendMarkToStr(newFileName,MAX_FILE_NAME_SIZE-newFileName.length());
                INode_arr[inode_index_f1].set_fileName(newFileName);
            }
            
            INode_arr[inode_index_f1].set_validBit(INode_arr[inode_index_f1].get_validBit()-1);
            
            INode_arr[inode_index_f1].set_time(returnTime());
            update_inode(fileName, superblock,INode_arr,inode_index_f1);
        }
        else
        {
            INode_arr[inode_index_f1] = my_INode();
            INode_arr[inode_index_f1].set_id(inode_index_f1);
            INode_arr[inode_index_f1].set_validBit(0);
            string inode_setname = "UNDEFINED";
            string temp;
            intToStringWithZero(inode_index_f1, temp, 3);
            inode_setname += temp + "00000000";
            INode_arr[inode_index_f1].set_fileName(inode_setname);

            update_inode(fileName, superblock, INode_arr, inode_index_f1);
        }  
    }
    else
    {
        cout << user_delete_filename << " is not exist in this directory" << endl;
        return;
    }
}

// ln operation
void ln_op(string fileName,vector<string> dirNames1,vector<string> dirNames2,my_superblock superblock,
            my_INode INode_arr[],my_Directory Directory_arr[],int maxINode,int maxDirectory)
{
    int dirNames1_size = dirNames1.size();
    int dirNames2_size = dirNames2.size();
    int i;

    string read_FileName_input1 = dirNames1.at(dirNames1_size - 1);

    vector<int> pathIndexes1 = returnDirIndexes(dirNames1, superblock, Directory_arr);
    int pathSize1 = pathIndexes1.size();

    if ((int)(dirNames1.size() - 1) != pathSize1)
    {
        cout << "Your path information is wrong!!" << endl;
        exit(1);
    }

    int lastDirIndex1;
    if (pathSize1 == 0)
        lastDirIndex1 = 0;
    else
        lastDirIndex1 = pathIndexes1[pathSize1 - 1];
    

    string read_FileName_input2 = dirNames2.at(dirNames2_size - 1);

    if(read_FileName_input2.length() == 0  || read_FileName_input2.length() > MAX_FILE_NAME_SIZE_DIR)
    {
        cout << "Your file name must have at most " << MAX_FILE_NAME_SIZE_DIR << " bytes, ";
        cout << "at least 1 byte" << endl;
        return;
    }

    vector<int> pathIndexes2 = returnDirIndexes(dirNames2, superblock, Directory_arr);
    int pathSize2 = pathIndexes2.size();

    if ((int)(dirNames2.size() - 1) != pathSize2)
    {
        cout << "Your path information is wrong!!" << endl;
        exit(1);
    }

    int lastDirIndex2;
    if (pathSize2 == 0)
        lastDirIndex2 = 0;
    else
        lastDirIndex2 = pathIndexes2[pathSize2 - 1];
    
    int hardLink_whichfile = -1;
    int inode_index_fn0_input1 =0;
    int inode_index_fn1_input1 =0;

    string dir_filename0_Input1 = Directory_arr[lastDirIndex1].get_fileName0();
    string inode_index_fn0_input1_str = dir_filename0_Input1.substr(dir_filename0_Input1.length()-3,dir_filename0_Input1.length());
    dir_filename0_Input1 = dir_filename0_Input1.substr(0,dir_filename0_Input1.find("?"));
    
    string dir_filename1_Input1 = Directory_arr[lastDirIndex1].get_fileName1();
    string inode_index_fn1_input1_str = dir_filename1_Input1.substr(dir_filename1_Input1.length()-3,dir_filename1_Input1.length());
    dir_filename1_Input1 = dir_filename1_Input1.substr(0,dir_filename1_Input1.find("?"));
    
    if(dir_filename0_Input1 != "" && read_FileName_input1 == dir_filename0_Input1)
    {
        inode_index_fn0_input1 = atoi(inode_index_fn0_input1_str.c_str());
        hardLink_whichfile = 0;
    }
    else if(dir_filename1_Input1 != "" && read_FileName_input1 == dir_filename1_Input1)
    {
        inode_index_fn1_input1 = atoi(inode_index_fn1_input1_str.c_str());
        hardLink_whichfile = 1;
    }
    else
    {
        cout<<read_FileName_input1<<" named file could not found"<<endl;
        return ;   
    }


    string dir_filename0_Input2 = Directory_arr[lastDirIndex2].get_fileName0();
    dir_filename0_Input2 = dir_filename0_Input2.substr(0,dir_filename0_Input2.find("?"));

    string dir_filename1_Input2 = Directory_arr[lastDirIndex2].get_fileName1();
    dir_filename1_Input2 = dir_filename1_Input2.substr(0,dir_filename1_Input2.find("?"));

    if(dir_filename0_Input2 != "" && read_FileName_input2 == dir_filename0_Input2)
    {
        cout<<"This file is already exist"<<endl;
        return;
    }
    else if(dir_filename1_Input2 != "" && read_FileName_input2 == dir_filename1_Input2)
    {
        cout<<"This file is already exist"<<endl;
        return;
    }

    if(dir_filename0_Input2 == "")
    {
        string write_filename = "";
        write_filename += read_FileName_input2;
        write_filename = appendMarkToStr(write_filename,MAX_FILE_NAME_SIZE_DIR - write_filename.length());
        write_filename.append("?");

        int currentINode;
        string currentINode_str;
        if(hardLink_whichfile == 0)
            currentINode = inode_index_fn0_input1;
        else if(hardLink_whichfile == 1)
            currentINode = inode_index_fn1_input1;
        else
        {
            cout<<"Something went wrong"<<endl;
            exit(1);
        }
        

        intToStringWithZero(currentINode,currentINode_str,3);
        write_filename += currentINode_str;

        Directory_arr[lastDirIndex2].set_fileName0(write_filename);
        Directory_arr[lastDirIndex2].set_date(returnTime());
        update_directory(fileName, superblock,Directory_arr,lastDirIndex2);
        
        int index;
        for(i=0; i<pathSize2-1; i++)
        {
            index = pathIndexes2.at(i);
            Directory_arr[index].set_date(returnTime());
            update_directory(fileName, superblock,Directory_arr,index);
        }
        
        INode_arr[currentINode].set_validBit(INode_arr[currentINode].get_validBit()+1);
        update_inode(fileName,superblock, INode_arr,currentINode);
        
        
    }
    else if(dir_filename1_Input2 == "")
    {
        string write_filename = "";
        write_filename += read_FileName_input2;
        write_filename = appendMarkToStr(write_filename,MAX_FILE_NAME_SIZE_DIR - write_filename.length());
        write_filename.append("?");

        int currentINode;
        string currentINode_str;
        if(hardLink_whichfile == 0)
            currentINode = inode_index_fn0_input1;
        else if(hardLink_whichfile == 1)
            currentINode = inode_index_fn1_input1;
        else
        {
            cout<<"Something went wrong"<<endl;
            exit(1);
        }

        intToStringWithZero(currentINode,currentINode_str,3);
        write_filename += currentINode_str;

        Directory_arr[lastDirIndex2].set_fileName1(write_filename);
        Directory_arr[lastDirIndex2].set_date(returnTime());
        update_directory(fileName, superblock,Directory_arr,lastDirIndex2);
        
        int index;
        for(i=0; i<pathSize2-1; i++)
        {
            index = pathIndexes2.at(i);
            Directory_arr[index].set_date(returnTime());
            update_directory(fileName, superblock,Directory_arr,index);
        }
        
        INode_arr[currentINode].set_validBit(INode_arr[currentINode].get_validBit()+1);
        update_inode(fileName,superblock, INode_arr,currentINode);

        
    }
    else
    {
        string dirName = Directory_arr[lastDirIndex2].get_DName();
        dirName = dirName.substr(0, dirName.find("?"));
        cout<<dirName<<" named directory do not have enough space for file"<<endl;
        return;
    }
}

// fsck operation
void fsck_op(my_superblock superblock, my_INode INode_arr[], int maxINode)
{
    int i;
    int eachblocksize_kb = superblock.get_sizeOfEachBlock() / BYTE_SIZE;
    cout<<"Total block for inode : "<<maxINode<<endl<<endl;
    cout<<"Block number"<<endl;
    for(i=0; i<maxINode; i++)
        printf("%3d ",i);
    
    cout<<endl<<"Blocks in use: ";
    for(i=0; i<maxINode; i++)
    {
        if(INode_arr[i].get_fileSize() < eachblocksize_kb)
            printf("-%d-%d-",0,0);
        else if(INode_arr[i].get_fileSize() == eachblocksize_kb)
            printf("-%d-%d-",1,0);
        else if(INode_arr[i].get_fileSize() > eachblocksize_kb)
            printf("-%d-%d-",1,1);
        
    }
    cout<<endl<<"Free blocks: ";
    for(i=0; i<maxINode; i++)
    {
        if(INode_arr[i].get_fileSize() < eachblocksize_kb)
            printf("-%d-%d-",1,1);
        else if(INode_arr[i].get_fileSize() == eachblocksize_kb)
            printf("-%d-%d-",0,1);
        else if(INode_arr[i].get_fileSize() > eachblocksize_kb)
            printf("-%d-%d-",0,0);
        
    }
    cout<<endl;
}

// Tum operasyonlarin kontrol edildiği yerdir. Ayrica hata ayiklamalari yapilir.
void make_operation(char *argv[], string fileName, my_superblock superblock, my_INode INode_arr[], my_Directory Directory_arr[],
                    int maxINode, int maxDirectory)
{
    string operationName = argv[2];
    cout<<"operationName -> "<<operationName<<endl;
    if (operationName == "list")
    {   
        if(argv[3] == NULL)
            return;
        
        string control = argv[3];    
        
        if(argv[3] == NULL || control == "" || findCharecterinStr(control, '/') == -1)
        {
            cout<<"Your path information is wrong"<<endl;
            exit(1);
        }

        if(findCharecterinStr(control, '?') >= 0)
        {
            cout<<"Your path can not include ? character"<<endl;
            exit(1);
        }

        char *Path = argv[3];

        const char *character = "/";
        cout << "Path " << Path << endl;

        if (Path == NULL)
        {
            cout << "Please give a path" << endl;
            return;
        }
        if (strcmp(Path, "/") == 0)
        {
            
            string filename0 = Directory_arr[0].get_fileName0();
            string inode_index_str0 = filename0.substr(filename0.length() - 3, filename0.length());

            filename0 = filename0.substr(0, filename0.find("?"));

            string filename1 = Directory_arr[0].get_fileName1();
            string inode_index_str1 = filename1.substr(filename1.length() - 3, filename1.length());

            filename1 = filename1.substr(0, filename1.find("?"));
            int totalByte = 0;
            string empty = "";
            printf("%12sName\t%sByte Size\t%sDate\n", empty.c_str(), empty.c_str(), empty.c_str());

            if (filename0 != "" && inode_index_str0 != "")
            {
                int inode_index_0 = atoi(inode_index_str0.c_str());
                string fileTime = INode_arr[inode_index_0].get_time();
                int data0 = INode_arr[inode_index_0].get_data0();
                int data1 = INode_arr[inode_index_0].get_data1();
                int sizeOfEachBlock = superblock.get_sizeOfEachBlock();
                int inode_datablock_byte_size = getFileSizeOfDatablock(fileName, data0, data1 + sizeOfEachBlock);
                totalByte += inode_datablock_byte_size;
                printf("%16s\t%7d\t%20s\n", filename0.c_str(), inode_datablock_byte_size, fileTime.c_str());
            }

            if (filename1 != "" && inode_index_str1 != "")
            {
                int inode_index_1 = atoi(inode_index_str1.c_str());
                string fileTime = INode_arr[inode_index_1].get_time();
                int data0 = INode_arr[inode_index_1].get_data0();
                int data1 = INode_arr[inode_index_1].get_data1();
                int sizeOfEachBlock = superblock.get_sizeOfEachBlock();
                int inode_datablock_byte_size = getFileSizeOfDatablock(fileName, data0, data1 + sizeOfEachBlock);
                totalByte += inode_datablock_byte_size;
                printf("%16s\t%7d\t%20s\n", filename1.c_str(), inode_datablock_byte_size, fileTime.c_str());
            }

            int Dadress0 = Directory_arr[0].get_DAdress0();
            int Dadress1 = Directory_arr[0].get_DAdress1();
            int Dadress2 = Directory_arr[0].get_DAdress2();

            string dname0 = Directory_arr[Dadress0].get_DName();
            dname0 = dname0.substr(0, dname0.find("?"));
            string dname1 = Directory_arr[Dadress1].get_DName();
            dname1 = dname1.substr(0, dname1.find("?"));
            string dname2 = Directory_arr[Dadress2].get_DName();
            dname2 = dname2.substr(0, dname2.find("?"));

            int sizeOfEachBlock = superblock.get_sizeOfEachBlock();
            if (Dadress0 != 0 && dname0 != "")
            {
                printf("%16s\t%7d\t%20s\n", dname0.c_str(), sizeOfEachBlock, Directory_arr[Dadress0].get_date().c_str());
                totalByte += sizeOfEachBlock;
            }
            if (Dadress1 != 0 && dname1 != "")
            {
                printf("%16s\t%7d\t%20s\n", dname1.c_str(), sizeOfEachBlock, Directory_arr[Dadress1].get_date().c_str());
                totalByte += sizeOfEachBlock;
            }
            if (Dadress2 != 0 && dname2 != "")
            {
                printf("%16s\t%7d\t%20s\n", dname2.c_str(), sizeOfEachBlock, Directory_arr[Dadress2].get_date().c_str());
                totalByte += sizeOfEachBlock;
            }
            int totalByte_KB = totalByte / BYTE_SIZE;
            cout << "total " << totalByte_KB << " KB" << endl;
            return;
        }
        else
        {
            vector<string> dirNames = parse_string(Path, character);
            list_op(fileName, dirNames, superblock, Directory_arr, INode_arr, maxINode);
        }
    }

    else if (operationName == "mkdir")
    {
        if(argv[3] == NULL)
            return;
        
        string control = argv[3];
        
        if(control == "/")
        {
            cout<<"You can not create root directory"<<endl;
            exit(1);
        }  
        
        if(argv[3] == NULL || control == "" || findCharecterinStr(control, '/') == -1 )
        {
            cout<<"Your path information is wrong"<<endl;
            exit(1);
        }

        if(findCharecterinStr(control, '?') >= 0)
        {
            cout<<"Your path can not include ? character"<<endl;
            exit(1);
        }

        char *Path = argv[3];
        const char *character = "/";
        cout << "Path " << Path << endl;
        vector<string> dirNames = parse_string(Path, character);
        mkdir_op(fileName, dirNames, superblock, Directory_arr, maxDirectory);
    }
    else if (operationName == "rmdir")
    {
        if(argv[3] == NULL)
            return;
        
        string control = argv[3];    
        
        if(argv[3] == NULL || control == "" || findCharecterinStr(control, '/') == -1)
        {
            cout<<"Your path information is wrong"<<endl;
            exit(1);
        }
        if(findCharecterinStr(control, '?') >= 0)
        {
            cout<<"Your path can not include ? character"<<endl;
            exit(1);
        }

        char *Path = argv[3];
        const char *character = "/";
        cout << "Path " << Path << endl;
        vector<string> dirNames = parse_string(Path, character);

        rmdir_op(fileName, dirNames, superblock, Directory_arr, maxDirectory);
    }
    else if (operationName == "dumpe2fs")
    {
        dumpe2fs_op(superblock, INode_arr, Directory_arr,maxINode,maxDirectory);
    }
    else if (operationName == "write")
    {
        if(argv[3] == NULL)
            return;
        if(argv[4] == NULL)
            return;
        string control = argv[3];    
        string markControl = argv[4];
        
        if(control == "/")
            return;
        
        if(argv[3] == NULL || control == "" || findCharecterinStr(control, '/') == -1)
        {
            cout<<"Your path information is wrong"<<endl;
            exit(1);
        }

        if(findCharecterinStr(control, '?') >= 0)
        {
            cout<<"Your path can not include ? character"<<endl;
            exit(1);
        }

        if(argv[4] == NULL || markControl == "" || findCharecterinStr(markControl, '?') >=0)
        {
            cout<<"Your path information is wrong"<<endl;
            exit(1);
        }

        char *Path = argv[3];

        const char *character = "/";
        cout << "Path " << Path << endl;
        vector<string> dirNames = parse_string(Path, character);

        write_op(argv, fileName, dirNames, superblock, Directory_arr, INode_arr, maxINode);
    }

    else if (operationName == "read")
    {
        
        if(argv[3] == NULL)
            return;
        if(argv[4] == NULL)
            return;
        string control = argv[3];    
        string markControl = argv[4];
        
        if(argv[3] == NULL || control == "" || control == "/" || findCharecterinStr(control, '/') == -1)
        {
            cout<<"Your path information is wrong"<<endl;
            exit(1);
        }

        if(findCharecterinStr(control, '?') >= 0)
        {
            cout<<"Your path can not include ? character"<<endl;
            exit(1);
        }
        
        if(argv[4] == NULL || markControl == "" || findCharecterinStr(markControl, '?') >=0)
        {
            cout<<"Your path information is wrong"<<endl;
            exit(1);
        }

        char *Path = argv[3];

        const char *character = "/";
        cout << "Path " << Path << endl;
        vector<string> dirNames = parse_string(Path, character);

        read_op(argv, fileName, dirNames, superblock, Directory_arr, INode_arr, maxINode);
    }

    else if (operationName == "del")
    {
        if(argv[3] == NULL)
            return;
        
        string control = argv[3];    
        
        if(control == "/")
        {
            cout<<"You can not delete root directory"<<endl;
            return;
        }

        if(argv[3] == NULL || control == "" || findCharecterinStr(control, '/') == -1)
        {
            cout<<"Your path information is wrong"<<endl;
            exit(1);
        }

        if(findCharecterinStr(control, '?') >= 0)
        {
            cout<<"Your path can not include ? character"<<endl;
            exit(1);
        }


        char *Path = argv[3];

        const char *character = "/";
        cout << endl
             << endl
             << "operationName " << operationName << endl;
        cout << "Path " << Path << endl;
        vector<string> dirNames = parse_string(Path, character);

        del_op(argv, fileName, dirNames, superblock, Directory_arr, INode_arr, maxINode,maxDirectory);
    }

    else if (operationName == "ln")
    {
        if(argv[3] == NULL)
            return;
        if(argv[4] == NULL)
            return;

        string control = argv[3];    
        
        if(argv[3] == NULL || control == "" || findCharecterinStr(control, '/') == -1 || control == "/")
        {
            cout<<"Your path information is wrong"<<endl;
            exit(1);
        }
        
        
        if(findCharecterinStr(control, '?') >= 0)
        {
            cout<<"Your path can not include ? character"<<endl;
            exit(1);
        }
        string control2 = argv[4];

        if(argv[4] == NULL || control2 == "" || control2 == "/" || findCharecterinStr(control2, '/') == -1)
        {
            cout<<"Your path information is wrong"<<endl;
            exit(1);
        }

        if(findCharecterinStr(control2, '?') >= 0)
        {
            cout<<"Your path can not include ? character"<<endl;
            exit(1);
        }

        char *Path1 = argv[3];
        char *Path2 = argv[4];
        const char *character = "/";
        cout << endl
             << endl
             << "operationName " << operationName << endl;
        cout << "Path1 " << Path1 << endl;
        vector<string> dirNames1 = parse_string(Path1, character);

        cout << "Path2 " << Path2 << endl;
        vector<string> dirNames2 = parse_string(Path2, character);
        
        
        ln_op(fileName, dirNames1, dirNames2, superblock, INode_arr,
                Directory_arr, maxINode, maxDirectory);
    }
    else if (operationName == "fsck")
    {
        fsck_op(superblock,INode_arr ,maxINode);
    }
    else
    {
        cout<<"UNKNOWN OPERATION!!"<<endl;
        exit(1);
    }
    


}

int main(int argc, char *argv[])
{
    // fileSystemOper fileSystem.data operation parameters[]
    
    if (argv[1] == NULL || argv[2] == NULL )
        return 1;

    string fileName = argv[1];
    string operation = argv[2];
    //string parameter[2] = {argv[3], argv[4]};
    if (isdigit(fileName[0]))
    {
        cout << "First character of the file name can not be integer " << endl;
        return 1;
    }

    cout << "file name : " << fileName << endl;
    FILE *file_read = fopen(fileName.c_str(), "r+");
    my_superblock superblock;

    read_superblock(file_read, superblock);

    if (superblock.get_fileName() != fileName)
    {
        cout << "Your file name input and file name parameter of superblock does not match!!";
        return 1;
    }

    int blockSize_KB = superblock.get_sizeOfEachBlock() / BYTE_SIZE;
    int maxINode = getNumberOfMaxINode(blockSize_KB);
    int maxDirectory = superblock.get_sizeOfEachBlock() / TOTALBYTE_DIRECTORY;

    my_INode INode_Arr[maxINode];
    my_Directory Directory_Arr[maxDirectory];

    read_INodes(file_read, superblock, INode_Arr, maxINode);
    read_Directories(file_read, superblock, Directory_Arr, maxDirectory);

    fclose(file_read);
    
    make_operation(argv, fileName, superblock, INode_Arr, Directory_Arr, maxINode, maxDirectory);
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

void my_superblock::print_superblock()
{
    cout << endl
         << "-o-o-o-o-o-o-o-o-o-o-o-o-o-o-" << endl
         << endl;
    cout << "Super Block Informations" << endl;
    cout << "File name: " << this->get_fileName() << endl;
    cout << "End of the file: " << this->get_endOfFile() << endl;
    cout << "Number of blocks: " << this->get_numberOfBlocks() << endl;
    cout << "Size of each block: " << this->get_sizeOfEachBlock() << endl;
    //cout<<"Start adress of free space: "<<this->get_startaddress_FreeSpace()<<endl;
    cout << "Start address of inodes: " << this->get_startaddress_inode() << endl;
    cout << "Start address of Root Directory: " << this->get_startaddress_directory() << endl;
    cout << "Start adress of data blocks: " << this->get_startaddress_dataBlock() << endl;
    cout << endl
         << "-o-o-o-o-o-o-o-o-o-o-o-o-o-o-" << endl
         << endl;
}

void my_INode::print_inode()
{
    cout << endl
         << "-o-o-o-o-o-o-o-o-o-o-o-o-o-o-" << endl
         << endl;
    cout << "INode Informations" << endl;
    cout << "validbit " << this->get_validBit() << endl;
    cout << "id " << this->get_id() << endl;
    cout << "filename " << this->get_fileName() << endl;
    cout << "filesize " << this->get_fileSize() << endl;
    cout << "data0 " << this->get_data0() << endl;
    cout << "data1 " << this->get_data1() << endl;
    cout << this->get_time() << endl;
    cout << endl
         << "-o-o-o-o-o-o-o-o-o-o-o-o-o-o-" << endl
         << endl;
}

ostream &operator<<(ostream &os, my_superblock &input)
{
    os << "File name: " << input.get_fileName() << endl;
    os << "End of the file: " << input.get_endOfFile() << endl;
    os << "Number of blocks: " << input.get_numberOfBlocks() << endl;
    os << "Size of each block: " << input.get_sizeOfEachBlock() << endl;
    //os<<"Start adress of free space: "<<input.get_startaddress_FreeSpace()<<endl;
    os << "Start address of inodes: " << input.get_startaddress_inode() << endl;
    os << "Start address of Root Directory: " << input.get_startaddress_directory() << endl;
    os << "Start adress of data blocks: " << input.get_startaddress_dataBlock() << endl;
    return os;
}

ostream &operator<<(ostream &os, my_INode &input)
{
    os << "validbit " << input.get_validBit() << endl;
    os << "id " << input.get_id() << endl;
    os << "filename " << input.get_fileName() << endl;
    os << "filesize " << input.get_fileSize() << endl;
    os << "data0 " << input.get_data0() << endl;
    os << "data1 " << input.get_data1() << endl;
    os << input.get_time() << endl;
    return os;
}

ostream &operator<<(ostream &os, my_Directory &input)
{
    os << "Dname " << input.get_DName() << endl;
    os << "fileName0 " << input.get_fileName0() << endl;
    os << "fileName1 " << input.get_fileName1() << endl;
    os << "DAdress0 " << input.get_DAdress0() << endl;
    os << "DAdress1 " << input.get_DAdress1() << endl;
    os << "DAdress2 " << input.get_DAdress2() << endl;
    os << input.get_date() << endl;
    return os;
}

string my_Directory::return_str()
{
    int DNameLength = this->get_DName().length();
    string myStr = "";
    myStr.append("Dname ");
    myStr.append(this->get_DName());

    myStr = appendMarkToStr(myStr, MAX_DIRECTORY_NAME_SIZE - DNameLength);
    myStr.append("\n");

    int fileNameLength = this->get_fileName0().length();
    myStr.append("fileName0 ");
    myStr.append(this->get_fileName0());

    myStr = appendMarkToStr(myStr, MAX_FILE_NAME_SIZE - fileNameLength);
    myStr.append("\n");

    fileNameLength = this->get_fileName1().length();
    myStr.append("fileName1 ");
    myStr.append(this->get_fileName1());

    myStr = appendMarkToStr(myStr, MAX_FILE_NAME_SIZE - fileNameLength);
    myStr.append("\n");

    string DAdress0_str, DAdress1_str, DAdress2_str;

    intToStringWithZero(this->get_DAdress0(), DAdress0_str, 7);
    intToStringWithZero(this->get_DAdress1(), DAdress1_str, 7);
    intToStringWithZero(this->get_DAdress2(), DAdress2_str, 7);

    myStr.append("DAdress0 ");
    myStr.append(DAdress0_str);
    myStr.append("\n");

    myStr.append("DAdress1 ");
    myStr.append(DAdress1_str);
    myStr.append("\n");

    myStr.append("DAdress2 ");
    myStr.append(DAdress2_str);
    myStr.append("\n");

    myStr.append(this->get_date());
    myStr += "\n";

    return myStr;
}

string my_INode::return_str()
{
    string myStr = "";
    string temp = "";
    myStr += "validbit " + to_string(this->get_validBit()) + "\n";
    intToStringWithZero(this->get_id(), temp, 4);
    myStr += "id " + temp + "\n";

    myStr += "filename " + this->get_fileName() + "\n";

    intToStringWithZero(this->get_fileSize(), temp, 2);
    myStr += "filesize " + temp + "\n";

    intToStringWithZero(this->get_data0(), temp, 7);
    myStr += "data0 " + temp + "\n";

    intToStringWithZero(this->get_data1(), temp, 7);
    myStr += "data1 " + temp + "\n";

    myStr += this->get_time();
    myStr += "\n";

    return myStr;
}
