// Note the weird thing is that the colors are normalized based on the min/max 
// derivatives values that actually occur.
// This seems incorrect, but using the real derivative values makes the changes
// tiny and subject to awful compression artifacts.

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_HDR
#define STBI_NO_GIF
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void add_margin(const unsigned char* src, unsigned char* dst, int cols, int rows, int comp)
{
	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < cols; ++c)
		{
			int src_index = (c + r * cols) * comp;	
			int dst_index = ((c + 1) + (r + 1) * (cols + 2)) * comp; 

			dst[dst_index] = src[src_index];

			for (int j = -1; j <= 1; ++j)
			{
				for (int i = -1; i <= 1; ++i)
				{
					if (i == 0 && j == 0) continue;

					int border_index = ((c + i + 1) + (r + j + 1) * (cols + 2)) * comp; 

					if (c + i == -1 || c + i == cols ||
							r + j == -1 || r + j == rows)
					{
						dst[border_index] = src[src_index];
					}
				}
			} 
		}
	}

}

int main(int argc, const char* argv[])
{ 
	if (argc < 3)
	{
		fprintf(stderr, "derivative [IN] [OUT].png");
		return 1;
	}

	int cols, rows;
	unsigned char* height_data = stbi_load(argv[1], &cols, &rows, NULL, 1);

	if (!height_data)
	{
		fprintf(stderr, "failed to load data");
		return 1;
	}

	unsigned char* height = malloc(sizeof(unsigned char) * (cols + 2) * (rows + 2));
	add_margin(height_data, height, cols, rows, 1);


	const int comp = 3;
	float* data = malloc(sizeof(float) * cols * rows * comp);

	// https://en.wikipedia.org/wiki/Sobel_operator
	float kernel_x[3][3] = {
		{-1.0, 0.0, 1.0},
		{-2.0, 0.0, 2.0},
		{-1.0, 0.0, 1.0}
	};

	float kernel_y[3][3] = {
		{-1.0, -2.0, -1.0},
		{0.0, 0.0, 0.0},
		{1.0, 2.0, 1.0}
	};


	float max_derive = 0.0;
	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < cols; ++c)
		{
			int row_len = cols + 2;
			int src_index = (c + 1) + (r + 1) * row_len;

			float dx = 0.0;
			float dy = 0.0;

			for (int j = 0; j < 3; ++j)
			{
				for (int i = 0; i < 3; ++i)
				{
					int index = src_index + (i - 1) + (j - 1) * row_len;
				    float p = (height[index] / 256.0) * 2.0 - 1.0;
				    dx += p * kernel_x[j][i];
				    dy += p * kernel_y[j][i];
				}
			}

			int dst_index = (c + r * cols) * comp;
			data[dst_index] = dx;
			data[dst_index + 1] = dy;
			data[dst_index + 2] = 0.0;

			if (fabs(dx) > max_derive) {
				max_derive = fabs(dx);			
			}
			if (fabs(dy) > max_derive) {
				max_derive = fabs(dy);			
			}
		}
	}

	unsigned char* pixels = malloc(sizeof(unsigned char) * cols * rows * comp);
	size_t len = rows * cols * comp;
	for (int i = 0; i < len; ++i)
	{
		if (i % 3 == 2) {
			pixels[i] = 0;
		} else {
			pixels[i] = (unsigned char)( ((data[i] / max_derive) * 0.5 + 0.5) * 256.0);
		}
	}

	stbi_write_png(argv[2], cols, rows, comp, pixels, 0);
	return 0;
}

