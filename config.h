static int parse_cfg(const char * path)
{
	FILE * fp;
	if (!path)
	{
		while (1)
		{
			char path[128];
			strcpy(path, getenv("HOME"));
			strcat(path, "/.config/binfetch/binfetch.cfg");
			if ((fp = fopen(path, "r")))
			{
				break;
			}
			// add what ever paths you want
			printf("failed to open standard config\n");
			return 1;
		}
		
	}
	else
	{
		fp = fopen(path, "r");
		if (!fp)
		{
			printf("failed to open defined config\n");
			return 1;
		}
	}
	
	char tok[128];
	while (fgets(tok, 128, fp))
	{
		if (!strncmp("COLORS", tok, 6))
		{
			while (fgets(tok, 128, fp))
			{
				if (tok[0] != '\t')
				{
					break ;
				}
				sscanf(tok + 1, "#%2x%2x%2x", (unsigned int *) &ascii_cols[max_colors].r, (unsigned int *) &ascii_cols[max_colors].g, (unsigned int *) &ascii_cols[max_colors].b);
				++max_colors;
			}
		}
		if (!strncmp("VALUES", tok, 6));
	}
	
	return 0;
}