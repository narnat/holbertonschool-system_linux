#include "multithreading.h"

/**
 * create_portions - creates portions array of the image
 * @img_blur: copy of the image which needs to be blurred
 * @img: image which needs to be blurred
 * @kernel: kernel matrix (Gaussian Blur)
 * Return: initialized portions array or NULL if failed
*/
blur_portion_t *create_portions(img_t *img_blur, img_t const *img,
				kernel_t const *kernel)
{
	blur_portion_t *portions;
	int i;
	size_t step, width;

	portions = malloc(sizeof(*portions) * NUM_THREADS);
	if (!portions)
		return (NULL);
	width = img->w / NUM_THREADS;
	for (i = 0, step = 0; i < NUM_THREADS; ++i, step += width)
	{
		portions[i].kernel = kernel;
		portions[i].img = img;
		portions[i].img_blur = img_blur;
		portions[i].h = img->h;
		portions[i].y = 0;
		portions[i].x = step;
		portions[i].w = img->w / NUM_THREADS;
		if (i == NUM_THREADS - 1)
			portions[i].w += img->w % NUM_THREADS;
	}
	return (portions);
}

/**
 * thread_entry - thread entry
 * @param: portion of the img which needs to be blurred
 * Return: NULL
*/
void *thread_entry(void *param)
{
	blur_portion((blur_portion_t *)param);
	pthread_exit(NULL);
}

/**
 * blur_image - blur image using threads
 * @img_blur: copy of the image which needs to be blurred
 * @img: image which needs to be blurred
 * @kernel: kernel matrix (Gaussian Blur)
*/
void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel)
{
	blur_portion_t *portions;
	int i;
	pthread_t tids[NUM_THREADS];

	portions = create_portions(img_blur, img, kernel);
	if (!portions)
		return;
	for (i = 0; i < NUM_THREADS; ++i)
		pthread_create(&tids[i], NULL, &thread_entry, &portions[i]);
	for (i = 0; i < NUM_THREADS; ++i)
		pthread_join(tids[i], NULL);
	free(portions);
}
