# Final Project: Hangman
## Controls
Here go the controls

## Extra Credit
We did some cool stuff that is worth more points

## Features
Our program is cool and can do this stuff

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials). 
This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)
 
### Ubuntu/Linux
The following dependencies are required:
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev
sudo apt-get install libassimp-dev
sudo apt-get install libmagick++-dev
sudo apt-get install libbullet-dev
sudo apt-get install libopenal-dev
```

### CMake Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary.

Use cmake with following commands (if cmake is not installed, install it with apt-get or brew):
```bash
mkdir build
cd build
cmake ..
make
./Hangman
```

## Ubuntu.cse.unr.edu
OpenGL 3.3 will run on the [ubuntu.cse.unr.edu](https://ubuntu.cse.unr.edu/) website. To do so follow the build instructions, but when running the Tutorial executable use this line to execute.
```bash
/usr/NX/scripts/vgl/vglrun ./Hangman
```







