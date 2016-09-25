#ifndef HISTORY_H
#define HISTORY_H

#include "fellow.h"
#include <memory>
#include <vector>
#include "content.h"
#include "post.h"
#include <sqlite3.h>
#include <unordered_map>

using namespace std;

struct HistoryRecord{
    time_point<steady_clock, milliseconds> when;
    shared_ptr<Fellow> who;
    shared_ptr<Content> what;
};

//日志记录
    //完成代码
    //调试代码
    //加入model，合并model功能
    //加入engine自动记录
    //按好友、日期查询最近记录
    //更新文件path
/**
 * @brief The History class 以Content为单位，记录和查询聊天记录
 * 还只是个半成品~
 */
class History
{
public:
    History();

public:
    bool init(const string& dbPath);
    void unInit();

public:
    void add(const HistoryRecord &record);
    vector<HistoryRecord> query(const string& selection, const vector<string> &args);
private:
    unique_ptr<Fellow> getFellow(int id);
    int findFellowId(const string& ip);
private:
    sqlite3* mDb = nullptr;
};

#endif // HISTORY_H
