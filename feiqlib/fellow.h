#ifndef FELLOW_H
#define FELLOW_H

#include <string>
#include <memory>
#include <sstream>
using namespace std;

class Fellow
{
public:
    string getIp() const{return mIp;}
    string getName() const{return mName.empty() ? mPcName : mName;}
    string getHost() const{return mHost;}
    string getMac() const{return mMac;}
    bool isOnLine() const{return mOnLine;}
    string version() const{return mVersion;}

    void setIp(const string& value){
        mIp = value;
    }

    void setName(const string& value){
        mName = value;
    }

    void setHost(const string& value){
        mHost = value;
    }

    void setMac(const string& value){
        mMac = value;
    }

    void setOnLine(bool value){
        mOnLine = value;
    }

    void setVersion(const string& value){
        mVersion = value;
    }

    void setPcName(const string& value){
        mPcName = value;
    }

    bool update(const Fellow& fellow)
    {
        bool changed = false;

        if (!fellow.mName.empty() && mName != fellow.mName){
            mName = fellow.mName;
            changed=true;
        }

        if (!fellow.mMac.empty() && mMac != fellow.mMac){
            mMac = fellow.mMac;
            changed=true;
        }

        if (mOnLine != fellow.mOnLine){
            mOnLine = fellow.mOnLine;
            changed=true;
        }

        return changed;
    }

    bool operator == (const Fellow& fellow)
    {
        return isSame(fellow);
    }

    bool isSame(const Fellow& fellow)
    {
        return mIp == fellow.mIp || (!mMac.empty() && mMac == fellow.mMac);
    }

    string toString() const
    {
        ostringstream os;
        os<<"["
         <<"ip="<<mIp
        <<",name="<<mName
        <<",host="<<mHost
        <<",pcname="<<mPcName
        <<",mac="<<mMac
        <<",online="<<mOnLine
        <<",version="<<mVersion
        <<"]";
        return os.str();
    }

private:
    string mIp;
    string mPcName;
    string mName;
    string mHost;
    string mMac;
    bool mOnLine;
    string mVersion;
};

#endif // FELLOW_H
