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
    delete[] dKeys;
    dKeys = NULL;
    return enStr;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

string playFairEn(string msg,string key,bool encrypt){
    const int nextSameRowClmn = (!encrypt)?-1:1;
    // if we are decrypting we would get the before letter, i.e. decrease 1, otherwise increase 1(by the same idea)
    static string alpha;
    buildPFMat(key,alpha);
    string cipher="";
    int currentChar=findNextChar(msg,-1);
    int nextChar=findNextChar(msg,currentChar);
    for(int i=0;i<(int)msg.length();i++){
        if( i!=currentChar)
            cipher+=msg[i];
        else if(msg[i]>='a'&&msg[i]<='z'){          //small letter
            if(nextChar==-1){                       //there is a current char but no other letter
                // so we add one padding after that letter and set the next char to current+1, i.e. the padding letter
                char padding = (msg[currentChar]!='x')?'x':'q';
                msg.insert(currentChar+1,1,padding);
                nextChar=currentChar+1;
            }
            int indexC=getIndex(msg[currentChar],alpha);
            int indexN=(msg[nextChar]<'a')?getIndex((char)((msg[nextChar]-'A')+'a'),alpha):
                                            getIndex(msg[nextChar],alpha);
            if(indexC<0 || indexN<0 ||indexC>24||indexN>24){
                cout<<"\n\nERROR HAPPENED character not found in alpha of PLAY FAIR\n\n";
                return "";
            } else if(indexC%5 != indexN%5){       // characters in different coulmns
                if(indexC/5 != indexN/5){          // in different rows also
                    cipher+=alpha[(indexC/5)*5+(indexN%5)];             // char at the row of current and the coulmn of next
                    for(unsigned int j=i+1;(int)j<nextChar;j++){        // add characters between this and the next char
                        cipher+=msg[j];
                    }
                    if(msg[nextChar]>='a' && msg[nextChar]<='z')        // was small
                        cipher+=alpha[(indexN/5)*5+(indexC%5)];         // char at the row of next and the coulmn of current
                    else if(msg[nextChar]>='A' && msg[nextChar]<='Z')
                        cipher+=(char)((alpha[(indexN/5)*5+(indexC%5)]-'a')+'A');
                }else{                              // in the same row
                    // if encrypting add 1 else decrease 1 which is the value of nextSameRowClmn
                    cipher+=alpha[(indexC/5)*5+(mod((indexC+nextSameRowClmn),5))];
                    // char at the same row but with the next coulmn in mod 5
                    for(unsigned int j=i+1;(int)j<nextChar;j++){        // add characters between this and the next char
                        cipher+=msg[j];
                    }
                    if(msg[nextChar]>='a' && msg[nextChar]<='z'){
                        cipher+=alpha[(indexN/5)*5+(mod((indexN+nextSameRowClmn),5))];
                    // char at the same row but with the next coulmn in mod 5
                    } else if(msg[nextChar]>='A' && msg[nextChar]<='Z')
                        cipher+=(char)((alpha[(indexN/5)*5+(mod((indexN+nextSameRowClmn),5))]-'a')+'A');
                }
                i=nextChar;                                     // we finished to the next char start from it as it is done
                currentChar=findNextChar(msg,nextChar);         // get the next char after i as current
                nextChar=findNextChar(msg,currentChar);         // get the next char after the currentChar as next
            } else {                        // characters in the same coulmns
                if(indexC/5 != indexN/5){   // in different rows
                    // if encrypting add 1 else decrease 1 which is the value of nextSameRowClmn
                    cipher+=alpha[(mod(((indexC/5)+nextSameRowClmn),5))*5+mod(indexC,5)];
                    // char at the next/before row of current and the same coulmn
                    for(unsigned int j=i+1;(int)j<nextChar;j++){     // add characters between this and the next char
                        cipher+=msg[j];
                    }
                    /// TODO check second letter capital or small
                    if(msg[nextChar]>='a' && msg[nextChar]<='z'){
                        cipher+=alpha[(mod(((indexN/5)+nextSameRowClmn),5))*5+mod(indexN,5)];
                    // char at the next/before row of next and the same coulmn
                    } else if(msg[nextChar]>='A' && msg[nextChar]<='Z')
                        cipher+=(char)((alpha[(mod(((indexN/5)+nextSameRowClmn),5))*5+mod(indexN,5)]-'a')+'A');
                    i=nextChar;                                     // we finished to the next char start from it as it is done
                    currentChar=findNextChar(msg,nextChar);         // get the next char after i as current
                    nextChar=findNextChar(msg,currentChar);         // get the next char after the currentChar as next
                }else{                      // in the same row and in the same coulmn,i.e. it is the same char pad with 'x'
                    char padding = (msg[currentChar]!='x')?'x':'q';
                    msg.insert(nextChar,1,padding);             // add the padding to the message and encrypt
                    msg.insert(currentChar+1,1,padding);
                    i=currentChar-1;                            // we did nothing just added paddings, so start again the current loop
                    // by decreasing the index by one as if it just finished the last task
                    // the currentChar index is the same as we did nothing but the nextchar will be changed to current +1
                    // due to padding character
                    nextChar=currentChar+1;
                }
            }
        } else if(msg[i]>='A'&&msg[i]<='Z'){        //Cpital letter
            if(nextChar==-1){                       //there is a current char but no other letter
                // so we add one padding after that letter and set the next char to current+1, i.e. the padding letter
                char padding = (msg[currentChar]!='X')?'X':'Q';
                msg.insert(currentChar+1,1,padding);
                nextChar=currentChar+1;
            }
            int indexC=getIndex((char)((msg[currentChar]-'A')+'a'),alpha);
            int indexN=(msg[nextChar]<'a')?getIndex((char)((msg[nextChar]-'A')+'a'),alpha):
                                            getIndex(msg[nextChar],alpha);
            if(indexC<0 || indexN<0 ||indexC>24||indexN>24){
                cout<<"\n\nERROR HAPPENED character not found in alpha of PLAY FAIR\n\n";
                return "";
            } else if(indexC%5 != indexN%5){       // characters in different coulmns
                if(indexC/5 != indexN/5){   // in different rows also
                    cipher+=(alpha[(indexC/5)*5+(indexN%5)]-'a')+'A';   // char at the row of current and the coulmn of next
                    for(unsigned int j=i+1;(int)j<nextChar;j++){     // add characters between this and the next char
                        cipher+=msg[j];
                    }
                    if(msg[nextChar]>='A' && msg[nextChar]<='Z')
                        cipher+=(alpha[(indexN/5)*5+(indexC%5)]-'a')+'A';   // char at the row of next and the coulmn of current
                    else if(msg[nextChar]>='a' && msg[nextChar]<='z')
                        cipher+=alpha[(indexN/5)*5+(indexC%5)];
                }else{                      // in the same row
                    // if encrypting add 1 else decrease 1 which is the value of nextSameRowClmn
                    cipher+=(alpha[(indexC/5)*5+(mod((indexC+nextSameRowClmn),5))]-'a')+'A';
                    // char at the same row but with the next/previous coulmn in mod 5
                    for(unsigned int j=i+1;(int)j<nextChar;j++){     // add characters between this and the next char
                        cipher+=msg[j];
                    }
                    if(msg[nextChar]>='A' && msg[nextChar]<='Z'){
                        cipher+=(alpha[(indexN/5)*5+(mod((indexN+nextSameRowClmn),5))]-'a')+'A';
                        // char at the same row but with the next/previous coulmn in mod 5
                    } else if(msg[nextChar]>='a' && msg[nextChar]<='z')
                        cipher+=alpha[(indexN/5)*5+(mod((indexN+nextSameRowClmn),5))];
                }
                i=nextChar;                                     // we finished to the next char start from it as it is done
                currentChar=findNextChar(msg,nextChar);         // get the next char after i as current
                nextChar=findNextChar(msg,currentChar);         // get the next char after the currentChar as next
            } else {                        // characters in the same coulmns
                if(indexC/5 != indexN/5){   // in different rows
                    // if encrypting add 1 else decrease 1 which is the value of nextSameRowClmn
                    if(msg[nextChar]>='A' && msg[nextChar]<='Z'){
                        cipher+=(alpha[mod(((indexC/5)+nextSameRowClmn),5)*5+mod(indexC,5)]-'a')+'A';
                        // char at the next/previous row of current and the same coulmn
                    } else if(msg[nextChar]>='a' && msg[nextChar]<='z')
                        cipher+=alpha[mod(((indexC/5)+nextSameRowClmn),5)*5+mod(indexC,5)];
                    for(unsigned int j=i+1;(int)j<nextChar;j++){     // add characters between this and the next char
                        cipher+=msg[j];
                    }
                    cipher+=(alpha[mod(((indexN/5)+nextSameRowClmn),5)*5+mod(indexN,5)]-'a')+'A';
                    // char at the next/previous row of next and the same coulmn
                    i=nextChar;                                     // we finished to the next char start from it as it is done
                    currentChar=findNextChar(msg,nextChar);       // get the next char after i as current
                    nextChar=findNextChar(msg,currentChar);         // get the next char after the currentChar as next
                }else{                      // in the same row and in the same coulmn,i.e. it is the same char pad with 'x'
                    char padding = (msg[currentChar]!='X')?'X':'Q';
                    msg.insert(nextChar,1,padding);             // add the padding to the message and encrypt
                    msg.insert(currentChar+1,1,padding);
                    i=currentChar-1;                            // we did nothing just added paddings, so start again the current loop
                    // by decreasing the index by one as if it just finished the last task
                    // the currentChar index is the same as we did nothing but the nextchar will be changed to current +1
                    // due to padding character
                    nextChar=currentChar+1;
                }
            }
        }
    }
    if(!encrypt){ // remove paddings
        currentChar=findNextChar(cipher,-1);
        currentChar=findNextChar(cipher,currentChar);//get second character
        nextChar=findNextChar(cipher,currentChar);   //get the next character, 3rd
        while(currentChar>=0){                    // did not reach the end yet
            if(nextChar==-1 &&(cipher[currentChar]=='q'||cipher[currentChar]=='Q'||cipher[currentChar]=='X'||cipher[currentChar]=='x')){
                // no more characters i.e, current is the last char so if it is x or q, i.e. padding remove it
                cipher.erase(cipher.begin()+currentChar);
                currentChar = -1;
                // ::::END STATE:::::
            } else if (((cipher[currentChar]=='q'||cipher[currentChar]=='Q')&&(cipher[nextChar]=='q'||cipher[nextChar]=='Q'))
                       ||((cipher[currentChar]=='x'||cipher[currentChar]=='X')&&(cipher[nextChar]=='x'||cipher[nextChar]=='X'))){
                        // padding characters
                // remove this
                cipher.erase(cipher.begin()+nextChar);
                cipher.erase(cipher.begin()+currentChar);
                // get he next char
                // let i be current, j is next
                // before removing
                //             i <non char> j
                // - - - - - - -   ...      ----- - - - - --- -
                // after
                //             i <non char> j
                // - - - - - - x   ...      x---- - - - - --- -
                //   (find)  i-> <non char> j
                // - - - - - - x   ...      x---- - - - - --- -
                // so i would be to find the first char after previous letter, i-1
                currentChar = findNextChar(cipher,currentChar-1);
                nextChar = findNextChar(cipher,currentChar);
                //update indices
            } else {    // no paddings
                currentChar = findNextChar(cipher,nextChar);
                nextChar = findNextChar(cipher,currentChar);
                // jump to the next two characters
                // update indices
            }
        }
    }
    return cipher;
}

unsigned int getIndex(char c,string s){
    if(c=='j')
        c='i';
    for(unsigned int i=0;i<s.length();i++){
        if(c==s[i])
            return i;
    }
    return -1;
}

int findNextChar(string msg,int after){
    after++;        // get next char to start from it
    if(after>=(int)msg.length()||after<0)
        return -1;
    while ((msg[after]<'a'||msg[after]>'z')&&(msg[after]<'A'||msg[after]>'Z')) {
        //get next character
        after++;
        if(after>=(int)msg.length()){
            return -1;
        }
    }
    return after;
}

void buildPFMat(string key,string &alpha){
    static string lastKey="";
    toLower(key);
    if(lastKey==key)
        return;
    lastKey=key;
    alpha="";
    for(char ch='a';ch<='z';ch++)
        key+=ch;
    for(unsigned int j=0;j<key.length();j++)
        addChar(key[j],alpha);
    /*for(char ch:key)
        addChar(ch,alpha);*/
}

void addChar(char c,string &alpha){
    if(c<'a'||c>'z')
        return;
    if(c=='j')
        c='i';
    for(unsigned int j=0;j<alpha.length();j++)
        if(c==alpha[j])
            return;
    /*for(char c2:alpha)
        if(c==c2)
            return;*/
    alpha+=c;
}

void toLower(string & s){
    for(unsigned int i=0;i<s.length();i++){
        if(s[i]<='Z'&&s[i]>='A'){
            s[i]=(s[i]-'A')+'a';
        }
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

string vigenereCipher(string msg,string key,bool encrypt){
    toLower(key);       // convert to lower case
    smallAlpha(key);    // remover all unnecessary letters
    if(key.empty()){    // no key => no encryption
        return msg;
    }
    while(key.length()<msg.length())
        key+=key;       // duplicate key until it covers the message
    string cipher="";
    int enc=(encrypt)?1:-1; // enc =1 at encrypting so it will do adding and subtract otherwise
    for(unsigned int j=0;j<msg.length();j++){
        if(msg[j]>='a' && msg[j]<='z'){         // lower case
            cipher += (char) mod(((msg[j]-'a')+enc*(key[j]-'a')),26)+'a';
        } else if(msg[j]>='A' && msg[j]<='Z'){  // UPPER CASE
            cipher += (char) mod(((msg[j]-'A')+enc*(key[j]-'a')),26)+'A';
        } else {                                // other characters like spaces, ... etc.
            cipher += msg[j];
        }
    }
    return cipher;
}

void smallAlpha(string & str){
    for(int j=0;j<(int)str.length();j++)
        if(str[j]<'a'||str[j]>'z'){  //out of range
            str.erase(str.begin()+j);
            j--;
        }
}

string oneTimePad(string msg,string& newKey){
    srand(time(NULL));
    string key="";
    while(key.length()<msg.length())
        key +=(mod(rand(),26)+'a');
    newKey = key;
    return vigenereCipher(msg,key,true);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

string railFence(string msg,bool encrypt){
    string cipherTxt="";
    if(encrypt){
        for(unsigned int j=0;j<msg.length();j=j+2)  //add even letters
            cipherTxt+=msg[j];
        for(unsigned int j=1;j<msg.length();j=j+2)  //add odd letters
            cipherTxt+=msg[j];
    } else {
        for(unsigned int j=0;j<msg.length();j++)
            cipherTxt+='a';                         // initialize with 'a'
        for(unsigned int j=0;j<msg.length();j++)
            if(2*j<msg.length())                    // put first half in
                cipherTxt[2*j]=msg[j];              // the even positions
            else    // no more even positions, means try to fill the odd ones now
                cipherTxt[(2*(j-ceil( (double)msg.length()/2)))+1]=msg[j];
            // note j ranges from 0 to length -1
            // even positions in cipher are at range 0 to ceil(length/2)-1
            // as they are ceil(n/2) letters at even indices so as their
            // range start at 0 it will end at ceil(n/2)-1
            // so odd positions start at ceil(n/2) which is number one
            // i.e. odd positions are at range ceil(n/2)+{0,1,2,3,4,...,floor(n/2)}
            // so by subtracting j-ceil(n/2) will result in sequence {0,1,...}
            // which should be mapped to {1,3,5,...} so you guessed it
            // multiply by two and add 1 which is done here
    }
    return cipherTxt;
}

int ceil(double number){
    if((number-(int)number)>0)
        return ((int)number +1);
    else
        return (int)number;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

string permuteCipher(string msg,string &k ,bool encrypt){
    if(msg.length()<3){
        // too small message
        return msg;
    }
    if(encrypt){
        // distinct numbers
        if (!numberDistinct(k)||k.length()<2){
            string strKeyNum = "";
            static char numbers[10]={'0','1','2','3','4','5','6','7','8','9'};
            srand(time(NULL));// initialize the seed of random with number of seconds
            unsigned int key=0;
            do{
                key=mod(rand(),10);
            }while(key<3 || key>msg.length());// determine the number of coulmns
            unsigned int coulmnNum = key;
            for(unsigned int j=0;j<coulmnNum;j++)
                strKeyNum+='a';     // initialize key with default values 'a'
            for(unsigned int j=0;j<coulmnNum;j++){
                do{
                    key=mod(rand(),coulmnNum);
                }while(strKeyNum[key]!='a');
                strKeyNum[key]=numbers[j];
            }
            k= strKeyNum;
        }
        while (mod(msg.length(),k.length())!=0) {
            msg+='x';
        }
        // perfect size match, adding paddings
        // now, permute to encipher
        return permute(msg,k,encrypt);
    } else {
        return permute(msg,k,encrypt);          // permute to decipher
    }
}


string permute(string msg,string key,bool encrypt){
//    if(!encrypt && key.empty()){    // enter the key
//        cout<<" Please enter the key provided by the algorithm [only numbers] \n";
//        cout<<" The key : ";
//        getline(cin,key);
//        if(cin.fail()){
//            cin.clear();
//            cin.ignore(100,'\n');
//        }
//        cout<<"--------------------------------------->\n";
//    }
//    while (!numberDistinct(key)) {
//        cout<<" Sorry it is not a valid key for permutation cipher\n";
//        cout<<" Here some notes, key must have distinct numbers only\n";
//        cout<<" ranges from 0 to its size-1 with any sequence\n";
//        cout<<" Please re-enter the key : ";
//        getline(cin,key);
//        if(cin.fail()){
//            cin.clear();
//            cin.ignore(100,'\n');
//        }
//        cout<<"--------------------------------------->\n";
//    }
    string permutedMsg="";
    if(!numberDistinct(key)||mod(msg.length(),key.length())!=0){
        // invalid key
        return msg;
    }

    for(unsigned int j=0;j<msg.length();j++)
        permutedMsg+='a';
    // initialize with a
    // number of values per single coulmn = the number of rows = number of elements added
    // for each iteration to the cipher
    unsigned int rowCount = msg.length()/key.length();
    unsigned int coulmnNum=0;
    for(unsigned int col=0;col<key.length();col++){
        // loop on all values of the key to know where is the place of each coulmn
        coulmnNum = (unsigned int)(key[col]-'0');             // get the value of that coulmn
        // put the letters in that coulmn to its specific place int the cipher
        // its original place is the places of that coulmn specified by <col> variable
        // so in the original msg col specify its coulmn and the rows loop by <k> variable
        // k*numberOfCoulmns(strKeyNum.length())+col
        // and that would be mapped, in cipher, to row number specified by coulmnValue
        // and loop by <k> to cover all coulmns in that row.
        for(unsigned int k=0;k<rowCount;k++){
            // add the values of that coulmn to cipher
            if(encrypt)
                permutedMsg[coulmnNum*rowCount+k]=msg[k*key.length()+col];
            else    // do reverse in deciphering
                permutedMsg[k*key.length()+col]=msg[coulmnNum*rowCount+k];
        }
    }
    while (!encrypt && permutedMsg[permutedMsg.length()-1]=='x') {  // remove paddings at deciphering
        permutedMsg.erase(permutedMsg.end()-1);
    }
    return permutedMsg;
}

bool numberDistinct(string key){
    // checks distinct number and within specific range
    if(key.empty())
        return false;
    char alpha[]={'a','a','a','a','a','a','a','a','a','a'};
    for(unsigned int j=0;j<key.length();j++)
        if((key[j]-'0')>=(int)key.length()|| (key[j]-'0')< 0|| alpha[(key[j]-'0')]!='a')
            return false;
        else
             alpha[(key[j]-'0')]='v';
    return true;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

string initialPermute(string msg){
    while(mod(msg.length(),8)!=0)
        msg+='x';   // paddings
    string *block = new string[msg.length()/8];
    string *permutedBlocks = new string[msg.length()/8];
    for(unsigned int i=0;i<msg.length()/8;i++)
        block[i]=msg.substr(i*8,8); // 8 char = 64 bit each
    static const int permute[]={
        57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,
        61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7,
        56,48,40,32,24,16,8,0,58,50,42,34,26,18,10,2,
        60,52,44,36,28,20,12,4,62,54,46,38,30,22,14,6
    };
    // get bit representation
    string tmp="";
    string res="";
    for(unsigned int i=0;i<msg.length()/8;i++){//for each substring of message
        for(int j=0;j<8;j++){          //for each char in each substring
            //convert to binary
            tmp= bitset<8>((int)((block[i])[j])).to_string();
            reverse(tmp.begin(),tmp.end());
            res += tmp;
        }
        // now res is 64 values represents 64 bit representation of substring
        block[i]=permutedBlocks[i]=res;   //put values in the array
        res="";         //reset the res variable for further manipulations
    }
    res="";             // clear so it could hold the message converted to bits
    for(unsigned int i=0;i<msg.length()/8;i++){ //for each block
        for(int j=0;j<64;j++){      //permute it in
            (permutedBlocks[i])[j]=(block[i])[permute[j]];
        }
        res+=permutedBlocks[i];
    }
    delete[] block;
    delete[] permutedBlocks;
    block = permutedBlocks = NULL;
    return res;
}

string finalPermute(string msg){
    // message is in bit representation
    string *block = new string[msg.length()/64];
    string *permutedBlocks = new string[msg.length()/64];
    for(unsigned int i=0;i<msg.length()/64;i++)
        block[i]=permutedBlocks[i]=msg.substr(i*64,64); // 64 bit for each block
    static const int permute[]={
        39,7,47,15,55,23,63,31,38,6,46,14,54,22,62,30,
        37,5,45,13,53,21,61,29,36,4,44,12,52,20,60,28,
        35,3,43,11,51,19,59,27,34,2,42,10,50,18,58,26,
        33,1,41,9,49,17,57,25,32,0,40,8,48,16,56,24
    };
    // get bit representation
    string res="";
    for(unsigned int i=0;i<msg.length()/64;i++){ //for each block
        for(int j=0;j<64;j++){      //permute it in
            (permutedBlocks[i])[j]=(block[i])[permute[j]];
        }
        res+=permutedBlocks[i];
    }
    // now res has the message after permutation
    delete[] permutedBlocks;
    delete[] block;
    permutedBlocks = block = NULL;
    // now convert it back to chars
    string tmp="";
    bitset<8> * val = NULL;
    // erase the message to hold the new one
    msg="";
    for(unsigned int i=0;i<res.length()/8;i++){
        // convert back the chars
        // get the next 8-bits
        tmp = res.substr(i*8,8);
        // reverse them again as the bits are reversed in
        // the representation of this program
        reverse(tmp.begin(),tmp.end());
        // construct a bitset that holds the value
        val = new bitset<8>(tmp);
        // convert it to character and add it to the message
        msg+= (char)val->to_ulong();
        // delete so no memory leak happen
        delete val;
        // just to make dealing with pointers more safe
        val = NULL;
    }
    return msg;
}

string permuteKeyDES(string k){
    if(k.length()<8){
        return "";
    }
    string tmp="";
    string res="";
    for(unsigned int i=0;i<8;i++){     //for each char i.e. 8bits
        //convert to binary
        tmp= bitset<8>((int)(k[i])).to_string();
        reverse(tmp.begin(),tmp.end());
        res += tmp;
    }
    tmp="";
    static const int permute[]={
        56,48,40,32,24,16,8,0,57,49,41,33,25,17,
        9,1,58,50,42,34,26,18,10,2,59,51,43,35,
        62,54,46,38,30,22,14,6,61,53,45,37,29,21,
        13,5,60,52,44,36,28,20,12,4,27,19,11,3
    };
    for(unsigned int j=0;j<56;j++){
        tmp += res[permute[j]];
    }
    return tmp;
}

void roundKeyShifterDES(string &leftKey,string &rightKey,int roundNum,bool encrypt){
    if(encrypt){
        // left and right key must be 28 bit,
        // and round num is at most 63 and at least 0 [0-63]
        char lftKBit = leftKey[27];
        char rghtKBit = rightKey[27];   // save left most bit
        for(unsigned int j=27;j>0;j--){
            // in circular shift left
            // each bit is assigned to the bit before it
            // and bit 0 will be assigned to bit 27 (rightmost)
            leftKey[j]=leftKey[j-1];
            rightKey[j]=rightKey[j-1];
        }
        leftKey[0] = lftKBit;
        rightKey[0]= rghtKBit;
        if(roundNum!=0 && roundNum!=1 && roundNum!=8 && roundNum!=15){
            // SHIFT again so it will equal 2-bit circular shift
            lftKBit = leftKey[27];
            rghtKBit = rightKey[27];   // save left most bit
            for(unsigned int j=27;j>0;j--){
                // in circular shift left
                // each bit is assigned to the bit before it
                // and bit 0 will be assigned to bit 27 (rightmost)
                leftKey[j]=leftKey[j-1];
                rightKey[j]=rightKey[j-1];
            }
            leftKey[0] = lftKBit;
            rightKey[0]= rghtKBit;
        }
    } else {
        if(roundNum==0){
            for(unsigned int i=0;i<16;i++){
                // shift the key 16 times to get last key as in encryption
                roundKeyShifterDES(leftKey,rightKey,i,true);
            }
        } else {
            // to decrypt just start with last key
            // and for each round cirular shift RIGHT
            char lftKBit = leftKey[0];
            char rghtKBit = rightKey[0];   // save left most bit
            for(unsigned int j=0;j<27;j++){
                // in circular shift RIGHT
                // each bit is assigned to the bit after it
                // and bit 27 will be assigned to bit 0 (0 is leftmost bit)
                leftKey[j]=leftKey[j+1];
                rightKey[j]=rightKey[j+1];
            }
            leftKey[27] = lftKBit;
            rightKey[27]= rghtKBit;
            if((15-roundNum)!=1 && (15-roundNum)!=2 && (15-roundNum)!=9 && (15-roundNum)!=15){
                // SHIFT again so it will equal 2-bit circular shift
                lftKBit = leftKey[0];
                rghtKBit = rightKey[0];   // save left most bit
                for(unsigned int j=0;j<27;j++){
                    // in circular shift RIGHT
                    // each bit is assigned to the bit after it
                    // and bit 27 will be assigned to bit 0 (LEFTMOST)
                    leftKey[j]=leftKey[j+1];
                    rightKey[j]=rightKey[j+1];
                }
                leftKey[27] = lftKBit;
                rightKey[27]= rghtKBit;
            }
        }
    }
}

void DES(string &msg,string key,bool encrypt){
    // NOTE : in representation
    // in real binary system
    // 55 = 0011 0111
    // my representation is flipped
    // 55 = 1110 0011
    // so first position, i.e. bit 0 is the actual leftmost bit
    key = permuteKeyDES(key);
    if(key.empty()){
        msg+="\n PLEASE INSERT AT LEAST 8 CHARACTERS AS A KEY\n";
        return;
    }
    msg = initialPermute(msg);
    // each block of the message is 64 bit then number of blocks = length /64
    // and each block is divided to left and right
    string * rightBlock = new string[msg.length()/64];
    string * leftBlock = new string[msg.length()/64];
    // for compression of Key
    static const int DBox[] ={
        13,16,10,23,0,4,2,27,14,5,20,9,22,18,11,3,25,7,
        15,6,26,19,12,1,40,51,30,36,46,54,29,39,50,44,32,47,
        43,48,38,55,33,52,45,41,49,35,28,31
    };
    // for expansion of each block
    static const int EBox[]={
        31,0,1,2,3,4,3,4,5,6,7,8,7,8,9,10,11,12,11,12,13,14,15,16,15,16,17,18,19,20,
        19,20,21,22,23,24,23,24,25,26,27,28,27,28,29,30,31,0
    };
    // for permuting the rightblock within the fiestel function
    static const int permuteBox[]={ // 32-bit
        16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,
        2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25
    };
    //initialize blocks of the message
    for(unsigned int i=0;i<msg.length()/64;i++){
        // NOTE: bit 0 is the rightmost bit
        rightBlock[i]= msg.substr(i*64,32);   // each is 32-bit
        leftBlock[i] = msg.substr(i*64+32,32);// each is 32-bit
        // but right start from position 0 to position 31
        // and left start from 0+32 = 32 to 65
    }
    string rKey = key.substr(0,28);     // first 28 bit is the right
    string lKey = key.substr(28,28);    // last  28 bit is the left
    string lBlock,rBlock;               // current left and right blocks
    //::::::::::::::::: 16 ROUNDS ::::::::::::::::::::::::
    string resKey;
    for(unsigned int i=0;i<16;i++) {
        // :::::::::: Key calculations :::::::::::::::::::
        roundKeyShifterDES(lKey,rKey,i,encrypt);
        key = rKey+lKey;
        // compression from 56 bit key to 48 bit key
        resKey="";
        for(unsigned int j=0;j<48;j++)
            resKey+=key[DBox[j]];
        // :::::::::::: msg manipulation :::::::::::::::::
        for(unsigned int j=0;j<msg.length()/64;j++){
            lBlock = leftBlock[j];
            rBlock = rightBlock[j];

            // ------------------------------------------
            leftBlock[j]=rBlock;
            // ------------------------------------------

            rightBlock[j]="";
            // expand the right block to 48 bit
            for(unsigned int k=0;k<48;k++)
                rightBlock[j]+=rBlock[EBox[k]];
            rBlock=rightBlock[j];
            // -------------------------------------------
            // xor with the 48-bit key => in resKey
            rightBlock[j]=""; //erase, so it could hold the new values
            for(unsigned int k=0;k<48;k++)
                rightBlock[j]+=((int)(rBlock[j]-'0')^(int)(resKey[j]-'0'))+'0';
            // now the right block is XORed with the key, and the result is
            // 48-bit key, so make it to 32 bit using SBOXes
            SBox(rightBlock[j]);
            rBlock=rightBlock[j];
            // now right block is 32-bit last thing in the function
            rightBlock[j]="";
            for(unsigned int k=0;k<32;k++)
                rightBlock[j]+=rBlock[permuteBox[k]];
            rBlock=rightBlock[j];
            // --------------------------------------------
            // xor with the 32-bit left block => in lBlock
            rightBlock[j]=""; //erase, so it could hold the new values
            for(unsigned int k=0;k<32;k++)
                rightBlock[j]+=((int)(rBlock[j]-'0')^(int)(lBlock[j]-'0'))+'0';
            // now the round is done

            //---------------------------------------------

            // after last round swap, i.e. at i==15 is done
            if(i==15){
                lBlock = leftBlock[j];
                leftBlock[j] = rightBlock[j];
                rightBlock[j] = lBlock;
            }
        }
    }
    //::::::::::::::::: ROUNDS DONE ::::::::::::::::::::::::
    // now collect the total message to perform final permutation
    lBlock="";
    for(unsigned int i=0;i<msg.length()/64;i++){
        lBlock+= rightBlock[i]+leftBlock[i];
    }
    msg = lBlock;
    // now do the final permutation and DONE
    msg = finalPermute(msg);
    delete[] rightBlock;
    delete[] leftBlock;
    rightBlock=leftBlock=NULL;
}

void SBox(string &rightBlock){
    // rightBlock is 48-bit representation and the output should be
    // 32-bit representation
    string tmpRBlock = "";
    string blocks[8];   // eight SBOXes, one for each block
    for(unsigned int i=0;i<8;i++){
        blocks[i]=rightBlock.substr(i*6,6);
        sBoxSubstitution(blocks[i],i);
        tmpRBlock+=blocks[i];
    }
    rightBlock = tmpRBlock;
    // now right is 32-bit representation
}

void sBoxSubstitution(string & s,int boxNum){
    // NOTE this code is generated by a program I wrote
    // which takes the sbox as values and it would generate
    // these if statements, you would find it
    // with the name of 'sBoxGeneration.cpp'
    // also the data is in another file called 'SBOX data.txt'
    if(boxNum==0){  // sbox 1
        if(s=="001110"||s=="100000"||s=="011111"||s=="110111")
            s = "0000";
        else if(s=="011000"||s=="010001"||s=="101101")
            s = "1000";
        else if(s=="000100"||s=="110100"||s=="001101"||s=="111001")
            s = "0100";
        else if(s=="000010"||s=="100010"||s=="101110"||s=="000111"||s=="101011")
            s = "1100";
        else if(s=="010000"||s=="111000"||s=="000001"||s=="100101")
            s = "0010";
        else if(s=="000110"||s=="110110"||s=="001111"||s=="100011")
            s = "1010";
        else if(s=="001010"||s=="110010"||s=="010101"||s=="101111")
            s = "0110";
        else if(s=="011110"||s=="101000"||s=="011011"||s=="111101")
            s = "1110";
        else if(s=="011100"||s=="111110"||s=="011001"||s=="101001")
            s = "0001";
        else if(s=="010110"||s=="100110"||s=="001011"||s=="110101")
            s = "1001";
        else if(s=="010010"||s=="111100"||s=="010111"||s=="100111")
            s = "0101";
        else if(s=="001100"||s=="111010"||s=="011101"||s=="110011")
            s = "1101";
        else if(s=="011010"||s=="101010"||s=="010011"||s=="110001")
            s = "0011";
        else if(s=="001000"||s=="101100"||s=="000101"||s=="111111")
            s = "1011";
        else if(s=="000000"||s=="100100"||s=="001001"||s=="111011")
            s = "0111";
        else if(s=="010100"||s=="110000"||s=="000011"||s=="100001")
            s = "1111";
    } else if(boxNum == 1){ // sbox 2
        if(s=="010110"||s=="110010"||s=="000001"||s=="100111")
            s = "0000";
        else if(s=="010000"||s=="101010"||s=="011101"||s=="111001")
            s = "1000";
        else if(s=="001010"||s=="110100"||s=="001111"||s=="111101")
            s = "0100";
        else if(s=="001100"||s=="100000"||s=="010111"||s=="100101")
            s = "1100";
        else if(s=="011100"||s=="101000"||s=="010101"||s=="101101")
            s = "0010";
        else if(s=="001110"||s=="111110"||s=="000011"||s=="110111")
            s = "1010";
        else if(s=="000100"||s=="100110"||s=="011011"||s=="110011")
            s = "0110";
        else if(s=="010010"||s=="111000"||s=="001001"||s=="101011")
            s = "1110";
        else if(s=="001000"||s=="101100"||s=="010011"||s=="110001")
            s = "0001";
        else if(s=="000010"||s=="110110"||s=="000111"||s=="111111")
            s = "1001";
        else if(s=="011110"||s=="111010"||s=="000101"||s=="101001")
            s = "0101";
        else if(s=="010100"||s=="101110"||s=="011001"||s=="100011")
            s = "1101";
        else if(s=="000110"||s=="100010"||s=="001011"||s=="111011")
            s = "0011";
        else if(s=="011010"||s=="110000"||s=="001101"||s=="100001")
            s = "1011";
        else if(s=="011000"||s=="111100"||s=="010001"||s=="101111")
            s = "0111";
        else if(s=="000000"||s=="100100"||s=="011111"||s=="110101")
            s = "1111";
    } else if(boxNum == 2){ // sbox 3
        if(s=="010000"||s=="101000"||s=="011101"||s=="111001")
            s = "0000";
        else if(s=="000010"||s=="111110"||s=="010011"||s=="100001")
            s = "1000";
        else if(s=="001110"||s=="100010"||s=="001011"||s=="101111")
            s = "0100";
        else if(s=="010100"||s=="100100"||s=="001101"||s=="111011")
            s = "1100";
        else if(s=="010110"||s=="110100"||s=="001001"||s=="100011")
            s = "0010";
        else if(s=="011100"||s=="101010"||s=="000111"||s=="110111")
            s = "1010";
        else if(s=="000100"||s=="101100"||s=="010001"||s=="100101")
            s = "0110";
        else if(s=="011010"||s=="110000"||s=="011111"||s=="111101")
            s = "1110";
        else if(s=="011110"||s=="110010"||s=="000101"||s=="101101")
            s = "0001";
        else if(s=="001000"||s=="111000"||s=="011001"||s=="110101")
            s = "1001";
        else if(s=="000000"||s=="111100"||s=="010111"||s=="110001")
            s = "0101";
        else if(s=="000110"||s=="110110"||s=="000011"||s=="100111")
            s = "1101";
        else if(s=="001010"||s=="100110"||s=="011011"||s=="111111")
            s = "0011";
        else if(s=="010010"||s=="100000"||s=="000001"||s=="101001")
            s = "1011";
        else if(s=="011000"||s=="111010"||s=="001111"||s=="101011")
            s = "0111";
        else if(s=="001100"||s=="101110"||s=="010101"||s=="110011")
            s = "1111";
    } else if(boxNum == 3){ // sbox 4
        if(s=="000100"||s=="101100"||s=="011001"||s=="101001")
            s = "0000";
        else if(s=="000010"||s=="100110"||s=="010011"||s=="110101")
            s = "1000";
        else if(s=="010010"||s=="101010"||s=="010111"||s=="101111")
            s = "0100";
        else if(s=="011000"||s=="111100"||s=="001011"||s=="100001")
            s = "1100";
        else if(s=="001110"||s=="100010"||s=="011111"||s=="110011")
            s = "0010";
        else if(s=="011010"||s=="111000"||s=="000111"||s=="101011")
            s = "1010";
        else if(s=="010100"||s=="100100"||s=="010001"||s=="111001")
            s = "0110";
        else if(s=="000000"||s=="110010"||s=="001101"||s=="110111")
            s = "1110";
        else if(s=="001010"||s=="110000"||s=="001111"||s=="111101")
            s = "0001";
        else if(s=="001100"||s=="111110"||s=="001001"||s=="100011")
            s = "1001";
        else if(s=="011100"||s=="110110"||s=="000001"||s=="100101")
            s = "0101";
        else if(s=="000110"||s=="101000"||s=="010101"||s=="111011")
            s = "1101";
        else if(s=="010110"||s=="111010"||s=="000101"||s=="100111")
            s = "0011";
        else if(s=="010000"||s=="100000"||s=="011101"||s=="101101")
            s = "1011";
        else if(s=="001000"||s=="101110"||s=="011011"||s=="111111")
            s = "0111";
        else if(s=="011110"||s=="110100"||s=="000011"||s=="110001")
            s = "1111";
    } else if (boxNum == 4){ // sbox 5
        if(s=="010110"||s=="110010"||s=="001111"||s=="101011")
            s = "0000";
        else if(s=="011000"||s=="111100"||s=="001001"||s=="100101")
            s = "1000";
        else if(s=="000000"||s=="101000"||s=="010001"||s=="101101")
            s = "0100";
        else if(s=="001010"||s=="100110"||s=="010111"||s=="111111")
            s = "1100";
        else if(s=="001000"||s=="100100"||s=="000001"||s=="110111")
            s = "0010";
        else if(s=="010010"||s=="100010"||s=="011011"||s=="101111")
            s = "1010";
        else if(s=="011100"||s=="111110"||s=="000111"||s=="100011")
            s = "0110";
        else if(s=="000100"||s=="110100"||s=="001101"||s=="111001")
            s = "1110";
        else if(s=="000010"||s=="101110"||s=="011101"||s=="110001")
            s = "0001";
        else if(s=="011110"||s=="110110"||s=="010011"||s=="111011")
            s = "1001";
        else if(s=="010100"||s=="111010"||s=="000101"||s=="100111")
            s = "0101";
        else if(s=="001100"||s=="110000"||s=="011001"||s=="100001")
            s = "1101";
        else if(s=="010000"||s=="111000"||s=="001011"||s=="101001")
            s = "0011";
        else if(s=="000110"||s=="101100"||s=="010101"||s=="111101")
            s = "1011";
        else if(s=="001110"||s=="100000"||s=="011111"||s=="110101")
            s = "0111";
        else if(s=="011010"||s=="101010"||s=="000011"||s=="110011")
            s = "1111";
    } else if (boxNum == 5){ // sbox 6
        if(s=="000010"||s=="100110"||s=="010011"||s=="110111")
            s = "0000";
        else if(s=="010000"||s=="110010"||s=="000111"||s=="101011")
            s = "1000";
        else if(s=="010100"||s=="111000"||s=="000101"||s=="101001")
            s = "0100";
        else if(s=="001010"||s=="101110"||s=="011101"||s=="110001")
            s = "1100";
        else if(s=="011010"||s=="101000"||s=="001011"||s=="100001")
            s = "0010";
        else if(s=="001110"||s=="111100"||s=="011001"||s=="110101")
            s = "1010";
        else if(s=="001100"||s=="100010"||s=="011111")
            s = "0110";
        else if(s=="010110"||s=="100100"||s=="000011"||s=="111011")
            s = "1110";
        else if(s=="011100"||s=="111110"||s=="010101"||s=="101111")
            s = "0001";
        else if(s=="000100"||s=="101100"||s=="000001"||s=="100101")
            s = "1001";
        else if(s=="001000"||s=="100000"||s=="011011"||s=="111101"||s=="100111")
            s = "0101";
        else if(s=="011110"||s=="110110"||s=="001111"||s=="100011")
            s = "1101";
        else if(s=="000000"||s=="110100"||s=="001101"||s=="111001")
            s = "0011";
        else if(s=="010010"||s=="101010"||s=="010111"||s=="111111")
            s = "1011";
        else if(s=="000110"||s=="111010"||s=="010001"||s=="110011")
            s = "0111";
        else if(s=="011000"||s=="110000"||s=="001001"||s=="101101")
            s = "1111";
    } else if (boxNum == 6){ // sbox 7
        if(s=="010100"||s=="110000"||s=="000111"||s=="101011")
            s = "0000";
        else if(s=="011110"||s=="101100"||s=="000001"||s=="100101")
            s = "1000";
        else if(s=="001000"||s=="100110"||s=="011111"||s=="110111")
            s = "0100";
        else if(s=="000010"||s=="110010"||s=="010101"||s=="101111")
            s = "1100";
        else if(s=="000000"||s=="100100"||s=="010001"||s=="110101")
            s = "0010";
        else if(s=="000110"||s=="101010"||s=="010111"||s=="110011")
            s = "1010";
        else if(s=="001110"||s=="111110"||s=="001011"||s=="100001")
            s = "0110";
        else if(s=="011010"||s=="111000"||s=="001101"||s=="111101")
            s = "1110";
        else if(s=="001100"||s=="101110"||s=="011011"||s=="111001")
            s = "0001";
        else if(s=="001010"||s=="110100"||s=="001111"||s=="100011")
            s = "1001";
        else if(s=="010110"||s=="111100"||s=="000011"||s=="101101")
            s = "0101";
        else if(s=="010000"||s=="101000"||s=="001001"||s=="110001")
            s = "1101";
        else if(s=="010010"||s=="111010"||s=="000101"||s=="111111")
            s = "0011";
        else if(s=="011100"||s=="100000"||s=="011001"||s=="101001")
            s = "1011";
        else if(s=="011000"||s=="100010"||s=="011101"||s=="100111")
            s = "0111";
        else if(s=="000100"||s=="110110"||s=="010011"||s=="111011")
            s = "1111";
    } else if (boxNum == 7){ // sbox 8
        if(s=="010110"||s=="000011")
            s = "0000";
        else if(s=="011100"||s=="100000"||s=="011001"||s=="110001")
            s = "1000";
        else if(s=="010000"||s=="111110"||s=="011101"||s=="100001")
            s = "0100";
        else if(s=="001010"||s=="110100"||s=="010111"||s=="100111")
            s = "1100";
        else if(s=="011000"||s=="111100"||s=="001001"||s=="100101")
            s = "0010";
        else if(s=="000110"||s=="110010"||s=="001111"||s=="110111")
            s = "1010";
        else if(s=="000100"||s=="101010"||s=="010011"||s=="101111")
            s = "0110";
        else if(s=="011110"||s=="101100"||s=="000001"||s=="111001")
            s = "1110";
        else if(s=="001000"||s=="111000"||s=="011111"||s=="101101")
            s = "0001";
        else if(s=="010010"||s=="101110"||s=="000101"||s=="101011"||s=="111011")
            s = "1001";
        else if(s=="000010"||s=="100100"||s=="100110"||s=="001011"||s=="011011"||s=="110101")
            s = "0101";
        else if(s=="001100"||s=="111010"||s=="010001"||s=="111111")
            s = "1101";
        else if(s=="001110"||s=="100010"||s=="010101"||s=="110011")
            s = "0011";
        else if(s=="000000"||s=="101000"||s=="111101")
            s = "1011";
        else if(s=="011010"||s=="110110"||s=="001101"||s=="101001")
            s = "0111";
        else if(s=="010100"||s=="110000"||s=="000111"||s=="100011")
            s = "1111";
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
