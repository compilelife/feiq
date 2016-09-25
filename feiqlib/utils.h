#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
using namespace std;
vector<string> splitAllowSeperator(vector<char>::iterator from, vector<char>::iterator to, char sep);
void stringReplace(string& target,const string& pattern,const string& candidate);
string getFileNameFromPath(const string& path);
bool startsWith(const string& str, const string& patten);
bool endsWith(const string& str, const string& patten);
string toString(const vector<char>& buf);
#endif // UTILS_H
