#include "multithreading.h"

/**
 * blur_pixel - blurs a pixel
 * @portion: portion struct
 * @pos_x: position of a pixel at X-axis
 * @pos_y: position of a pixel at Y-axis
 */
void blur_pixel(blur_portion_t const *portion, size_t pos_x, size_t pos_y)
{
	size_t half_kernel = portion->kernel->size / 2;
	size_t start_x = pos_x - half_kernel, start_y = pos_y - half_kernel;
	size_t x = start_x, y = start_y;
	size_t k_x, k_y;
	float r_sum, g_sum, b_sum, k_sum = 0;

	k_x = k_y = r_sum = g_sum = b_sum = 0;
	while (y < start_y + portion->kernel->size)
	{
		r_sum += portion->kernel->matrix[k_x][k_y]
			* portion->img->pixels[portion->img->w * y + x].r;
		g_sum += portion->kernel->matrix[k_x][k_y]
			* portion->img->pixels[portion->img->w * y + x].g;
		b_sum += portion->kernel->matrix[k_x][k_y]
			* portion->img->pixels[portion->img->w * y + x].b;
		k_sum += portion->kernel->matrix[k_x][k_y];
		++k_x;
		++x;
		if (x == start_x + portion->kernel->size)
		{
			x = start_x;
			++y;
			k_x = 0;
			++k_y;
		}
	}
	portion->img_blur->pixels[portion->img->w * pos_y + pos_x].r = r_sum / k_sum;
	portion->img_blur->pixels[portion->img->w * pos_y + pos_x].g = g_sum / k_sum;
	portion->img_blur->pixels[portion->img->w * pos_y + pos_x].b = b_sum / k_sum;
}

/**
 * check_edges - checks for out of bound errors
 * @portion: portion struct
 * @x: position of a pixel at X-axis
 * @y: position of a pixel at Y-axis
 * Return: 0 if out of bound, 1 otherwise
*/
int check_edges(blur_portion_t const *portion, size_t x, size_t y)
{
	size_t half_kernel = portion->kernel->size / 2;

	if (x < half_kernel || y < half_kernel)
		return (0);
	return (1);
}

/**
 * blur_portion - blurs an image using gaussian blur
 * @portion: portion of the image
 */
void blur_portion(blur_portion_t const *portion)
{
	size_t x, y;

	x = portion->x;
	y = portion->y;

	while (y < portion->y + portion->h)
	{
		/* black(portion, portion->img->w * y + x); */
		if (check_edges(portion, x, y))
			blur_pixel(portion, x, y);
		++x;
		if (x == portion->x + portion->w)
		{
			x = portion->x;
			++y;
		}

	}
}
