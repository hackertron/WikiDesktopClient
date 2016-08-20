# MODEL class

Model class is a class that  helps in populating the views ( manage pages , offline pages) of the app .

Model class is inspired from *QAbstractitem Model subclass*  

Refer : [http://doc.qt.io/qt-5/qtquick-modelviewsdata-cppmodels.html](refer : http://doc.qt.io/qt-5/qtquick-modelviewsdata-cppmodels.html "refer : http://doc.qt.io/qt-5/qtquick-modelviewsdata-cppmodels.html")

It's functions are as follows :

- Adding saved page lists to manage page / offline pages view 

- Removing the page lists from manage page / offline page view 

- Updating both views whenever there is change in the views 



Functions available in model class

```C++

 	void addpages(const list &list)

 	Q_INVOKABLE void deletepages(int row);

 	Q_INVOKABLE void deletelist();

 	Q_INVOKABLE void new_page(QString current_title);

 	Q_INVOKABLE void update(QString pageid, int row);


```

	
##void listmodel::addpages(const list &list)

The addpages function does what it sounds like . It add the pages to the app ( populate the view) at runtime . It is called by **main.cpp** .  

**main.cpp** sends the page_title and page_id as a *list* type .


##void listmodel::deletepages(int row)

The deletepages function deletes the page from the view when a user clicks on delete button . It works by taking the  **row** of the page from list . 

for example :  i have 3 pages namely page1 , page2 and page3 and  have index as 0 , 1 , 2 respectively . When user  clicks on delete button the row(index) of page is sent to the function and remove it from the list .


##void listmodel::deletelist()

This function is called when user wants to delete all the available pages in the list . This function is pretty straight-forward . It works as follows 

* count the size of list 

* sends the index of pages to **deletepages()** function

* **deletepages()** removes the pages as explained above.


##void listmodel::new_page(QString current_title)

new_page functions adds the new page to the list . It gets the title of the page from **main.qml** and make a database query . It looks for page_id of the page and sends both pate_title and page_id to the addpages function .


##void listmodel::update(QString pageid , int row)

Update function does what it sounds like , whenever a user clicks on update button it will check for update online and if there is update it will update the list .

This function is never called alone . It needs ** dbmanager's update()** to work properly . 
