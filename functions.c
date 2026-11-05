
#include "mp3_header.h"
#include <strings.h>

int validate_cla(int argc, char *argv[], struct MP3 *mp3)
{
	if (argc == 1)
	{
		printf("------------------------------------------------------------------------------\n");
		printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
		printf("USAGE : To view please pass like: ./a.out -v mp3filename\n");
		printf("To edit please pass like: ./a.out -e -t/-a/-A/-y/-c/-C [changing_text] mp3filename\n");
		printf("To get help pass like : ./a.out --help\n");
		printf("------------------------------------------------------------------------------\n");
		return FAILURE;
	}

	if (strcmp(argv[1], "--help") == 0)
	{
		printf("------------------------------->HELP<-----------------------------------------\n");
		printf("1. -v -> to view mp3 file contents\n");
		printf("2. -e -> to edit mp3 file contents\n");
		printf("\t\t2.1. -t -> to edit song title\n");
		printf("\t\t2.2. -a -> to edit artist name\n");
		printf("\t\t2.3. -A -> to edit album name\n");
		printf("\t\t2.4. -y -> to edit year\n");
		printf("\t\t2.5. -c -> to edit genre\n");
		printf("\t\t2.6. -C -> to edit comment\n");
		printf("------------------------------------------------------------------------------\n");
		return FAILURE;
	}

	if (strcmp(argv[1], "-v") == 0)
	{
		if (argc != 3)
		{
			printf("View takes exactly one file!!!\n");
			return FAILURE;
		}

		int len = strlen(argv[2]);

		if (len <= 4)
		{
			printf("Invalid file name!!!\n");
			return FAILURE;
		}

		if (strcasecmp(argv[2] + len - 4, ".mp3") != 0)
		{
			printf("File must be an .mp3!!!\n");
			return FAILURE;
		}

		FILE *fp = fopen(argv[2], "rb");

		if (fp == NULL)
		{
			printf("NO FILE FOUND!!!\n");
			return FAILURE;
		}

		fclose(fp);
		mp3->mp3_filename = argv[2];

		return VIEW;
	}

	if (strcmp(argv[1], "-e") == 0)
	{
		if (argc != 4 && argc != 5)
		{
			printf("Edit usage: ./a.out -e -y [changing_text] mp3filename\n");
			return FAILURE;
		}

		if (strcmp(argv[2], "-a") != 0 &&
			strcmp(argv[2], "-A") != 0 &&
			strcmp(argv[2], "-t") != 0 &&
			strcmp(argv[2], "-y") != 0 &&
			strcmp(argv[2], "-C") != 0 &&
			strcmp(argv[2], "-c") != 0)
		{
			printf("Invalid edit option!!!\n");
			return FAILURE;
		}

		int file_index = (argc == 5) ? 4 : 3;
		int len = strlen(argv[file_index]);

		if (len <= 4)
		{
			printf("Invalid file name!!!\n");
			return FAILURE;
		}

		if (strcasecmp(argv[file_index] + len - 4, ".mp3") != 0)
		{
			printf("File must be an .mp3!!!\n");
			return FAILURE;
		}

		FILE *fp = fopen(argv[file_index], "rb");

		if (fp == NULL)
		{
			printf("NO FILE FOUND!!!\n");
			return FAILURE;
		}

		fclose(fp);
		mp3->mp3_filename = argv[file_index];

		return EDIT;
	}

	printf("Invalid mode!!!\n");
	return FAILURE;
}

void toggle_endianess(struct MP3 *mp3, int *size)
{
	unsigned char buffer[4];

	if (*size != 4)
		return;

	if (fread(buffer, 1, *size, mp3->org_mp3_fptr) != *size)
		return;

	for (int i = 0; i < *size / 2; i++)
	{
		unsigned char temp = buffer[i];
		buffer[i] = buffer[*size - 1 - i];
		buffer[*size - 1 - i] = temp;
	}

	fseek(mp3->org_mp3_fptr, -*size, SEEK_CUR);
}

void tag_to_name(char *tag)
{
	if (strcmp(tag, "TPE1") == 0)
		printf("ARTIST : ");
	else if (strcmp(tag, "TALB") == 0)
		printf("ALBUM : ");
	else if (strcmp(tag, "TIT2") == 0)
		printf("TITLE : ");
	else if (strcmp(tag, "TYER") == 0)
		printf("YEAR : ");
	else if (strcmp(tag, "TCON") == 0)
		printf("GENRE : ");
	else if (strcmp(tag, "COMM") == 0)
		printf("COMMENT : ");
	else
		printf("INVALID TAG!!!");
}
