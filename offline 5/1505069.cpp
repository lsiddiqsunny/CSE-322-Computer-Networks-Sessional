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


    cout<<"data block <ascii code of m characters per row>: "<<endl;
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


    for(int i=0; i<(sz/m); i++)
    {
       // cout<<i<<endl;
        int total=m+2;
        for(int j=0; j<total; j++)
        {
            int mask=1;
            mask<<=j;
            mask--;
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











}

/*
Computer Networks
4
.04
1010111
*/
