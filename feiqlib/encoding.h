#ifndef ENCODING_H
#define ENCODING_H

#include <vector>
#include <string>
using namespace std;
#include <iconv.h>

class Encoding
{
public:
    Encoding(const string& fromCharset, const string& toCharset);
    ~Encoding();
    vector<char> convert(const vector<char>& str);
    string convert(const string& str);

    /**
     * @brief convert 编码转换
     * @param input 源内存首地址
     * @param len 源长度
     * @param output 目标内存首地址
     * @param outLen 输入输出参数，输入目标缓冲区大小，输出实际使用大小
     * @return
     */
    bool convert(const char* input, size_t len, char* output, size_t* outLen);
private:
    iconv_t mIconv;
};

extern Encoding* encOut;
extern Encoding* encIn;


#endif // ENCODING_H
