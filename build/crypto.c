#include <stdio.h>
#include <string.h>
#include <zlib.h>

extern char ascii_art[128][128];
extern int max_width;
extern int max_height;
extern int max_colors ;

static int clamp(int min, int max, int in)
{
	return in < min ? min : in > max ? max : in;
}

void drunken_bishop(FILE * fp)
{
	const int w = 18;
	const int h = 10;
	const int it = 200;
	const char set[] = " .o+=*B0X@%&#/^SE";
	
	fseek(fp, 0L, SEEK_END);
	long file_size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char* file_data = (char*)malloc(file_size);
	fread(file_data, 1, file_size, fp);
	unsigned long in = crc32(0L, Z_NULL, 0);
	in = crc32(in, (const Bytef*)file_data, file_size);
	free(file_data);
	rewind(fp);
	
	srand(in);
	
	
	int map[256][256];
	for (int bz = 0; bz < h; ++bz)
	{
		bzero(map[bz], w);
	}
	
	int x = rand() % w, y = rand() % h;
	int i = 0;
	while (i < it)
	{
		switch (rand() % 4)
		{
			case 0: // up left
			{
				if (x > 0 && y < h - 1)
				{
					++map[++y][--x];
				}
				break;
			};
			case 1: // up right
			{
				if (x < w - 1 && y < h - 1)
				{
					++map[++y][++x];
				}
				break;
			};
			case 2: // down left
			{
				if (x > 0 && y > 0)
				{
					++map[--y][--x];
				}
				break;
			};
			case 3: // down right
			{
				if (x < w - 1 && y > 0)
				{
					++map[--y][++x];
				}
				break;
			};
		}
		++i;
	}
	
	max_width = w;
	max_height = h;
	for (y = 0; y < h; ++y)
	{
		for (x = 0; x < w; ++x)
		{
			ascii_art[y][x] = set[clamp(0, 16, map[y][x])];
		}
	}
	
	return;
}
