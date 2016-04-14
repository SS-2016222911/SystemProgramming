#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>

void main(int ac, char *av[])
{
	struct 	stat info;

	if(ac == 3)  //갯수 채크
	{
		if(!strcmp(basename(av[1]),basename(av[2])))  //이름같은지
		{
			fprintf(stderr, "name is same\n");
			exit(0);
		}
		if(stat(av[2], &info) != 1)
		{
			if(S_ISDIR(info.st_mode))	//디렉토리 인지 아닌지
			{
				char buf[255] = ""; 
				char argv2_dir[100] = ""; 
				char av1_base[100] ="";

				strcpy(av1_base, basename(av[1]));
				realpath(av[2],argv2_dir);
				
				strcat(buf, argv2_dir); //경로 만들기
				strcat(buf, "/");
				strcat(buf, av1_base);
				
				if(rename(av[1], buf) == 1) 	//
				{
					fprintf(stderr, "cannot move the file\n");
					exit(0);
				}
			}
			else
			{
				if(rename(av[1], av[2]) == 1)	//file mv
				{
					fprintf(stderr, "cannot move the file\n");
					exit(0);
				}					
			}
		}
		else
		{
                        fprintf(stderr, "%s is not a  file\n", av[2]);
                        exit(0);
          	}
	}
	else
	{
		fprintf(stderr, "input parameter mush be 3\n");
		exit(0);
	}
}
