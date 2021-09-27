#include "ImageProcessing.h"
#include <vector>
#include <iostream>

void OverflowCorrector(float& num)
{
	if (num < 0)
		num = 0;
	else if (num > 255)
		num = 255;
}

std::vector<TGA_File::Pixel> Multiply(std::vector<TGA_File::Pixel>& topLayer, std::vector<TGA_File::Pixel>& bottomLayer)
{
	TGA_File::Pixel tempPixel;
	std::vector<TGA_File::Pixel> editedImage;

	for (unsigned int i = 0; i < topLayer.size(); i++)
	{
		tempPixel.blueData = (((float) topLayer[i].blueData * (float) bottomLayer[i].blueData) / 255) + 0.5f;
		tempPixel.greenData = (((float)topLayer[i].greenData * (float)bottomLayer[i].greenData) / 255) + 0.5f;
		tempPixel.redData = (((float)topLayer[i].redData * (float)bottomLayer[i].redData) / 255) + 0.5f;

		editedImage.push_back(tempPixel);
	}

	return editedImage;
}

std::vector<TGA_File::Pixel> Subtract(std::vector<TGA_File::Pixel>& topLayer, std::vector<TGA_File::Pixel>& bottomLayer)
{
	TGA_File::Pixel tempPixel;
	std::vector<TGA_File::Pixel> editedImage;
	float bufferValue = 0; // in case the value overflows

	for (unsigned int i = 0; i < topLayer.size(); i++)
	{
		bufferValue = ((float)bottomLayer[i].blueData - (float)topLayer[i].blueData) + 0.5f;
		OverflowCorrector(bufferValue);
		tempPixel.blueData = bufferValue;
		bufferValue = ((float)bottomLayer[i].greenData - (float)topLayer[i].greenData) + 0.5f;
		OverflowCorrector(bufferValue);
		tempPixel.greenData = bufferValue;
		bufferValue = ((float)bottomLayer[i].redData - (float)topLayer[i].redData) + 0.5f;
		OverflowCorrector(bufferValue);
		tempPixel.redData = bufferValue;
		
		editedImage.push_back(tempPixel);
	}

	return editedImage;
}

std::vector<TGA_File::Pixel> Screen(std::vector<TGA_File::Pixel>& topLayer, std::vector<TGA_File::Pixel>& bottomLayer)
{
	TGA_File::Pixel tempPixel;
	std::vector<TGA_File::Pixel> editedImage;
	float bufferValue = 0; // in case the value overflow

	for (unsigned int i = 0; i < topLayer.size(); i++)
	{
		// standardizes the RGB data and performs calculations: Image = 1-[(1-topLayer)*(1-bottomLayer)], then reconverts to float
		bufferValue = ((1 - ((1 - ((float)topLayer[i].blueData / 255)) * (1 - ((float)bottomLayer[i].blueData / 255)))) * 255) + 0.5f;
		OverflowCorrector(bufferValue);
		tempPixel.blueData = bufferValue;
		bufferValue = ((1 - ((1 - ((float)topLayer[i].greenData / 255)) * (1 - ((float)bottomLayer[i].greenData / 255)))) * 255) + 0.5f;
		OverflowCorrector(bufferValue);
		tempPixel.greenData = bufferValue;
		bufferValue = ((1 - ((1 - ((float)topLayer[i].redData / 255)) * (1 - ((float)bottomLayer[i].redData / 255)))) * 255) + 0.5f;
		OverflowCorrector(bufferValue);
		tempPixel.redData = bufferValue;

		editedImage.push_back(tempPixel);
	}

	return editedImage;
}

std::vector<TGA_File::Pixel> Overlay(std::vector<TGA_File::Pixel>& topLayer, std::vector<TGA_File::Pixel>& bottomLayer)
{
	TGA_File::Pixel tempPixel;
	std::vector<TGA_File::Pixel> editedImage;
	float bufferValue = 0; // in case the value overflow
	float backgroundTone = 0; // pixel's RGB percentage to compare against 50% gray value (RGB: 127-127-127; %: 0.498)

	for (unsigned int i = 0; i < topLayer.size(); i++)
	{
		backgroundTone = ((float)bottomLayer[i].blueData + (float)bottomLayer[i].greenData + (float)bottomLayer[i].redData) / (255 * 3);
		
		// if the pixel is lighter than 50% Gray
		if (backgroundTone <= 0.5)
		{
			// Implements Multiply function
			tempPixel.blueData = ((((float)topLayer[i].blueData * (float)bottomLayer[i].blueData) / 255) * 2) + 0.5f;
			tempPixel.greenData = ((((float)topLayer[i].greenData * (float)bottomLayer[i].greenData) / 255) * 2) + 0.5f;
			tempPixel.redData = ((((float)topLayer[i].redData * (float)bottomLayer[i].redData) / 255) * 2) + 0.5f;
		}
		// if the pixel is darker than 50% Gray
		else if (backgroundTone > 0.5)
		{
			// Implements Screen function using standardized values, then reconverted to float
			bufferValue = ((1 - (2 * ((1 - (((float)topLayer[i].blueData) / 255)) * (1 - (((float)bottomLayer[i].blueData) / 255))))) * 255) + 0.5f;
			OverflowCorrector(bufferValue);
			tempPixel.blueData = bufferValue;
			bufferValue = ((1 - (2 * ((1 - (((float)topLayer[i].greenData) / 255)) * (1 - (((float)bottomLayer[i].greenData) / 255))))) * 255) + 0.5f;
			OverflowCorrector(bufferValue);
			tempPixel.greenData = bufferValue;
			bufferValue = ((1 - (2 * ((1 - (((float)topLayer[i].redData) / 255)) * (1 - (((float)bottomLayer[i].redData) / 255))))) * 255) + 0.5f;
			OverflowCorrector(bufferValue);
			tempPixel.redData = bufferValue;
		}

		editedImage.push_back(tempPixel);
	}

	return editedImage;
}

std::vector<TGA_File::Pixel> Add(std::vector<TGA_File::Pixel>& topLayer, char rgb, int increase)
{
	TGA_File::Pixel tempPixel;
	std::vector<TGA_File::Pixel> editedImage;
	float bufferValue = 0; // in case the value overflows

	// Increase the Red values
	if (rgb == 'r')
	{
		for (unsigned int i = 0; i < topLayer.size(); i++)
		{
			bufferValue = (float)topLayer[i].redData + increase;
			OverflowCorrector(bufferValue);
			tempPixel.redData = bufferValue;
			tempPixel.greenData = topLayer[i].greenData;
			tempPixel.blueData = topLayer[i].blueData;

			editedImage.push_back(tempPixel);
		}
	}

	// Incease the Green values
	else if (rgb == 'g')
	{
		for (unsigned int i = 0; i < topLayer.size(); i++)
		{
			bufferValue = (float)topLayer[i].greenData + increase;
			OverflowCorrector(bufferValue);
			tempPixel.greenData = bufferValue;
			tempPixel.blueData = topLayer[i].blueData;
			tempPixel.redData = topLayer[i].redData;

			editedImage.push_back(tempPixel);
		}
	}

	// Increase the Blue values
	else if (rgb == 'b')
	{
		for (unsigned int i = 0; i < topLayer.size(); i++)
		{
			bufferValue = (float)topLayer[i].blueData + increase;
			OverflowCorrector(bufferValue);
			tempPixel.blueData = bufferValue;
			tempPixel.redData = topLayer[i].redData;
			tempPixel.greenData = topLayer[i].greenData;

			editedImage.push_back(tempPixel);
		}
	}

	return editedImage;
}

std::vector<TGA_File::Pixel> Scale(std::vector<TGA_File::Pixel>& topLayer, float redScale, float greenScale, float blueScale)
{
	TGA_File::Pixel tempPixel;
	std::vector<TGA_File::Pixel> editedImage;
	float bufferValue = 0; // in case the value overflows

	// multiply each RGB value by its scale, catch overflows
	for (unsigned int i = 0; i < topLayer.size(); i++)
	{
		bufferValue = (float)topLayer[i].redData * redScale;
		OverflowCorrector(bufferValue);
		tempPixel.redData = bufferValue;

		bufferValue = (float)topLayer[i].greenData * greenScale;
		OverflowCorrector(bufferValue);
		tempPixel.greenData = bufferValue;

		bufferValue = (float)topLayer[i].blueData * blueScale;
		OverflowCorrector(bufferValue);
		tempPixel.blueData = bufferValue;

		editedImage.push_back(tempPixel);
	}

	return editedImage;
}

std::vector<TGA_File::Pixel> ChannelImage(std::vector<TGA_File::Pixel>& topLayer, char rgb)
{
	TGA_File::Pixel tempPixel;
	std::vector<TGA_File::Pixel> editedImage;

	if (rgb == 'r')
	{
		// all Pixel RGB assigned to the Red value
		for (unsigned int i = 0; i < topLayer.size(); i++)
		{
			tempPixel.redData = topLayer[i].redData;
			tempPixel.greenData = topLayer[i].redData;
			tempPixel.blueData = topLayer[i].redData;

			editedImage.push_back(tempPixel);
		}
	}
	else if (rgb == 'g')
	{
		// all Pixel RGB assigned to the Green value
		for (unsigned int i = 0; i < topLayer.size(); i++)
		{
			tempPixel.redData = topLayer[i].greenData;
			tempPixel.greenData = topLayer[i].greenData;
			tempPixel.blueData = topLayer[i].greenData;

			editedImage.push_back(tempPixel);
		}
	}
	else if (rgb == 'b')
	{
		// all Pixel RGB assigned to the Blue value
		for (unsigned int i = 0; i < topLayer.size(); i++)
		{
			tempPixel.redData = topLayer[i].blueData;
			tempPixel.greenData = topLayer[i].blueData;
			tempPixel.blueData = topLayer[i].blueData;

			editedImage.push_back(tempPixel);
		}
	}

	return editedImage;
}

std::vector<TGA_File::Pixel> CombineImage(std::vector<TGA_File::Pixel>& redChannel, std::vector<TGA_File::Pixel>& greenChannel, std::vector<TGA_File::Pixel>& blueChannel)
{
	TGA_File::Pixel tempPixel;
	std::vector<TGA_File::Pixel> editedImage;

	// Takes each image's pixel data and assigns it to the tempPixel in RGB order respectively
	for (unsigned int i = 0; i < redChannel.size(); i++)
	{
		tempPixel.redData = redChannel[i].redData;
		tempPixel.greenData = greenChannel[i].greenData;
		tempPixel.blueData = blueChannel[i].blueData;

		editedImage.push_back(tempPixel);
	}

	return editedImage;
}

std::vector<TGA_File::Pixel> FlipImage(std::vector<TGA_File::Pixel>& topLayer)
{
	TGA_File::Pixel tempPixel;
	std::vector<TGA_File::Pixel> editedImage;
	int lastIndex = topLayer.size()-1;
	
	// Start at the end of the referenced image array and reverse it into another image array
	for (int i = lastIndex; i > -1; i--)
	{
		tempPixel.redData = topLayer[i].redData;
		tempPixel.greenData = topLayer[i].greenData;
		tempPixel.blueData = topLayer[i].blueData;

		editedImage.push_back(tempPixel);
	}

	return editedImage;
}

std::vector<TGA_File::Pixel> StitchImage(std::vector<TGA_File::Pixel>& bottomLeft, std::vector<TGA_File::Pixel>& bottomRight, std::vector<TGA_File::Pixel>& topLeft, std::vector<TGA_File::Pixel>& topRight)
{
	TGA_File::Pixel tempPixel;
	std::vector<TGA_File::Pixel> editedImage;
	unsigned int index = 0;
	unsigned int row = 0;

	for (unsigned int i = 0; i < 512; i++)
	{
		index = (i * 512);
		row = (i + 1) * 512;

		for (unsigned int j = index; j < row; j++)
		{
			//std::cout << "Quadrant A: " << index << std::endl;
			//std::cout << "Starting point: " << j << std::endl;
			//std::cout << "Red Pixel " << j << " is: " << bottomLeft[j].redData;
			tempPixel.redData = bottomLeft[j].redData;
			tempPixel.greenData = bottomLeft[j].greenData;
			tempPixel.blueData = bottomLeft[j].blueData;

			editedImage.push_back(tempPixel);
		}

		for (unsigned int k = index; k < row; k++)
		{
			tempPixel.redData = bottomRight[k].redData;
			tempPixel.greenData = bottomRight[k].greenData;
			tempPixel.blueData = bottomRight[k].blueData;

			editedImage.push_back(tempPixel);
		}
	}
	for (unsigned int i = 0; i < 512; i++)
	{
		index = (i * 512);

		for (unsigned int l = index; l < row; l++)
		{
			tempPixel.redData = topLeft[l].redData;
			tempPixel.greenData = topLeft[l].greenData;
			tempPixel.blueData = topLeft[l].blueData;

			editedImage.push_back(tempPixel);
		}

		for (unsigned int m = index; m < row; m++)
		{
			tempPixel.redData = topRight[m].redData;
			tempPixel.greenData = topRight[m].greenData;
			tempPixel.blueData = topRight[m].blueData;

			editedImage.push_back(tempPixel);
		}
	}

	return editedImage;
}

void TesterMethod(unsigned char data, int i, char c)
{
	if ((int)data < 0 || (int)data > 255) 
	{
		std::cout << "Invalid data point at index: " << i << ", Quadrant: " << c << std::endl;
	}
}