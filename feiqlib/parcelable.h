#ifndef PARCELABLE_H
#define PARCELABLE_H

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class Parcel
{
private:
    class Head
    {
    public:
        Head(int size)
        {
            this->size = size;
        }

        Head(istream& ss)
        {
            read(ss);
        }

        int size;

        void myWriteInt(ostream& os, int val)
        {
            char buf[9] = {0};
            snprintf(buf, sizeof(buf), "%08d", val);
            os<<buf;
        }

        int myReadInt(istream& is)
        {
            char buf[9] = {0};
            is.read(buf, sizeof(buf)-1);
            return stoi(buf);
        }

        void write(ostream& os){
            myWriteInt(os, size);
        }

        void read(istream& is){
            size = myReadInt(is);
        }
    };

public:
    template<typename T>
    void write(const T& val){
        writePtr(&val, 1);
    }

    template<typename T>
    void read(T& val){
        readPtr(&val);
    }

    template<typename T>
    void writePtr(const T* ptr, int n){
        Head head{(int)(n * sizeof(T))};
        head.write(ss);
        ss.write((const char*)ptr, head.size);
    }

    template<typename T>
    void readPtr(T* ptr){
        Head head(ss);
        unique_ptr<char[]> buf(new char[head.size]);
        ss.read(buf.get(), head.size);
        memcpy(ptr, buf.get(), head.size);
    }

    void writeString(const string& val)
    {
        writePtr(val.c_str(), val.length());
    }

    void readString(string& val)
    {
        auto size = nextSize();
        unique_ptr<char[]> buf(new char[size+1]);
        readPtr(buf.get());
        buf[size]=0;
        val=buf.get();
    }

    void resetForRead()
    {
        ss.seekg(0, ss.beg);
    }

    void fillWith(const void* data, int len)
    {
        ss.clear();
        ss.write(static_cast<const char*>(data), len);
    }

public:
    streampos mark()
    {
        return ss.tellg();
    }

    void unmark(streampos markPos)
    {
        ss.seekg(markPos);
    }

public:
    vector<char> raw()
    {
        auto size = ss.tellp();
        resetForRead();

        vector<char> buf(size);
        ss.read(buf.data(), size);

        return buf;
    }

private:
    int nextSize()
    {
        auto pos = mark();
        Head head(ss);
        unmark(pos);
        return head.size;
    }

private:
    stringstream ss;
};

class Parcelable
{
public:
    virtual void writeTo(Parcel& out) const =0;
    virtual void readFrom(Parcel& in) =0;
};

#endif // PARCELABLE_H
