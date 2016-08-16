# DBMANAGER Class

dbmanager class is a custom class that provides functions to 

- add a page : add page to database and save it locally 

- delete a page : delete a page from database and from filesystem

- Update page : provide  update page feature to user 


**dbmanager** class inherits **QObject** .

Let's look at the available functions provided in **dbmanager.h**


```C++
 // to download the page locally and add it's entry to database
    Q_INVOKABLE QString add(QString p_url);

    // to delete the page locally and remove it's entries from database
    Q_INVOKABLE QString del(QString pageid);

    // to delete all the pages and remove the entries from database
    Q_INVOKABLE void deleteAll();

    // to update the page specified by user
    Q_INVOKABLE void update();

    // forcefully update all pages
    Q_INVOKABLE void update_page(QString pageid);

    // downloads svg/mathml content 
    void doDownload(const QVariant &v);

    // downloads the png images 
   void png_download(const QStringList &v , const QStringList &n);

```
Let's explore each of the functions  provided in **dbmanager.h** one by one 

##QString dbmanager::add(QString p_url)

the add function does what it looks like . It will save a page locally and make an entry in database .

This function is called from Main.qml file when a user clicks on  *save offline* button .

Once this function gets it's input  , it will construct a url and call WTL API for the page .  The API will return JSON and it will be parsed by **QJsonObject** 

```C++
QJsonObject jsonObj = jsonResponse.object();

  //text  == contains the html content of the specified page 
 text = jsonObj["parse"].toObject()["text"].toObject()["*"].toString();

 //pageid == contains the page id of the page .
 pageid = jsonObj["parse"].toObject()["pageid"].toInt();

 //revid == contains revision number of the page
 revid = jsonObj["parse"].toObject()["revid"].toInt();

 //page_title == contains the title of the page 
 page_title = jsonObj["parse"].toObject()["title"].toString();
```

Once the desired content is extracted from the JSON 

- Page is saved locally in **WTL_appdata** folder ( Generic location ) as html (save_file function is used to save the file locally)

- An entry of the saved page is added in *WTL.DB*. ( add_in_db function is called to make an entry in database)


##QString dbmanager::del(QString pageid)

the *del* function is an **Q_INVOKABLE** function which is called when a user wants to delete a page that he has saved locally . This function is pretty straightforward it takes the pageid of page and deletes it from both database and file system .


##void dbmanager::deleteAll()

deleteAll() function deletes all the pages saved locally and remove the entries from database too.


##void dbmanager::update()

update function is an universal update function . It will update every page recursively . 

###Working 
	Every page  has a revid ( revision number ).  When update function is called 

	- it reads the database and take revid of a page one at a time 
	- it makes a  call to WTL API and matches the revid with it 
	- IF there is a change in revid then it removes the page and downloads the updated page .
	- ELSE IF revid is same it does nothing ( that means page is up-to-date)


##void dbmanager::update_page(QString pageid)

the *update_page* function updates a single page specified by user . It basically works exactly same as *update()* function but just for a single page .


	



