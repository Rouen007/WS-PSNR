# WS-PSNR
WS-PSNR 

##For Linux user:
in directory WS-PSNR/WS-PSNR

 $> make

 $> ./main <command>

##For Windows user:(VS2010)

 click WS-PSNR.sln
 
 main.exe <command>

![figure1](https://github.com/Rouen007/WS-PSNR/tree/master/pic/001.jpg)

WS-PSNR is a metric to evaluate the difference (quality) between two video clips in spherical domain. Here WS-PSNR is utilized as the objective quality metrics between objective quality between the reference video X and the test video X’. WS-PSNR   can supports evaluatinge the quality of omnidirectional video with YUV4:2:0 format in the meaning of sphere. Currently, it only supports video with YUV4:2:0 format and The constraint is that original and reconstructed videos should belong toshares the same format with sameand resolution, as show in Figure 1, in which Video X’ means Video X with some distortion but the format of X’ and X is same.. And we will release new versions of software to support more general cases.  Video X’ means Video X with some distortion but the format of X’ and X is same. 

#Input parameters
##Mandatory:
	(if you add the parameter “-help”:   It will give you a hint how how to use the software)

-w:     Video width
-h :     Video height
-n :     Number of frames
-o :     Original file
-r:      Reconstructed file

##Optional:

	-x:  Index of Image colorspace  defautdefault:  Index=1 (8bit/sample) 
(Index=1: 8bit/sample  Index=2:10bit/sample)

-f:     Index of video format      default：Index=0 (Equirectangular)

![figure2](https://github.com/Rouen007/WS-PSNR/tree/master/pic/002.jpg)

##	Example
1. Calcultate WS-PSNR of original video A and reconstructed B with equirectangular format (4096*2048) for 100 frames (YUV420 8bit/sample)

-w 4096 -h 2048 -n 100 –o A.yuv -r B.yuv

2.  Calcultate WS-PSNR of original video C and reconstructed D with Cubic with 2*3 type format (2048*3072) for 300 frames (YUV420 10bit/sample)

-w 2048 -h 3072 -n 300 –x 2 -f 2 –o C.yuv –r D.yuv




###Yule Sun, Zhejiang University, sunyule@zju.edu.cn
