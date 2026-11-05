#include <stdio.h>
#include <string.h>
#include "mp3_header.h"
#include <strings.h>

int main(int argc, char *argv[])
{
    struct MP3 mp3;

    int ret = validate_cla(argc, argv, &mp3);

    if (ret == VIEW)
    {
        view(&mp3);
    }
    else if (ret == EDIT)
    {
        edit(&mp3, argv , argc);
    }

    return 0;
}
