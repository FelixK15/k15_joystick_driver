@echo off
setlocal enableextensions enabledelayedexpansion

set main_c_file=driver/driver.ino
set fpqn="arduino:avr:micro:cpu=atmega32u4"
echo Searching for Arduino compiler...

if "%ARDUINO_HOME%" == "" (
    echo Please specify a ARDUINO_HOME environment variable which contains the path to a valid arduino installation ^(eg. C:\Program Files\Arduino^).
    exit /b 1
)

set arduino_path=%ARDUINO_HOME%
if !arduino_path:~-1! NEQ "\" (
    set arduino_path=!arduino_path!^\
)

if not exist "!arduino_path!arduino-builder.exe" (
    echo Could not find 'arduino-builder^.exe' in ARDUINO_HOME, please check that ARDUINO_HOME is pointing to a valid Arduino installation ^(eg. C:\Program Files\Arduino^).
    exit /b 1
)

if not exist "%cd%\bin" (
    mkdir "%cd%\bin"
)

set command=!arduino_path!arduino-builder.exe -compile -build-path "%cd%\bin" -tools "!arduino_path!tools" -tools "!arduino_path!hardware\tools" -tools "!arduino_path!tools-builder" -hardware "!arduino_path!hardware" -libraries "!arduino_path!libraries" -fqbn !fpqn! -warnings default !main_c_file!
call !command!
