#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>

#define KEYSIZE 16 
using namespace std;


PBYTE pFileBuffer = NULL;
char key[]      = "B^]_k6yMZnFH(+3,";
string path     = "C:\\Users\\Administrator\\AppData\\LocalLow\\Southpaw Games\\Skul\\";
string pathBak  = ".dat";
string steamID  = "76561199393580476";
string filePath = "C:\\Users\\Administrator\\Desktop\\"+ steamID + pathBak;

/**
* 初始化读取加密文件
*/
size_t initFile() {
    path = path + steamID;
    path = path + pathBak;
    FILE* file = fopen(path.c_str(), "r+b");
    if (file == NULL)
        return 0;
    size_t flength;
    fseek(file, 0, SEEK_END);
    flength = ftell(file);
    fseek(file, 0, SEEK_SET);
    pFileBuffer = (PBYTE)malloc(flength);
    if (pFileBuffer == NULL)
        return 0;
    size_t readSize = fread(pFileBuffer, 1, flength, file);
    fclose(file);
    return flength;
}

/**
* 写出文件
*/
void toFile(string data) {
    FILE* file = fopen(filePath.c_str(), "wb");
    for (int i = 0; i < data.size();i++) {
        //printf("  %#X", data.c_str()[i]);
    }
    fwrite(data.c_str(), data.size(), 1,file);
    fclose(file);
}
/**
* 加密
*/
string encryption(const char* original,size_t fileLength) {
    string enc;
    int i;
    for (i = 0; i < fileLength; i++) {
        enc.push_back(original[i] ^ key[i % KEYSIZE]);
        //printf("  %#X", enc.at(i));
    }
    return enc;
}

/**
* 解密
* 返回解密后的源数据
*/
string decrypt(size_t fileLength) {
    string dec;
    int i;
    //解密
    for (i = 0; i < fileLength; i++) {
        dec.push_back(*pFileBuffer ^ key[i % KEYSIZE]);
        //printf("  %#X", dec.at(i));
        *pFileBuffer++;
    }
    //cout << dec << endl;
    return dec;
}
/*
* 判断文件是否存在
*/
bool fileExists(const string name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

/*
* 读取文件到String中（网上Copy的）
*/
std::string ReadFileToString(const std::string& file_path)
{
    int fileLength = 0;
    std::ifstream inFile(file_path, std::ios::binary);
    if (!inFile.is_open())
    {
        inFile.close();
    }

    // 跳到文件尾
    inFile.seekg(0, std::ios::end);

    // 获取字节长度
    fileLength = inFile.tellg();

    // 跳到文件开头
    inFile.seekg(0, std::ios::beg);

    char* buffer = new char[fileLength];

    // 读取文件
    inFile.read(buffer, fileLength);

    std::string result_str(buffer, fileLength);

    delete[] buffer;

    inFile.close();

    return result_str;
}

int main(){
    size_t fileLength = initFile();//首先初始化
    int i = 0;
    string originalData;
    string encryptionData;
    originalData = decrypt(fileLength);//解密
    //写出文件
    toFile(originalData);
    system("pause");
    if (fileExists(filePath)) {
        cout << "解密文件写出成功，现写出的加密文件" << endl;
        //读取文件到String中
        string tmp = ReadFileToString(filePath);
        //加密
        encryptionData = encryption(tmp.c_str(), tmp.size());
        //写出文件
        toFile(encryptionData);
    }
    else {
        cout << "解密文件写出失败，现写出源文件的加密文件" << endl;
        //加密
        encryptionData = encryption(originalData.c_str(), originalData.size());
        //写出文件
        toFile(encryptionData);
    }
    return 0;
}
