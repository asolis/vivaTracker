#### Trackers
* [sKCF](#skcf)
* [KCF](#kcf)
* [STRUCK](#struck)
* [OpenTLD](#opentld)
* [NCC](#ncc)


#### Datasets

* [VOT 2013](http://www.votchallenge.net/vot2013/dataset.html)
* [VOT 2014](http://www.votchallenge.net/vot2014/dataset.html)
* [VOT 2015](http://www.votchallenge.net/vot2015/dataset.html)
* [VOT TIR 2015](http://www.votchallenge.net/vot2015/dataset.html)





#### Descriptions

#####sKCF (Best real-time method VOT 2015)
> Description:  
 A fast Scalable Kernalized Correlation Filter framework [1]. The implementation allows the user to select between 15 algorithms combinations such as: gaussian, polynomial, and linear (sDCF) correlation filters. It also permits to select different features such as: RGB, HSV, HLS, GRAY (sCSK) color space and fast HOG descriptors. It allows to implement different strategies for its scale estimation step, add more features and/or correlation filters. 
> Authors:   
[Andres Solis Montero](http://www.solism.ca)
> License:   
BSD 3-Clause License  
> References:  
[1] Andres Solis Montero, Jochen Lang, and Robert Laganiere, ["Scalable Kernel Correlation Filter With Sparse Feature Integration"](http://www.cv-foundation.org/openaccess/content_iccv_2015_workshops/w14/html/Montero_Scalable_Kernel_Correlation_ICCV_2015_paper.html), The IEEE International Conference on Computer Vision (ICCV) Workshops, 2015.  

#####KCF
> Description:  
 This package includes a C++ class with several tracking methods based on the Kernelized Correlation Filter (KCF)[2,3]. Three algorithms are implemented. First, KCF on HOG features, ported to C++ OpenCV. The original Matlab tracker placed 3rd in VOT 2014. Second, KCF on HOG and Lab features, ported to C++ OpenCV. The Lab features are computed by quantizing CIE-Lab colors into 15 centroids, obtained from natural images by k-means. Finally, the CSK tracker [2] is also implemented by using raw grayscale as features.

> Authors:   
Joao Faro, Christian Bailer, [Joao F. Henriques](http://home.isr.uc.pt/~henriques/)
> License:   
BSD 3-Clause License  
> References:  
[2] J. F. Henriques, R. Caseiro, P. Martins, J. Batista,   
["High-Speed Tracking with Kernelized Correlation Filters"](), TPAMI 2015.

[3] J. F. Henriques, R. Caseiro, P. Martins, J. Batista,   
["Exploiting the Circulant Structure of Tracking-by-detection with Kernels"](), ECCV 2012. 