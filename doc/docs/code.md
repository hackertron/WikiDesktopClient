# Getting the source 

1. git clone [https://github.com/hackertron/W2L.git](https://github.com/hackertron/W2L.git)


## Project Structure

your directory should look like this 
Structure: ![project-structure](https://raw.githubusercontent.com/hackertron/W2L/master/doc/docs/structure.png "Title")


## Let's take a deeper look 
	
#### wtl.pro

This is your project file . it holds the settings for your project .Project files contain all the information required by qmake to build your application, library, or plugin.

the lines below tells the Qmake to include these modules . 

`QT += quick quickcontrols2`

`QT += qml  webview`

`QT += sql`

`QT +=  network`

The **CONFIG** variable is another special variable that qmake uses when generating a Makefile. 

`CONFIG += c++11`

**SOURCES** variables are used to tell qmake about source files in the same directory as the project file. In our case we have main.ccp  , dbmanager.cpp , model.cpp

`SOURCES += main.cpp \`
`    dbmanager.cpp \`
 `   model.cpp`

**HEADERS** vairable are used to tell qmake about header files in the same directory as the projet file . In our case we have  dbmanager.h , model.h

`HEADERS += \`
`    dbmanager.h \`
`    model.h`

**RESOURCES** variable tells A list of resource (.rc) files to be included in the final project.

`RESOURCES += qml.qrc`


To learn more about the project files (.pro) visit : [http://doc.qt.io/qt-5.7/qmake-project-files.html](http://doc.qt.io/qt-5.7/qmake-project-files.html)

### Header files

##### dbmanager.h

