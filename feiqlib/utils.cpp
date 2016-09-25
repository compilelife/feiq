#include "utils.h"

vector<string> splitAllowSeperator(vector<char>::iterator from, vector<char>::iterator to, char sep)
{
    vector<string> values;
    vector<char> buf;
    while(from < to)
    {
        if (*from == sep)
        {
            if (from+1 != to && *(from+1) == sep)
            {
                ++from;
            }
            else
            {
                char* value = new char[buf.size()+1];
                memcpy(value, buf.data(), buf.size());
                value[buf.size()]=0;
                values.push_back(value);
                delete[] value;
                buf.clear();
                ++from;
                continue;
            }
        }

        buf.push_back(*from);
        ++from;
    }

    return values;
}

void stringReplace(string& target,const string& pattern,const string& candidate)
{
    auto pos=0;
    auto ps=pattern.size();
    auto cs=candidate.size();
    while((pos=target.find(pattern,pos)) != string::npos)
    {
        target.replace(pos,ps,candidate);
        pos+=cs;
    }
}

string getFileNameFromPath(const string &path)
{
    auto sep = '/';
    auto result = path;
    if (result.at(result.length()-1) == sep)
        result = result.substr(0, result.length()-1);

    auto pos = result.find_last_of('/');
    if (pos == string::npos)
        return result;

    return result.substr(pos+1);
}

bool startsWith(const string &str, const string &patten)
{
    if (str.length() < patten.length())
        return false;
    return std::equal(patten.begin(), patten.end(), str.begin());
}

bool endsWith(const string &str, const string &patten)
{
    if (str.length() < patten.length())
        return false;
    return std::equal(patten.rbegin(), patten.rend(), str.rbegin());
}

string toString(const vector<char> &buf)
{
    auto len = buf.size();
    char* value = new char[len+1];
    memcpy(value, buf.data(), len);
    value[len]=0;
    string result = value;
    delete[] value;
    return result;
}
