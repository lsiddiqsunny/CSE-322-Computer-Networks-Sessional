#include<bits/stdc++.h>
#include <windows.h>
using namespace std;

string s;
int m;
double p;
string poly;


bool isPowerofTwo(int x)
{
    while(x%2==0)
    {
        x/=2;
    }
    if(x==1)
        return 1;
    else
        return 0;

}


int main()
{

    cout<<"enter data string : ";
    getline(cin,s);

    cout<<"enter number of data bytes in a row <m> : ";
    cin>>m;

    cout<<"enter probability <p> : ";
    cin>>p;

    cout<<"enter generator polynomial : ";
    cin>>poly;


    while((((int)s.size())%m)!=0)
    {
        s+="~";
    }

    cout<<"\n\nData string after padding : "<<s<<endl<<endl;


    cout<<"data block <ascii code of m characters per row> : "<<endl;
    int sz=s.size();
    vector<int>datablock[sz/m];
    int k=-1;
    for(int i=0; i<sz; i++)
    {
        if(i%m==0)
        {
            k++;
        }
        char c=s[i];
        int mask=1;
        vector<int>v;
        for(int j=0; j<8; j++)
        {
            mask=mask<<j;
            //  cout<<mask<<endl;
            if((c&mask)==0)
            {
                v.push_back(0);
            }
            else
                v.push_back(1);
            mask=1;
        }
        reverse(v.begin(),v.end());

        for(int j=0; j<8; j++)
        {
            datablock[k].push_back(v[j]);
        }
    }

    for(int i=0; i<sz/m; i++)
    {
        for(int j=0; j<datablock[i].size(); j++)
        {
            cout<<datablock[i][j];

        }
        cout<<endl;
    }

    cout<<"\ndata block after adding check bits : "<<endl;

int total=m+3;
    for(int i=0; i<(sz/m); i++)
    {
        // cout<<i<<endl;

        for(int j=0; j<total; j++)
        {
            int mask=1;
            mask<<=j;
            mask--;
           // cout<<mask<<endl;
           if(mask>datablock[i].size()){
               total--;
            continue;
           }
            datablock[i].insert(datablock[i].begin()+mask,0);

        }
        for(int k=0; k<total; k++)
        {
            int mask=1;
            mask<<=k;
            int parity=0;
            // cout<<k<<endl;
            for(int j=0; j<datablock[i].size(); j++)
            {
                if(((j+1)&mask)!=0)
                {
                    //cout<<j<<" ";
                    parity^=datablock[i][j];
                }

            }
            // cout<<endl;
            mask--;
            datablock[i][mask]=parity;
        }

    }


    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    WORD wOldColorAttrs;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    wOldColorAttrs = csbiInfo.wAttributes;

    for(int i=0; i<sz/m; i++)
    {
        for(int j=0; j<datablock[i].size(); j++)
        {
            if(isPowerofTwo(j+1))
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                cout<<datablock[i][j];

            }
            else
            {
                SetConsoleTextAttribute ( h, wOldColorAttrs);
                cout<<datablock[i][j];
            }
        }
        cout<<endl;
    }

    cout<<"\ndata bits after column-wise serialization : "<<endl;
    vector<int>serializeddata;
    for(int i=0; i<(m*8+total); i++)
    {
        for(int j=0; j<(sz/m); j++)
        {
            serializeddata.push_back(datablock[j][i]);
        }
    }
    for(int i=0; i<serializeddata.size(); i++)
    {
        cout<<serializeddata[i];
    }
    cout<<endl<<endl;

    vector<int>remserializeddata;
    remserializeddata=serializeddata;
    cout<<"data bits after appending CRC checksum <send frame> : "<<endl;
    int sz1=poly.size();
    for(int i=1; i<sz1; i++)
    {
        serializeddata.push_back(0);
    }
    for(int i=0; i<=serializeddata.size()-sz1; i++)
    {
        if(serializeddata[i]==0)
            continue;
        for(int j=i; j<i+sz1; j++)
        {
            serializeddata[j]^=(poly[j-i]-'0');
        }
    }

    vector<int>reminder;
    bool ok=false;
    for(int i=remserializeddata.size(); i<serializeddata.size(); i++)
    {


        reminder.push_back(serializeddata[i]);
    }
    serializeddata=remserializeddata;
    for(int i=0; i<reminder.size(); i++)
    {
        serializeddata.push_back(reminder[i]);
    }
    //cout<<serializeddata.size()<<" "<<sz<<endl;
    for(int i=0; i<serializeddata.size(); i++)
    {
        if(i>=((sz/m)*(total+m*8)))
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);

        }
        cout<<serializeddata[i];
    }
    cout<<endl;
    SetConsoleTextAttribute ( h, wOldColorAttrs);
    cout<<"\nreceived frame : "<<endl;
    int havetotoggle=ceil((serializeddata.size())*(p));
    int framesize=serializeddata.size();
    vector<int>pos;
    std::mt19937_64 rng;
    // initialize the random number generator with time-dependent seed
    uint64_t timeSeed = chrono::high_resolution_clock::now().time_since_epoch().count();
    seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    rng.seed(ss);
    // initialize a uniform distribution between 0 and 1
    uniform_real_distribution<double> unif(0, 1);
    // ready to generate random numbers

    for(int i=0; i<framesize; i++)
    {
        double currentRandomNumber = unif(rng);
       // cout<<currentRandomNumber<<endl;
        if(currentRandomNumber>p){
            continue;
        }

        pos.push_back(i);
    }
    for(int i=0; i<pos.size(); i++)
    {
        //cout<<pos[i]<<endl;
        serializeddata[pos[i]]^=1;
    }

    for(int i=0; i<serializeddata.size(); i++)
    {
        if(find(pos.begin(),pos.end(),i)!=pos.end())
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);

        }
        else
        {
            SetConsoleTextAttribute ( h, wOldColorAttrs);

        }
        cout<<serializeddata[i];
    }
    SetConsoleTextAttribute ( h, wOldColorAttrs);
    cout<<endl;

    ok=true;
    remserializeddata=serializeddata;
    for(int i=0; i<=serializeddata.size()-sz1; i++)
    {
        if(serializeddata[i]==0)
            continue;
        for(int j=i; j<i+sz1; j++)
        {
            serializeddata[j]^=(poly[j-i]-'0');
        }
    }
    for(int i=0; i<serializeddata.size(); i++)
    {
        if(serializeddata[i])
        {
            ok=false;
        }
    }

    cout<<"\nresult of CRC checksum matching : ";
    if(ok)
    {
        cout<<"no error detected"<<endl;
    }
    else
        cout<<"error detected"<<endl;

    cout<<"\ndata block after removing CRC checksum bits : "<<endl;

    vector<int>datablock1[sz/m];
    vector<int>receiveddata[sz/m];
    serializeddata.clear();
    for(int i=0; i<((sz/m)*(total+m*8)); i++)
    {

        serializeddata.push_back(remserializeddata[i]);
    }
    int l=sz/m;
    for(int i=0; i<l; i++)
    {
        for(int k=0; k<total+m*8; k++)
        {
            datablock1[i].push_back(serializeddata[i+l*k]);
        }
    }
    for(int i=0; i<l; i++)
    {
        for(int j=0; j<datablock1[i].size(); j++)
        {
            int k=(i+l*j);


            if(find(pos.begin(),pos.end(),k)!=pos.end())
            {
                //cout<<k<<endl;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);

            }
            else
            {
                SetConsoleTextAttribute ( h, wOldColorAttrs);

            }
            cout<<datablock1[i][j];
        }
        cout<<endl;
    }
    SetConsoleTextAttribute ( h, wOldColorAttrs);
    cout<<"\ndata block after removing checkbits : "<<endl;






    for(int i=0; i<(sz/m); i++)
    {
        // cout<<i<<endl;
        pos.clear();
        for(int k=0; k<total; k++)
        {
            int mask=1;
            mask<<=k;
            int parity=0;
            // cout<<k<<endl;
            for(int j=0; j<datablock1[i].size(); j++)
            {
                if(((j+1)&mask)!=0)
                {
                    //cout<<j<<" ";
                    parity^=datablock1[i][j];
                }

            }
            //   cout<<parity<<" ";
            pos.push_back(parity);
            mask--;
            //datablock[i][mask]=parity;
        }
        // cout<<endl;
        int position=0;
        int num=1;
        for(int k=0; k<pos.size(); k++)
        {
            position+=(num*pos[k]);
            num*=2;
        }
        position--;
        if(position<0 || position>=(m*8+total))
            continue;
        // cout<<position<<endl;
        datablock1[i][position]^=1;

        //cout<<endl;

    }
    for(int i=0; i<sz/m; i++)
    {
        for(int j=0; j<datablock1[i].size(); j++)
        {
            if(isPowerofTwo(j+1))
            {
                continue;
            }
            receiveddata[i].push_back(datablock1[i][j]);
        }

    }

    for(int i=0; i<sz/m; i++)
    {
        for(int j=0; j<receiveddata[i].size(); j++)
        {
            cout<<receiveddata[i][j];
        }
        cout<<endl;
    }

    cout<<"\noutput frame : ";
    string ans="";
    for(int i=0; i<sz/m; i++)
    {
        for(int j=0; j<receiveddata[i].size(); j=j+8)
        {
            bitset<8>ch;
            for(int k=j; k<j+8; k++)
            {
                ch[7-k+j]=receiveddata[i][k];
            }
            ans.push_back((char)ch.to_ulong());

        }

    }


    cout<<ans<<endl;

}

/*
Computer Networks
4
.04
1010111
*/
/*
no error
3
0
101
*/
/*
a
1
0
101
*/
/*
Hamming Code
2
.05
10101
*/
/*
Error Detection
3
.02
10111
*/
/*
many errors
4
.1
10001
*/
/*
no error
3
0
101
*/
