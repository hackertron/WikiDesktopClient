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
#include <QCryptographicHash>
#include <QVector>
#include "model.h"



int current= 0 , png_curr = 0;
QStringList down_links;
QStringList png_down_links;
QStringList png_hash;
QString imgpath;
bool math_svg , image_png = false;
int revision_number = 0;
QString data_path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);



dbmanager::dbmanager(QObject *parent) : QObject(parent)
{

}





bool del_from_db(QString id,int revid)
{
    bool done;

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
    //text = text.replace("\n","");
    text = text.replace("\"//restbase.wikitolearn.org", "\"http://restbase.wikitolearn.org");
    text = text.replace("src=\"//pool.wikitolearn.org" , "src=\"http://pool.wikitolearn.org");
    return(text);
}

bool check_links(QString text)
{

    QRegularExpression link_regex("http://restbase.wikitolearn.org/en.wikitolearn.org/v1/media/math/render/svg");
    QRegularExpressionMatch contain = link_regex.match(text);
    qDebug() << contain;

    QRegularExpression png_regex("http://pool.wikitolearn.org");
    QRegularExpressionMatch png = png_regex.match(text);
    qDebug() << png;

    if(contain.capturedLength() > 0 && png.capturedLength() > 0)
    {
        // change the status to true , so that i can download dependencies
        math_svg = true;
        image_png = true;
        return true;
    }

    else if(contain.capturedLength() > 0)
    {
        // change the status to true , to download the dependencies
        math_svg = true;
        return  true;
    }
    else if(png.capturedLength() > 0)
    {
        // change the status to true , to download the images
        image_png = true;
        return true;
    }
    else
    {
        return false;
    }

}



bool add_depend(QString filename , int revision_number)
{

    QDir dir(data_path); // set the path to application data dir
    dir.cd("WTL_appdata"); //  change the directory to application data dir

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
        db.close(); //  dependencies are added now
        return(true);
    }
    else
    {
        qDebug() << query.lastError();
        db.close();

    }
    return (false);

}




bool add_in_db(int pageid , int revid , QString page_title)
{
    revision_number = revid ;


    QDir dir(data_path);// set the path to application data dir
    dir.cd("WTL_appdata"); //  change the directory to application data dir

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

    query.prepare("INSERT INTO pages (page_ID,page_revision,page_title) "
                  "VALUES (? , ? , ?)");
    query.bindValue(0,pageid);
    query.bindValue(1, revid);
    query.bindValue(2, page_title);

    if(query.exec()) // entry added in database
    {
        qDebug() << "done";
        return(true);
        db.close();
    }
    else
    {
        qDebug() << query.lastError(); // check the error
        db.close();

    }
    return (false);
}

bool save_images(QString filename , int pageid) // image saving function
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
        content = file.readAll(); // read contents of html file
        qDebug() <<"testing : " << content;
        //  download images here

        QRegularExpression link_regex("\"http://restbase.wikitolearn.org/en.wikitolearn.org/v1/media/math/render/svg/(.*?)\"");
        QRegularExpressionMatchIterator links = link_regex.globalMatch(content);


        QRegularExpression png_regex("src=\"http://pool.wikitolearn.org(.*?).png");
        QRegularExpressionMatchIterator png = png_regex.globalMatch(content);


        while (links.hasNext()) {

            // got the links and constructing the url
            QRegularExpressionMatch match = links.next();
            qDebug() << match;

            QString down_link = match.captured(1);
            //content = content.replace(down_link,down_link+".svg");
            down_link = "http://restbase.wikitolearn.org/en.wikitolearn.org/v1/media/math/render/svg/" + down_link;
            down_links << down_link;  //prepare list of downloads
            //start downloading images

            // clearn img src in local html file
            QString d = content.replace("http://restbase.wikitolearn.org/en.wikitolearn.org/v1/media/math/render/svg/",""); //clean img src in local html file
            qDebug() << imgpath ;
            newpath = d.replace("\" class=\"mwe-math",".svg\" class=\"mwe-math");
        }

        while (png.hasNext()){
            QRegularExpressionMatch png_match = png.next();
            QString png_links = png_match.captured(1);
            qDebug() << "png_links" << png_links;

            png_links = "http://pool.wikitolearn.org"+png_links+".png";
            qDebug() << "png_links" << png_links;
            png_down_links << png_links;
            png_links = png_links.replace("http://pool.wikitolearn.org","");
            qDebug() << "png_links" << png_links;
            png_links = png_links.replace(".png","");
            qDebug() << "png_links" << png_links;
            newpath   = newpath.replace(png_links,"");

            // converting the png urls to hashes
            // file name == hash of the url

            QByteArray hash = png_links.toUtf8();
            QString hash_me = QString(QCryptographicHash::hash((hash),QCryptographicHash::Md5).toHex());
            png_links = hash_me ;
            qDebug() << "hash_me" << hash_me;
            png_hash << hash_me;


            png_links = png_links+".png";
            qDebug() << png_links;
            newpath   = newpath.replace("http://pool.wikitolearn.org.png",png_links);


        }



        qDebug() << "size" << down_links.size(); // got the number of files to download

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

        QString styling = "<head>"
                          "<link rel=\"stylesheet\" type=\"text/css\" href=\"main.css\">"
                          "<link rel=\"stylesheet\" type=\"text/css\" href=\"wikitolearnskin.css\">"
                          "<link rel=\"stylesheet\" type=\"text/css\" href=\"bootstrap.css\">"
                          "</head>";

        newpath = styling +newpath;

        // added the css links to the page

        out << newpath;
        // qDebug()<<newpath;

        file.close();

        // move html file and css to their respective folder
        QString fname = QString::number(pageid);
        QDir dir(data_path);
        dir.cd("WTL_appdata");
        QString temp_name = dir.absoluteFilePath(filename);
        QString new_name = temp_name.replace(".html","");
        QString css_path = new_name;
        new_name = new_name + "/" + fname +".html";
        file.rename(filename,new_name);

        // copy required css files.
        css_path = css_path + "/main.css";
        file.copy(dir.absoluteFilePath("main.css"),css_path);

        css_path = css_path.replace("/main.css","/bootstrap.css");
        file.copy(dir.absoluteFilePath("bootstrap.css"),css_path);

        css_path = css_path.replace("/bootstrap.css","/wikitolearnskin.css");
        file.copy(dir.absoluteFilePath("wikitolearnskin.css"),css_path);


    }

    if(math_svg!=false){


        // if there are  maths ml content (SVG) download them
        dbmanager *d = new dbmanager(0) ;
        d->doDownload(down_links);
    }

    if(image_png!=false){


        //if there are images (png) download them
        dbmanager *p = new dbmanager(0) ;
        p->png_download(png_down_links, png_hash);
    }
    return true ;

}


//start downloading images




void dbmanager::doDownload(const QVariant& v)
{
    if (v.type() == QVariant::StringList) {


        QNetworkAccessManager *manager= new QNetworkAccessManager(this);

        QUrl url = v.toStringList().at(current);

        filename = url.toString().remove("http://restbase.wikitolearn.org/en.wikitolearn.org/v1/media/math/render/svg/");
        m_network_reply = manager->get(QNetworkRequest(QUrl(url)));

        connect(m_network_reply, SIGNAL(downloadProgress (qint64, qint64)),this, SLOT(updateDownloadProgress(qint64, qint64))); // checks download progress
        connect(m_network_reply,SIGNAL(finished()),this,SLOT(downloadFinished())); // signal that download is finished


    }
}



void dbmanager::downloadFinished(){
    qDebug()<<filename;
    if(m_network_reply->error() == QNetworkReply::NoError){

        // set filename and location to store the file  ( Generic location of application data )
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
    // increment the counter on the basis of total number of files to download
    int total = down_links.count();
    if(current<total-1){
        current++;
        qDebug()<<"current = "<<current<<"total = "<<total;
        doDownload(down_links);}
    else if(current==total-1)
    {
        qDebug()<<"download complete";
        down_links.clear();
        math_svg = false;
    }

    bool success = false ;
    QString fname = filename;
    success = add_depend(fname,revision_number); //  add the dependencies entry in database i.e. images
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


void dbmanager::png_download(const QStringList &v, const QStringList &n)
{

    QNetworkAccessManager *mgr= new QNetworkAccessManager(this);

    QUrl url = v.at(current);

    png_filename = n.at(png_curr);
    png_network_reply = mgr->get(QNetworkRequest(QUrl(url)));

    connect(png_network_reply, SIGNAL(downloadProgress (qint64, qint64)),this, SLOT(update_png_download(qint64, qint64)));
    connect(png_network_reply,SIGNAL(finished()),this,SLOT(png_finished()));

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
        png_down_links.clear();
        png_hash.clear();
        image_png = false;
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


void save_file(QString text , int pageid , int revid , QString page_title)
{
    if(!check_links(text)) // check the html to see if there are any dependencies that needs to be downloaded
    {
        QDir dir(data_path); // set path to application data location
        dir.cd("WTL_appdata"); // change to the application data location

        QString Folder_name = dir.absoluteFilePath(QString::number(pageid));
        // Folder_name = Generic location of application data ( varies from OS to OS ) + filename(pageid)
        QString fname = QString::number(pageid);

        if(QDir(Folder_name).exists()) // checks if the file already exists or not
        {
            qDebug() << " already exist ";

        }
        else{ // save the file

            QDir dir(data_path);
            dir.cd("WTL_appdata");
            dir.mkdir(Folder_name);
            QString filename = dir.absoluteFilePath(Folder_name+".html");
            QFile file(filename);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream out(&file);

            text = "<link rel=\"stylesheet\" type=\"text/css\" href=\"main.css\">" +text;
            text = "<link rel=\"stylesheet\" type=\"text/css\" href=\"wikitolearnskin.css\">" + text;
            text = "<link rel=\"stylesheet\" type=\"text/css\" href=\"bootstrap.css\">" + text;
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

            bool success = add_in_db(pageid,revid,page_title);
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


        dbmanager d; // created object of dbmanager to access private variables and functions of dbmanager.h
        QDir dir(data_path);
        dir.cd("WTL_appdata");
        // imageDownloadPath is going to be the same path as of it's html file
        d.imageDownloadPath = dir.absoluteFilePath(QString::number(pageid));
        imgpath = d.imageDownloadPath;

        if(QDir(d.imageDownloadPath).exists()) // checks if the files are already saved
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
            // add entries to the database ( i.e. in Pages table )
            bool success = add_in_db(pageid,revid,page_title);
            if(success == true)
            {
                qDebug() <<"entry added to DB successfully ";
            }
            else
            {
                qDebug() <<" failed to add in DB ";
            }

            // call save_images function  save the images
            // filename is html filename which will be read to extract URLs from it
            // filename has the same name as of pageid
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

// function to recursively delete the specific saved page ( i.e. entire folder )
void del_file(QString pageid)
{
    QDir dir(data_path);
    dir.cd("WTL_appdata");
    QString Folder_name = dir.absoluteFilePath(pageid);

    if(QDir(Folder_name).exists()) // checks if the folder exist , if exist delete it
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

    QString text , page_title ;
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
        /********************************************************************************
         *  API returns JSON . JSON needs to be parsed to get the contents from it      *
         *  contents that we need are  html contents , pageid , revision number of page *
         * text  == contains the html content of the specified page                     *
         * pageid == contains the page id of the page .                                 *
         * revid == contains revision number of the page                                *
         * page_title == contains the title of the page                                 *
         * ******************************************************************************
         */

        text = jsonObj["parse"].toObject()["text"].toObject()["*"].toString();
        pageid = jsonObj["parse"].toObject()["pageid"].toInt();
        revid = jsonObj["parse"].toObject()["revid"].toInt();
        page_title = jsonObj["parse"].toObject()["title"].toString();

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

    // ******************* save file here  ****************

    save_file(text , pageid , revid , page_title);




    static auto i = 0;
    return QString("%1: %2").arg(++i).arg(p_url);


}

QString dbmanager::del(QString pageid)
{
    qDebug() <<"DELETION CODE GOES HERE";
    del_file(pageid);


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
    // every page and it's dependencies  has a revid associated with it .
    // revid is unique , used it to delete the pages

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

// checks if the page is updated . IF yes then delete old page and download the updated page

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
        QString page_title = jsonObj["parse"].toObject()["title"].toString();
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
            save_file( text ,  pageid ,  revision_number , page_title);


        }

    }

    return true;

}



void dbmanager::update()
{

    // universal update function . called when users wants to  update all pages

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

void dbmanager::deleteAll() // when users wants to delete all pages
{
    QVector<QString> id ;



    QSqlQuery query("SELECT page_ID  FROM Pages");
    while (query.next()) {
        QString i = query.value(0).toString();
        id.push_back(i);
    }
    for(int i = 0; i < id.size(); ++i)
    {
        qDebug() << id[i];
        del(id[i]);
    }



}

void dbmanager::update_page(QString pageid)
{
    // update a single page as requested by user .
    int revid ;
    QSqlQuery query("SELECT page_revision FROM Pages WHERE page_ID = '" + pageid + "'");
    while(query.next()){
        revid = query.value(0).toInt();
    }
    check_revision(pageid,revid);


}

void dbmanager::quit()
{
    qApp->quit();
}

void dbmanager::clear_list()
{
    down_links.clear();
    png_down_links.clear();
    png_hash.clear();
}
