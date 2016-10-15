#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <list>
#include <tuple>

typedef std::string string;
typedef std::tuple<std::vector<int>, string> row;
typedef std::vector<row> sizecol;

void show(std::vector<string> &terms)
{
    for(int i=0; i < terms.size(); i++)
    {
        std::cout << i << "-" << terms[i] << std::endl;
    }
}

bool validate(int size, string item)
{
    if (item.length() != size)
        return false;
    for (int i=0; i<item.length(); i++)
    {
        if (item.at(i) != '0' && item.at(i) != '1')
            return false;
    }
    return true;
}

int match(string first, string second)
{
    bool found = false;
    int pos = -1;
    for (int i=0; i < first.length(); i++)
    {
        if(first.at(i) != second.at(i))
        {
            if(!found)
            {
                pos = i; found = true;
            }
            else
                return -1;
        }
    }
    return pos;
}

void addtolist(std::vector<string> &terms, string item)
{
    bool proceed = true;
    if (terms.size() > 0)
        proceed = validate(terms[0].length(), item);
    if (proceed)
        terms.push_back(item);
    else
        std::cout << "invalid input!" << std::endl;
}

sizecol gensize2(std::vector<string> &terms, sizecol &primeimplicants)
{
    const int length = terms.size();
    int matchcount[length];
    std::fill_n(matchcount, length, 0);
    int totalmatches = 0;
    sizecol size2;
    int matchpos;
    string combined;
    std::vector<int> association;
    for (int i=0; i<length; i++)
    {
        for (int j=i+1; j<length; j++)
        {
            matchpos = match(terms[i], terms[j]);
            if (matchpos >= 0)
            {
                combined = terms[i];
                association.clear();
                association.push_back(i);
                association.push_back(j);
                size2.push_back(std::make_tuple(association, combined.replace(matchpos, 1, "X")));
                matchcount[i] += 1;
                matchcount[j] += 1;
            }
        }
    }
    for (int i=0; i<length; i++)
    {
        if (matchcount[i] == 0)
        {
            association.clear();
            association.push_back(i);
            primeimplicants.push_back(std::make_tuple(association, terms[i]));
        }
    }
    return size2;
}

bool vectorcontains(std::vector<string> &v, string target)
{
    for (int i=0; i<v.size(); i++)
    {
        if (v[i] == target)
            return true;
    }
    return false;
}

sizecol gennextsize(sizecol &terms, sizecol &primeimplicants)
{
    const int length = terms.size();
    int matchcount[length];
    std::fill_n(matchcount, length, 0);
    std::vector<int> association;
    sizecol nextsize;
    int matchpos;
    string combined;
    string term1, term2;
    std::vector<int> *assoc1;
    std::vector<int> *assoc2;
    std::vector<string> blacklist;
    for (int i=0; i<terms.size(); i++)
    {
        for (int j=i+1; j<terms.size(); j++)
        {
            term1 = std::get<1>(terms[i]);
            term2 = std::get<1>(terms[j]);
            matchpos = match(term1, term2);
            if (matchpos >= 0)
            {
                assoc1 = &(std::get<0>(terms[i]));
                assoc2 = &(std::get<0>(terms[j]));
                combined = term1.replace(matchpos, 1, "X");
                if (!vectorcontains(blacklist, combined))
                {
                    association.clear();
                    association.insert(association.end(), assoc1->begin(), assoc1->end());
                    association.insert(association.end(), assoc2->begin(), assoc2->end());
                    nextsize.push_back(std::make_tuple(association, combined));
                    blacklist.push_back(combined);
                }
                matchcount[i] += 1;
                matchcount[j] += 1;
            }
        }
    }
    for (int i=0; i<length; i++)
    {
        if (matchcount[i] == 0)
        {
            term1 = std::get<1>(terms[i]);
            assoc1 = &(std::get<0>(terms[i]));
            association.clear();
            association.insert(association.end(), assoc1->begin(), assoc1->end());
            primeimplicants.push_back(std::make_tuple(association, term1));
        }
    }
    return nextsize;
}

sizecol findprimeimplicants(std::vector<string> &terms)
{
    sizecol primeimplicants;
    sizecol sizen;
    int cursize = 2;
    sizen = gensize2(terms, primeimplicants);
    while (sizen.size() > 0)
    {
        sizen = gennextsize(sizen, primeimplicants);
    }
    return primeimplicants;
}

void printsizecol(sizecol &col)
{
    std::vector<int> *assoc;
    for (int i=0; i<col.size(); i++)
    {
        std::cout << "  " << std::get<1>(col[i]);
        assoc = &(std::get<0>(col[i]));
        std::cout << std::setw(5) << "{";
        for (int j=0; j<assoc->size()-1; j++)
        {
            std::cout << assoc->at(j) << ",";
        }
        std::cout << assoc->at(assoc->size()-1) << "}" << std::endl;
    }
}

void solve(std::vector<string> &terms)
{
    sizecol primeimplicants = findprimeimplicants(terms);
    printsizecol(primeimplicants);
}

void help()
{
    std::cout << "test" << std::endl;
}

int main(int argc, char* argv[])
{   
    string response;
    std::vector<string> minterms;

    //testvars
    string match1;
    string match2;
    sizecol size2;
    sizecol nextsize;
    sizecol primeimplicants;
    
    minterms.push_back("0100");
    minterms.push_back("1000");
    minterms.push_back("1001");
    minterms.push_back("1010");
    minterms.push_back("1100");
    minterms.push_back("1011");
    minterms.push_back("1110");
    minterms.push_back("1111");


    while(true)
    {
        std::cout << "> ";
        std::cin >> response;
        if (response == "help")
            help();
        else if (response == "show")
            show(minterms);
        else if (response == "exit")
            return 0;
        else if (response == "match")
        {
            std::cin >> match1;
            std::cin >> match2;
            std::cout << match(match1, match2) << std::endl;
        }
        else if (response == "size2")
        {
            size2 = gensize2(minterms, primeimplicants);
            nextsize = gennextsize(size2, primeimplicants);
            nextsize = gennextsize(nextsize, primeimplicants);
        }
        else if (response == "solve")
        {
            solve(minterms);
        }
        else
        {
            addtolist(minterms, response);
        }
    }
}
