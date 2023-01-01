@echo off
echo %1
echo %~dp0

set ArduinoBuilderCMD="C:\Program Files (x86)\Arduino\arduino-builder" 
set ArduinoHWDir=-hardware "C:\Program Files (x86)\Arduino\hardware"
set ArduinoToolsDir=-tools "C:\Program Files (x86)\Arduino\tools-builder"
set OtherToolsDir=-tools "C:\Program Files (x86)\Arduino\hardware\tools\avr"
set ArduinoBuiltInLibs=-built-in-libraries "C:\Program Files (x86)\Arduino\libraries"

set TeensyFQBN=-fqbn=teensy:avr:teensy41:usb=serial,speed=600,opt=o2std,keys=en-us
set ArduinoIDEVersion=-ide-version=10819

set BuildPath=-build-path %~dp0\build
set BuildCache=-build-cache %~dp0\cache
set TEENSYINO=%~dp0\Teensy.ino

set BuilderOptsDump=-dump-prefs
set BuilderOptsDump=-compile

Rem set BuilderOptsCommon=-logger=machine %ArduinoHWDir% %ArduinoToolsDir% %OtherToolsDir% %ArduinoBuiltInLibs% %TeensyFQBN% %ArduinoIDEVersion% %BuildPath% -warnings=none %BuildCache% -verbose %TEENSYINO%
set BuilderOptsCommon=-logger=machine %ArduinoHWDir% %ArduinoToolsDir% %OtherToolsDir% %ArduinoBuiltInLibs% %TeensyFQBN% %ArduinoIDEVersion% %BuildPath% -warnings=none %BuildCache% %TEENSYINO%

%ArduinoBuilderCMD% %BuilderOptsDump% %BuilderOptsCommon%
%ArduinoBuilderCMD% %BuilderOptsBuild% %BuilderOptsCommon%