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
    header << " " << endl;
    header << "/* Set and Clear Bits from register value */" << endl;
    header << "#define SETBITV(R, P) R |= 0x01<<P" << endl;
    header << "#define CLRBITV(R, P) R &= ~(0x01<<P)" << endl;
    header << "/* Set and Clear Bits from register address */" << endl;
    header << "#define SETBITA(R, P)  *R |= 0x01<<P" << endl;
    header << "#define CLRBITA(R, P)  *R &= ~(0x01<<P)" << endl;
    header << "/* Read the value of a bit from register value */" << endl;
    header << "#define READBITV(R, P) ((R>>P)%2)" << endl;
    header << "/* Read the value of a bit from register address */" << endl;
    header << "#define READBITA( R, P ) ((*R>>P)%2)" << endl;
    header << " " << endl;
    header << "/* Set Multiple Bits in a Register */" << endl;
    header << "#define SETREGV(R, S, V, L) R = (R&~(L<<S))|(V<<S)" << endl;
    header << "#define SETREGA(R, S, V, L) *R = (*R&~(L<<S))|(V<<S)" << endl;
    header << "/* Read Multiple Bits in a Register */" << endl;
    header << "#define READREGV(R, S, L) (R&(L<<S))>>S" << endl;
    header << "#define READREGA(R, S, L) (*R&(L<<S))>>S" << endl;
    header << " " << endl;
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

int helptext()
{
    cout << "GENREG - A Bit Mask Macro Generator for C and C++" << endl;
    cout << " " << endl;
    cout << "usage: genreg [options]" << endl;
    cout << " " << endl;
    cout << "OPTIONS" << endl;
    cout << "\t-o [output file name]\t\tset the name of the output file (default genreg.h)" << endl;
    cout << "\t-r [register length]\t\tset the target register length" << endl;
    cout << "\t-l [maximum mask length]\tset the maximum number of consecutive bits to generate masks for" << endl;
    cout << " " << endl;
    cout << "MACRO NAMING OPTIONS" << endl;
    cout << "\t-F [mask macro format]\t\tset the naming convention of mask macros (any string)" << endl;
    cout << "\t   e.g. for the format BITS_{S}_{E}, the macro for the mask 000111000 will be named BITS_3_5" << endl;
    cout << "\t   {S} -> start bit, {E} -> end bit, {L} -> length of region masked" << endl;
    cout << "\t-f [value offset macro format\t\t set the naming convention of value offset macros (any string)" << endl;
    cout << "\t   e.g. for the format {0}O{O}, the macro for 101000 (5 offset by 3 bits) will be named 5O3" << endl;
    cout << "\t   {V} -> value being offset, {O} -> number of bits to offset by" << endl;
    return 0;
}

int main(int argc, char* argv[])
{
    uint8_t reglength = 32;
    uint8_t maxlength = 3;
    string  output = "regops.h";
    string  mformat = "BITS_{S}_{E}";
    string  vformat = "V{V}O{O}";
    string longarg;
    for (uint8_t i = 1; i < argc; i++)
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
                case '-':
                    longarg = argv[i];
                    if (longarg == "--help")
                        return helptext();
                    break;
            }
        }
    }
    genmacros(output, reglength, maxlength, mformat, vformat);
    cout << "output to file: " << output << " | reg: " << to_string(reglength) << " | max length: " << to_string(maxlength) << endl;
}
