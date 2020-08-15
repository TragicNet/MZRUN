#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <dirent.h>

void main() {
	clrscr();
	/*struct dirent *de;
	DIR *dr = opendir("./MZRUN/LEVELS");
	if(dr == NULL) return;
	int cnt=0;
	while((de = readdir(dr)) != NULL)  {
		printf("%s\n", de->d_name);
		cnt++;
	}
	closedir(dr);*/
	struct dirent *de;
	DIR *dr = opendir("./MZRUN/LEVELS");
	if(dr == NULL) return;
	int cnt=0, s;
	char ext[] = ".LVL";
	while((de = readdir(dr)) != NULL)  {
		s = strlen(de->d_name);	
		for(int i=s-4; i>0; i--)
			if(de->d_name[i] != ext[s-4-i])	continue;
			else {
				printf("%s\n", de->d_name);
				cnt++;
			}
	}
	closedir(dr);
	printf("%d",cnt);
	getch();
}