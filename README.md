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