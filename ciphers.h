//#include<iomanip>
#include <iostream>
#include <algorithm>
#include <bitset>

using namespace std;

#ifndef CIPHERS_H
#define CIPHERS_H


void increaseEnc();
void decreaseEnc();
int mod(int a,int k);
string caeserEncrypt(string &s,int key);
string encryptMul(string &s,int key,bool pad);
void findMulInv(int a,int indexOfA,int b,int* arr);
string decryptMul(string &s, int key, bool pad);

string playFairEn(string msg,string key,bool encrypt);
unsigned int getIndex(char c,string s);
int findNextChar(string msg,int after);
void buildPFMat(string key,string &alpha);
void addChar(char c,string &alpha);
void toLower(string & s);

string vigenereCipher(string msg,string key,bool encrypt);
void smallAlpha(string & str);
string oneTimePad(string msg,string& newKey);

string railFence(string msg,bool encrypt);
string permuteCipher(string msg,string &k ,bool encrypt);
int ceil(double number);
string permute(string msg,string key,bool encrypt);
bool numberDistinct(string key);

string initialPermute(string msg);
string finalPermute(string msg);
string permuteKeyDES(string k);
void roundKeyShifterDES(string &leftKey,string &rightKey,int roundNum,bool encrypt);
void DES(string &msg,string key,bool encrypt);
void SBox(string &rightBlock);
void sBoxSubstitution(string & s,int boxNum);

#endif // CIPHERS_H
