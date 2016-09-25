#include "encoding.h"
#include <iconv.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

Encoding* encIn = new Encoding("GBK", "UTF-8");
Encoding* encOut = new Encoding("UTF-8", "GBK");

Encoding::Encoding(const string &fromCharset, const string &toCharset)
{
    //iconv_open is a freek(to goes first)
    mIconv = iconv_open(toCharset.c_str(), fromCharset.c_str());
}

Encoding::~Encoding()
{
    if (mIconv != (iconv_t)-1)
        iconv_close(mIconv);
}

vector<char> Encoding::convert(const vector<char> &str)
{
    vector<char> result(str.size()*3);
    auto len = result.size();
    if (convert(str.data(), str.size(), result.data(), &len))
    {
        result.resize(len);
        result.shrink_to_fit();
        return result;
    }

    return str;
}

string Encoding::convert(const string &str)
{
    auto len = str.length()*3;
    unique_ptr<char[]> buf(new char[len]);
    if (convert(str.data(), str.size(), buf.get(), &len))
    {
        string result = buf.get();
        return result;
    }

    return str;
}

bool Encoding::convert(const char *input, size_t len, char *output, size_t *outLen)
{
    if (mIconv == (iconv_t)-1)
        return false;

    //copy in str
    size_t inLen = len;
    auto inBuf = unique_ptr<char[]>(new char[inLen+1]);
    char* pIn = inBuf.get();
    memcpy(pIn, input, inLen);
    pIn[inLen]=0;

    //do convert
    int ret = iconv(mIconv, &pIn, &inLen, &output, outLen);
    if (ret == -1)
    {
        perror("convert failed");
        return false;
    }

    *output = 0;

    return true;
}
