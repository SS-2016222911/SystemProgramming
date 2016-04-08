#include        <stdio.h> 
#include        <sys/types.h> 
#include        <dirent.h> 
#include        <sys/stat.h> 
#include        <string.h>
#include	<stdlib.h>
 
void do_ls(char[]);
void do_ls_R(char[]); 
void dostat(char *); 
void show_file_info( char *, struct stat *); 
void mode_to_letters( int , char [] );
char *uid_to_name( uid_t );
char *gid_to_name( gid_t );


int main(int ac, char *av[])
{
        int idx = 0;
        if ( ac == 1 )
        {
                do_ls( "." );
        }
        else
        {
                for(idx = 0; idx < ac; idx++)
                {
			if(strcmp(av[idx], "-R") == 0)
			{
				if(ac == 2)
				{
					do_ls_R( "." );
				}
				else
				{
					while ( --ac ){
                                        	printf("%s:\n", *++av );
                                        	do_ls_R( *av );
                                	}
				}
				return 0;
			}
                }
                while ( --ac ){
                        printf("%s:\n", *++av );
                        do_ls( *av );
                }
        }
        return 0;
}


void do_ls( char dirname[] )
{
        DIR             *dir_ptr;               /* the directory */
        struct dirent   *direntp;               /* each entry    */
        char filefullpath[100]; //에러 수정

        if ( ( dir_ptr = opendir( dirname ) ) == NULL )
                fprintf(stderr,"ls2: cannot open %s\n", dirname);
        else
        {
                while ( ( direntp = readdir( dir_ptr ) ) != NULL ) {
			if (strcmp(direntp->d_name,".")==0 ||
				strcmp(direntp->d_name,"..")==0) continue;
			sprintf(filefullpath, "%s/%s", dirname, direntp->d_name); //에러 수정
                        //dostat( direntp->d_name );
                        dostat(filefullpath ); //에러 수정
                }
                closedir(dir_ptr);
        }
}


void do_ls_R( char dirname[] )
/*
 *      list files in directory called dirname
 */
{
        DIR             *dir_ptr;               /* the directory */
        struct dirent   *direntp;               /* each entry    */

        int maxNumOfDir = 100;
        int maxLenOfPath = 100;
        char filefullpath[maxLenOfPath]; //에러 수정
        int dirCnt = 0;
        int i = 0;
        struct stat info;

        char** dirNameList;

        dirNameList = (char**)malloc(sizeof(char*)* maxNumOfDir);

	if(strcmp(dirname, "-R") == 0)
	{
		return;
	}

        for( i = 0; i < maxNumOfDir; i++)
        {
                dirNameList[i] = (char*)malloc(sizeof(char)*maxLenOfPath);
        }

        if ( ( dir_ptr = opendir( dirname ) ) == NULL )
        {
                fprintf(stderr,"ls2: cannot open %s\n", dirname);

        }
        else
        {
                while ( ( direntp = readdir( dir_ptr ) ) != NULL ) {
                        //dostat( direntp->d_name );

			if (strcmp(direntp->d_name,".")==0 ||
				strcmp(direntp->d_name,"..")==0) continue;
                        sprintf(filefullpath, "%s/%s", dirname, direntp->d_name); //에러 수정
                        dostat(filefullpath ); //에러 수정

                        if ( stat(filefullpath, &info) == -1 )          /* cannot stat   */
                        {
                                perror( filefullpath );                 /* say why       */
                        }
                        else
                        {
                                if( S_ISDIR(info.st_mode))
                                {
                                        sprintf(dirNameList[dirCnt], "%s", filefullpath );
                                        //printf("%s is D\n", dirNameList[dirCnt]);
                                        ++dirCnt;
                                }
                        }
                }
                closedir(dir_ptr);
        }

        for( i = 0; i < dirCnt; i++)
        {
		printf("\n%s\n", dirNameList[i]);
		do_ls_R(dirNameList[i]);
        }

}


void dostat( char *filename )
{
        struct stat info;

        if ( stat(filename, &info) == -1 )              /* cannot stat   */
                perror( filename );                     /* say why       */
        else                                    /* else show info        */
                show_file_info( filename, &info );
}


void show_file_info( char *filename, struct stat *info_p )
{
        char    *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
        void    mode_to_letters();
        char    modestr[11];

        mode_to_letters( info_p->st_mode, modestr );
                                                             printf( "%s"    , modestr );
        printf( "%4d "  , (int) info_p->st_nlink);
        printf( "%-8s " , uid_to_name(info_p->st_uid) );
        printf( "%-8s " , gid_to_name(info_p->st_gid) );
        printf( "%8ld " , (long)info_p->st_size);
        printf( "%.12s ", 4+ctime(&info_p->st_mtime));
        printf( "%s\n"  , filename );

}

void mode_to_letters( int mode, char str[] )
{
    strcpy( str, "----------" );           /* default=no perms */

    if ( S_ISDIR(mode) )  str[0] = 'd';    /* directory?       */
    if ( S_ISCHR(mode) )  str[0] = 'c';    /* char devices     */
    if ( S_ISBLK(mode) )  str[0] = 'b';    /* block device     */

    if ( mode & S_IRUSR ) str[1] = 'r';    /* 3 bits for user  */
    if ( mode & S_IWUSR ) str[2] = 'w';
    if ( mode & S_IXUSR ) str[3] = 'x';

    if ( mode & S_IRGRP ) str[4] = 'r';    /* 3 bits for group */
    if ( mode & S_IWGRP ) str[5] = 'w';
    if ( mode & S_IXGRP ) str[6] = 'x';

    if ( mode & S_IROTH ) str[7] = 'r';    /* 3 bits for other */
    if ( mode & S_IWOTH ) str[8] = 'w';
    if ( mode & S_IXOTH ) str[9] = 'x';
}

#include        <pwd.h>

char *uid_to_name( uid_t uid )
{
        struct  passwd *getpwuid(), *pw_ptr;
        static  char numstr[10];

        if ( ( pw_ptr = getpwuid( uid ) ) == NULL ){
                sprintf(numstr,"%d", uid);
                return numstr;
        }
        else
                return pw_ptr->pw_name ;
}

#include        <grp.h>

char *gid_to_name( gid_t gid )
{
        struct group *getgrgid(), *grp_ptr;
        static  char numstr[10];

        if ( ( grp_ptr = getgrgid(gid) ) == NULL ){
                sprintf(numstr,"%d", gid);
                return numstr;
        }
        else
                return grp_ptr->gr_name;
}

