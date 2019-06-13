#include "mysql_install.h"
#include "ui_mysql_install.h"
#include "JlCompress.h"
#include "quazip.h"


/*
卸载MySQL数据库：管理员打开cmd，切换到mysql 的安装路径/bin里，mysqld -remove
因为MySQL的密码，数据库内容会保存在原路径下，假如在其他盘符安装MySQL时，删除原MySQL的源文件
问安装的MySQL密码为：123456
*/
QString trans_CODEC2UTF8(const QByteArray &in, const QByteArray &codec)
{
    QTextCodec *c = QTextCodec::codecForName(codec);
    return c->toUnicode(in);
}

QByteArray trans_u2code(const QString &in, const QByteArray &codec)
{
    QTextCodec *c = QTextCodec::codecForName(codec);
    QByteArray r = c->fromUnicode(in);
    return r;
}

QString trans_GBK2UFT8(const QByteArray &in)
{
    return trans_CODEC2UTF8(in, "GBK");
}

QByteArray trans_UTF82GBK(const QString &in)
{
    QTextCodec *c = QTextCodec::codecForName("GBK");
    return c->fromUnicode(in);
}

mysql_install::mysql_install(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mysql_install)
{
    ui->setupUi(this);
    d.setMessageBar(ui->statusBar);

    ui->label->setText(ui->label->text() + "\n" + "sysname:" + d.sysname);
    ui->label->setText(ui->label->text() + "\n" + "sysbit:" + QString::number(d.sysbit));


    ui->label->setText(ui->label->text() + "\n" + "buildCpuArchitecture:" + QSysInfo::buildCpuArchitecture());
    ui->label->setText(ui->label->text() + "\n" + "currentCpuArchitecture:" + QSysInfo::currentCpuArchitecture());
    ui->label->setText(ui->label->text() + "\n" + "buildAbi:" + QSysInfo::buildAbi());

    ui->label->setText(ui->label->text() + "\n" + "kernelType:" + QSysInfo::kernelType());
    ui->label->setText(ui->label->text() + "\n" + "kernelVersion:" + QSysInfo::kernelVersion());
    ui->label->setText(ui->label->text() + "\n" + "productType:" + QSysInfo::productType());
    ui->label->setText(ui->label->text() + "\n" + "productVersion:" + QSysInfo::productVersion());
    ui->label->setText(ui->label->text() + "\n" + "prettyProductName:" + QSysInfo::prettyProductName());

    ui->label->setText(ui->label->text() + "\n" + "machineHostName:" + QSysInfo::machineHostName());
}

mysql_install::~mysql_install()
{
    delete ui;
}

void mysql_install::on_pushButton_check_clicked()
{
    switch(d.MySQLCheck()) {
    case mysql_install_obj::MySQL_STATE_RUNNING:
        QMessageBox::warning(this, "", "MySQL_STATE_RUNNING");
        break;
    case mysql_install_obj::MySQL_STATE_STOPPED:
        QMessageBox::warning(this, "", "MySQL_STATE_STOPPED");
        break;
    case mysql_install_obj::MySQL_STATE_UNEXISTED:
        QMessageBox::warning(this, "", "MySQL_STATE_UNEXISTED");
        break;
    case mysql_install_obj::MySQL_STATE_ERROR:
        QMessageBox::warning(this, "", "MySQL_STATE_ERROR");
        break;
    default:
        QMessageBox::warning(this, "", "DEFAULT_ERROR");
        break;
    }
}



void mysql_install::on_pushButton_run_clicked()
{
    QByteArray readall;
    QString errstring;
    if(!d.cmd("net start mysql", QStringList(), errstring, &readall)) {
        QMessageBox::warning(this, "", errstring);
        return;
    }
    QMessageBox::information(this, "", trans_GBK2UFT8(readall));
}

void mysql_install::on_pushButton_stop_clicked()
{
    QByteArray readall;
    QString errstring;
    if(!d.cmd("net stop mysql", QStringList(), errstring, &readall)) {
        QMessageBox::warning(this, "", errstring);
        return;
    }
    QMessageBox::information(this, "", trans_GBK2UFT8(readall));
}

void mysql_install::on_pushButton_install_completely_clicked()
{
    QString info;
    d.MySQLInstall(info);
    QMessageBox::information(this, "", info);
}

void mysql_install::on_pushButton_exit_clicked()
{
    this->close();
}

mysql_install_obj::mysql_install_obj(QObject *parent)
    : QObject(parent)
{
    this->messageBar = 0;
    getWindowVersion(sysname, sysbit);

    QString root = qApp->applicationDirPath();
    while(!root.endsWith(":"))
        root.chop(1);
    ProgramFiles_path = root + "/Program Files";
    if(sysbit == 64) {
        MySQL_path = ProgramFiles_path + "/mysql-5.7.18-winx64";
        MySQL_zip_filepath = qApp->applicationDirPath() + "/quazip/mysql-5.7.18-winx64.zip";
    }
    if(sysbit == 32) {
        MySQL_path = ProgramFiles_path + "/mysql-5.7.18-win32";
        MySQL_zip_filepath = qApp->applicationDirPath() + "/quazip/mysql-5.7.18-win32.zip";
    }

}

mysql_install_obj::MySQL_STATE mysql_install_obj::MySQLCheck()
{
    QByteArray r;
    QString errstring;
    if(!cmd("sc query MySQL", QStringList(), errstring, &r)) {
        qDebug() << errstring;
        return MySQL_STATE_ERROR;
    }
    QBuffer buff(&r);
    buff.open(QIODevice::ReadOnly);
    while(!buff.atEnd()) {
        QByteArray line = buff.readLine();
        if(line.contains("STATE")) {
            if(line.trimmed().endsWith("RUNNING")) {
                return MySQL_STATE_RUNNING;
            }
            return MySQL_STATE_STOPPED;
        }
    }
    return MySQL_STATE_UNEXISTED;
}

bool mysql_install_obj::unzip(QString fileCompressed, QString dir)
{
    showMessage("unzip:" + fileCompressed);
    return JlCompress::extractDir(fileCompressed, dir).count() != 0;
}

void mysql_install_obj::getWindowVersion(QString &sysname, int &sysbit)
{
    typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
    LPFN_ISWOW64PROCESS fnIsWow64Process;
    BOOL bIsWow64 = FALSE;
    fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress( GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
    if (NULL != fnIsWow64Process)
    {
        fnIsWow64Process(GetCurrentProcess(),&bIsWow64);
    }
    if(bIsWow64)
        sysbit = 64;// = "64位";
    else
        sysbit = 32;//"32位";

    sysname = QSysInfo::prettyProductName();
}

void mysql_install_obj::setMessageBar(QObject *messageBar)
{
    this->messageBar = messageBar;
}

bool mysql_install_obj::install()
{
    QString info;
    bool f = MySQLInstall(info);
    showMessage(info);
    if(!f)
        return false;
    return MySQLCheck() == MySQL_STATE_RUNNING;
}

bool mysql_install_obj::cmd(QString program, const QStringList &args, QString &errstring, QByteArray *readall)
{
    showMessage(program + " " + args.join(" "));
    QProcess p;
    if(program.contains("Program Files"))
        program = "\"" + program + "\"";
    if(args.count() == 0)
        p.start(program);
    else
        p.start(program, args);
    if(!p.waitForFinished(-1)) {
        errstring = p.errorString();
        return false;
    }
    if(readall)
        *readall = p.readAll();
    return true;
}

bool mysql_install_obj::MySQLInstall(QString &info)
{
    QString errstring;
    MySQL_STATE state;
    state = MySQLCheck();
    if(MySQL_STATE_RUNNING == state) {
        info = "mysql is running";
        return true;
    }

    if(MySQL_STATE_STOPPED == state) {
        if(!cmd("net start mysql", QStringList(), errstring)){
            info = "net start mysql failed";
            return false;
        }
        info = "net start mysql successfully";
        return true;
    }
    if(MySQL_STATE_UNEXISTED == state) {
        if(!QDir(MySQL_path).exists()) {
            if(!unzip(MySQL_zip_filepath, ProgramFiles_path)) {
                info = "mysql unzip failed";
                return false;
            }
        }
        if(!QFile::exists(MySQL_path + "/my.ini")) {
            QSettings conf(MySQL_path + "/my.ini", QSettings::IniFormat);
            conf.setValue("mysqld/basedir", MySQL_path);
            conf.setValue("mysqld/datadir", MySQL_path + "/data");
            conf.setValue("mysqld/port", 3306);
            conf.sync();
        }
        if(!cmd(MySQL_path + "/bin/mysqld", QStringList() << "-install", errstring)) {
            info = "mysql install failed";
            return false;
        }
        if(!cmd(MySQL_path + "/bin/mysqld", QStringList() << "--initialize-insecure", errstring)) {
            info = "mysql init failed";
            return false;
        }
        if(!cmd("net start mysql", QStringList(), errstring)) {
            info = "net start mysql failed";
            return false;
        }
        if(!cmd(MySQL_path + "/bin/mysqladmin", QStringList() << "-u" << "root" << "password" << "123456", errstring)) {
            info = "mysql init password failed";
            return false;
        }
        info = "install successfully";
        return true;
    }

    info = "unknown error";
    return false;
}

void mysql_install_obj::showMessage(const QString &text)
{
    QMetaObject::invokeMethod(messageBar, "showMessage", Qt::AutoConnection, Q_ARG(QString,text));
}
