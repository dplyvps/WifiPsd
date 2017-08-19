#include <stdio.h>
#include <tipwc/ini.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define WIFI_PATH "/etc/NetworkManager/system-connections"

int main(int argc,char *argv[])
{
	INI *ini;
	struct dirent *dir;
	DIR *dirp;
	const char *ssid,*psd;
	struct stat buf;

	if((dirp=opendir(WIFI_PATH)) == NULL)
	{
		perror(argv[1]);
		return -1;
	}

	chdir(WIFI_PATH);

	while(dir=readdir(dirp))
	{
		if(!strcmp(dir->d_name,".")|| !strcmp(dir->d_name,".."))
			continue;

		if(stat(dir->d_name,&buf) == -1)
		{
			perror(dir->d_name);
			continue;
		}

		if(S_ISDIR(buf.st_mode))
			continue;

		ini=ini_parse_with_filename(dir->d_name);
		if(!ini)
		{
			ini_perror(dir->d_name);
			continue;
		}

		ssid=ini_get_key_value(ini,"wifi","ssid");
		psd=ini_get_key_value(ini,"wifi-security","psk");

		printf("SSID: %s\nPassword: %s\n\n",ssid,psd?psd:"<空>");
		ini_free(ini);
	}

	closedir(dirp);

	return 0;
}
