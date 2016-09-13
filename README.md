# WS-PSNR 

##For Linux user:
in directory WS-PSNR/WS-PSNR

 $> make

 $> ./main "Input parameters"

##For Windows user:(VS2010 X64)

 click WS-PSNR.sln
 
 main.exe "Input parameters"

![figure1](/pic/003.PNG)

WS-PSNR is a metric to evaluate the difference (quality) between two video clips in spherical domain. Here WS-PSNR is utilized as the objective quality metrics between the reference video X and the test video X’. WS-PSNR supports evaluating the quality of omnidirectional video in the meaning of sphere. Currently, it only supports video with YUV4:2:0 format and original and reconstructed videos shares the same format and resolution, as show in Figure 1, in which Video X’ means Video X with some distortion but the format of X’ and X is same. And we will release new versions of software to support more general cases. 

#Input parameters
##Mandatory:
	(if you add the parameter “-help”:   It will give you a hint on how to use the software)

-w:      Video width

-h :     Video height

-n :     Number of frames

-o :     Original file

-r:      Reconstructed file

##Optional:

-x:     Index of Image colorspace  defautdefault:  Index=1 (8bit/sample) 
(Index=1: 8bit/sample  Index=2:10bit/sample)

-f:     Index of video format      default：Index=0 (Equirectangular)

![figure2](/pic/004.PNG)

##	Example
### Calcultate WS-PSNR of original video A and reconstructed B with equirectangular format (4096 * 2048) for 100 frames (YUV420 8bit/sample)

-w 4096 -h 2048 -n 100 –o A.yuv -r B.yuv

### Calcultate WS-PSNR of original video C and reconstructed D with Cubic with 2*3 type format (2048 * 3072) for 300 frames (YUV420 10bit/sample)

-w 2048 -h 3072 -n 300 –x 2 -f 2 –o C.yuv –r D.yuv


### Remarks
Other formats can also be added to be evaluated using WS-PSNR if needed.


###Yule Sun, Zhejiang University, sunyule@zju.edu.cn
###Lu Yu,    Zhejiang University,     yul@zju.edu.cn
###Ang Lu,   Zhejiang University, luangss@zju.edu.cn
