#include "dbmanager.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QFile>
#include <QRegularExpression>
#include <QString>
#include <QTextStream>
#include <QRegularExpression>
#include <QStringList>
#include <QFileInfo>
#include <QVector>



int current= 0 , png_curr = 0;
QStringList down_links;
QStringList png_down_links;
QStringList png_hash ;
QString imgpath;
int revision_number = 0;
QString data_path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);



dbmanager::dbmanager(QObject *parent) : QObject(parent)
{

}





bool del_from_db(QString id,int revid)
{
    bool done;
    QDir databasePath;
    QDir dir(data_path);
    dir.cd("WTL_appdata");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(dir.absoluteFilePath("WTL.db"));
    if(!db.open())
    {
        qDebug() <<"error in opening DB";
    }
    else
    {
        qDebug() <<"connected to DB" ;

    }
    QSqlQuery query;

    query.prepare("DELETE FROM Pages WHERE page_ID = '" + id + "'");



    if(query.exec())
    {
        qDebug() << "deleted from table Pages";
        done = true;

    }
    else
    {
        qDebug() << query.lastError();

    }
    query.prepare("DELETE FROM Dependencies WHERE revision_number = '" + QString::number(revid) + "'");


    if(query.exec())
    {
        qDebug() << "deleted from table Dependencies";
        done = true;

    }
    else
    {
        qDebug() << query.lastError();

    }
    if(done == true)
        return true;
    else
        return false;


}

QString clean_text(QString text)
{
    text = text.replace("\n","");
    text = text.replace("&#39;/index.php", "http://en.wikitolearn.org/index.php");
    text = text.replace("&amp;","&");
    text = text.replace("MathShowImage&amp;", "MathShowImage&");
    text = text.replace("mode=mathml&#39;", "mode=mathml""");
    text = text.replace("<meta class=\"mwe-math-fallback-image-inline\" aria-hidden=\"true\" style=\"background-image: url(" ,"<img style=\"background-repeat: no-repeat; background-size: 100% 100%; vertical-align: -0.838ex;height: 2.843ex;\""   "src=");
    text = text.replace("<meta class=\"mwe-math-fallback-image-display\" aria-hidden=\"true\" style=\"background-image: url(" ,"<img style=\"background-repeat: no-repeat; background-size: 100% 100%; vertical-align: -0.838ex;height: 2.843ex;\""  "src=");
    text = text.replace("&mode=mathml);" , "&mode=mathml\">");
    text = text.replace("src=\"//pool.wikitolearn.org" , "src=\"http://pool.wikitolearn.org");
    return(text);
}

bool check_links(QString text)
{

    QRegularExpression link_regex("src=(?<path>.*?)>");
    QRegularExpressionMatch contain = link_regex.match(text);
    qDebug() << contain;

    if(contain.capturedLength() > 0)
    {
        return  true;
    }
    else
    {
        return false;
    }

}



bool add_depend(QString filename , int revision_number)
{
    QDir databasePath;

    QDir dir(data_path);
    dir.cd("WTL_appdata");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(dir.absoluteFilePath("WTL.db"));
    if(!db.open())
    {
        qDebug() <<"error in opening DB";
    }
    else
    {
        qDebug() <<"connected to DB" ;
    }

    QSqlQuery query;

    query.prepare("INSERT INTO Dependencies (depe_fileName,revision_number) "
                  "VALUES (:depe_filename , :revision_number )");



    query.bindValue(":depe_filename",filename);
    query.bindValue(":revision_number", revision_number);



    if(query.exec())
    {
        qDebug() << "done";
        db.close();
        return(true);
    }
    else
    {
        qDebug() << query.lastError();
        db.close();

    }
    return (false);

}




bool add_in_db(int pageid , int revid)
{
    revision_number = revid ;
    QDir databasePath;

    QDir dir(data_path);
    dir.cd("WTL_appdata");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(dir.absoluteFilePath("WTL.db"));
    if(!db.open())
    {
        qDebug() <<"error in opening DB";
    }
    else
    {
        qDebug() <<"connected to DB" ;
    }

    QSqlQuery query;

    query.prepare("INSERT INTO pages (page_ID,page_revision) "
                  "VALUES (? , ?)");
    query.bindValue(0,pageid);
    query.bindValue(1, revid);

    if(query.exec())
    {
        qDebug() << "done";
        return(true);
        db.close();
    }
    else
    {
        qDebug() << query.lastError();
        db.close();

    }
    return (false);
}

bool save_images(QString filename , int pageid)
{
    QString content , newpath ;

    qDebug() << filename +" <- html filename ";
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() <<"unable to open file";
        return false;
    }
    else{
        content = file.readAll();
        //  download images here

        QRegularExpression link_regex("src=(?<path>.*?)>");
        QRegularExpressionMatchIterator links = link_regex.globalMatch(content);

        QRegularExpression png_regex("src=\"http://pool.wikitolearn.org(.*?).png");
        QRegularExpressionMatchIterator png = png_regex.globalMatch(content);

        while (links.hasNext()) {
            QRegularExpressionMatch match = links.next();
            QString down_link = match.captured(1).remove(QString("&mode=mathml\"")); ;
            //            qDebug()<<down_link.remove(QString("&mode=mathml\""));
            down_links << down_link;  //prepare list of downloads
            //start downloading images
            QString d = content.replace("&mode=mathml\"",".svg"); //clean img src in local html file
            qDebug() << imgpath ;
            newpath = d.replace("http://en.wikitolearn.org/index.php?title=Special:MathShowImage&hash=",""); // clean img src in local html file and prepare the local path those are to be saved in html file

        }
        while (png.hasNext()){
                    QRegularExpressionMatch png_match = png.next();
                    QString png_links = png_match.captured(1);
                    qDebug() << "11" << png_links;

                    png_links = "http://pool.wikitolearn.org"+png_links+".png";
                    qDebug() << "11" << png_links;
                    png_down_links << png_links;
                    png_links = png_links.replace("http://pool.wikitolearn.org","");
                    qDebug() << "11" << png_links;
                    png_links = png_links.replace(".png","");
                    qDebug() << "11" << png_links;
                    newpath   = newpath.replace(png_links,"");



                    QByteArray hash = png_links.toUtf8();
                    QString hash_me = QString(QCryptographicHash::hash((hash),QCryptographicHash::Md5).toHex());
                    png_links = hash_me ;
                    qDebug() << "hash_me" << hash_me;
                    png_hash << hash_me;


                    png_links = png_links+".png";
                    qDebug() << png_links;
                    newpath   = newpath.replace("http://pool.wikitolearn.org.png",png_links);


                }



        qDebug() << down_links; //got the list of downloads
        qDebug() << png_down_links;
        qDebug() << png_hash;
        file.close();

    }

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() <<"unable to write to file";
        return false;
    }

    else
    {
        qDebug() <<"write to file here";
        QTextStream out(&file);

        newpath = "<link rel=\"stylesheet\" type=\"text/css\" href=\"main.css\">" + newpath;
        newpath = newpath.replace("svg> background-repeat:", "svg> <!--background-repeat:");
        newpath = newpath.replace("ex;\" />", "-->");


        out << newpath;
        // qDebug()<<newpath;

        file.close();

        // move html file to their respective folder
        QString fname = QString::number(pageid);
        QDir dir(data_path);
        dir.cd("WTL_appdata");
        QString temp_name = dir.absoluteFilePath(filename);
       QString new_name = temp_name.replace(".html","");
       QString css_path = new_name;
        new_name = new_name + "/" + fname +".html";
        file.rename(filename,new_name);
        css_path = css_path + "/main.css";
        file.copy(dir.absoluteFilePath("main.css"),css_path);

    }

    dbmanager *d = new dbmanager(0) ;
    d->doDownload(down_links);
    dbmanager *p = new dbmanager(0) ;
    p->png_download(png_down_links, png_hash);


    return true ;

}


//start downloading images




void dbmanager::doDownload(const QVariant& v)
{
    if (v.type() == QVariant::StringList) {


        QNetworkAccessManager *manager= new QNetworkAccessManager(this);

        QUrl url = v.toStringList().at(current);

        filename = url.toString().remove("http://en.wikitolearn.org/index.php?title=Special:MathShowImage&hash=");
        m_network_reply = manager->get(QNetworkRequest(QUrl(url)));

        connect(m_network_reply, SIGNAL(downloadProgress (qint64, qint64)),this, SLOT(updateDownloadProgress(qint64, qint64)));
        connect(m_network_reply,SIGNAL(finished()),this,SLOT(downloadFinished()));


    }
}



void dbmanager::downloadFinished(){
    qDebug()<<filename;
    if(m_network_reply->error() == QNetworkReply::NoError){


        m_file =  new QFile(imgpath+"/"+filename+".svg");
        qDebug()<<imgpath+"/"+filename;
        if(!m_file->open(QIODevice::ReadWrite | QIODevice::Truncate)){
            qDebug() << m_file->errorString();
        }
        m_file->write(m_network_reply->readAll());

        QByteArray bytes = m_network_reply->readAll();
        QString str = QString::fromUtf8(bytes.data(), bytes.size());
        int statusCode = m_network_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << QVariant(statusCode).toString();
    }
    m_file->flush();
    m_file->close();
    int total = down_links.count();
    if(current<total-1){
        current++;
        qDebug()<<"current = "<<current<<"total = "<<total;
        doDownload(down_links);}
    else if(current==total-1)
    {
        qDebug()<<"download complete";
    }

    bool success = false ;
    QString fname = filename;
    success = add_depend(fname,revision_number);
    if(success == true)
    {
        qDebug() <<"added in dependency table";
    }
    else
    {
        qDebug() << " error in adding to dependency table";
    }


}





void dbmanager::updateDownloadProgress(qint64 bytesRead, qint64 totalBytes)
{
    //    ui->progressBar->setMaximum(totalBytes);
    //    ui->progressBar->setValue(bytesRead);
    qDebug()<<bytesRead<<totalBytes;
}


// start downloading png

void dbmanager::png_download(const QVariant& v , const QVariant& n )
{
    if (v.type() == QVariant::StringList) {


        QNetworkAccessManager *manager= new QNetworkAccessManager(this);

        QUrl url = v.toStringList().at(png_curr);
        qDebug() << "*******  " << url;

        png_filename = n.toStringList().at(png_curr);
        png_network_reply = manager->get(QNetworkRequest(QUrl(url)));

        connect(png_network_reply, SIGNAL(downloadProgress (qint64, qint64)),this, SLOT(update_png_download(qint64, qint64)));
        connect(png_network_reply,SIGNAL(finished()),this,SLOT(png_finished()));


    }
}



void dbmanager::png_finished(){
    qDebug()<<png_filename;
    if(png_network_reply->error() == QNetworkReply::NoError){


        png_file =  new QFile(imgpath+"/"+png_filename+".png");
        qDebug()<<imgpath+"/"+png_filename;
        if(!png_file->open(QIODevice::ReadWrite | QIODevice::Truncate)){
            qDebug() << png_file->errorString();
        }
        png_file->write(png_network_reply->readAll());

        QByteArray bytes = png_network_reply->readAll();
        QString str = QString::fromUtf8(bytes.data(), bytes.size());
        int statusCode = png_network_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << QVariant(statusCode).toString();
    }
    png_file->flush();
    png_file->close();
    int total = png_down_links.count();
    if(png_curr<total-1){
        png_curr++;
        qDebug()<<"current = "<<png_curr<<"total = "<<total;
        png_download(png_down_links,png_hash);}
    else if(png_curr==total-1)
    {
        qDebug()<<"download complete";
    }

    bool success = false ;
    QString fname = png_filename;
    success = add_depend(fname,revision_number);
    if(success == true)
    {
        qDebug() <<"added in dependency table";
    }
    else
    {
        qDebug() << " error in adding to dependency table";
    }


}

void dbmanager::update_png_download(qint64 bytesRead, qint64 totalBytes)
{

    qDebug()<<bytesRead<<totalBytes;
}

void save_file(QString text , int pageid , int revid)
{
    if(!check_links(text))
    {
        QDir dir(data_path);
        dir.cd("WTL_appdata");
        QString Folder_name = dir.absoluteFilePath(QString::number(pageid));
        QString fname = QString::number(pageid);
        if(QDir(Folder_name).exists())
        {
            qDebug() << " already exist ";

        }
        else{           

             QDir dir(data_path);
             dir.cd("WTL_appdata");
            dir.mkdir(Folder_name);
            QString filename = dir.absoluteFilePath(Folder_name+".html");
            QFile file(filename);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream out(&file);

            text = "<link rel=\"stylesheet\" type=\"text/css\" href=\"main.css\">" +text;
            out << text;

            // optional, as QFile destructor will already do it:
            file.close();
            // move html file to their respective folder
            QString temp_name = dir.absoluteFilePath(filename);
           QString new_name = temp_name.replace(".html","");           
           QString css_path = new_name;
            new_name = new_name + "/" + fname +".html";
            file.rename(filename,new_name);
            css_path = css_path + "/main.css";            
            file.copy(dir.absoluteFilePath("main.css"),css_path);

            bool success = add_in_db(pageid,revid);
            if(success == true)
            {
                qDebug() <<"entry added to DB successfully ";
            }
            else
            {
                qDebug() <<" failed to add in DB ";
            }

        }
    }

        else {


            dbmanager d;
            QDir dir(data_path);
            dir.cd("WTL_appdata");
           d.imageDownloadPath = dir.absoluteFilePath(QString::number(pageid));
            imgpath =d.imageDownloadPath;

            if(QDir(d.imageDownloadPath).exists())
            {
                qDebug() << " already exist ";
            }
            else{
                dir.mkdir(d.imageDownloadPath);

                QString filename = d.imageDownloadPath+".html";

                QFile file(filename);
                file.open(QIODevice::WriteOnly | QIODevice::Text);
                QTextStream out(&file);
                out << text;

                // optional, as QFile destructor will already do it:
                file.close();
                bool success = add_in_db(pageid,revid);
                if(success == true)
                {
                    qDebug() <<"entry added to DB successfully ";
                }
                else
                {
                    qDebug() <<" failed to add in DB ";
                }

                success = save_images(filename,pageid);

                if(success == true)
                {
                    qDebug() << "images downloaded successfully ";
                }
                else
                {
                    qDebug() << "error in downloading images";
                }


            }

        }
}

void del_file(QString pageid)
{
    QDir dir(data_path);
    dir.cd("WTL_appdata");
    QString Folder_name = dir.absoluteFilePath(pageid);

    if(QDir(Folder_name).exists())
    {
        dir = Folder_name;
        qDebug() << dir ;
        dir.removeRecursively();

    }
    else{
        qDebug() << "cannot delete or folder does not exist";

}
}


QString dbmanager::add(QString p_url)
{

    QString text ;
    int pageid , revid;

    QString requested_url = "http://en.wikitolearn.org/api.php?action=parse&page="+p_url+"&format=json";

    // create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req( requested_url );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        //success
        //qDebug() << "Success" <<reply->readAll();
        QString   html = (QString)reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(html.toUtf8());

        QJsonObject jsonObj = jsonResponse.object();


        text = jsonObj["parse"].toObject()["text"].toObject()["*"].toString();
        pageid = jsonObj["parse"].toObject()["pageid"].toInt();
        revid = jsonObj["parse"].toObject()["revid"].toInt();

        //clean the result from the API
        text = clean_text(text);
        //  qDebug() << text;
        qDebug() <<pageid;

        delete reply;
    }

    else {
        //failure
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
    }

    // ******************* here ****************

    save_file(text , pageid , revid);

    // ***************************

    static auto i = 0;
       return QString("%1: %2").arg(++i).arg(p_url);


    }

    QString dbmanager::del(QString pageid)
    {
        qDebug() <<"DELETION CODE GOES HERE";
        del_file(pageid);

        QDir databasePath;

        QDir dir(data_path);
        dir.cd("WTL_appdata");

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
        db.setDatabaseName(dir.absoluteFilePath("WTL.db"));
        if(!db.open())
        {
            qDebug() <<"error in opening DB";
        }
        else
        {
            qDebug() <<"connected to DB" ;

        }
        int revid ;
        QSqlQuery query;

      query.prepare("Select page_revision from Pages where page_ID = :id");
      query.bindValue(":id", pageid);
      query.exec();

      if (query.next()) {
           revid = query.value(0).toInt();

      }
      db.close();

       del_from_db(pageid,revid);

        static auto i = 0;
           return QString("%1: %2").arg(++i).arg(pageid);

    }

    bool check_revision(QString id , int revision_number)
    {
        int pageid;

        QEventLoop eventLoop;

        // "quit()" the event-loop, when the network request "finished()"
        QNetworkAccessManager mg;
        QObject::connect(&mg, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

        // the HTTP request
        QString url = "http://en.wikitolearn.org/api.php?action=parse&pageid="+id+"&format=json";
        QNetworkRequest re( ( url ) );
        QNetworkReply *reply = mg.get(re);
        eventLoop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            //success
            //qDebug() << "Success" <<reply->readAll();
            QString   html = (QString)reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(html.toUtf8());

            QJsonObject jsonObj = jsonResponse.object();

            int  revid = jsonObj["parse"].toObject()["revid"].toInt();
            pageid = jsonObj["parse"].toObject()["pageid"].toInt();
            qDebug() << jsonObj["parse"].toObject()["title"].toString();


            if(revision_number == revid)
            {
                delete reply;
                return true ;
            }
            else
            {
                qDebug() << "update page";
                QString text = jsonObj["parse"].toObject()["text"].toObject()["*"].toString();
                text = clean_text(text);
                bool del = del_from_db(id,revid);
                //check if deletion was successfull
                if(del == true)
                {
                    qDebug() << "deletion from DB done";
                }
                else
                {
                    qDebug() << "error in deletion from DB";
                }
            QString pid = QString::number(pageid);
            del_file(pid);
            save_file( text ,  pageid ,  revision_number);

                
            }

    }
        return true ;

    }



    void dbmanager::update()
    {

        QDir databasePath;

        QDir dir(data_path);
        dir.cd("WTL_appdata");

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
        db.setDatabaseName(dir.absoluteFilePath("WTL.db"));
        if(!db.open())
        {
            qDebug() <<"error in opening DB";
        }
        else
        {
            qDebug() <<"connected to DB" ;
        }


        bool change = false ;
        QSqlQuery count;

        QVector<QString> id ;
        QVector<int> revid;


         QSqlQuery query("SELECT page_ID , page_revision FROM Pages");
           while (query.next()) {
                QString i = query.value(0).toString();
                id.push_back(i);
               int r = query.value(1).toInt();
               revid.push_back(r);

           }
           for(int i = 0 ; i < id.size() ; i++){
           change  = check_revision(id[i] , revid[i]);
               qDebug() << id[i];
               qDebug() << revid[i];
           if(change == true)
           {
               qDebug() << " same";
           }
           else
           {
               qDebug() << "need update";

           }
        }

    }

