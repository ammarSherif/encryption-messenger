#include <iostream>
#include <bitset>
#include <algorithm>

using namespace std;

int mod(int a,int k){
    if(k<=0)    // error
        return -1;
    a = (a - a/k*k);
    if(a<0)
        a+=k;
    return a;
}

void representSBox(){
    int r;
    int c;
    string row;
    string col;
    string val;
    bool first = true;
    int sbox[64];
    for(unsigned int r=0;r<64;r++)
        cin>>sbox[r];
    cout<<"\n\n====================================================\n";
    for(unsigned int i=0;i<16;i++){
        first = true;
        //cout<<"\n"<<i<<" : \n";
        if(i!=0)
            cout<<"else ";
        cout<<"if(";
        for(unsigned int v=0;v<64;v++){
            if(sbox[v]==i){
                r=v/16;
                c=mod(v,16);
                row = bitset<2>(r).to_string();
                col = bitset<4>(c).to_string();
                reverse(row.begin(),row.end());
                reverse(col.begin(),col.end());
                if(!first)
                    cout<<"||";
                cout<<"s==\""<<row[0]<<col<<row[1]<<"\"";
                first = false;
            }
        }
        cout<<")";
        val = bitset<4>(i).to_string();
        reverse(val.begin(),val.end());
        cout<<"\n\t s = \""<<val<<"\";\n";
    }
    cout<<"\n\n\n";
}

int main(){
    representSBox();
    return 0;
}
