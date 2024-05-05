#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "utils.h"
#include "pixel.h"


Pixel pixel_from_screen_coord(int x, int y)
{
    XColor c;
    Display* d = XOpenDisplay(NULL);
    XImage* image = XGetImage(d, XRootWindow(d, XDefaultScreen(d)),
                              x, y, 1, 1, AllPlanes, XYPixmap);
    c.pixel = XGetPixel(image, 0, 0);
    XQueryColor(d, XDefaultColormap(d, XDefaultScreen(d)), &c);
    return (Pixel) {.r = c.red   / 256,
                    .g = c.green / 256,
                    .b = c.blue  / 256};
}

int pixels_from_screen_coords(int* argc, char*** argv, Pixel* arr, size_t* len)
{
    if (*argc < 2) {
        fprintf(stderr, "[ERROR] Empty selections are not valid.\n");
        return EXIT_FAILURE;
    }

    while (*argc) {
        const char* xarg = shift(argc, argv);
        if (*argc == 0) {
            fprintf(stderr, "[ERROR] Missing `y` value of last pixel.\n");
            return EXIT_FAILURE;
        }
        const char* yarg = shift(argc, argv);

        char* xendptr = NULL;
        char* yendptr = NULL;
        long x = strtol(xarg, &xendptr, 10);
        long y = strtol(yarg, &yendptr, 10);

        if (errno == ERANGE
        || xendptr == xarg || *xendptr
        || yendptr == yarg || *yendptr) {
            fprintf(stderr, "[ERROR] "
                "Invalid or out of range arguments: %s %s\n", xarg, yarg);
            return EXIT_FAILURE;
        }

        arr[(*len)++] = pixel_from_screen_coord(x, y);
    }

    return EXIT_SUCCESS;
}

int pixel_array_average(const Pixel* arr, size_t len, Pixel* average)
{
    if (!arr || !len) {
        fprintf(stderr, "[ERROR] Cannot calculate "
                "the average pixel color value of an empty array.\n");
        return EXIT_FAILURE;
    }

    size_t r = 0, g = 0, b = 0;
    for (size_t i = 0; i < len; ++ i) {
        r += arr[i].r;
        g += arr[i].g;
        b += arr[i].b;
    }
    *average = (Pixel) { r / len, g / len, b / len };
    return EXIT_SUCCESS;
}

#define _PIXEL_ARRAY_MC_MAXLEN 1024
int pixel_array_most_common(const Pixel* arr, size_t len, Pixel* mcommon)
{
    if (!arr || !len) {
        fprintf(stderr, "[ERROR] Cannot calculate "
                "the most common pixel color value of an empty array.\n");
        return EXIT_FAILURE;
    }

    if (len >= _PIXEL_ARRAY_MC_MAXLEN) {
        fprintf(stderr, "[ERROR] Cannot calculate "
                "the most common pixel color value of an array with "
                "more than %d elements\n", _PIXEL_ARRAY_MC_MAXLEN);
        return EXIT_FAILURE;
    }

    Pixel  freqPixels[_PIXEL_ARRAY_MC_MAXLEN] = {};
    size_t freqCounts[_PIXEL_ARRAY_MC_MAXLEN] = {};
    size_t freqLen = 0;

    for (size_t i = 0; i < len; ++i) {
        size_t j;
        for (j = 0; j < freqLen; ++j) {
            if (arr[i].r == freqPixels[j].r
            &&  arr[i].g == freqPixels[j].g
            &&  arr[i].b == freqPixels[j].b) {
                freqCounts[j]++;
                break;
            }
        }
        if (j == freqLen) {
            freqPixels[freqLen] = arr[i];
            freqCounts[freqLen] = 1;
            freqLen++;
        }
    }

    size_t maxCount = 0;
    size_t maxId    = -1;

    for (size_t i = 0; i < freqLen; ++i) {
        if (freqCounts[i] > maxCount) {
            maxCount = freqCounts[i];
            maxId = i;
        }
    }

    *mcommon = freqPixels[maxId];

    return EXIT_SUCCESS;
}

