How to build the VulKan ToolS (VKTS) examples:
----------------------------------------------

General requirements:
---------------------
  
- [LunarG Vulkan SDK](http://vulkan.lunarg.com) version has to be 1.0.3.1.
- Vulkan headers and libraries have to be visible by the used compiler and/or IDE. 


Following requirements do depend on the platform:

![Android](images/android_logo.png) Android:
---

- Only 32bit Intel systems are supported.
- Building: ndk-build
    - Windows batch files included.
    - [Android NDK](http://developer.android.com/ndk/index.html) required.  


![Linux](images/linux_logo.png) Linux:
---

- Only 64bit Intel systems are supported. Anyway, 32bit should be buildable.
- Building: Eclipse IDE for C/C++ Developers (recommended)
    - [Eclipse IDE for C/C++ Developers](https://www.eclipse.org/downloads/) project files included. 
- Building: make  
    - Minimum [CMake](https://cmake.org/) version has to be 3.2.0.


![Windows](images/windows_logo.png) Windows:
---

- Only 64bit Intel systems are supported. Anyway, 32bit should be buildable.  
- Building: Visual C++ (recommended)
    - Minimum [CMake](https://cmake.org/) version has to be 3.2.0.
    - [Visual Studio 2015](https://www.visualstudio.com/en-us/downloads/visual-studio-2015-downloads-vs.aspx) or later is required.
- Building: Eclipse IDE for C/C++ Developers
    - [Eclipse IDE for C/C++ Developers](https://www.eclipse.org/downloads/) project files included. 
- Building: make  
    - Minimum CMake version has to be 3.2.0.
    - [Msys2 with mingw-w64](http://msys2.github.io/) required.


---

Using Eclipse:
--------------

1. Install [Eclipse IDE for C/C++ Developers](https://www.eclipse.org/downloads/) for Linux or Windows.
2. Install GCC for Linux or [Msys2 with mingw-w64](http://msys2.github.io/) for Windows.
3. Import all folders as an existing project in Eclipse. 
4. Set the build configuration of VKTS and the VKTS_Example* in Eclipse to your operating system.
5. Build VKTS.
6. Build VKTS_Example*.
7. The executables are located in the [VKTS_Binaries](VKTS_Binaries/README.md) folder.


---

Using Visual C++ 2015:
----------------------

1. Install [Visual Studio 2015 Community Edition](https://www.visualstudio.com/en-us/downloads/visual-studio-2015-downloads-vs.aspx) for Windows.
2. Run CMake and configure for "Visual Studio 14 2015 Win64".
3. Set the source code directory to VKTS.
4. Set the build directory to VKTS/MSVC (Note: This build directory is expected by the examples).
5. Open the Visual C++ solution file and build VKTS.
6. Do step 3 to 5 with VKTS_Example*.
7. The executables are located in the [VKTS_Binaries](VKTS_Binaries/README.md) folder.


---

Using make:
-----------

1. Install a GNU GCC toolchain.
2. Run CMake and configure "MSYS Makefiles" for Windows or "Unix Makefiles" for Linux.
3. Set the source code directory to VKTS.
4. Set the build directory to VKTS (Note: This build directory is expected by the examples). 
5. Run make and build VKTS.
6. Do step 3 to 5 with VKTS_Example*.
7. The executables are located in the [VKTS_Binaries](VKTS_Binaries/README.md) folder.


---

Using Android NDK:
------------------

The assets are not packaged in the apk file. Instead they are located in the `/sdcard/Download/VKTS_Binaries`
folder on the device.
The scripts `android_copy.bat` and `android_copy.sh` located in [VKTS_Binaries](VKTS_Binaries/README.md) do
create the directory and copies all the files from the host system to the device.


The following command creates the Android project:

1. Open console in Android project folder and execute following command without the comment:
    - android.bat update project -p . -t android-22    // Has only do be done once.

    
The following command builds and installs the Android project:

1. Open console in Android project folder and execute following commands without the comments:
    - cd jni
    - ndk-build
    - cd ..
    - ant debug
    - adb install -r bin/NativeActivity-debug.apk


The following command executes the Android application:

1. Open console in Android project folder and execute following command without the comment:
    - adb shell am start -n tv.nopper.VKTS_Example01/android.app.NativeActivity


The following commands allows to display the log output:

1. Open another console execute following commands without the comments:
    - adb logcat -c                    // Clear log output
    - adb logcat -s VKTS               // Only display VKTS log output


Setting up the Android device:
------------------------------

The following has only be done once:

1. Enable developer mode on the Android device.
2. Enable the wireless network on the Android device.
2. Execute the following commands without the comments:
    - adb root                         // Restart adb as root
    - adb remount                      // Remount /system for read and write
    - adb pull /system/build.prop      // Get the provided file
3. Append 'service.adb.tcp.port=5555' in donwloaded build.prop to autostart adbd in TCPIP mode
4. Execute the following command without the comment:
    - adb push build.prop /system      // Store the modified file


Common adb commands:
--------------------

- List Android devices:
    - adb devices
- Restart adbd daemon with usb:
    - adb usb
- Restart adbd daemon with tcpip and given port:
    - adb tcpip 5555
- Connect to a TCPIP enabled device:
    - adb connect 192.168.0.12:5555    // Change the IP address


---

Installing Msys2:
-----------------

1. Install and update as found here: http://msys2.github.io/
2. Install components by executing the following commands without the comments:
    - pacman -S mingw-w64-x86_64-gcc   // gcc
    - pacman -S make                   // make
    - pacman -S gdb                    // gdb
    - pacman -S git                    // git
    - pacman -Su                       // Update to latest version
