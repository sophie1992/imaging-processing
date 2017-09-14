# imaging-processing

In the programming project, we create a program that tracks markers in video files
from a stereo camera rig and triangulates the marker positions to get the positions in
the 3D space.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

What things you need to install the software and how to install them

we suggest using Visual Studio 2012 or 2013 (Express edition is sufficient) on Windows. Also you need to install:
OpenCV and CMake used to create projects for the development enviroment
 
the sequence and calibration data can be load from  https://tux.ti1.tuharburg.de/3dcv/signin.php with student's identity at TUHH 
 
```
Give examples
```

### Installing

A step by step series of examples that tell you have to get a development env running

OpenCV
   1 download and unzip OpenCV. see link in ##Built With
   2 after you unzipped OpenCV, you have to add OpenCV to the environment variable PATH. The folder to add to the PATH variable is build/x86/vc11/bin inside the unzipped OpenCV folder for Visual Studio 2012 and build/x86/vc12/bin for Visual Studio 2013.

CMake 
   1 download and install CMake. see link in ##Built With
   2 Start the CMake GUI and choose the checked out folder as source folder. Choose also the build folder. This folder will be used for the output of the compiler. You could e.g. use a folder build inside the repository, but be sure not to add it to the version control. The simplest way is to add it to the ignore list.
   3 click Configure and follow the instructions to choose the build environment. Now there will be variables in the middle of the window. Activate the option Grouped and open the group OpenCV. Check if the variable OpenCV_DIR points to the directory build inside
your unzipped OpenCV folder. Otherwise change it accordingly and press Configure again. Now you are able to create the project by clicking on Generate.

```
Give the example
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```
## Deployment

Add additional notes about how to deploy this on a live system

## Built With
*[OpenCV](http://www.opencv.org/)
*[CMake](http://www.cmake.org/)
*[VS 2012/2013](http://www.microsoft.com/dede/download/details.aspx?id=34673)


## Versioning

We use [TortoiseSVN](http://tortoisesvn.net/) for versioning. 

## Authors

* **Junpei Xu** - *Initial work done by Prof. Dr.-Ing. R.-R. Grigat B. Sc. Matthias Schlüter
M. Sc. Sven Painer* - [3dcv](https://tux.ti1.tuharburg.de/3dcv/signin.php)

## License

This project is licensed under the GNU License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone who's code was used
* Inspiration
* etc
