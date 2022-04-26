#include "dictionary.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QFile>
#include <QDebug>
#include <QSqlRecord>
Dictionary::Dictionary(QObject* parent) : QObject (parent){}

bool Dictionary::connectDB()
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");


    //    QFile file("/storage/emulated/0/data/lookup.db");
    //    if(!file.exists() || file.size() == 0)
    //    {
    //        QFile::copy("assets:/dbfile/lookup.db","/storage/emulated/0/data/lookup.db");
    //        file.setPermissions(QFile::ReadUser  | QFile::WriteUser);
    //    }
    //    db.setDatabaseName("/storage/emulated/0/data/lookup.db");

    db.setDatabaseName("./lookup.db");
    if(!db.open())
    {
        qDebug() << "fail to open DB";
        return false;
    }
    return true;
}

QVariantList Dictionary::searchWord(QString wordName,QString prefiex)
{
    if(connectDB()==false)
    {
        return QVariantList();
    }
    QSqlQuery query;
    //查询
    bool sqlRes = query.exec(QString("select word from '%1' order by word ASC").arg(wordName));
    if(!sqlRes)
    {
        qDebug() << "error :" << query.lastError();
        return QVariantList();
    }
    QVariantList zoneIdList;
    bool start = false;
    while(query.next())
    {
        QString tmpWord = query.value(0).toString();
        if( tmpWord.indexOf(prefiex) == 0 )
        {
            start = true;
            zoneIdList.push_back(tmpWord);
        }
        else if( start ) break;
    }
    return  zoneIdList;
}

QVariantList Dictionary::searchTargetWord(QString wordName, QString word)
{
    if(connectDB()==false)
    {
        qDebug() << "connect db fail";
        return QVariantList();
    }
    QSqlQuery query;
    //查询
    qDebug() << "The word is = " << word;
    bool sqlRes = query.exec(QString("select * from '%1' where word = '%2'").arg(wordName).arg(word));
    if(!sqlRes)
    {
        qDebug() << "error :" << query.lastError();
        return QVariantList();
    }
    QVariantList zoneIdList;
    query.next();
    for(int i = 0 ; i < query.record().count() ; ++ i)
    {
        qDebug() << "word context : = " << query.value(i).toString();
        zoneIdList.push_back(query.value(i).toString());
    }
    query.next();
    zoneIdList.push_back(query.value(1).toString());

    return zoneIdList;
}

QVariant Dictionary::getSentence(QString str)
{
    if(connectDB()==false)
    {
        qDebug() << "connect db fail";
        return QVariantList();
    }
    QSqlQuery query;
    //查询
    qDebug() << "The word is = " << str;
    bool sqlRes = query.exec(QString("select * from sentence where englishWord = '%1'").arg(str));
    if(!sqlRes)
    {
        qDebug() << "error :" << query.lastError();
        return QVariantList();
    }
    QVariantList zoneIdList;
    query.next();
    for(int i = 3 ; i < 7 ; ++ i)
    {
        qDebug() << "word context : = " << query.value(i).toString();
        if(query.value(i).toString() == "") return zoneIdList;
        zoneIdList.push_back(query.value(i).toString());
    }
    return zoneIdList;
}

bool Dictionary::createCollectTable(int sno)
{
    if(connectDB()==false)
    {
        qDebug() << "connect db fail";
        return false;
    }
    QSqlQuery query;
    QString tablename = "collectTable" + QString::number(sno);
    qDebug() << "The tablename is = " << tablename;
    bool sqlRes = query.exec(QString("create table %1 "
                                     "(word text primary key,accent text,mean_cn text,freq int,"
                                     "wordlength int,exID int,tenses text,voice text)").arg(tablename));
    if(!sqlRes)
    {
        qDebug() << "error :" << query.lastError();
        return false;
    }
    return true;
}

void Dictionary::createAllWordTable(QString sno)
{
    if(connectDB()==false)
    {
        qDebug() << "connect db fail";
        return;
    }
    QSqlQuery query;
    QString tablename = "allWords" + sno;
    qDebug() << "The tablename is = " << tablename;
    bool sqlRes = query.exec(QString("create table %1 as select * from allWords").arg(tablename));
    if(!sqlRes)
        qDebug() << "error :" << query.lastError();
    return;
}

bool Dictionary::createImportTable(int sno)
{
    if(connectDB()==false)
    {
        qDebug() << "connect db fail";
        return false;
    }
    QSqlQuery query;
    QString tablename = "ImportTable" + QString::number(sno);
    qDebug() << "The tablename is = " << tablename;
    bool sqlRes = query.exec(QString("create table %1 (word text primary key,mean_cn text)").arg(tablename));
    if(!sqlRes)
    {
        qDebug() << "error :" << query.lastError();
        return false;
    }
    return true;
}

bool Dictionary::saveWordToBook(QString bookname,QString word,QString accent,QString mean_cn,
                                QString tenses,QString origin){
    QSqlQuery query;
    bool res = query.exec(QString("update '%1' set accent = '%2' where word = '%3'").arg(bookname).arg(accent).arg(word));
    res = query.exec(QString("update '%1' set mean_cn = '%2' where word = '%3'").arg(bookname).arg(mean_cn).arg(word));
    res = query.exec(QString("update '%1' set tenses = '%2' where word = '%3'").arg(bookname).arg(tenses).arg(word));
    res = query.exec(QString("update '%1' set origin = '%2' where word = '%3'").arg(bookname).arg(origin).arg(word));
    if(!res)
    {
        qDebug() << "insert error :" << query.lastError();
        return false;
    }
    return true;
}

bool Dictionary::collectWord(QString bookname,QString word)
{
    QSqlQuery query;
    bool res = query.exec(QString("update '%1' set collect = 1 where word = '%3'")
                          .arg(bookname).arg(word));
    if(!res)
    {
        qDebug() << "insert error :" << query.lastError();
        return false;
    }
    return true;
}

bool Dictionary::cancelCollect(QString bookname,QString word)
{
    QSqlQuery query;
    qDebug() << "Table name is = " << bookname << "  and = " << word;
    bool res = query.exec(QString("update %0 set collect = 0 where word = '%2'").arg(bookname).arg(word));
    if(!res)
    {
        qDebug() << "insert error :" << query.lastError();
        return false;
    }
    return true;
}

bool Dictionary::isCollect(QString bookname,QString word)
{
    QString tablename = "collectTable" + bookname;
    QSqlQuery query;
    bool res = query.exec(QString("select word from %1 where word = '%2'").arg(tablename).arg(word));
    query.next();
    return query.value(0).toString() != "";
}

QVariantList Dictionary::collectWords(QString bookname)
{
    if(connectDB()==false)
    {
        qDebug() << "connect db fail";
        return QVariantList{};
    }
    QString tableName = "allWords" + bookname;
    qDebug() << "collect table is = " << tableName;
    QSqlQuery query;
    bool res = query.exec(QString("select word,mean_cn from %1 where collect = 1").arg(tableName));
    if(!res)
    {
        qDebug() << "insert error :" << query.lastError();
        return QVariantList{};
    }
    QVariantList wordlist;
    while(query.next())
    {
        QString tmps = query.value(0).toString() + '&' + query.value(1).toString();
        wordlist.push_back(tmps);
    }
    return wordlist;
}

bool Dictionary::importWord(QString sno,QString word,QString mean){
    createImportTable(sno.toInt());
    QString tablename = "ImportTable" + sno;
    //插入
    QSqlQuery query;
    bool res = query.exec(QString("insert into %1 values('%2','%3')").arg(tablename).arg(word).arg(mean));
    if(!res)
    {
        qDebug() << "insert error :" << query.lastError();
        return false;
    }
    return true;
}

QVariantList Dictionary::rememberWord(QString tablename,int num)
{
    if(connectDB()==false)
    {
        qDebug() << "connect db fail";
        return QVariantList{};
    }
    QSqlQuery query;
    qDebug() << "the num = " << num << "   and name is = " << tablename;
    bool res = query.exec(QString("select * from '%1' limit 0,%2").arg(tablename).arg(num));
    if(!res)
    {
        qDebug() << "select error : = " << query.lastError();
        return QVariantList{};
    }
    qDebug() << "select success!";
    QVariantList wordlst;
    while(query.next())
    {
        QString tmp = "word:" + query.value(1).toString() + "-mean_cn:" + query.value(3).toString();
        wordlst.push_back(tmp);
    }
    return wordlst;
}

QVariantList Dictionary::getAllWords(QString tablename){
    if(connectDB() == false) return QVariantList{};
    QSqlQuery query;
    qDebug() << "The table name is = " << tablename;
    bool res = query.exec(QString("select word,mean_cn from '%1'").arg(tablename));
    if(!res)
    {
        qDebug() << "select error : = " << query.lastError();
        return QVariantList{};
    }
    QVariantList wordlist;
    while(query.next())
    {
        QString tmps = query.value(0).toString() + "\n" + query.value(1).toString();
        wordlist.push_back(tmps);
    }
    return wordlist;
}

QVariantList Dictionary::getAllSentence(){
    if(connectDB() == false) return QVariantList{};
    QSqlQuery query;
    QString tablename = "sentence";
    qDebug() << "The table name is = " << tablename;
    bool res = query.exec(QString("select * from '%1'").arg(tablename));
    if(!res)
    {
        qDebug() << "select error : = " << query.lastError();
        return QVariantList{};
    }
    QVariantList wordlist;
    while(query.next())
    {
        //单词+句子1英中+句子二英中
        QString tmps = query.value(0).toString() + "&" + query.value(3).toString() + "&" + query.value(4).toString()
                + "&" + query.value(5).toString() + "&" + query.value(6).toString();
        wordlist.push_back(tmps);
    }
    return wordlist;
}
