## Welcome to vivaTracker

The idea of this open source project is to help you create, unify and publish your C, and C++ visual tracking algorithms. 
We would also like to integrate annotated tracking datasets for easy integration and analysis of the different algorithms. 


This project does **NOT** evaluate and compare tracking methods. Many benchmarks with different evaluation methodologies for tracking algorithms are public available.
Among the most relevant benchmarks you could find the [Visual Tracker Benchmark](https://sites.google.com/site/trackerbenchmark/benchmarks/v10) and the [VOT Challenges](http://www.votchallenge.net).
We are just interested in recollect and make public available existing tracking algorithms for the community.
This way, researches and developers can learn, compare and execute the different tracking methods on their own datasets or existing ones. 
 
We provide a cross-platform framework for easy algorithm integration and execution of tracking algorithms on annotated datasets. The project uses cmake system generator to facilitate the development and testing of the algorithms in your own platform/IDE.

Currently, the project incorporates some tracking algorithms (i.e., sKCF, KCF, STRUCK, OpenTLD, and NCC) and all the VOT Challenge datasets.

The **vivaTracker** framework is a C++11 cross-platform project to create, compare and test object tracking algorithms using OpenCV (C, C++). 

Enclosed in this project you will also find the **vivalib**  and **trackerlib** libraries.
The **vivaTracker** project uses concurrent [**vivalib**](https://github.com/asolis/vivaVideo/wiki) 
to process sequences (i.e., video files, web files, image sequences, etc...) while displaying and generating video outputs of your algorithm. 

The **trackerlib** allows simple integration and unifies multiple tracking implementations under the same framework.  
The **trackerlib** sits on top of the **vivalib** to allow access to annotated datasets for object tracking (e.g., VOT datasets) while allowing 
the use of your own dataset or image sequences. 
 
For more detailed information reefer to the project's  [wiki](https://github.com/asolis/vivaTracker/wiki).

