#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <stdint.h>
#include <cmath>
#include <list>
#include <bitset>
using namespace std;

string tohex(uint64_t target)
{
    uint64_t num;
    uint8_t dig;
    char add;
    string ret = "";
    num = target;
    while (num > 0)
    {
        dig = num%16;
        if (dig < 10)
            add = (char)(dig+48);
        else
            add = (char)(dig+55);
        ret = add + ret;
        num /= 16;
    }
    return ret;
}

void print(uint64_t target)
{
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    int test;
    int reglength = log2((double)target);
    cout << "hex: " << uppercase << setw(28) << ("0x" + tohex(target)) << "    ";
    cout << "dec: " << setw(27) << dec << target << " |" << endl;
    if (reglength < 32)
    {
        cout << "bin: " << setw(64) << std::bitset<32>(target) << " |" << endl;
    }
    else if (reglength < 64)
    {
        cout << "bin: " << setw(64) << std::bitset<64>(target) << " |" << endl;
    }
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

uint64_t todecimal(const char* target, int base)
{
    uint8_t cur;
    uint64_t out=0;
    uint8_t len=strlen(target);
    for (uint8_t i=0; i<len; i++)
    {
        if ((uint8_t)target[i] < 58)
            cur = ((uint8_t)target[i])-48;
        else
            cur = ((uint8_t)target[i])-55;
        out += cur*pow(base,(len-i-1)); 
    }
    return out;
}

list<uint64_t> extractparts(char* target)
{
    list<uint64_t> parts = list<uint64_t>();
    string cur="";
    for (uint8_t i=0; i<strlen(target); i++)
    {
        if (target[i] != ',')
            cur.push_back(target[i]);
        else
        {
            parts.push_back(todecimal(cur.c_str(), 10)); 
            cur="";
        }
    }
    parts.push_back(todecimal(cur.c_str(), 10)); 
    return parts;
}

uint64_t decfrompos(char* target)
{
    list<uint64_t> parts = extractparts(target);
    uint8_t len = parts.size();
    uint64_t ret = 0;
    for (uint8_t i = 0; i < len; i++)
    {
        ret += pow(2, parts.front());
        parts.pop_front();
    }
    return ret;
}

uint64_t decfromrange(char* target)
{
    list<uint64_t> parts = extractparts(target);
    if (parts.size() != 2) 
        return -1;
    uint8_t start = parts.front();
    uint8_t end  = parts.back();
    uint64_t ret = 0;
    for (uint8_t i = start; i<=end; i++)
    {
        ret += pow(2, i);
    }
    return ret;
}

void convert(char* target, char base, char form)
{
    uint64_t val;
    if (((string)target).find(',') != std::string::npos)
    {
        if (form == 'p')
            val = decfrompos(target);
        if (form == 'r')
            val = decfromrange(target);
        print(val);
        return;
    }
    if (base == 'h')
        val = todecimal(target, 16);
    else if (base == 'd')
        val = todecimal(target, 10);
    else if (base == 'b')
    {
        if (form == 'v')
            val = todecimal(target, 2);
        else if (form == 'p')
            val = decfrompos(target);
        else if (form == 'r')
            val = decfromrange(target);
    }
    print(val);
    return;
}

int main(int argc, char* argv[]) 
{
    //cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    char inptype = 'b';
    char inpform = 'p';
    uint8_t size = 32;
    for (int i=1; i < argc; i++)
    {
        if (argv[i][0] == '-') 
        {
            for (int j=1; j < strlen(argv[i]); j++)
            {
                if (argv[i][j] == 'h' || argv[i][j] == 'd' || argv[i][j] == 'b')
                {
                    inptype = argv[i][j];
                }
                else if (argv[i][j] == 'v' || argv[i][j] == 'p' || argv[i][j] == 'r')
                {
                    inpform = argv[i][j];
                }
            }
        }
        else
        {
            convert(argv[i], inptype, inpform);
        }
    }
    //cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    return 0;
}
