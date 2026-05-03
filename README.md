# Candlelight

## Architecture

The engine is split into:

- __Core__ — core elements of the engine such as basic debugging tools and main OOP abstractions.
- __Rendering__ — features the rendering logic of the engine. Basically a wrapper for directx12.
- __General__ — the main library that combines all logic and exposes some useful public API.

## Goals

The ideal goals of the project:

- Have a decoupled architecture where each element can be developed separately
- Support modern `rendering techniques and algorithms
- Learn to write and maintain large C++ projects
- Decouple the project from windows and support other systems (along with Vulkan for rendering)

###` Current goals of the project

- Add an input manager
- Add transform matrix uniforms for mesh manipulation
- Add mesh texturing
- Do not inherit contexts, only pass what is necessary in order not to blow up in scale


## Code rules

Several rules for writing code in this project:

- Classes should follow normal capitalization rules
- For most classes that we can create several instances of we also define a typedef shared pointer with an H before the name. This is our class handle.
- Members follow the following pattern:
    - `s_Xxx` for static fields
    - `m_Xxx` for normal private members
    - `XXX` for constants
- Raw pointers shouldn't be used - stick to c++ smart pointers
- Avoid standard C enums and use enum classes
- 

## Building

`cmake .. -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" -A x64`
`cmake --build . --config Release`
