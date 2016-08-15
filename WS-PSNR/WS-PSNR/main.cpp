/*
  The copyright in this software is being made available under the BSD
  License, included below. This software may be subject to other third party
  and contributor rights, including patent rights, and no such rights are
  granted under this license.
 
  Copyright (c) 2016, Zhejiang University.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   * Neither the name of Samsung Electronics Co., Ltd. nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
 
 *  Authors:
 *      Yule Sun,        Zhejiang University (ZJU),               sunyule@zju.edu.cn
 *      Ang Lu,          Zhejiang University (ZJU),               luangss@zju.edu.cn
 *      Lu  Yu,          Zhejiang University (ZJU),               yul@zju.edu.cn
 *
 *  Reference:
 *     WS-PSNR for 360 video quality evaluation  Yule Sun, Ang Lu, Lu Yu,     
 *     ISO/IEC JTC1/SC29/WG11 MPEG2016/M38551, May 2016, Geneva                 
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	double getWeight(int form,int i,int j,int width,int height);

	int width		= -1;
	int height	= -1;
	int num_frame = -1;
	int CountFrame;
	int form		= 0;
	int ColorSpace= 1;
	double psnr_sphere_Y = 0;
	double psnr_sphere_U = 0;
	double psnr_sphere_V = 0;

	char *Comp_File1 = (char *)malloc(sizeof(char)*100);
	char *Comp_File2 = (char *)malloc(sizeof(char)*100);

	FILE *FpOriginalFile;
	FILE *FpReconFile;

	// ========== patch processing  ==========  
	int iArgIdx = 1;
	if (argc > 1) {
	if (strcmp("-help",argv[iArgIdx]) == 0) {
		printf("\n");
		printf("-w		width\n");
		printf("-h		height\n");
		printf("-n		number of frames to be compared\n");
		printf("-f		format of the video\n");
		printf("-x		image color space\n");
		printf("-o		original file\n");
		printf("-r		reconstructed file\n");
		//system("pause");
		exit(0);
	}
	}

	for( iArgIdx = 1; iArgIdx < argc; iArgIdx += 2) {
		if      (strcmp("-w", argv[iArgIdx]) == 0)  width      = atoi(argv[iArgIdx + 1]);
		else if (strcmp("-h", argv[iArgIdx]) == 0)  height     = atoi(argv[iArgIdx + 1]);
		else if (strcmp("-n", argv[iArgIdx]) == 0)  num_frame  = atoi(argv[iArgIdx + 1]);
		else if (strcmp("-f", argv[iArgIdx]) == 0)  form       = atoi(argv[iArgIdx + 1]);
		else if (strcmp("-x", argv[iArgIdx]) == 0)  ColorSpace = atoi(argv[iArgIdx + 1]);
		else if (strcmp("-o", argv[iArgIdx]) == 0)  strcpy(Comp_File1,     argv[iArgIdx + 1]);
		else if (strcmp("-r",argv[iArgIdx])  == 0)  strcpy(Comp_File2,    argv[iArgIdx + 1]);

	}

  //==========  check and parse the input parameters  ==========  
    if (width == -1 || height == -1) {
		printf("please input the resolution of the video \n");
		//system("pause");
		exit(1);
    }

    if (num_frame == -1) {
		printf("please input the number of frames to be compared \n");
		//system("pause");
		exit(1);
    }

    if (Comp_File1 == NULL || Comp_File2 == NULL) {
		printf("please input the comparative file \n");
		//system("pause");
		exit(1);
    }

	if		(form == 0)	  printf(" Format : Equirectangular \n");
	else if (form == 1)	  printf(" Format : Cubic with 3*2 type\n");
	else if (form == 2)	  printf(" Format : Cubic with 2*3 type\n");
	else if (form == 3)	  printf(" Format : Cubic with 6*1 type\n");
	else if (form == 4)   printf(" Format : Cubic with 1*6 type\n");
	else if (form == 5)   printf(" Format : Cubic with T type\n");
	// else if (form==6)  printf(" Format : Icosahedron \n");
	else {
	  printf("Invalid input option for format index \n");
	  //system("pause");
	  exit(1);
	}
	
	printf(" Resolution: %d * %d\n",width,height);
	printf(" number of frames: %d\n\n",num_frame);
	printf(" Frame  WS-PSNR_Y  WS-PSNR_U  WS-PSNR_V\n");
	
	if (ColorSpace == 1) {
		unsigned char *OriginalFileBuffer;
		unsigned char *ReconFileBuffer;
		OriginalFileBuffer  = (unsigned char *)malloc(sizeof(unsigned char)*width*height*3/2);
		ReconFileBuffer		= (unsigned char *)malloc(sizeof(unsigned char)*width*height*3/2);

		for(CountFrame = 0;CountFrame < num_frame;CountFrame++) {
			printf("   %d ",CountFrame);
			FpOriginalFile = fopen(Comp_File1,"rb");

			fseek(FpOriginalFile,height*width*3/2*CountFrame,SEEK_SET);
			if (fread(OriginalFileBuffer,height*width*3/2,1, FpOriginalFile) != 1) {
				printf("can't open the file %s \n",Comp_File1);
				//system("pause");
				exit(1);
			}
			fclose(FpOriginalFile);

			FpReconFile = fopen(Comp_File2,"rb");
			fseek(FpReconFile,height*width*3/2*CountFrame,SEEK_SET);
			if (fread(ReconFileBuffer,height*width*3/2,1, FpReconFile) != 1) {
				printf("can't open the file %s \n",Comp_File2);
				//system("pause");
				exit(1);
			}
			fclose(FpReconFile);

			double ssdR_Y = 0;
			double ssdR_U = 0;
			double ssdR_V = 0;

			double latWeight   = 0;
			double totWeight_Y = 0;
			double totWeight_U = 0;
			double totWeight_V = 0;

			for(int j = 0;j < height;j++) {
				for(int i=0;i<width;i++) {
					latWeight = getWeight(form,i,j,width,height);
					double v=OriginalFileBuffer[j*width+i]-ReconFileBuffer[j*width+i];

					ssdR_Y+=100000*v*v*latWeight;
					totWeight_Y+=latWeight;
				}
			}
			double mseR_Y = ssdR_Y/totWeight_Y/100000;
			double psrR_Y= 10*log10(255*255/mseR_Y);

			printf("    %.4f",psrR_Y);
			psnr_sphere_Y+=psrR_Y;

			for(int j=0;j<height/2;j++) {
				for(int i=0;i<width/2;i++) {
					latWeight = getWeight(form,i,j,width/2,height/2);
					double v=OriginalFileBuffer[width*height+j*width/2+i]-ReconFileBuffer[width*height+j*width/2+i];

					ssdR_U+=100000*v*v*latWeight;
					totWeight_U+=latWeight;
				}
			}
			double mseR_U = ssdR_U/totWeight_U/100000;
			double psrR_U= 10*log10(255*255/mseR_U);

			printf("    %.4f",psrR_U);
			psnr_sphere_U+=psrR_U;

			for(int j=0;j<height/2;j++) {
				for(int i=0;i<width/2;i++) {
					latWeight = getWeight(form,i,j,width/2,height/2);
					double v=OriginalFileBuffer[width*height*5/4+j*width/2+i]-ReconFileBuffer[width*height*5/4+j*width/2+i];

					ssdR_V+=100000*v*v*latWeight;
					totWeight_V+=latWeight;
				}
			}
            double mseR_V = ssdR_V/totWeight_V/100000;
            double psrR_V = 10*log10(255*255/mseR_V);

            printf("    %.4f\n",psrR_V);
            psnr_sphere_V+=psrR_V;
		}

	}
	else if (ColorSpace==2)	{
		unsigned short *OriginalFileBuffer;
		unsigned short *ReconFileBuffer;
		OriginalFileBuffer  =  (unsigned short *)malloc(sizeof(unsigned short)*width*height*3/2);
		ReconFileBuffer     = (unsigned short *)malloc(sizeof(unsigned short)*width*height*3/2);

		for(CountFrame=0;CountFrame<num_frame;CountFrame++) {
			printf("   %d ",CountFrame);
			FpOriginalFile=fopen(Comp_File1,"rb");

			fseek(FpOriginalFile,height*width*3*CountFrame,SEEK_SET);
			if (fread(OriginalFileBuffer,height*width*3,1, FpOriginalFile)!=1) {
				printf("can't open the file %s \n",Comp_File1);
				//system("pause");
				exit(1);
			}
			fclose(FpOriginalFile);

			FpReconFile=fopen(Comp_File2,"rb");
			fseek(FpReconFile,height*width*3*CountFrame,SEEK_SET);
			if (fread(ReconFileBuffer,height*width*3,1, FpReconFile)!=1) {
				printf("can't open the file %s \n",Comp_File2);
				//system("pause");
				exit(1);
			}
			fclose(FpReconFile);

			double ssdR_Y=0;
			double ssdR_U=0;
			double ssdR_V=0;

			double latWeight=0;
			double totWeight_Y=0;
			double totWeight_U=0;
			double totWeight_V=0;

			for(int j = 0;j < height;j++){
				for(int i = 0;i < width;i++) {
					latWeight = getWeight(form,i,j,width,height);
					double v=OriginalFileBuffer[j*width+i]-ReconFileBuffer[j*width+i];

					ssdR_Y+=100000*v*v*latWeight;
					totWeight_Y+=latWeight;
				}
			}
			double mseR_Y = ssdR_Y/totWeight_Y/100000;
			double psrR_Y = 10*log10(1023*1023/mseR_Y);

			printf("    %.4f",psrR_Y);
			psnr_sphere_Y+=psrR_Y;

			for(int j = 0;j < height/2;j++) {
				for(int i = 0;i < width/2;i++) {
			        latWeight = getWeight(form,i,j,width/2,height/2);
					double v=OriginalFileBuffer[width*height+j*width/2+i]-ReconFileBuffer[width*height+j*width/2+i];

					ssdR_U+=100000*v*v*latWeight;
					totWeight_U+=latWeight;
				}
			}
			double mseR_U = ssdR_U/totWeight_U/100000;
			double psrR_U = 10*log10(1023*1023/mseR_U);

			printf("    %.4f",psrR_U);
			psnr_sphere_U+=psrR_U;

			for(int j = 0;j < height/2;j++) {
				for(int i = 0;i < width/2;i++) {
					latWeight = getWeight(form,i,j,width/2,height/2);
					double v=OriginalFileBuffer[width*height*5/4+j*width/2+i]-ReconFileBuffer[width*height*5/4+j*width/2+i];

		            ssdR_V+=100000*v*v*latWeight;
					totWeight_V+=latWeight;
				}
			}
            double mseR_V = ssdR_V/totWeight_V/100000;
            double psrR_V = 10*log10(1023*1023/mseR_V);

            printf("    %.4f\n",psrR_V);
            psnr_sphere_V += psrR_V;
		}

	}
	else {
		printf("Invalid input option for color space \n");
		//system("pause");
		exit(1);
	}

	psnr_sphere_Y = psnr_sphere_Y/num_frame;
	psnr_sphere_U = psnr_sphere_U/num_frame;
	psnr_sphere_V = psnr_sphere_V/num_frame;

	printf("\n  Average W-PSNR_Y = %.4f \n",psnr_sphere_Y);
	printf("  Average W-PSNR_U = %.4f \n",psnr_sphere_U);
	printf("  Average W-PSNR_V = %.4f \n\n\n",psnr_sphere_V);

	//system("pause");
}

double getWeight(int form,int i,int j,int width,int height){
	double a;
 //======  format0: Equirectangular  =======  
	if (form == 0) {
		a = cos((j-(height/2-0.5))*3.1415926/height);
		return a;
	}

 //======  format1: Cubic with 3*2 faces =======  
             // 1 1 2 2 3 3
             // 1 1 2 2 3 3
             // 4 4 5 5 6 6
             // 4 4 5 5 6 6

	else if (form == 1) {
    int ci, cj,r2;
    double d2;

		if (i < width/3 && j < height/2) {
			ci = width/6;
			cj = height/4;
		}
		else if (i < width/3 && j >= height/2) {
			ci = width/6;
			cj = 3*height/4;
		}
		else if (i >= width/3 &&i < 2*width/3 && j < height/2) {
			ci = width/2;
			cj = height/4;
		}
		else if (i >= width/3 &&i < 2*width/3 && j >= height/2) {
			ci = width/2;
			cj = 3*height/4;
		}
		else if (i >= 2*width/3 && j < height/2) {
			ci = 5*width/6;
			cj = height/4;
		}
		else if (i >= 2*width/3 && j > height/2) {
			ci = 5*width/6;
			cj = 3*height/4;
		}

		d2 = (i+0.5-ci)*(i+0.5-ci)+(j+0.5-cj)*(j+0.5-cj);
		r2 = (height/4)*(height/4);
		a  = 1.0/((1+d2/r2)*sqrt(1.0*(1+d2/r2)));
		return a;
	}

  //======  format2: Cubic with 2*3 faces   =======  
          // 1 1 2 2 
          // 1 1 2 2 
          // 3 3 4 4
          // 3 3 4 4
          // 5 5 6 6
          // 5 5 6 6

	else if (form == 2) {
    int ci, cj,r2;
    double d2;

		if (i < width/2 && j < height/3) {
			ci = width/4;
			cj = height/6;
		}
		else if (i >= width/2 && j < height/3) {
			ci = 3*width/4;
			cj = height/6;
		}
		else if (j >= height/3 &&j < 2*height/3 && i < width/2) {
			ci = width/4;
			cj = height/2;
		}
		else if (j >= height/3 &&j < 2*height/3 && i >= width/2) {
			ci = 3*width/4;
			cj = height/2;
		}
		else if (j >= 2*height/3 && i < width/2) {
			ci = width/4;
			cj = 5*height/6;
		}
		else if (j >= 2*height/3 && i >= width/2) {
			ci = 3*width/4;
			cj = 5*height/6;
		}

	    d2 = (i+0.5-ci)*(i+0.5-ci)+(j+0.5-cj)*(j+0.5-cj);
		r2 = (width/4)*(width/4);
		a  = 1.0/((1+d2/r2)*sqrt(1.0*(1+d2/r2)));
		return a;
	}

  //======  format3:  Cubic with 6*1 faces=======  
  //   1 1 2 2 3 3 4 4 5 5 6 6 
  //   1 1 2 2 3 3 4 4 5 5 6 6 

	else if (form == 3) {
    int ci, cj,r2;
    double d2;

		if (i < width/6 ) {
			ci = width/12;
			cj = height/2;
		}
		else if (i >= width/6 && i < width/3) {
			ci = width/4;
			cj = height/2;
		}
		if (i >= width/3 &&i < width/2) {
			ci = 5*width/12;
			cj = height/2;
		}
		if (i >= width/2 && i < 2*width/3) {
			ci = 7*width/12;
			cj = height/2;
		}
		if (i >= 2*width/3 && i < 5*width/6) {
			ci = 3*width/4;
			cj = height/2;
		}
		if (i >= 5*width/6) {
			ci = 11*width/12;
			cj = height/2;
		}

		d2 = (i+0.5-ci)*(i+0.5-ci)+(j+0.5-cj)*(j+0.5-cj);
		r2 = (height/2)*(height/2);
		a  = 1.0/((1+d2/r2)*sqrt(1.0*(1+d2/r2)));
		return a;
	}

  //======  format4:  Cubic with 1*6 faces  =======  
	else if (form == 4) {
		int ci, cj,r2;
    double d2;

		if (j < height/6 ) {
		    ci = width/2;
			cj = height/12;
		}
		if (j >= height/6 && j < height/3) {
		    ci = width/2;
			cj = height/4;
		}
		if (j >= height/3 &&j < height/2) {
			ci = width/2;
			cj = 5*height/12;
		}
		if (j >= height/2 && j < 2*height/3) {
			ci = width/2;
			cj = 7*height/12;
		}
		if (j >= 2*height/3 && j < 5*height/6) {
			ci = width/2;
			cj = 3*height/4;
		}
		if (j >= 5*height/6) {
			ci = width/2;
			cj = 11*height/12;
		}

		d2 = (i+0.5-ci)*(i+0.5-ci)+(j+0.5-cj)*(j+0.5-cj);
		r2 = (width/2)*(width/2);
		a  = 1.0/((1+d2/r2)*sqrt(1.0*(1+d2/r2)));
		return a;
	}

  //======  format5:  Cubic with T type   =======  
  // 1 1 * * * * * *
  // 1 1 * * * * * *
  // 2 2 3 3 4 4 5 5
  // 2 2 3 3 4 4 5 5
  // 6 6 * * * * * *
  // 6 6 * * * * * *
  
	else if (form == 5) {
    int ci, cj,r2;
    double d2;

		if (i < width/4 && j < height/3 ) {
			ci = width/8;
			cj = height/6;
		}
		else if (i < width/4 && j >= height/3 && j < 2*height/3) {
			ci = width/8;
			cj = height/2;
		}
		else if (i < width/4 &&j >= 2*height/3) {
			ci = width/8;
			cj = 5*height/6;
		}
		else if (i >= width/4 && i < width/2 && j >= height/3 && j < 2*height/3) {
			ci = 3*width/8;
			cj = height/2;
		}
		else if (i >= width/2 && i < 3*width/4 && j >= height/3 && j < 2*height/3) {
			ci = 5*width/8;
			cj = height/2;
		}
		else if (i > 3*width/4 && j >= height/3 && j < 2*height/3) {
			ci = 7*width/8;
			cj = height/2;
		}
		else {
			return 0;
		}
		d2 = (i+0.5-ci)*(i+0.5-ci)+(j+0.5-cj)*(j+0.5-cj);
		r2 = (width/8)*(width/8);
		a  = 1.0/((1+d2/r2)*sqrt(1.0*(1+d2/r2)));
		return a;
  }

  //======  format6: Icosahedron =======  
  //else if (form==6)
  //{

  //  return a;
  //}
	return 0;
}
