#include "ciphers.h"

static unsigned int NUMKEY = 8;

void increaseEnc(){
    if(NUMKEY<12){
        NUMKEY++;
        cout<<"\t\tDONE\n";
    }else
        cout<<" Sorry but I think I reached the best I could do\n";
    cout<<"--------------------------------------->\n";
}

void decreaseEnc(){
    if(NUMKEY>3){
        NUMKEY--;
        cout<<"\t\tDONE\n";
    }else
        cout<<" Sorry but I think I should not do worse than this\n";
    cout<<"--------------------------------------->\n";
}

int mod(int a,int k){
    if(k<=0)    // error
        return -1;
    a = (a - a/k*k);
    if(a<0)
        a+=k;
    return a;
}

string caeserEncrypt(string &s,int key){
    key = mod(key,26);
    string enStr="";
    for(unsigned int i=0;i<s.length();i++){
        if(s[i]>='a' && s[i]<='z'){
            enStr+=(char)(mod(((s[i]-'a')+key),26)+'a');
        } else if(s[i]>='A' && s[i]<='Z'){
            enStr+=(char)((((s[i]-'A')+key)%26)+'A');
        } else {
            enStr+=s[i];
        }
    }
    return enStr;
}

string encryptMul(string &s,int key,bool pad){
    key = mod(key,26);
    int mulK=15;
    string enStr="";
    unsigned int padding = 0;
    for(unsigned int i=0;i<s.length();i++){
        mulK = (mod(i+padding,NUMKEY)==0)?15: mulK+2;    // odd numbers from 15 : 29
        if(s[i]>='a' && s[i]<='z'){
            if( pad && ((int)i-1)>=0 && s[i-1]==' '&&(i+1)<s.length()&&s[i+1]==' '){
                enStr+=(char)(mod(('z'-'a'+key)*mulK,26)+'a');
                padding++;
                mulK = (mod(i+padding,NUMKEY)==0)?15: mulK+2;
                enStr+=(char)(mod((s[i]-'a'+key)*mulK,26)+'a');
                padding++;
                mulK = (mod(i+padding,NUMKEY)==0)?15: mulK+2;
                enStr+=(char)(mod(('q'-'a'+key)*mulK,26)+'a');
            }else if( pad && ((int)i-1)>=0 && s[i-1]==' '&&(i+2)<s.length()&&s[i+2]==' '){
                enStr+=(char)(mod(('z'-'a'+key)*mulK,26)+'a');
                padding++;
                mulK = (mod(i+padding,NUMKEY)==0)?15: mulK+2;
                enStr+=(char)(mod((s[i]-'a'+key)*mulK,26)+'a');
            } else
                enStr+=(char)(mod((s[i]-'a'+key)*mulK,26)+'a');
        } else if(s[i]>='A' && s[i]<='Z'){
            if( pad && ((int)i-1)>=0 && s[i-1]==' '&&(i+1)<s.length()&&s[i+1]==' '){
                enStr+=(char)(mod(('Z'-'A'+key)*mulK,26)+'A');
                padding++;
                mulK = (mod(i+padding,NUMKEY)==0)?15: mulK+2;
                enStr+=(char)(mod((s[i]-'A'+key)*mulK,26)+'A');
                padding++;
                mulK = (mod(i+padding,NUMKEY)==0)?15: mulK+2;
                enStr+=(char)(mod(('q'-'a'+key)*mulK,26)+'a');
            }else if( pad && ((int)i-1)>=0 && s[i-1]==' '&&(i+2)<s.length()&&s[i+2]==' '){
                enStr+=(char)(mod(('Z'-'A'+key)*mulK,26)+'A');
                padding++;
                mulK = (mod(i+padding,NUMKEY)==0)?15: mulK+2;
                enStr+=(char)(mod((s[i]-'A'+key)*mulK,26)+'A');
            } else
                enStr+=(char)(mod((s[i]-'A'+key)*mulK,26)+'A');
        } else {
            enStr+=s[i];
        }
    }
    return enStr;
}

void findMulInv(int a,int indexOfA,int b,int* arr){
    for(int i=1;i<b;i=i+2){
        if(mod(a*i,b)==1){
            arr[indexOfA]=i;
            return;
        }
    }
    cout<<"error the message was encrypted with invalid key ["<<a<<"]"<<endl;
}

string decryptMul(string &s, int key, bool pad){
    int* dKeys = new int[NUMKEY];
    for(unsigned int j=0;j<NUMKEY;j++){ // initialize keys for fast decryption
        dKeys[j]=-1;
        findMulInv( (15+2*j) ,j,26,dKeys);
        if(dKeys[j]<0)
            return "";
    }
    key = mod(-1*key,26);
    string enStr="";
    char c;
    for(unsigned int i=0;i<s.length();i++){
        if(s[i]>='a' && s[i]<='z'){
            if( pad ){
                c=(char)(mod((s[i]-'a')*dKeys[mod(i,NUMKEY)]+key,26)+'a');
                if( ((int)i-1)>=0 && s[i-1]==' ' && c=='z'&&(i+3) <s.length() && s[i+3]==' '){
                            // padded letter discard it
                    i++;    // i = +1; the real letter
                    enStr+=(char)(mod((s[i]-'a')*dKeys[mod(i,NUMKEY)]+key,26)+'a');
                    i++;    // i = +2; padding if it is q
                    c=(char)(mod((s[i]-'a')*dKeys[mod(i,NUMKEY)]+key,26)+'a');
                    if(c!='q')
                        enStr+=c;
                }else
                    enStr+=(char)(mod((s[i]-'a')*dKeys[mod(i,NUMKEY)]+key,26)+'a');
            } else
                enStr+=(char)(mod((s[i]-'a')*dKeys[mod(i,NUMKEY)]+key,26)+'a');
        } else if(s[i]>='A' && s[i]<='Z'){
            if( pad ){
                c=(char)(mod((s[i]-'A')*dKeys[mod(i,NUMKEY)]+key,26)+'A');
                if( ((int)i-1)>=0 && s[i-1]==' ' && c=='Z'&&(i+3) <s.length() && s[i+3]==' '){
                            // padded letter discard it
                    i++;    // i = +1; the real letter
                    enStr+=(char)(mod((s[i]-'A')*dKeys[mod(i,NUMKEY)]+key,26)+'A');
                    i++;    // i = +2; padding if it is q
                    if(s[i]>='A' && s[i]<='Z'){ // it is capital not padding
                        enStr+=(char)(mod((s[i]-'A')*dKeys[mod(i,NUMKEY)]+key,26)+'A');
                    }else{
                        c=(char)(mod((s[i]-'a')*dKeys[mod(i,NUMKEY)]+key,26)+'a');
                        if(c!='q')
                            enStr+=c;
                    }
                }else
                    enStr+=(char)(mod((s[i]-'A')*dKeys[mod(i,NUMKEY)]+key,26)+'A');
            } else
                enStr+=(char)(mod((s[i]-'A')*dKeys[mod(i,NUMKEY)]+key,26)+'A');
        } else {
            enStr+=s[i];
        }
    }
    return enStr;
}
