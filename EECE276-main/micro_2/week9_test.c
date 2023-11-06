#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#include <math.h>
#include <stdio.h>
#include <float.h>


void downsampling (unsigned char* in, int const height, int const width, int const channel, unsigned char* out); 
void upsampling (unsigned char* in, int const height, int const width, int const channel, unsigned char* out); 
void bilinear (unsigned char* in, int const height, int const width, int const channel, unsigned char* out); 
double imagePSNR(unsigned char* frame1, unsigned char* frame2, unsigned int size);

int main()
{
 
	int height;
	int width;
	int channel;

    	float PSNR_up,PSNR_bilinear;

	char command;

 	printf("Take a picture? (y/n)\n");
 	scanf("%c", &command);

 	if(command == 'n')
 		exit(1);


 	printf("Cheeze !\r\n");
	system("libcamera-still --width 960 --height 720 -o src.bmp");

 	unsigned char* imgIn = stbi_load("src.bmp", &width, &height, &channel, 3);
	unsigned char* imgOut_down0 = (unsigned char*) malloc (sizeof(unsigned char)*3*320*240);
	unsigned char* imgOut_up0 = (unsigned char*) malloc (sizeof(unsigned char)*3*960*720);
	unsigned char* imgOut_up1 = (unsigned char*) malloc (sizeof(unsigned char)*3*960*720);
 	
	downsampling(imgIn, height, width, channel, imgOut_down0);
	upsampling(imgOut_down0, height, width, channel, imgOut_up0);
	bilinear(imgOut_down0, height, width, channel, imgOut_up1);
	PSNR_up = imagePSNR(imgIn,imgOut_up0, 3*960*720);
	PSNR_bilinear = imagePSNR(imgIn,imgOut_up1, 3*960*720);
    	printf("======PSNR RESULT======\n");
	printf("%f, %f\n",PSNR_up,PSNR_bilinear);
	printf("=======================\n");
				
	stbi_write_bmp("image_down.bmp", width/3, height/3, channel, imgOut_down0);
	stbi_write_bmp("image_up.bmp", width, height, channel, imgOut_up0);
	stbi_write_bmp("image_bi.bmp", width, height, channel, imgOut_up1);
	
	stbi_image_free(imgIn);
	free(imgOut_down0);
	free(imgOut_up0);
	free(imgOut_up1);
 

	return 0;
}

void downsampling (unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {  
	for (int i = 0; i < (height / 3); i++) {
		for (int j = 0; j < (width / 3); j++) {
			for (int k = 0; k<channel; k++) {
				out[i * width * channel + j * channel + k] = in[3 * i * width * channel + 3 * j * channel + k];
			}
		}
	 }
}

void upsampling (unsigned char* in, int const height, int const width, int const channel, unsigned char* out) { 
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = 0; k < channel; k++) {
				for (int l = 0; l < 3; l++) {
					for (int m = 0; m < 3; m++) {
						out[(3 * i+l) * width * channel + (3 * j+m) * channel + k] = in[i * width * channel + j * channel + k];
						}
					}
				}
			}
		}
}

void bilinear (unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {  
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = 0; k < channel; k++) {
				out[3 * i * width * channel + 3 * j * channel + k] = in[i * width * channel + j * channel + k];
			}
		}
	}

	for (int i = 0; i < (height-1); i++) {
		for (int j = 0; j < width; j++) {
			for (int k = 0; k < channel; k++) {
				out[(3 * i+1) * width * channel + 3 * j * channel + k]
					= (2*in[i * width * channel + j * channel + k]+ in[(i+1) * width * channel + j * channel + k])/3;

				out[(3 * i + 2) * width * channel + 3 * j * channel + k]
					= (in[i * width * channel + j * channel + k] + 2*in[(i + 1) * width * channel + j * channel + k])/3;
			}
		}
	}

	for (int i = 0; i < (3*height-2); i++) {
		for (int j = 0; j < (width-1); j++) {
			for (int k = 0; k < channel; k++) {
				out[i * width * channel + (3 * j+1) * channel + k] 
					= (2*out[i * width * channel + 3 * j * channel + k]+ out[i * width * channel + 3 * (j+1) * channel + k])/3;
				out[i * width * channel + (3 * j + 2) * channel + k]
					= (out[i * width * channel + 3 * j * channel + k] + 2*out[i * width * channel + 3 * (j + 1) * channel + k]) / 3;
			}
		}
	}

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < (3*width - 2); j++) {
			for (int k = 0; k < channel; k++) {
				out[(3 * height - 2 + i) * width * channel + j * channel + k]
					= out[(3 * height - 3) * width * channel + j * channel + k];
			}
		}
	}

	for (int i = 0; i < (3 * height - 2); i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < channel; k++) {
				out[i * width * channel + (3*width-2+j) * channel + k]
					= out[i * width * channel + (3*width-3) * channel + k];
			}
		}
	}

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < channel; k++) {
				out[(3*height-2+i)* width * channel + (3 * width - 2 + j) * channel + k]
					= out[(3*height-3) * width * channel + (3 * width - 3) * channel + k];
			}
		}
	}


}

//Calculates image PSNR value
double imagePSNR(unsigned char* frame1, unsigned char* frame2, unsigned int size){   
	double mse;
	mse = 0;
	for (int i = 0; i < size; i++) {
		mse = mse + (frame1[i] - frame2[i]) * (frame1[i] - frame2[i]);
	}
	mse = mse / size;
	return 20 * log10(255) - 10 * log10(mse);
}
