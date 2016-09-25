#ifndef DOWNLOADFILEDLG_H
#define DOWNLOADFILEDLG_H

#include <QDialog>
#include <memory>
#include "feiqlib/filetask.h"
#include "feiqlib/feiqengine.h"

using namespace std;

namespace Ui {
class DownloadFileDlg;
}


class FileManagerDlg : public QDialog
{
    Q_OBJECT

    typedef function<bool (const FileTask &)> SearchPredict;
public:
    explicit FileManagerDlg(QWidget *parent = 0);
    ~FileManagerDlg();

public:
    void setEngine(FeiqEngine* engine);
    void select(FileTask* task);

public slots:
    void statChanged(FileTask* task);
    void progressChanged(FileTask* task);

private slots:
    void delSelTask();
    void saveSelTask();
    void clear();
    void refresh();

private:
    void reloadWith(SearchPredict predict);
    QString typeString(FileTaskType type);
    QString stateString(const FileTask *task);
    QString progressString(const FileTask* task);
    FileTask* getTaskOfCurrentRow();
    int findRowByTask(const FileTask* task);

protected:
    virtual void showEvent(QShowEvent *) override;

private:
    Ui::DownloadFileDlg *ui;
    FeiqEngine* mEngine;
};

#endif // DOWNLOADFILEDLG_H
