#include "ImageProcessing.h"
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

// Helper Function Prototypes 
void SetHeaderData(string fileName, TGA_File::Header& header);
vector<TGA_File::Pixel> StoreImageData(string fileName, int pixelCount, TGA_File::Pixel& pixel);
void WriteImageData(string fileName, TGA_File::Header& header, vector<TGA_File::Pixel>& image);
void ImageComparison(string exampleFile, vector<TGA_File::Pixel>& image, float part);


int main()
{

	TGA_File::Header imageHeader;
	TGA_File::Pixel imagePixel;
	vector<TGA_File::Pixel> image1, image2, image3, image4;
	vector<TGA_File::Pixel> processedImage;
	int pixelCount = 0;

	// PART 1: Multiply layer1 and pattern1
	{
		SetHeaderData("layer1", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image1 = StoreImageData("layer1", pixelCount, imagePixel);

		SetHeaderData("pattern1", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image2 = StoreImageData("pattern1", pixelCount, imagePixel);


		processedImage = Multiply(image1, image2);
		WriteImageData("part1", imageHeader, processedImage);
		ImageComparison("part1", processedImage, 1);
	}
	
	// PART 2: Subtract layer2 and car
	{
		SetHeaderData("layer2", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image1 = StoreImageData("layer2", pixelCount, imagePixel);

		SetHeaderData("car", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image2 = StoreImageData("car", pixelCount, imagePixel);


		processedImage = Subtract(image1, image2);
		WriteImageData("part2", imageHeader, processedImage);
		ImageComparison("part2", processedImage, 2);
	}
	
	// PART 3: Multiply and Screen layer1, pattern2, and text
	{
		SetHeaderData("layer1", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image1 = StoreImageData("layer1", pixelCount, imagePixel);

		SetHeaderData("pattern2", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image2 = StoreImageData("pattern2", pixelCount, imagePixel);

		SetHeaderData("text", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image3 = StoreImageData("text", pixelCount, imagePixel);


		processedImage = Multiply(image1, image2);
		processedImage = Screen(processedImage, image3);
		WriteImageData("part3", imageHeader, processedImage);
		ImageComparison("part3", processedImage, 3);
	}
	
	// PART 4: Multiply and Subtract layer2, circles and pattern2
	{
		SetHeaderData("layer2", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image1 = StoreImageData("layer2", pixelCount, imagePixel);

		SetHeaderData("circles", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image2 = StoreImageData("circles", pixelCount, imagePixel);

		SetHeaderData("pattern2", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image3 = StoreImageData("pattern2", pixelCount, imagePixel);


		processedImage = Multiply(image1, image2);
		processedImage = Subtract(image3, processedImage);
		WriteImageData("part4", imageHeader, processedImage);
		ImageComparison("part4", processedImage, 4);
	}
	
	// PART 5: Overlay layer1 and pattern1
	{
		SetHeaderData("layer1", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image1 = StoreImageData("layer1", pixelCount, imagePixel);

		SetHeaderData("pattern1", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image2 = StoreImageData("pattern1", pixelCount, imagePixel);


		processedImage = Overlay(image1, image2);
		WriteImageData("part5", imageHeader, processedImage);
		ImageComparison("part5", processedImage, 5);
	}
	
	// PART 6: Increase green in car
	{
		SetHeaderData("car", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image1 = StoreImageData("car", pixelCount, imagePixel);


		processedImage = Add(image1, 'g', 200);
		WriteImageData("part6", imageHeader, processedImage);
		ImageComparison("part6", processedImage, 6);
	}
	
	// PART 7: Scale green and red in car
	{
		SetHeaderData("car", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image1 = StoreImageData("car", pixelCount, imagePixel);


		processedImage = Scale(image1, 4, 1, 0);
		WriteImageData("part7", imageHeader, processedImage);
		ImageComparison("part7", processedImage, 7);

	}
	
	// PART 8: Create RGB channel images of car
	{
		SetHeaderData("car", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image1 = StoreImageData("car", pixelCount, imagePixel);


		processedImage = ChannelImage(image1, 'r');
		WriteImageData("part8_r", imageHeader, processedImage);
		ImageComparison("part8_r", processedImage, 8.1);

		processedImage = ChannelImage(image1, 'g');
		WriteImageData("part8_g", imageHeader, processedImage);
		ImageComparison("part8_g", processedImage, 8.2);

		processedImage = ChannelImage(image1, 'b');
		WriteImageData("part8_b", imageHeader, processedImage);
		ImageComparison("part8_b", processedImage, 8.3);

	}
	
	// PART 9: Combine layer_red, layer_green, and layer_blue
	{
		SetHeaderData("layer_red", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image1 = StoreImageData("layer_red", pixelCount, imagePixel);

		SetHeaderData("layer_green", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image2 = StoreImageData("layer_green", pixelCount, imagePixel);

		SetHeaderData("layer_blue", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image3 = StoreImageData("layer_blue", pixelCount, imagePixel);


		processedImage = CombineImage(image1, image2, image3);
		WriteImageData("part9", imageHeader, processedImage);
		ImageComparison("part9", processedImage, 9);

	}
	
	// PART 10: Rotate text2 180 degrees
	{
		SetHeaderData("text2", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image1 = StoreImageData("text2", pixelCount, imagePixel);


		processedImage = FlipImage(image1);
		WriteImageData("part10", imageHeader, processedImage);
		ImageComparison("part10", processedImage, 10);

	}
	
	// EXTRA CREDIT: Stitch car, circles, pattern1, and text1 into a single image
	{
		SetHeaderData("car", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image1 = StoreImageData("car", pixelCount, imagePixel);

		SetHeaderData("circles", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image2 = StoreImageData("circles", pixelCount, imagePixel);

		SetHeaderData("pattern1", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image3 = StoreImageData("pattern1", pixelCount, imagePixel);

		SetHeaderData("text", imageHeader);
		pixelCount = imageHeader.width * imageHeader.height;
		image4 = StoreImageData("text", pixelCount, imagePixel);


		processedImage = StitchImage(image4, image3, image1, image2);
		// redefine the image width and height to match image size
		imageHeader.width = 2 * imageHeader.width;
		imageHeader.height = 2 * imageHeader.height;

		WriteImageData("extracredit", imageHeader, processedImage);
		ImageComparison("extracredit", processedImage, 11);
	}

	return 0;
};

// takes a filename to assign values to the header object passed in the paramters
void SetHeaderData(string fileName, TGA_File::Header& header)
{
	ifstream file("../input/" + fileName + ".tga", ios_base::binary);
	file.seekg(0); // file is always read at the beginning
	file.read(&header.idLength, sizeof(header.idLength));
	file.read(&header.colorMapType, sizeof(header.colorMapType));
	file.read(&header.imageType, sizeof(header.imageType));
	file.read((char*)&header.colorMapOrigin, sizeof(header.colorMapOrigin));
	file.read((char*)&header.colorMapLength, sizeof(header.colorMapLength));
	file.read(&header.colorMapDepth, sizeof(header.colorMapDepth));
	file.read((char*)&header.xOrigin, sizeof(header.xOrigin));
	file.read((char*)&header.yOrigin, sizeof(header.yOrigin));
	file.read((char*)&header.width, sizeof(header.width));
	file.read((char*)&header.height, sizeof(header.height));
	file.read(&header.pixelDepth, sizeof(header.pixelDepth));
	file.read(&header.imageDescriptor, sizeof(header.imageDescriptor));
}
// takes a filename, the number of pixels, and a pixel reference to return an array of pixels for the image data
vector<TGA_File::Pixel> StoreImageData(string fileName, int pixelCount, TGA_File::Pixel& pixel)
{
	vector<TGA_File::Pixel> imageData;

	ifstream file("../input/" + fileName + ".tga", ios_base::binary);
	file.seekg(18, ios_base::beg); // skips to 18 bytes from the beginning, after the header data

	// iterates through and assigns the referenced pixel its corresponding RGB values
	for (int i = 0; i < pixelCount; i++)
	{
		file.read((char*)&pixel.blueData, sizeof(pixel.blueData));
		file.read((char*)&pixel.greenData, sizeof(pixel.greenData));
		file.read((char*)&pixel.redData, sizeof(pixel.redData));

		imageData.push_back(pixel);
	}

	return imageData;
}
// takes a string, header object, and pixel array to output a TGA file in binary
void WriteImageData(string fileName, TGA_File::Header& header, vector<TGA_File::Pixel>& image)
{
	ofstream file("../output/" + fileName + ".tga", ios_base::binary);

	// write the requisite file header data
	file.write(&header.idLength, sizeof(header.idLength));
	file.write(&header.colorMapType, sizeof(header.colorMapType));
	file.write(&header.imageType, sizeof(header.imageType));
	file.write((char*)&header.colorMapOrigin, sizeof(header.colorMapOrigin));
	file.write((char*)&header.colorMapLength, sizeof(header.colorMapLength));
	file.write(&header.colorMapDepth, sizeof(header.colorMapDepth));
	file.write((char*)&header.xOrigin, sizeof(header.xOrigin));
	file.write((char*)&header.yOrigin, sizeof(header.yOrigin));
	file.write((char*)&header.width, sizeof(header.width));
	file.write((char*)&header.height, sizeof(header.height));
	file.write(&header.pixelDepth, sizeof(header.pixelDepth));
	file.write(&header.imageDescriptor, sizeof(header.imageDescriptor));

	// write the RGB for each pixel
	for (unsigned int i = 0; i < image.size(); i++)
	{
		file.write((char*)&image[i].blueData, sizeof(image[i].blueData));
		file.write((char*)&image[i].greenData, sizeof(image[i].greenData));
		file.write((char*)&image[i].redData, sizeof(image[i].redData));
	}

}
// tester functions to see if output and examples match up
void ImageComparison(string exampleFile, vector<TGA_File::Pixel>& image, float part)
{
	bool partAssesment = true;
	TGA_File::Header exampleHeader;
	TGA_File::Pixel examplePixel;
	short width = 0;
	short height = 0;
	int pixelCount = 0;
	unsigned int i = 0;
	vector<TGA_File::Pixel> exampleImage;

	ifstream file("../examples/EXAMPLE_" + exampleFile + ".tga", ios_base::binary);

	file.seekg(12, ios_base::beg);
	file.read((char*)&width, sizeof(width));
	file.read((char*)&height, sizeof(height));
	pixelCount = width * height;

	file.seekg(18, ios_base::beg);
	for (i = 0; i < pixelCount; i++)
	{
		file.read((char*)&examplePixel.blueData, sizeof(examplePixel.blueData));
		file.read((char*)&examplePixel.greenData, sizeof(examplePixel.greenData));
		file.read((char*)&examplePixel.redData, sizeof(examplePixel.redData));

		exampleImage.push_back(examplePixel);
	}

	for (i = 0; i < image.size(); i++)
	{
		if (image[i].blueData != exampleImage[i].blueData || image[i].greenData != exampleImage[i].greenData || image[i].redData != exampleImage[i].redData)
		{
			partAssesment = false;
			break;
		}
	}

	if (partAssesment == true)
		cout << "Part #" << part << " .......... Success" << endl;
	else if (partAssesment == false)
		cout << "Part #" << part << " .......... Failure || Occurred at Pixel: " << i << endl;
}