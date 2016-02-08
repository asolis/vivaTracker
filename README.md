## Welcome to vivaTracker

This project contains a collection of visual tracking algorithms implemented in C++ and/or C. 
The idea of the project is to help you create, unify and publish your visual tracking algorithms. 
We also include public tracking datasets for easy integration and analysis of the algorithms. 


This project does **NOT** evaluate and compare tracking methods. Many benchmarks with different evaluation methodologies for tracking algorithms are public available.
Among the most relevant benchmarks you could find the [Visual Tracker Benchmark](https://sites.google.com/site/trackerbenchmark/benchmarks/v10) and the [VOT Challenges](http://www.votchallenge.net).
We are just interested in collecting and publishing the algorithm implementations and datasets.
This way, researches and developers can learn, compare and execute the different methods on their own sequences or existing datasets. 
 
We provide a cross-platform framework for easy integration and execution of tracking algorithms on annotated datasets. The project uses cmake system generator to facilitate the development and testing of the algorithms in your own platform/IDE.

The **vivaTracker** framework is a C++11 cross-platform project to create, compare and test object tracking algorithms using OpenCV 3.x.x. 

For more detailed information about the implementations and datasets provided in this project reefer to the project's  [list](https://github.com/asolis/vivaTracker/wiki).

## Requirements
* [CMake](http://cmake.org) > 3.0
* C++11 compiler
* [OpenCV](http://opencv.org) > 3.0
* [Eigen Library](http://eigen.tuxfamily.org/index.php?title=Main_Page) > 3.0


## Supported Platforms
Mac, Linux, Windows


## License

     BSD 3-Clause License (https://www.tldrlegal.com/l/bsd3)
     
     Copyright (c) 2015 Andrés Solís Montero <http://www.solism.ca>, All rights reserved.
     
     
     Redistribution and use in source and binary forms, with or without modification,
     are permitted provided that the following conditions are met:
     
     1. Redistributions of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.
     2. Redistributions in binary form must reproduce the above copyright notice,
        this list of conditions and the following disclaimer in the documentation
        and/or other materials provided with the distribution.
     3. Neither the name of the copyright holder nor the names of its contributors
        may be used to endorse or promote products derived from this software
        without specific prior written permission.
     
     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
     AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
     IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
     ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
     LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
     DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
     THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
     OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.