#include "multithreading.h"

/**
 * check_edges - checks for out of bound errors
 * @portion: portion struct
 * @x: position of a pixel at X-axis
 * @y: position of a pixel at Y-axis
 * Return: 0 if out of bound, 1 otherwise
 */
void check_borders(blur_portion_t const *portion, size_t *start_x,
		   size_t *start_y, size_t *end_x, size_t *end_y,
		   size_t *kx_start, size_t *ky_start, size_t *kx_end)
{
	size_t half_kernel = portion->kernel->size / 2;

	/* Limit from start */
	if (*start_x < half_kernel)
	{
		*kx_start = half_kernel - *start_x;
		*start_x = 0;
	}
	else
	{
		*kx_start = 0;
		*start_x = *start_x - half_kernel;
	}
	if (*start_y < half_kernel)
	{
		*ky_start = half_kernel - *start_y;
		*start_y = 0;
	}
	else
	{
		*ky_start = 0;
		*start_y = *start_y - half_kernel;
	}

	/* Limit from end */
	if (*start_x + portion->kernel->size >= portion->img->w)
	{
		*kx_end = portion->img->w - *start_x;
		*end_x = portion->img->w;
	}
	else
	{
		*kx_end = portion->kernel->size;
		*end_x = *start_x + portion->kernel->size;
	}

	if (*start_y + portion->kernel->size >= portion->img->h)
	{
		*end_y = portion->img->h;
	}
	else
	{
		*end_y = *start_y + portion->kernel->size;
	}
}

/**
 * blur_pixel - blurs a pixel
 * @portion: portion struct
 * @pos_x: position of a pixel at X-axis
 * @pos_y: position of a pixel at Y-axis
 */
void blur_pixel(blur_portion_t const *portion, size_t pos_x, size_t pos_y)
{
	size_t start_x = pos_x, start_y = pos_y, end_x, end_y;
	size_t x, y;
	size_t k_x, k_y, kx_start, ky_start, kx_end;
	float r_sum, g_sum, b_sum, k_sum = 0;

	k_x = k_y = r_sum = g_sum = b_sum = 0;
	check_borders(portion, &start_x, &start_y, &end_x,
		      &end_y, &kx_start, &ky_start, &kx_end);
	x = start_x, y = start_y;
	k_x = kx_start, k_y = ky_start;
	while (y < end_y)
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
		if (k_x == kx_end)
		{
			k_x = kx_start;
			++k_y;
		}
		if (x == end_x)
		{
			x = start_x;
			++y;
		}
	}
	portion->img_blur->pixels[portion->img->w * pos_y + pos_x].r = r_sum / k_sum;
	portion->img_blur->pixels[portion->img->w * pos_y + pos_x].g = g_sum / k_sum;
	portion->img_blur->pixels[portion->img->w * pos_y + pos_x].b = b_sum / k_sum;
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
		blur_pixel(portion, x, y);
		++x;
		if (x == portion->x + portion->w)
		{
			x = portion->x;
			++y;
		}

	}
}
