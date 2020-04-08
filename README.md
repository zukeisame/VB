# VB (VariableBuffer)

- Description:
  - A generic, variable-size container acts like an array

- Usage:
  - Pull the files and place it whatever you want on the file system
'''sh
git clone https://github.com/zukeisame/VB.git
'''
  - Into the directory and run "setup.sh" to compile it and place library to appropriate location
'''sh
cd VB
./setup.sh
'''
  - Remember to include VB.h using the right path and compile your program with -lVB options
'''sh
gcc example.c -o example -lVB
'''

- Warning:
  - You should only use the macros and functions declared in the MANUALS of VB.h
