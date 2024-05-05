#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "utils.h"
#include "pixel.h"


#define MAX_ARGC 1024


typedef struct {
    const char* name;
    const char* shortn;
    const char* desc;
    const char* usage;
    int (*func)(int* argc, char*** argv);
} Command;


int cmd_help_func(int *argc, char*** argv);
int cmd_average_func(int *argc, char*** argv);
int cmd_mcommon_func(int *argc, char*** argv);
const Command cmds[] = {
    { .name = "help", .shortn = "h",
      .desc = "Prints the list of commands and their usage.",
      .usage = "help [command]",
      .func = cmd_help_func
    },
    { .name = "average", .shortn = "a",
      .desc = "Computes the average pixel value from the selection.",
      .usage = "average <x1> <y1> [x2] [y2]... [xn] [yn]...",
      .func = cmd_average_func
    },
    { .name = "mcommon", .shortn = "mc",
      .desc = "Computes the most common pixel from the selection.",
      .usage = "mcommon <x1> <y1> [x2] [y2]... [xn] [yn]...",
      .func = cmd_mcommon_func
    },
};
const size_t cmds_count = sizeof(cmds) / sizeof(*cmds);


int main(int argc, char** argv)
{
    if (argc > MAX_ARGC) {
        fprintf(stderr, "[ERROR] Too many arguments.\n");
        return EXIT_FAILURE;
    }
    shift(&argc, &argv);  // skip program name

    const char* cmd = shift(&argc, &argv);

    for (size_t i = 0; i < cmds_count; ++i) {
        if (!strcmp(cmd, cmds[i].name) || !strcmp(cmd, cmds[i].shortn)) {
            return cmds[i].func(&argc, &argv);
        }
    }

    fprintf(stderr, "[ERROR] Command not found: %s\n", cmd);
    printf("Type `pixgrab help` to list all available commands.\n");
    return EXIT_FAILURE;
}


int cmd_help_func(int *argc, char*** argv)
{
    const char* cmd = shift(argc, argv);

    if (!cmd) {
        printf("This program can be used to process pixels from a selection "
               "from the current screen, and output the result to stdout.\n\n");
        printf("I'm currently using this with my system bar made with `eww` "
               "such that it self updates its color based on what is "
               "below it, similar to MacOS's behaviour.\n\n");
        printf("Usage: pixgrab <subcommand> [ARGS]...\n");
        printf("Subcommands:\n");

        for (size_t i = 0; i < cmds_count; ++i) {
            printf("\t%s\n" "\t\t-> %s\n" "\t\t-> Usage: %s\n",
                   cmds[i].name, cmds[i].desc, cmds[i].usage);
        }
        return EXIT_SUCCESS;
    }

    size_t matched_cmd_count = 0;
    for (size_t i = 0; i < cmds_count; ++i) {
        if (!strcmp(cmd, cmds[i].name) || !strcmp(cmd, cmds[i].shortn)) {
            printf("%s\nUsage: %s\n", cmds[i].desc, cmds[i].usage);
            matched_cmd_count++;
        }
    }

    if (!matched_cmd_count) {
        printf("Invalid command: %s\nAvailable commands:\n", cmd);
        for (size_t i = 0; i < cmds_count; ++i) {
            printf("\t - %s\n", cmds[i].name);
        }
        printf("Type `pixgrab help` for usage\n");
    }

    return EXIT_SUCCESS;
}

int cmd_average_func(int *argc, char*** argv)
{
    Pixel pixels[MAX_ARGC];
    size_t pixel_count = 0;
    if (pixels_from_screen_coords(argc, argv, pixels, &pixel_count)
        != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    Pixel average;
    if (pixel_array_average(pixels, pixel_count, &average) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    printf("#%02x%02x%02x\n", average.r, average.g, average.b);

    return EXIT_SUCCESS;
}

int cmd_mcommon_func(int *argc, char*** argv)
{
    Pixel pixels[MAX_ARGC];
    size_t pixel_count = 0;
    if (pixels_from_screen_coords(argc, argv, pixels, &pixel_count)
        != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    Pixel mcommon;
    if (pixel_array_most_common(pixels, pixel_count, &mcommon)
        != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    printf("#%02x%02x%02x\n", mcommon.r, mcommon.g, mcommon.b);

    return EXIT_SUCCESS;
}

