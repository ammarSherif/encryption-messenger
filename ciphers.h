//#include<iomanip>
#include <iostream>
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

#endif // CIPHERS_H
