#include "filemanagerdlg.h"
#include "ui_downloadfiledlg.h"
#include <sstream>
#include <QMessageBox>
#include <QFileDialog>

#define TYPE_COL 0
#define STATE_COL 1
#define FELLOW_COL 2
#define FILENAME_COL 3
#define PROGRESS_COL 4

FileManagerDlg::FileManagerDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadFileDlg)
{
    ui->setupUi(this);

    connect(ui->clearBtn, SIGNAL(clicked(bool)), this, SLOT(clear()));
    connect(ui->delBtn, SIGNAL(clicked(bool)), this, SLOT(delSelTask()));
    connect(ui->saveBtn, SIGNAL(clicked(bool)), this, SLOT(saveSelTask()));
    connect(ui->searchEdit, SIGNAL(textChanged(QString)), this, SLOT(refresh()));
}

FileManagerDlg::~FileManagerDlg()
{
    delete ui;
}

void FileManagerDlg::setEngine(FeiqEngine *engine)
{
    mEngine = engine;
}

void FileManagerDlg::select(FileTask *task)
{
    refresh();
    auto row = findRowByTask(task);
    if (row == -1)
        return;

    ui->taskTable->setCurrentCell(row, TYPE_COL);
}

void FileManagerDlg::statChanged(FileTask *task)
{
    auto row = findRowByTask(task);
    if (row == -1)
        return;

    auto str = stateString(task);
    auto item = ui->taskTable->item(row, STATE_COL);
    item->setText(str);
}

void FileManagerDlg::progressChanged(FileTask *task)
{
    auto row = findRowByTask(task);
    if (row == -1)
        return;

    auto str = progressString(task);
    auto item = ui->taskTable->item(row, PROGRESS_COL);
    item->setText(str);
}

void FileManagerDlg::delSelTask()
{
    auto task = getTaskOfCurrentRow();
    if (task)
    {
        if (task->getState() == FileTaskState::Running)
        {
            QMessageBox::warning(this, "无法删除", "不能删除正上传或下载中的任务", QMessageBox::Ok);
        }
        else
        {
            mEngine->getModel().removeFileTask([task](const FileTask& t){
                return &t == task;
            });
            refresh();
        }
    }
}

void FileManagerDlg::saveSelTask()
{
    auto task = getTaskOfCurrentRow();
    if (task && task->type() == FileTaskType::Download)
    {
        if (task->getContent()->path.empty())
        {
            QString path = QFileDialog::getExistingDirectory(this, "选择保存到……");
            if (!path.isEmpty())
            {
                task->getContent()->path = path.toStdString()+"/"+task->getContent()->filename;
            }
        }

        if (!task->getContent()->path.empty())
            mEngine->downloadFile(task);
    }
}

void FileManagerDlg::clear()
{
    mEngine->getModel().removeFileTask([](const FileTask& task){
        return task.getState() != FileTaskState::Running && task.getState() != FileTaskState::NotStart;
    });
    refresh();
}

void FileManagerDlg::refresh()
{
    auto filter = ui->searchEdit->text().toStdString();
    reloadWith([filter](const FileTask& task){
        if (!filter.empty())
        {
            return task.getContent()->filename.find(filter)!=string::npos
                    ||task.fellow()->getName().find(filter)!=string::npos;
        }
        return true;
    });
}

void FileManagerDlg::reloadWith(FileManagerDlg::SearchPredict predict)
{
    if (!mEngine)
        return;

    auto table = ui->taskTable;
//    table->clearContents();
    auto rowCount = table->rowCount();
    for (int i = 0; i < rowCount; i++)
        table->removeRow(0);
    auto tasks = mEngine->getModel().searchTask(predict);
    for (shared_ptr<FileTask> task : tasks)
    {
        auto row = table->rowCount();
        table->insertRow(row);
        auto item = new QTableWidgetItem(typeString(task->type()));
        item->setData(Qt::UserRole, QVariant::fromValue((void*)(task.get())));//把task保存到UserRole中
        table->setItem(row, TYPE_COL, item);
        table->setItem(row, STATE_COL, new QTableWidgetItem(stateString(task.get())));
        table->setItem(row, FELLOW_COL, new QTableWidgetItem(task->fellow()->getName().c_str()));
        table->setItem(row, FILENAME_COL, new QTableWidgetItem(task->getContent()->filename.c_str()));
        table->setItem(row, PROGRESS_COL, new QTableWidgetItem(progressString(task.get())));
    }
    table->resizeColumnToContents(TYPE_COL);
    table->resizeColumnToContents(TYPE_COL);
    table->resizeColumnToContents(FELLOW_COL);
    table->resizeColumnToContents(FILENAME_COL);

    table->setCurrentCell(0,0);
}

QString FileManagerDlg::typeString(FileTaskType type)
{
    switch (type)
    {
    case FileTaskType::Download:
        return "下载";
    case FileTaskType::Upload:
        return "上传";
    default:
        break;
    }
    return "";
}

QString FileManagerDlg::stateString(const FileTask* task)
{
    switch (task->getState()) {
    case FileTaskState::Canceled:
        return "已取消";
    case FileTaskState::Error:
        return task->getDetailInfo().c_str();
    case FileTaskState::Finish:
        return "已完成";
    case FileTaskState::NotStart:
        return "还未开始";
    case FileTaskState::Running:
        if (task->type()==FileTaskType::Upload)
            return "正在上传";
        else
            return "正在下载";
    default:
        break;
    }

    return "";
}

QString FileManagerDlg::progressString(const FileTask *task)
{
    stringstream ss;
    float percent = task->getProcess()*100.0f/task->getContent()->size;
    ss<<task->getProcess()<<"/"<<task->getContent()->size<<"("<<percent<<"%)";
    return ss.str().c_str();
}

FileTask *FileManagerDlg::getTaskOfCurrentRow()
{
    auto row = ui->taskTable->currentRow();
    auto widget = ui->taskTable->item(row, TYPE_COL);
    if (widget == nullptr)
        return nullptr;

    auto task = widget->data(Qt::UserRole);
    return static_cast<FileTask*>(task.value<void*>());
}

int FileManagerDlg::findRowByTask(const FileTask *task)
{
    int rowCount = ui->taskTable->rowCount();
    for (int i = 0; i < rowCount; i++)
    {
        auto widget = ui->taskTable->item(i, TYPE_COL);
        auto itemTask = static_cast<FileTask*>(widget->data(Qt::UserRole).value<void*>());
        if (itemTask == task)
            return i;
    }

    return -1;
}

void FileManagerDlg::showEvent(QShowEvent *)
{
    refresh();
}
