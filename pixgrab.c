#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>


int main(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "[ERROR] Bad argument count: %d\n", argc);
        printf("Usage: %s x y\n", argv[0]);
        return EXIT_FAILURE;
    }

    char* xendptr;
    char* yendptr;
    int x = strtol(argv[1], &xendptr, 10);
    int y = strtol(argv[2], &yendptr, 10);

    if (errno == ERANGE
    || xendptr == argv[1] || *xendptr
    || yendptr == argv[2] || *yendptr) {
        fprintf(stderr, "[ERROR] Invalid or out of range arguments: %s %s\n",
                argv[1], argv[2]);
        return EXIT_FAILURE;
    }

    XColor c;
    Display* d = XOpenDisplay(NULL);

    XImage* image = XGetImage(d, XRootWindow(d, XDefaultScreen(d)), x, y, 1, 1, AllPlanes, XYPixmap);
    c.pixel = XGetPixel(image, 0, 0);
    XQueryColor(d, XDefaultColormap(d, XDefaultScreen(d)), &c);
    printf("#%x%x%x\n", c.red / 256, c.green / 256, c.blue / 256);

    return EXIT_SUCCESS;
}
