# sdl-opengl3-project
A test project to see how well SDL2 and OpenGL3 can work together.

## Dependancies
* G++
* SDL2 [Link](https://github.com/libsdl-org/SDL/releases/tag/release-2.26.2)
* Glad [Link](https://glad.dav1d.de/)

Currently it's setup for Windows but should be fairly easy to add Linux / Mac to the Makefile
Edit the Makefile INCLUDES to match where you extract the SDL2 and Glad source files to.
Then where your Makefile is located open a cmd promt and run this command
~~~
make
~~~
It should compile an exe in the build folder, and open a window with a light blue tinted Triangle with a darker blue background.

![image](https://user-images.githubusercontent.com/11281480/215659296-c0e7b9f9-2b50-4d75-b9db-c32279641979.png)
