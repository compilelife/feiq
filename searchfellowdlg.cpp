#include "searchfellowdlg.h"
#include "ui_searchfellowdlg.h"

SearchFellowDlg::SearchFellowDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchFellowDlg)
{
    ui->setupUi(this);

    connect(ui->searchEdit, SIGNAL(textChanged(QString)), this, SLOT(search(QString)));
    connect(ui->cancelBtn, SIGNAL(pressed()), this, SLOT(hide()));
    connect(ui->okBtn, SIGNAL(pressed()), this, SLOT(searchDone()));
    connect(ui->resultListWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(searchDone()));
}

SearchFellowDlg::~SearchFellowDlg()
{
    delete ui;
}

void SearchFellowDlg::setSearchDriver(function<vector<const Fellow*>(const QString &)> driver)
{
    mSearchDriver = driver;
}

void SearchFellowDlg::search(const QString &text)
{
    if (mSearchDriver)
    {
        mCurResult = mSearchDriver(text);
        ui->resultListWidget->clear();
        for (const Fellow* fellow : mCurResult)
            ui->resultListWidget->addItem(QString(fellow->toString().c_str()));
        if (mCurResult.size() > 0)
            ui->resultListWidget->setCurrentRow(0);

        ui->okBtn->setEnabled(mCurResult.size()>0);
    }
}

void SearchFellowDlg::searchDone()
{
    auto row = ui->resultListWidget->currentRow();
    if (row >= 0)
    {
        emit onFellowSelected(mCurResult[row]);
        hide();
    }
}

void SearchFellowDlg::showEvent(QShowEvent *)
{
    loadAllFellows();
}

void SearchFellowDlg::loadAllFellows()
{
    search("");//search 空字符串将获得所有好友
}
