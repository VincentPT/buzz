# buzz
Using injection technique to visualize debugging.

## Getting Started

The project is focus on dll injection technique and using Cinder.
* Using dll injection to read object from another process
* Using Cinder to visualize object as graphics in buzz app.

### Prerequisites
    * Windows (only support for Window)
    * Visual studio
    * CMake 3.2 or above
    * Cinder 0.9
    * OpenCV 3.1 or above
    
### Install development environment
    * Cinder.
        Go to link https://libcinder.org/download and download 0.9.x version for visual studio 2013
        or you can build your own lib from cinder's source on Github(https://github.com/cinder/Cinder)
        The project is using cinder as static library. So, you should also build cinder as static library for further required steps.
    
    * OpenCV.
        Go to link http://opencv.org/releases.html and download and install OpenCV version 3.x.y(x >= 1)
        Same as, Cinder, you can also build your own library from OpenCV's source on Githut(https://github.com/opencv/)
  
## Build.
    After download and install all prerequisites. Go ahead to build the application.
    First, go to folder .\src\buid.
    Choose configure.bat to generate x64 platform projects, configure-x86.bat for x86 projects.
    Update path for Cache variables: CINDER_INCLUDE_DIR, CINDER_LIB_DIR, OPENCV_INCLUDE_DIR, OPENCV_LIB_DIR to your installed libraries.
    
    the open command-line and cd to above build folder.
    run the bat file that you choose.
    
    open solution file 'buzz.sln' in Visual Studio and start the build.
    After building success, the application and dependencies is place in folder src\buid\BuzzApp\<Debug or Release>
    
## Run
    Before run the app. you should create a sample project that use opencv.
    Current application support some OpenCV objects such as Mat, Rect, Contours.
    So, if you want to se how it works, you should create a sample that using above objects.
    
    step to run.
    1. Run buzz app.
    2. press 'w' and fill process name filed with your application file name(with extension .exe)
    3. open your app in visual studio.
    4. set a break point where the OpenCV object is going to be created when instruction reach there.
    5. Run your app in Debug mode and wait until the break point is reached.
    6. In 'Watch' window type variables'name holding the opencv objects.
       if your variables is not a pointer type '&' before type the variables's name to get address.
    7. select a row of Watch window and press Ctrl + C or Right click and choose Copy.
    
    8. See resut in Buzz window if success the object that the variable holding will be displayed there.
        see console window for details information.