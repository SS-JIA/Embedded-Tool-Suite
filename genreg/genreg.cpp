#include <regex>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <stdint.h>
#include <stdexcept>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;
using std::regex;
using std::regex_replace;
using std::string;

string tohex(uint64_t target, int length)
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
    for (int i = ret.length(); i < length/4; i++)
    {
        ret = "0"+ret;
    }
    return "0x"+ret;
}

string tostring(int target, int base)
{
    if (target == 0)
        return "0";
    uint64_t num;
    uint8_t dig;
    char add;
    string ret = "";
    num = target;
    while (num > 0)
    {
        dig = num%base;
        if (dig < 10)
            add = (char)(dig+48);
        else
            add = (char)(dig+55);
        ret = add + ret;
        num /= base;
    }
    return ret;
}

string valmacroname(string format, int val, int off)
{
    string valstr = tostring(val,10);
    string offstr = tostring(off,10);
    regex valpat("\\{V\\}");
    regex offpat("\\{O\\}");
    string macro = regex_replace(format, valpat, valstr);
    macro = regex_replace(macro, offpat, offstr);
    return macro;
}

string maskmacroname(string format, int start, int length)
{
    string startstr = tostring(start, 10);
    string lengthstr = tostring(length, 10);
    string endstr    = tostring(start+length-1, 10);
    regex startpat("\\{S\\}");
    regex lengthpat("\\{L\\}");
    regex endpat("\\{E\\}");
    string macro = regex_replace(format, startpat, startstr);
    macro = regex_replace(macro, endpat, endstr);
    macro = regex_replace(macro, lengthpat, lengthstr);
    return macro;
}

void genmacros(string output, uint8_t regsize, uint8_t maxlength, string mformat, string vformat)
{
    time_t currentTime = time(0);
    struct tm *now = localtime(&currentTime);
    ofstream header;
    header.open(output);
    header << "/*********************************************************************************" << endl;
    header << " * FILE:        " << output << endl;
    header << " * DESCRIPTION: Defines bit masks for " << (int)regsize << " bit registers." << endl; 
    header << " *              Contains masks for regions up to " << (int)maxlength << " bits." << endl;
    header << " *              This file was automatically generated!" << endl;
    header << " * MODIFIED:    " << (now->tm_mon) << "/" << (now->tm_mday) << "/" << (now->tm_year + 1900) << endl;
    header << " * " << endl;
    header << " * Copyright SSJIA " << (now->tm_year + 1900) << endl;
    header << " ********************************************************************************/" << endl;
    string macro;
    header << endl;
    uint64_t maskbase = 0;
    for (uint8_t length=1; length<=maxlength; length++)
    {
        header << "/* Bit Masks - " << (int)length << " bit regions*/" << endl;
        maskbase = pow(2,length)-1;
        for (uint8_t off=0; off<regsize; off++)
        {
            if (off+length <= regsize)
            {
                macro = maskmacroname(mformat, off, length);
                header << "#define " << setw(30) << left << macro << "\t\t" << tohex((uint64_t)maskbase<<off, regsize) << endl;
            }
        }
    }
    for (uint8_t val=1; val<=pow(2,maxlength)-1; val++)
    {
        header << "/* Value Offsets - " << (int)val << "*/" << endl;
        for (uint8_t off=0; off<regsize; off++)
        {
            if (off+log2((double)val+1) <= regsize)
            {
                macro = valmacroname(vformat, val, off);
                header << "#define " << setw(30) << left << macro << "\t\t" << tohex((uint64_t)val<<off, regsize) << endl;
            }
        }
    }
    header.close();
}

int main(int argc, char* argv[])
{
    uint8_t reglength = 32;
    uint8_t maxlength = 3;
    string  output = "regops.h";
    string  mformat = "BITS_{S}_{E}";
    string  vformat = "{V}O{O}";
    for (uint8_t i = 1; i<argc; i++)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
                case 'o':
                    i++; 
                    if (i < argc)
                        output = (string)argv[i]; 
                    break;
                case 'r':
                    i++; 
                    if (i < argc)
                        reglength = stoi((string)argv[i]); 
                    break;
                case 'l':
                    i++; 
                    if (i < argc)
                        maxlength = stoi((string)argv[i]); 
                    break;
                case 'F':
                    i++;
                    if (i < argc)
                        mformat = (string)argv[i];
                    break;
                case 'f':
                    i++;
                    if (i < argc)
                        vformat = (string)argv[i];
                    break;
            }
        }
    }
    genmacros(output, reglength, maxlength, mformat, vformat);
    cout << "output to file: " << output << " | reg: " << to_string(reglength) << " | max length: " << to_string(maxlength) << endl;
}