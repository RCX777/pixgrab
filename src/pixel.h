#pragma once

#include <stddef.h>


typedef struct {
    unsigned char r, g, b;
} Pixel;


/*
 * Returns a `Pixel` struct from the data found at the `x` and `y` coordinates
 * on the screen, using the `libX11` API.
 *
 * TODO: Handle bad inputs in a better way. Currently, `libX11` will print an
 * error by itself and crash the program.
 */
Pixel pixel_from_screen_coord(int x, int y);

/*
 * Shifts through the arguments, computing each `Pixel` value for each `x` and
 * `y` pair that was parsed, and stores the results into `arr`, and their
 * count into `len`.
 *
 * Returns `EXIT_SUCCESS` in case of success, else `EXIT_FAILURE`.
 *
 * Notes:
 * - This function does not allocate memory for `arr`, it
 * is the caller's responsability to do so.
 * - Initial `argc` and `argv` values are lost after the function returns.
 */
int pixels_from_screen_coords(int* argc, char*** argv, Pixel* arr, size_t* len);

/*
 * Stores the average pixel value of the input array into the `average`
 * parameter.
 *
 * Returns `EXIT_SUCCESS` in case of success, else `EXIT_FAILURE`.
 */ 
int pixel_array_average(const Pixel* arr, size_t len, Pixel* average);

/*
 * Stores the most common pixel value of the input array into the `average`
 * parameter.
 *
 * TODO: This function is very inefficient, and must be used for arrays
 * with at most 1024 pixels. This should be improved in the future
 *
 * Returns `EXIT_SUCCESS` in case of success, else `EXIT_FAILURE`.
 */
int pixel_array_most_common(const Pixel* arr, size_t len, Pixel* mcommon);

