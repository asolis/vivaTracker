#### Trackers
* [sKCF](#skcf)
* [KCF](#kcf)
* [KCF2](#kcf2)
* [STRUCK](#struck)
* [OpenTLD](#opentld)
* [NCC](#ncc)


#### Datasets

* [VOT 2013](http://www.votchallenge.net/vot2013/dataset.html)
* [VOT 2014](http://www.votchallenge.net/vot2014/dataset.html)
* [VOT 2015](http://www.votchallenge.net/vot2015/dataset.html)
* [VOT TIR 2015](http://www.votchallenge.net/vot2015/dataset.html)


#### Descriptions

#####sKCF
> <b>Description</b>:  
 (Best real-time method VOT 2015). A fast Scalable Kernalized Correlation Filter framework [1]. The implementation allows the user to select between 15 algorithms combinations such as: gaussian, polynomial, and linear (sDCF) correlation filters. It also permits to select different features such as: RGB, HSV, HLS, GRAY (sCSK) color space and fast HOG descriptors. It allows to implement different strategies for its scale estimation step, add more features and/or correlation filters.  
 
> <b>Authors</b>:   
[Andres Solis Montero](http://www.solism.ca)   

> <b>License</b>:   
BSD 3-Clause License  

> <b>References</b>:  
[1] Andres Solis Montero, Jochen Lang, and Robert Laganiere, ["Scalable Kernel Correlation Filter With Sparse Feature Integration"](http://www.cv-foundation.org/openaccess/content_iccv_2015_workshops/w14/html/Montero_Scalable_Kernel_Correlation_ICCV_2015_paper.html), The IEEE International Conference on Computer Vision (ICCV) Workshops, 2015.  

#####KCF
> <b>Description</b>:  
 This package includes a C++ class with several tracking methods based on the Kernelized Correlation Filter (KCF)[2,3]. Three algorithms are implemented. First, KCF on HOG features, ported to C++ OpenCV. The original Matlab tracker placed 3rd in VOT 2014. Second, KCF on HOG and Lab features, ported to C++ OpenCV. The Lab features are computed by quantizing CIE-Lab colors into 15 centroids, obtained from natural images by k-means. Finally, the CSK tracker [2] is also implemented by using raw grayscale as features.

> <b>Authors</b>:   
Joao Faro, Christian Bailer, [Joao F. Henriques](http://home.isr.uc.pt/~henriques/)

> <b>License</b>:   
BSD 3-Clause License  

> <b>References</b>:  
[2] J. F. Henriques, R. Caseiro, P. Martins, J. Batista,   
["High-Speed Tracking with Kernelized Correlation Filters"](), TPAMI 2015.

>[3] J. F. Henriques, R. Caseiro, P. Martins, J. Batista,   
["Exploiting the Circulant Structure of Tracking-by-detection with Kernels"](), ECCV 2012. 


#####KCF2
> <b>Description</b>:  
 This is a C++ reimplementation of algorithm presented in "High-Speed Tracking with Kernelized Correlation Filters" paper.
For more info and implementation in other languages visit the [autor's webpage!](http://home.isr.uc.pt/~henriques/circulant/).

> <b>Authors</b>:   
Tomas Vojir

> <b>License</b>:   
>Copyright (c) 2014, Tomáš Vojíř
>
>Permission to use, copy, modify, and distribute this software for research
>purposes is hereby granted, provided that the above copyright notice and 
>this permission notice appear in all copies.
>
>THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
>WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
>MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
>ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
>WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
>ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
>OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
>
> <b>References</b>:  
[2] J. F. Henriques, R. Caseiro, P. Martins, J. Batista,   
["High-Speed Tracking with Kernelized Correlation Filters"](), TPAMI 2015.


#####OpenTLD
> <b>Description</b>:  
  C++ implementation of OpenTLD that was originally published in MATLAB by Zdenek Kalal. OpenTLD is used for tracking objects in video streams. This implementation is based solely on open source libraries, meaning that you do not need any commercial products to compile or run it.

> <b>Authors</b>:   
Georg Nebehay

> <b>License</b>:   
>GNU General Public License  

> <b>References</b>:  
[4] Georg Nebehay,  ["Robust Object Tracking Based on Tracking-Learning-Detection"](http://www.gnebehay.com/publications/master_thesis/master_thesis.pdf), Fakultät für Informatik der Technischen Universität Wien, 2012.


#####Struck
> <b>Description</b>:  
 Structured Output Tracking with Kernels is a C++ reimplementation of the algorithm presented in [5].

> <b>Authors</b>:   
Sam Hare

> <b>License</b>:   
>GNU General Public License  

> <b>References</b>:  
[5] Sam Hare, Amir Saffari, Philip H. S. Torr,  [" Struck: Structured Output Tracking with Kernels"](http://ieeexplore.ieee.org/xpl/login.jsp?tp=&arnumber=6126251&url=http%3A%2F%2Fieeexplore.ieee.org%2Fxpls%2Fabs_all.jsp%3Farnumber%3D6126251),International Conference on Computer Vision (ICCV), 2011.


#####NCC
> <b>Description</b>:  
 A simple normalized cross-correlation tracker. This implementation is based in the NCC tracker utilized to compare and accept trackers into [VOT Challenges](http://www.votchallenge.net) submissions. This algorithm is capable of compute the normalized cross-correlation in inputs with single and multiple channels such as: GRAY, RGB, HSV, HSL, LAB ... etc . 

> <b>Authors</b>:   
Andres Solis Montero

> <b>License</b>:   
>BSD 3-Clause License

> <b>References</b>:  
[VOT Challenge](http://www.votchallenge.net)