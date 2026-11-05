
#include "mp3_header.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void edit(struct MP3 *mp3, char *argv[], int argc)
{
    int file_index = (argc == 5) ? 4 : 3;

    mp3->org_mp3_fptr = fopen(argv[file_index], "rb");

    if (mp3->org_mp3_fptr == NULL)
    {
        printf("Unable to open MP3 file\n");
        return;
    }

    mp3->dup_mp3_fptr = fopen("temp.mp3", "wb");

    if (mp3->dup_mp3_fptr == NULL)
    {
        fclose(mp3->org_mp3_fptr);
        return;
    }

    char header[10];

    if (fread(header, 1, 10, mp3->org_mp3_fptr) != 10)
    {
        printf("Unable to read header\n");
        fclose(mp3->org_mp3_fptr);
        fclose(mp3->dup_mp3_fptr);
        remove("temp.mp3");
        return;
    }

    if (fwrite(header, 1, 10, mp3->dup_mp3_fptr) != 10)
    {
        fclose(mp3->org_mp3_fptr);
        fclose(mp3->dup_mp3_fptr);
        remove("temp.mp3");
        return;
    }

    printf("HEADER COPIED!!!\n");

    char choice[5];
    char select[20];

    if (strcmp(argv[2], "-a") == 0)
    {
        strcpy(select, "ARTIST");
        strcpy(choice, "TPE1");
    }
    else if (strcmp(argv[2], "-A") == 0)
    {
        strcpy(select, "ALBUM");
        strcpy(choice, "TALB");
    }
    else if (strcmp(argv[2], "-t") == 0)
    {
        strcpy(select, "TITLE");
        strcpy(choice, "TIT2");
    }
    else if (strcmp(argv[2], "-y") == 0)
    {
        strcpy(select, "YEAR");
        strcpy(choice, "TYER");
    }
    else if (strcmp(argv[2], "-c") == 0)
    {
        strcpy(select, "GENRE");
        strcpy(choice, "TCON");
    }
    else if (strcmp(argv[2], "-C") == 0)
    {
        strcpy(select, "COMMENT");
        strcpy(choice, "COMM");
    }
    else
    {
        printf("Invalid tag option\n");
        fclose(mp3->org_mp3_fptr);
        fclose(mp3->dup_mp3_fptr);
        remove("temp.mp3");
        return;
    }

    char new_con[100] = {0};
    int found = 0;

    for (int i = 0; i < 6; i++)
    {
        char tag[5] = {0};

        if (fread(tag, 1, 4, mp3->org_mp3_fptr) != 4)
        {
            break;
        }

        if (fwrite(tag, 1, 4, mp3->dup_mp3_fptr) != 4)
        {
            fclose(mp3->org_mp3_fptr);
            fclose(mp3->dup_mp3_fptr);
            remove("temp.mp3");
            return;
        }

        if (strcmp(tag, choice) == 0)
        {
            found = 1;

            if (argc == 4)
            {
                printf("Enter the new content: ");

                if (scanf(" %99[^\n]", new_con) != 1)
                {
                    printf("Invalid input\n");
                    fclose(mp3->org_mp3_fptr);
                    fclose(mp3->dup_mp3_fptr);
                    remove("temp.mp3");
                    return;
                }
            }
            else
            {
                if (strlen(argv[3]) >= sizeof(new_con))
                {
                    printf("Content is too long\n");
                    fclose(mp3->org_mp3_fptr);
                    fclose(mp3->dup_mp3_fptr);
                    remove("temp.mp3");
                    return;
                }

                strcpy(new_con, argv[3]);
            }

            int new_len = strlen(new_con) + 1;
            unsigned char size_buffer[4];

            if (fread(size_buffer, 1, 4, mp3->org_mp3_fptr) != 4)
            {
                fclose(mp3->org_mp3_fptr);
                fclose(mp3->dup_mp3_fptr);
                remove("temp.mp3");
                return;
            }

            unsigned int old_len =
                ((unsigned int)size_buffer[0] << 24) |
                ((unsigned int)size_buffer[1] << 16) |
                ((unsigned int)size_buffer[2] << 8) |
                size_buffer[3];

            if (old_len < 1)
            {
                printf("Invalid frame size\n");
                fclose(mp3->org_mp3_fptr);
                fclose(mp3->dup_mp3_fptr);
                remove("temp.mp3");
                return;
            }

            unsigned char new_size[4];

            new_size[0] = (new_len >> 24) & 0xFF;
            new_size[1] = (new_len >> 16) & 0xFF;
            new_size[2] = (new_len >> 8) & 0xFF;
            new_size[3] = new_len & 0xFF;

            if (fwrite(new_size, 1, 4, mp3->dup_mp3_fptr) != 4)
            {
                fclose(mp3->org_mp3_fptr);
                fclose(mp3->dup_mp3_fptr);
                remove("temp.mp3");
                return;
            }

            char flag[2];

            if (fread(flag, 1, 2, mp3->org_mp3_fptr) != 2 ||
                fwrite(flag, 1, 2, mp3->dup_mp3_fptr) != 2)
            {
                fclose(mp3->org_mp3_fptr);
                fclose(mp3->dup_mp3_fptr);
                remove("temp.mp3");
                return;
            }

            char encoding;

            if (fread(&encoding, 1, 1, mp3->org_mp3_fptr) != 1 ||
                fwrite(&encoding, 1, 1, mp3->dup_mp3_fptr) != 1)
            {
                fclose(mp3->org_mp3_fptr);
                fclose(mp3->dup_mp3_fptr);
                remove("temp.mp3");
                return;
            }

            if (fwrite(new_con, 1, new_len - 1, mp3->dup_mp3_fptr) != new_len - 1)
            {
                fclose(mp3->org_mp3_fptr);
                fclose(mp3->dup_mp3_fptr);
                remove("temp.mp3");
                return;
            }

            if (fseek(mp3->org_mp3_fptr, old_len - 1, SEEK_CUR) != 0)
            {
                fclose(mp3->org_mp3_fptr);
                fclose(mp3->dup_mp3_fptr);
                remove("temp.mp3");
                return;
            }
        }
        else
        {
            unsigned char size_buffer[4];

            if (fread(size_buffer, 1, 4, mp3->org_mp3_fptr) != 4 ||
                fwrite(size_buffer, 1, 4, mp3->dup_mp3_fptr) != 4)
            {
                fclose(mp3->org_mp3_fptr);
                fclose(mp3->dup_mp3_fptr);
                remove("temp.mp3");
                return;
            }

            unsigned int len =
                ((unsigned int)size_buffer[0] << 24) |
                ((unsigned int)size_buffer[1] << 16) |
                ((unsigned int)size_buffer[2] << 8) |
                size_buffer[3];

            char flag[2];

            if (fread(flag, 1, 2, mp3->org_mp3_fptr) != 2 ||
                fwrite(flag, 1, 2, mp3->dup_mp3_fptr) != 2)
            {
                fclose(mp3->org_mp3_fptr);
                fclose(mp3->dup_mp3_fptr);
                remove("temp.mp3");
                return;
            }

            char *data = malloc(len ? len : 1);

            if (data == NULL)
            {
                printf("Memory allocation failed\n");
                fclose(mp3->org_mp3_fptr);
                fclose(mp3->dup_mp3_fptr);
                remove("temp.mp3");
                return;
            }

            if (fread(data, 1, len, mp3->org_mp3_fptr) != len ||
                fwrite(data, 1, len, mp3->dup_mp3_fptr) != len)
            {
                free(data);
                fclose(mp3->org_mp3_fptr);
                fclose(mp3->dup_mp3_fptr);
                remove("temp.mp3");
                return;
            }

            free(data);
        }
    }

    if (!found)
    {
        printf("Tag not found\n");
        fclose(mp3->org_mp3_fptr);
        fclose(mp3->dup_mp3_fptr);
        remove("temp.mp3");
        return;
    }

    printf("ALL TAGS PROCESSED!!!\n");

    char buffer[65536];
    size_t bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer), mp3->org_mp3_fptr)) > 0)
    {
        if (fwrite(buffer, 1, bytes, mp3->dup_mp3_fptr) != bytes)
        {
            fclose(mp3->org_mp3_fptr);
            fclose(mp3->dup_mp3_fptr);
            remove("temp.mp3");
            return;
        }
    }

    if (ferror(mp3->org_mp3_fptr))
    {
        fclose(mp3->org_mp3_fptr);
        fclose(mp3->dup_mp3_fptr);
        remove("temp.mp3");
        return;
    }

    fclose(mp3->org_mp3_fptr);
    fclose(mp3->dup_mp3_fptr);

    if (rename("temp.mp3", argv[file_index]) != 0)
    {
        perror("Rename failed");
        return;
    }

    printf("\n_____________%s CHANGED SUCCESSFULLY_____________\n", select);
    printf("\n");
    printf("             %s = %s\n", select, new_con);
    printf("___________________________________________________\n");
}
