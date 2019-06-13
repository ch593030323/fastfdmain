#ifndef MYSQL_INSTALL_H
#define MYSQL_INSTALL_H

#include <QMainWindow>
#include <QtWidgets>

namespace Ui {
class mysql_install;
}

class mysql_install_obj : public QObject {
    Q_OBJECT
public:
    enum MySQL_STATE{
        MySQL_STATE_RUNNING,
        MySQL_STATE_STOPPED,
        MySQL_STATE_UNEXISTED,
        MySQL_STATE_ERROR
    };

public:
    mysql_install_obj(QObject *parent = 0);

    MySQL_STATE MySQLCheck();

    bool unzip(QString fileCompressed, QString dir);

    void getWindowVersion(QString &sysname, int &sysbit);

    void setMessageBar(QObject *messageBar);

    bool install();

public:
    bool cmd(QString program, const QStringList &args, QString &errstring, QByteArray *readall = 0);

    bool MySQLInstall(QString &info);

    void showMessage(const QString &text);

public:
    QString sysname;
    int sysbit;
    QString MySQL_path;
    QString ProgramFiles_path;
    QString MySQL_zip_filepath;
    QObject *messageBar;
};

class mysql_install : public QMainWindow
{
    Q_OBJECT

public:
    explicit mysql_install(QWidget *parent = 0);

    ~mysql_install();

private slots:
    void on_pushButton_check_clicked();

    void on_pushButton_run_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_install_completely_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::mysql_install *ui;

    mysql_install_obj d;
};

#endif // MYSQL_INSTALL_H
