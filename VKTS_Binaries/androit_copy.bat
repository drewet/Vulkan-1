@echo off

adb shell "mkdir /sdcard/Download/VKTS_Binaries"
adb push . /sdcard/Download/VKTS_Binaries
