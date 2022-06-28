# C Programming Project Setup (cpps)
<br><br>
<img src="https://img.shields.io/badge/Windows-0078D6?style=flat&logo=windows&logoColor=white" />
<img src="https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white" />
<img src="https://img.shields.io/badge/Notepad++-90E59A.svg?style=flat&logo=notepad%2B%2B&logoColor=black" />
[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
<img src="https://img.shields.io/badge/Version-0.1.1--alpha.2-blue?style=flat&color=blue" />
<br><br>

## About
A simple commandline C program to set up a C programming project. This is for programmers that prefer to do development using the terminal.<br>
<br>
__Sample output:__<br>
C Programming Project<br>
&nbsp;&nbsp;├─ bin<br>
&nbsp;&nbsp;├─ src<br>
&nbsp;&nbsp;│&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└─ sourcefile.c<br>
&nbsp;&nbsp;└─ Makefile<br>
<br><br>

## Requirement
An installation of MinGW is required to build, compile, and install this program in windows.<br>
<br>
__Note:__<br>
For those who are new to using MinGW, after installing it, please
read <a target="_blank" href="https://opensource.com/article/20/8/gnu-windows-mingw" title="Use GNU on Windows with MinGW">this</a> to set it properly.
<br><br>

## How to build, compile, and install
To build and compile *cpps*, just run the following command:<br>

```
$>make
...
$>make install
```

Then manually add ```C:\CPPS\bin``` to the environment variable *Path* or use the *cmd* script ```install-cpps.cmd``` to build and install *cpps*.<br>
<br><br>

## How to use
To set up a C programming project:<br>

```
$>cpps <project name> -f <file name> -e <executable name>
```
<br>

__Note:__<br>
* If *executable name* is omitted, it will use *project name* as the executable file name.<br>
* If *file name* is omitted, it will use *"main"* as the C source file name.<br>
<br><br>

## License
This program uses GNU General Public License Version 2.0.<br>
See [```COPYING```](url:COPYING) for more details.<br>
<br><br>


	