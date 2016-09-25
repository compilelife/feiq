#ifndef SEARCHFELLOWDLG_H
#define SEARCHFELLOWDLG_H

#include <QDialog>
#include <functional>
#include <vector>
#include "feiqlib/fellow.h"

using namespace std;

namespace Ui {
class SearchFellowDlg;
}

class SearchFellowDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SearchFellowDlg(QWidget *parent = 0);
    ~SearchFellowDlg();

    void setSearchDriver(function<vector<const Fellow*>(const QString &)> driver);

signals:
    void onFellowSelected(const Fellow* fellow);

public slots:
    void search(const QString& text);
    void searchDone();
protected:
    virtual void showEvent(QShowEvent *) override;

private:
    void loadAllFellows();

private:
    Ui::SearchFellowDlg *ui;
    function<vector<const Fellow*>(const QString& text)> mSearchDriver;
    vector<const Fellow*> mCurResult;
};

#endif // SEARCHFELLOWDLG_H
