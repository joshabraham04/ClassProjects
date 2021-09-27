#pragma once
#include <vector>

// TGA File class to hold the image data objects
class TGA_File
{
public:
	// struct for the file header info
	struct Header
	{
		char idLength = 0;
		char colorMapType = 0;
		char imageType = 0;
		short colorMapOrigin = 0;
		short colorMapLength = 0;
		char colorMapDepth = 0;
		short xOrigin = 0;
		short yOrigin = 0;
		short width = 0;
		short height = 0;
		char pixelDepth = 0;
		char imageDescriptor = 0;

	};

	// struct to hold individual pixel RGB data
	struct Pixel
	{
		unsigned char redData = 0;
		unsigned char greenData = 0;
		unsigned char blueData = 0;
	};
};

// clamps the max and min values between 0 and 255, aka prevent data type overflow if a function exceeds range
void OverflowCorrector(float& num);

// function that takes two images and combines the pixels. Takes in 2 arrays and returns a pixel array 
std::vector<TGA_File::Pixel> Multiply(std::vector<TGA_File::Pixel>& topLayer, std::vector<TGA_File::Pixel>& bottomLayer);

// functions that takes two images and subtracts the top layer from the bottom layer
std::vector<TGA_File::Pixel> Subtract(std::vector<TGA_File::Pixel>& topLayer, std::vector<TGA_File::Pixel>& bottomLayer);

// function that takes two standardized image pixels and lightens it, opposite of Multiply
std::vector<TGA_File::Pixel> Screen(std::vector<TGA_File::Pixel>& topLayer, std::vector<TGA_File::Pixel>& bottomLayer);

// function that increases or decreases image's tone based on background darkness
std::vector<TGA_File::Pixel> Overlay(std::vector<TGA_File::Pixel>& topLayer, std::vector<TGA_File::Pixel>& bottomLayer);

// function that increases a specified RGB channel by an integer value; Channel determined by char: Red = 'r', Green = 'g', Blue = 'b'
std::vector<TGA_File::Pixel> Add(std::vector<TGA_File::Pixel>& topLayer, char rgb, int increase);

// function that increases each RGB value by a float parameter; Input 1 to keep the value the same
std::vector<TGA_File::Pixel> Scale(std::vector<TGA_File::Pixel>& topLayer, float redScale, float greenScale, float blueScale);

// function that changes the image's RGB values to the chosen RGB channel; Channel determined by char: Red = 'r', Green = 'g', Blue = 'b'
std::vector<TGA_File::Pixel> ChannelImage(std::vector<TGA_File::Pixel>& topLayer, char rgb);

// function that takes 3 images and combines their Red, Green, and Blue channels respectively; parameters are input by RGB order
std::vector<TGA_File::Pixel> CombineImage(std::vector<TGA_File::Pixel>& redChannel, std::vector<TGA_File::Pixel>& greenChannel, std::vector<TGA_File::Pixel>& blueChannel);

// function that flips the image based off binary I/O; Takes in a pixel array and reverses the order
std::vector<TGA_File::Pixel> FlipImage(std::vector<TGA_File::Pixel>& topLayer);

// OOF this one's really messy. Function that stitches together 4 passed pixel arrays into 4 quadrants
std::vector<TGA_File::Pixel> StitchImage(std::vector<TGA_File::Pixel>& bottomLeft, std::vector<TGA_File::Pixel>& bottomRight, std::vector<TGA_File::Pixel>& topLeft, std::vector<TGA_File::Pixel>& topRight);

void TesterMethod(unsigned char data, int i, char c);