#include "RasterSurface.h"
#include <random>
#include <Windows.h>


#define REZ_X 500
#define REZ_Y 500

void draw(unsigned int x, unsigned int y, unsigned int color);
void clearBuffer(unsigned int color);
unsigned int convertCoor(unsigned int x, unsigned int y, unsigned int width = REZ_Y);
void bresenhamAlgorithm(unsigned int startX, unsigned int startY, unsigned int finalX, unsigned int finalY, unsigned int color);
void midpointAlgorithm(unsigned int startX, unsigned int startY, unsigned int finalX, unsigned int finalY, unsigned int color);
int implicitLineEquation(int x1, int x2, int y1, int y2, int midx, int midy);
void parametricAlgorithm(unsigned int startX, unsigned int startY, unsigned int finalX, unsigned int finalY, unsigned int startColor, unsigned int finalColor);
unsigned int interpColors(unsigned int sourceColor, unsigned int desColor, float ratio);


unsigned int myArray[250000];

int main()
{
	RS_Initialize(REZ_X, REZ_Y);


	unsigned int starArray[2250];
	unsigned int seed = rand();
	srand(seed);
	for (unsigned int i = 0; i < 2250; ++i)
		starArray[i] = rand() % 500;

	unsigned int randPoints[12];
	for (unsigned int i = 0; i < 12; ++i)
		randPoints[i] = rand() % 500;


	while (RS_Update(myArray, 250000))
	{
		clearBuffer(0x00000000);

		//stars
		for (unsigned int i = 0; i < 2249; i += 2)
			draw(starArray[i], starArray[i + 1], 0xFFFFFFFF);

		//NO STEEP LINES WORK

		//first line, drawn in red, using bresenham algorithm
		bresenhamAlgorithm(0, 100, 499, 400, 0x00FF0000);

		//Second line, drawn in green, using midpoint algorithm
		midpointAlgorithm(0, 90, 499, 390, 0x0000FF00);

		//Third line, interpolated between magenta and cyan, using parametric algorithm
		parametricAlgorithm(0, 80, 499, 380, 0x00FF00FF, 0x0000FFFF);

		//RandomLines
		bresenhamAlgorithm(randPoints[0], randPoints[1], randPoints[2], randPoints[3], 0x00FF0000);
		
		midpointAlgorithm(randPoints[4], randPoints[5], randPoints[6], randPoints[7], 0x0000FF00);
		
		parametricAlgorithm(randPoints[8], randPoints[9], randPoints[10], randPoints[11], 0x00FF00FF, 0x0000FFFF);

		if (GetAsyncKeyState(VK_SPACE))
		{
			for (unsigned int i = 0; i < 12; ++i)
				randPoints[i] = rand() % 500;
		}
	}

	RS_Shutdown();
	return 0;
}

void bresenhamAlgorithm(unsigned int startX, unsigned int startY, unsigned int finalX, unsigned int finalY, unsigned int color)
{
	unsigned int currY = 0;
	float error = 0;
	float slope = 0;

	unsigned int x1, x2, y1, y2;

	unsigned int deltaX = abs((int)startX - (int)finalX);
	unsigned int deltaY = abs((int)startY - (int)finalY);

	if (startX < finalX)
	{
		x1 = startX;
		x2 = finalX;
		y1 = startY;
		y2 = finalY;
		currY = startY;
	}
	else
	{
		x1 = finalX;
		x2 = startX;
		y2 = startY;
		y1 = finalY;
		currY = finalY;
	}

	//if (deltaX > deltaY)
	//{
	//if (startY < finalY)
		slope = (float)((float)y2 - y1) / (float)((float)x2 - x1);
	//else
	//	slope = (float)((float)startY - finalY) / (float)((float)finalX - startX);

	for (unsigned int i = x1; i <= x2; ++i)
	{
		draw(i, currY, color);
		if (slope > 0)
		{
			while (error > 0.5f)
			{
				currY += 1;
				error -= 1;
				draw(i, currY, color);

			}
		}
		else
		{
			while (error < 0.5f)
			{
				currY -= 1;
				error += 1;
				draw(i, currY, color);
			}
		}
		error += slope;
		if (error > 0.5)
		{
			currY++;
			error--;
		}
		else if (error < -0.5)
		{
			currY--;
			error++;
		}
	}
	/*}
	else
	{
		if (startX < finalX)
			slope = (float)(finalX - startX) / (float)(finalY - startY);
		else
			slope = (float)(finalX - startX) / (float)(startY - finalY);
		for (unsigned int i = y1; i < y2; ++i)
		{
			draw(currX, i, color);
			error += slope;
			if (error > 0.5)
			{
				if (startX < finalX)
					currX += 1;
				else
					currX -= 1;
				error -= 1;
			}
			else if (error < -0.5)
			{
				if (startX < finalX)
					currX += 1;
				else
					currX -= 1;
				error += 1;
			}
		}
	}*/

	//if (startX < finalX)
	//{
	//	if (yCheck)
	//		slope = (float)(finalY - startY) / (float)(finalX - startX);
	//	else
	//		slope = (float)(startY - finalY) / (float)(finalX - startX);
	//
	//
	//	for (unsigned int i = startX; i < finalX; ++i)
	//	{
	//		draw(i, currY, color);
	//		error += slope;
	//		if (error > 0.5)
	//		{
	//			if (yCheck)
	//				currY += 1;
	//			else
	//				currY -= 1;
	//			error -= 1;
	//		}
	//	}
	//}
	//else
	//{
	//	if (yCheck)
	//		slope = (float)(finalY - startY) / (float)(startX - finalX);
	//	else
	//		slope = (float)(startY - finalY) / (float)(startX - finalX);
	//
	//
	//	for (unsigned int i = startX; i > finalX; --i)
	//	{
	//		draw(i, currY, color);
	//		error += slope;
	//		if (error > 0.5)
	//		{
	//			if (yCheck)
	//				currY += 1;
	//			else
	//				currY -= 1;
	//			error -= 1;
	//		}
	//	}
	//}

	draw(startX, startY, 0x00FFFF00);
	draw(finalX, finalY, 0x00FFFF00);
}

void midpointAlgorithm(unsigned int startX, unsigned int startY, unsigned int finalX, unsigned int finalY, unsigned int color)
{

	unsigned int x1 = startX;
	unsigned int x2 = finalX;
	unsigned int y1 = startY;
	unsigned int y2 = finalY;

	unsigned int currY;
	unsigned int currX;
	float midpointX, midpointY;
	float xSlope = ((float)finalY - startY) / ((float)finalX - startX);
	float ySlope = ((float)finalX - startX) / ((float)finalY - startY);
	unsigned int deltaX = abs((int)startX - (int)finalX);
	unsigned int deltaY = abs((int)startY - (int)finalY);


	if (deltaX > deltaY)
	{
		if (startX > finalX)
		{
			x1 = finalX;
			x2 = startX;
			y1 = finalY;
			y2 = startY;
		}
		currY = y1;

		if (xSlope > 0)
		{
			for (unsigned int i = x1; i <= x2; ++i)
			{
				draw(i, currY, color);
				midpointX = (float)i + 1;
				midpointY = (float)currY + 0.5f;
				if (implicitLineEquation(x1, x2, y1, y2, (int)midpointX, (int)midpointY) < 0)
					currY += 1;
			}
		}
		else
		{
			for (unsigned int i = x1; i <= x2; ++i)
			{
				draw(i, currY, color);
				midpointX = (float)i + 1;
				midpointY = (float)currY + 0.5f;
				if (implicitLineEquation(x1, x2, y1, y2, (int)midpointX, (int)midpointY) > 0)
					currY -= 1;
			}
		}
	}

	else if (deltaX < deltaY)
	{
		if (startY > finalY)
		{
			x1 = finalX;
			x2 = startX;
			y1 = finalY;
			y2 = startY;
		}
		currX = x1;

		if (xSlope > 0)
		{
			for (unsigned int i = y1; i < y2; ++i)
			{
				draw(currX, i, color);
				midpointX = (float)currX + 0.5;
				midpointY = (float)i + 1;
				if (implicitLineEquation(x1, x2, y1, y2, (int)midpointX, (int)midpointY) > 0)
					currX += 1;
			}
		}
		else
		{
			for (unsigned int i = y1; i < y2; ++i)
			{
				draw(currX, i, color);
				midpointX = (float)currX + 0.5;
				midpointY = (float)i + 1;
				if (implicitLineEquation(x1, x2, y1, y2, (int)midpointX, (int)midpointY) < 0)
					currX -= 1;
			}
		}
	}
	
	draw(startX, startY, 0x00FFFF00);
	draw(finalX, finalY, 0x00FFFF00);
}

void parametricAlgorithm(unsigned int startX, unsigned int startY, unsigned int finalX, unsigned int finalY, unsigned int startColor, unsigned int finalColor)
{
	unsigned int deltaX = abs((int)startX - (int)finalX);
	unsigned int deltaY = abs((int)startY - (int)finalY);

	unsigned int x1, x2, y1, y2;
	if (startX < finalX)
	{
		x1 = startX;
		x2 = finalX;
	}
	else
	{
		x1 = finalX;
		x2 = startX;
	}
	if (startY < finalY)
	{
		y1 = startY;
		y2 = finalY;
	}
	else
	{
		y1 = finalY;
		y2 = startY;
	}
	//if slope is not steep
	if (deltaX > deltaY)
	{
		for (unsigned int i = x1; i < x2; ++i)
		{
			float ratio = ((float)i - (float)startX) / ((float)finalX - (float)startX);
			int currY = ((int)finalY - (int)startY)*ratio + (int)startY;
			unsigned int colorToDraw = interpColors(startColor, finalColor, ratio);
			draw(i, (unsigned int)floor((double)currY + 0.5), colorToDraw);
		}
	}
	else
	{
		for (unsigned int i = y1; i < y2; ++i)
		{
			float ratio = ((float)i - (float)startY) / ((float)finalY - (float)startY);
			int currX = ((int)finalX - (int)startX)*ratio + (int)startX;
			unsigned int colorToDraw = interpColors(startColor, finalColor, ratio);
			draw((unsigned int)floor((double)currX + 0.5), i, colorToDraw);
		}
	}
	//if slope is steep, try flipping x and y in the loop

	draw(startX, startY, 0x00FFFF00);
	draw(finalX, finalY, 0x00FFFF00);
}




int implicitLineEquation(int x1, int x2, int y1, int y2, int midx, int midy) { return ((y1 - y2)*midx + (x2 - x1)*midy + x1*y2 - y1*x2); }

void draw(unsigned int x, unsigned int y, unsigned int color)
{
	if (x > REZ_X || y > REZ_Y) {}
	else
		myArray[convertCoor(x, y)] = color;
}

void clearBuffer(unsigned int color)
{
	for (unsigned int i = 0; i < (REZ_X * REZ_Y); ++i)
		myArray[i] = color;
}

unsigned int convertCoor(unsigned int x, unsigned int y, unsigned int width) { return((y*width) + x); }

unsigned int interpColors(unsigned int sourceColor, unsigned int desColor, float ratio)
{
	//(b - a)*r + a

	//ARGB
	int r = (sourceColor & 0x00FF0000) >> 16;
	int g = (sourceColor & 0x0000FF00) >> 8;
	int b = (sourceColor & 0x000000FF);

	int newR = (desColor & 0x00FF0000) >> 16;
	int newG = (desColor & 0x0000FF00) >> 8;
	int newB = (desColor & 0x000000FF);

	int finalR = (newR - r) * (ratio)+r;
	int finalG = (newG - g) * (ratio)+g;
	int finalB = (newB - b) * (ratio)+b;

	return((finalR << 16) | (finalG << 8) | finalB);
}