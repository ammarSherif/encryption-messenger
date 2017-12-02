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



//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
