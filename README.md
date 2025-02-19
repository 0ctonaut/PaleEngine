# PaleEngine

## Intro

A toy.

## Setup

Currently only debugged on **Windows11** + **Visual Studio 2022**

Please clone this repo with submodules.
```shell
git clone --recursive git@github.com:0ctonaut/PaleEngine.git
cd PaleEngine
cmake -S . -Bbuild
```

Here are some model files:

+ [skybox](https://learnopengl-cn.github.io/data/skybox.rar)
+ [backpack](https://learnopengl.com/data/models/backpack.zip)

## Interface snapshot 250218

![](github/interface250218.png)


## TODO List

+ rendering
  + [X] blinn-phong
  + [X] PBR
  + [ ] IBL
    + [X] skybox
  + [ ] OIT
  + [ ] PCF & PCSS
  + [ ] Deferred shading
  + [ ] multi lights

+ GUI
  + [ ] log console
  + [ ] Assets brower
  + [ ] ImGUI gizmo

+ Profiler
  + [ ] performance
  + [ ] memory leak

+ Other
  + [ ] Input event system
  + My own formats to save...
    + [ ] Scene
    + [ ] Material
