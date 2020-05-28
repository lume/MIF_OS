# MIF_OS
A repository created to store source code for the Operating Systems course in University

Compilation:

Make sure you are in the root directory.
- For tests, use 'make test'
- For debug mode, use 'make debug'
- For release mode, use 'make release'

Complete OS preparation:
to install desired OS programs, compile them, and then install via OSInstaller python script:
```
python3 OSInstaller.py [compiled programs,]
```
Easier way is to call prepare.sh script. This will compile all the required components and return a prepared drive.

To install non-kernel programs, use 
```
python3 InstallProgram.py [program name]
```
