#include <stdio.h>

#include <iostream>
#include <fstream>

#include <sstream>

#include <string>
#include <vector>

#include <tensorflow/c/c_api.h>

#include "Image.cpp"	//class to contain the image, all related data and objects

//#include <opencv2/opencv.hpp>



Image get_image(std::string filename)
{
	Image placeholder;

	std::cout << std::endl << "Reading file from: " << filename << std::endl << std::endl;

	std::string line;
	std::ifstream file;

	file.open(filename.c_str());

	if(file.is_open()) 
	{
		while (std::getline(file, line))
		{
			std::cout << line << std::endl;	

			// neste på lista

			/*
			if line contains "Image size"
				image_size[3] = array of 3 integers from line

			if line contains "Top left" 
				top_left = array of 2 int from line

			if line contains "Original lagel for object"
				label = string after ':'

			if line contains "Bounding box"


			*/
		}
	} else {
		std::cout << "Shit... File "+filename+" did not open" << std::endl;
	}
	return placeholder;

}


int main(int argc, char const *argv[])
{

	//std::string img_annot_src = "~/Machinevision---Self-driving-car/dataset/pascal/VOC2005_1/Annotations/Caltech_cars/";
	std::string img_annot_src = "../../../dataset/pascal/VOC2005_1/Annotations/Caltech_cars/";

	Image testers = get_image(img_annot_src+"image_0001.txt");


	// Layer	Kernel	Stride 	output.shape
	
	// input					(416, 416, 3)
	
	// conv 	3x3		1		(416, 416, 16)
	// max		2x2		2		(208, 208, 16)

	// conv 	3x3		1		(208, 208, 32)
	// max		2x2		2		(104, 104, 32)

	// conv 	3x3		1		(104, 104, 64)
	// max		2x2		2		(52, 52, 64)

	// conv 	3x3		1		(52, 52, 128)
	// max		2x2		2		(26, 26, 128)

	// conv 	3x3		1		(26, 26, 256)
	// max		2x2		2		(13, 13, 256)

	// conv 	3x3		1		(13, 13, 512)
	// max		2x2		1		(13, 13, 512)

	// conv 	3x3		1		(13, 13, 1024)
	// conv 	3x3		1		(13, 13, 1024)

	// conv 	1x1		1		(13, 13, 125)


	return 0;
}










/*

/**/

