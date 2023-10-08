# Exploring the Process of Learning Game Development with C++ and OpenGL: An Adventure in Skill Acquisition
This is a work in progress and I'm learning as I go while documenting my progress. I want to use VS Code as my IDE so I know there might be some challenges.

I'm not an expert in C++ or OpenGL but I have a really basic understanding of both. My professional background is in web development using React and Node.js. I also have some experience with Godot Game Engine.

I'll be using [this YouTube playlist by The Cherno](https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=1) as a guide and a LOT of [ChatGPT](https://chat.openai.com/) to help me figure things out. I'll be using [Visual Studio Code](https://code.visualstudio.com/) as my IDE and [MinGW](http://www.mingw.org/) as my compiler.

## Environment Setup
I have already installed [VS Code](https://code.visualstudio.com/) and [Chocolatey](https://chocolatey.org/). I'm using Windows 10 just in case.

The first thing I did was install the following [C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack) extension for VS Code.

Then I installed MinGW using Chocolatey.

```bash
choco install mingw
```

To verify that MinGW was installed correctly, I ran the following command.

```bash
g++ --version
```

## Compiling and Running
To compile and run the program, I ran the following command.

```bash
g++ hello.cpp -o hello.exe && ./hello.exe
```

## GLFW
I'm using [GLFW](https://www.glfw.org/) to create a window and handle input. I downloaded the [pre-compiled binaries](https://www.glfw.org/download.html) for Windows and extracted the `include` and `lib-mingw-w64` folders to a folder called `dependencies/glfw-3.3.8.WIN64` in the root of the project.

Next, I created a `c_cpp_properties.json` file in the `.vscode` folder so VS Code is able find the GLFW header files.

Finally, I had to setup the library path. This took A LOT of time (3 hours?). These are my findings:

- Even though I was trying to build a 32-bit application, I had to use the 64-bit GLFW library files. I'm not sure why. Maybe it's because I'm using a 64-bit version of MinGW/Windows?
- I had issues debugging the build command. I had much better results when I discovered and created the tasks.json file. Important finding: THE task.json file must be in the .vscode folder. I had it in the root of the project and it was getting ignored.
- I was able to open the build output window by pressing Ctrl+Shift+B adding a new task that runs by default.

After all that, I was able to compile and run the program with `CTRL+SHIFT+B` and VOILA! A window appeared with a black background. ⬛🪟🥳

## GLEW
I'm using [GLEW](http://glew.sourceforge.net/) to load OpenGL functions. I downloaded the [pre-compiled binaries](http://glew.sourceforge.net/) for Windows and extracted the `include` and `lib-x64` folders to a folder called `dependencies/glew-2.1.0` in the root of the project.

Then I followed the same steps as I did for GLFW to setup the library path. The two main issues I had were:

- I had to set the `GLEW_STATIC` preprocessor definition. I had some issues with this because I was setting it in the `c_cpp_properties.json` file but it wasn't working. I had to set it in the `tasks.json` file.
- When adding the glew32s library, I had to add if before the opengl32 library. I'm not sure why but this is what worked for me.

## Modern OpenGL and Shaders
This was very straightforward. I just followed the instructions in the video and it worked. The only thing I had to do was change the `#version 330 core` to `#version 410` in the vertex and fragment shaders. In the video this worked but I was getting an error. Changing the version or adding `#extension GL_ARB_explicit_uniform_location : enable` to the top of the shaders fixed the issue. The version seemed to be the better solution for me.

After this step I had a shader parser that was able to load and compile shaders. And most important: I had a red triangle on the screen! 🔺🔺🔺

## The Mighty Square and Index Buffers
Drawing a square was easy: Just draw two triangles! But there's a better way to do this: Index Buffers. I followed the instructions to define a list of vertices and a list of indices. After some minor changes to the code, I had a square on the screen! 🟦

## Vertex Buffers
After setting the profile to 'core', OpenGL no longer creates a default vertex array object under the hood. This took a bit to understand so I left a lot of comments in the code. The idea is that the vertex array object is a container for vertex buffers and vertex attributes. Now I don't have to bind the vertex array object every time I want to draw something. I just bind it once and then bind the vertex buffer and vertex attributes. This is supposed to be a lot cleaner and easier to understand later on when I have more complex objects and scenes.