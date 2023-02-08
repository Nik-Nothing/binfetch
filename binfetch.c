#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>


#include "arch.h"
#include "osabi.h"


typedef enum ansi_color
{
	blank   = 0,
	red     = 31,
	green   = 32,
	yellow  = 33,
	blue    = 34,
	magenta = 35,
	cyan    = 36,
	white   = 37
}
color;


const color col = magenta;
const size_t max_tok = 128;


static void set_color(const color c)
{
	printf("\033[%dm", c);
}

static void print_label(const char * label)
{
	set_color(col);
	printf("%s: ", label);
	set_color(blank);
}

static void advance(byte * tok, const size_t n, FILE * fp)
{
	bzero(tok, max_tok);
	fread(tok, n, 1, fp);
}

int main(int argc, char **argv)
{
	
	if (argc < 2)
	{
		set_color(red);
		printf("you did not provide a binary\n");
		set_color(blank);
		return 1;
	}
	
	FILE * fp = fopen(argv[1], "rb");
	
	if (!fp)
	{
		set_color(red);
		printf("failed to open binary\n");
		set_color(blank);
		return 1;
	}
	
	print_label("Name");
	
	puts(basename(argv[1]));
	
	
	byte tok[max_tok];
	
	advance(tok, 4, fp);
	
	set_color(col);
	
	printf("Header: ");
	
	set_color(blank);
	
	if (!strcmp(tok + 1, "ELF"))
	{
		printf("ELF\n");
	}
	
	advance(tok, 4, fp);
	
	int bits = 0 ;// 32 or 64
	
	set_color(col);
	
	printf("Class: ");
	
	set_color(blank);
	
	if (tok[0] == 0x01)	
	{
		bits = 1;
		printf("32 bit\n");
	}
	else if (tok[0] == 0x02)
	{
		bits = 2;
		printf("64 bit\n");
	}
	
	set_color(col);
	
	printf("Endianness: ");
	
	set_color(blank);
	
	if (tok[1] == 0x01)
	{
		printf("little endian\n");
	}
	else if (tok[1] == 0x02)
	{
		printf("big endian\n");
	}
	
	set_color(col);
	
	printf("Version: ");
	
	set_color(blank);
	
	if (tok[2] == 0x01)
	{
		printf("ELFv1\n");
	}
	else if (tok[2] == 0x00)
	{
		printf("noncompliant\n");
	}
	
	set_color(col);
	
	printf("OS ABI: ");
	
	set_color(blank);
	
	size_t osabic = 0;
	while(osabic < sizeof osabis / sizeof(pr))
	{
		if (osabis[osabic].key == tok[3])
		{
			puts(osabis[osabic].str);
			break;
		}
		else if(++osabic == sizeof osabis / sizeof(pr))
		{
			printf("unknown abi %d", tok[3]);
		}
	}
	
	advance(tok, bits == 1 ? 4 : 8, fp);
	
	// do nothing
	
	advance(tok, 2, fp);
	
	set_color(col);
	
	printf("Type: ");
	
	set_color(blank);
	
	switch (tok[0])
	{
		case 0x01:
			printf("Object\n");
			break;
		case 0x02:
			printf("Static\n");
			break;
		case 0x03:
			printf("Shared\n");
			break;
		default:
			printf("unknown object type %d\n", tok[0]);
	}
	
	advance(tok, 2, fp);
	
	set_color(col);
	
	printf("Arch: ");
	
	set_color(blank);
	
	size_t archc = 0;
	while(archc < sizeof arches / sizeof(pr))
	{
		if (arches[archc].key == tok[0])
		{
			puts(arches[archc].str);
			break;
		}
		else if(++archc == sizeof arches / sizeof(pr))
		{
			printf("unknown arch %d", tok[0]);
		}
	}
	
	advance(tok, 4, fp);
	
	// do nothing
	
	advance(tok, bits == 1 ? 4 : 8, fp);
	
	set_color(col);
	
	printf("Entry: ");
	
	set_color(blank);
	
	printf("0x");
	
	{
		size_t b = 0;
		while (b < (bits == 1 ? 4 : 8))
		{
			printf("%x", tok[b]);
			++b;
		}
	}
	
	set_color(col);
	
	printf("\n");
	
	printf("Size: ");
	
	set_color(blank);
	
	char exec_path[] = "du -h ";
	strcat(exec_path, argv[1]);
	FILE *p = popen(exec_path, "r");
	
	if (!p)
	{
		printf("couldn't find 'du'\n");
	}
	
	char buf[64];
	fgets(buf, 64, p);
	
	char *save = NULL;
	char *size  = strtok_r(buf, " \t\n", &save);
	
	
	if (pclose(p))
	{
		printf("du failed\n");
	}
	else
	{
		puts(size);
	}
	
	fclose(fp);
	
	return 0;
}
