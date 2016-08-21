# Getting the source 

1. git clone [https://github.com/hackertron/W2L.git](https://github.com/hackertron/W2L.git)


## Project Structure

your directory should look like this 
Structure: ![project-structure](https://camo.githubusercontent.com/3023d1503bef4c19b33f4a25d81cff1c28b8f8aa/68747470733a2f2f73332e706f7374696d672e6f72672f6b386477693532777a2f7374727563747572652e706e672e "Title")


## Setting up development environment 

### Linux 

#### Download 

Qt open-source ( linux 32-bit or 64-bit) from [https://www.qt.io/download-open-source/#section-2](https://www.qt.io/download-open-source/#section-2)


#### Install

Adjust permission, run the installer and follow the instruction to complete the installation.

	chmod +x qt-opensource-linux-x64-5.7.0.run
	./qt-opensource-linux-x64-5.7.0.run
 

#### Getting requirements for development 

- Use the following commands to install the basic requirements for building Qt applications:

	(DEBIAN / UBUNTU)

		sudo apt-get install build-essential libgl1-mesa-dev

	Fedora/RHEL/CentOS (yum)

		sudo yum groupinstall "C Development Tools and Libraries"
		sudo yum install mesa-libGL-devel

	openSUSE (zypper)

		sudo zypper install -t pattern devel_basis




### Windows

Setting up Qt on windows is very straightforward .  Just download the qt from their official site and install it .

#### Download 

Qt open-source (  32-bit or 64-bit) from [https://www.qt.io/download-open-source/#section-2](https://www.qt.io/download-open-source/#section-2)


#### Install

Double-click on the downloaded exe file and follow the instructions . 

For in-depth details go to : [http://doc.qt.io/qt-5/windows-support.html](http://doc.qt.io/qt-5/windows-support.html)



### OS X 

#### Download 

Qt open-source  from [https://www.qt.io/download-open-source/#section-2](https://www.qt.io/download-open-source/#section-2)

Refer the official documentation of [Qt for OS X](http://doc.qt.io/qt-5/osx.html) for in-detail instructions.


### Android 

Please refer the official qt for android documentation : [http://doc.qt.io/qt-5/androidgs.html](http://doc.qt.io/qt-5/androidgs.html)


### Iphone / Ipad

Please refer the official qt for Iphone / ipod doucmentation : [http://doc.qt.io/qt-5/ios-support.html](http://doc.qt.io/qt-5/ios-support.html)




## Test if everything is setup correctly 

You can test that your Qt installation is successful by opening an existing example application project.


1. In the **Welcome** mode, select **Examples**

2. Select an example in the list of examples.

3. To check that the application code can be compiled and linked for a device, click the Kit Selector and select a kit for the device.

[![kits.png](https://s3.postimg.org/j4dyj9u43/kits.png)](https://postimg.org/image/972xq7mi7/)

4. Click  (Run) to build and run the application.

5. To see the compilation progress, press Alt+4 to open the Compile Output pane.


## Running the wikitolearn client on your sytem 

Once you have the source in your filesystem 

- open up Qt creator 

- Click on **File** -> Open **FILE** or **PROJECT**

- Browse to the location where you have saved the source 

- Select wtl.pro file and click OK !!

- Build & run the projet by Clicking on green arrow button in left side pane 


## References

- [https://wiki.qt.io/Install_Qt_5_on_Ubuntu](https://wiki.qt.io/Install_Qt_5_on_Ubuntu)

- [http://doc.qt.io/qt-5/linux.html](http://doc.qt.io/qt-5/linux.html)
