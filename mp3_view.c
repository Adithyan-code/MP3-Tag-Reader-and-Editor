
#include "mp3_header.h"

void view(struct MP3 *mp3)
{
    mp3->org_mp3_fptr = fopen(mp3->mp3_filename, "rb");

    if (mp3->org_mp3_fptr == NULL)
    {
        printf("Unable to open MP3 file\n");
        return;
    }

    if (fseek(mp3->org_mp3_fptr, 10, SEEK_SET) != 0)
    {
        fclose(mp3->org_mp3_fptr);
        return;
    }

    char tag_name[5] = {0};
    unsigned char content_size_bytes[4];
    unsigned int content_size = 0;

    printf("\n--------- MP3 TAG DETAILS ---------\n");

    for (int i = 0; i < 6; i++)
    {
        if (fread(tag_name, 1, 4, mp3->org_mp3_fptr) != 4)
            break;

        tag_name[4] = '\0';

        if (fread(content_size_bytes, 1, 4, mp3->org_mp3_fptr) != 4)
            break;

        content_size = ((unsigned int)content_size_bytes[0] << 24) |
                       ((unsigned int)content_size_bytes[1] << 16) |
                       ((unsigned int)content_size_bytes[2] << 8) |
                       (unsigned int)content_size_bytes[3];

        if (content_size < 1)
            break;

        if (fseek(mp3->org_mp3_fptr, 3, SEEK_CUR) != 0)
            break;

        tag_to_name(tag_name);

        for (unsigned int j = 0; j < content_size - 1; j++)
        {
            print_tag_data(mp3->org_mp3_fptr);
        }

        printf("\n");
    }

    printf("-----------------------------------\n");

    fclose(mp3->org_mp3_fptr);
    mp3->org_mp3_fptr = NULL;
}

void print_tag_data(FILE *fp)
{
    int ch = getc(fp);

    if (ch != EOF)
        printf("%c", ch);
}
