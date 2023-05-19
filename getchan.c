/* GetChan - A 4chan thread image downloader 

     This program(GetChan) is free software: you can redistribute it and/or modify 
it under the terms of the GNU General Public License as published by the Free 
Software Foundation, either version 3 of the License, or (at your option) any 
later version.

    This program is distributed in the hope that it will be useful, but WITHOUT 
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with 
this program. If not, see <https://www.gnu.org/licenses/>. 

*/

/* Uncomment to use the old, outdated version that 
downloads the direct given by 4chan */
//#define GETCHAN_DIRECT_LINK

#include <stdio.h>
#include <string.h>
#include <curl/curl.h>


#define LARGEST_PATH 4096

#define cmp_args(av,bv) !strcmp(argv[i], av) || !strcmp(argv[i], bv)


int find_file_str(const char *str, FILE *fp);
int download_file(const char *dir, const char *uri);
void usage(const char *, FILE *);


void usage(const char *cmd, FILE *fd){
	fprintf(fd, "Usage: %s [-oh] [URI/URL]\n\
\t-h, --help\t\tprints this help message\n\
\t-o, --output\t\tfollowing argument defines output directory\n", cmd);
}


int find_file_str(const char *str, FILE *fp){
	int i = 0, in_str = 0;
	char c=0;
	do{
		c = fgetc(fp);
		if(c == EOF){
			break;
		}
			
		if(c == *(str+i)){
			in_str = 1;
			i++;
			if(*(str+i) == 0){
				return(1);
			}
		}
		else{
			in_str = 0;
		}
	}while(c != EOF);
	
	return(0);
}//find_file_str



int download_file(const char *dir, const char *uri){
	
	char *fl = (char *)uri;
	while(*(fl++));

	int has_ext = 0;
	while(*fl != '/'){
		if(*fl == '.'){ has_ext = 1; }
		fl--;
	}
	#ifndef GETCHAN_DIRECT_LINK
	char *bfl = fl-1;
	while(*bfl != '/'){ //include the board name
		bfl--;
	}
	#endif
	if(!has_ext){ return(0); }
	if(strstr(uri, ".js") != NULL){ return(1);}

	char savefile[LARGEST_PATH];
	memset(savefile, 0, LARGEST_PATH);
	strcpy(savefile, dir);
	strcat(savefile, fl);

	char url[LARGEST_PATH];
	memset(url, 0, LARGEST_PATH);
	#ifdef GETCHAN_DIRECT_LINK
		strcpy(url, "https://");
		strcat(url, uri);
	#else  /* new verison */
		strcpy(url, "https://i.4cdn.org");
		strcat(url, bfl);
	#endif

	printf("Downloading %s to %s\n", url, savefile);

	FILE *fp = fopen(savefile, "wb");

	CURL *curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if(curl) {

		//Download the the thread page
   		curl_easy_setopt(curl, CURLOPT_URL, url);// set the URL	
   		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);// set the write file
	
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		
		res = curl_easy_perform(curl);// get the file
		if(res != CURLE_OK){
			fprintf(stderr, "Error retrieving the file with curl.\n");
			return(1);
		}

		curl_easy_cleanup(curl);	

	}

	fclose(fp);

	return(0);
}//download_file




int main(int argc, const char *argv[]){

	int ioutdir = 0;
	int iuri = 0;
	char dirname[LARGEST_PATH];

	if(argc < 2){
		fprintf(stderr, "Too few arguments.\n");
		usage(argv[0], stderr);
		return(-1);
	}

	int i=0;
	for(i=1; i<argc; i++){
		if(cmp_args("-o", "--output")){
			ioutdir = ++i;
			i++;
		}
		else if(cmp_args("-h", "--help")){
			usage(argv[0], stdout);
			return(0);
		}
		else{
			iuri = i;	
		}
	}//for

	//Check if a URI arg was found
	if(!iuri){
		fprintf(stderr, "No URI specified.\n");
		usage(argv[0], stderr);
		return(-1);
	}

	//set the directory to save the files to
	strcpy(dirname, ioutdir ? argv[ioutdir] : ".");

	FILE *tfp = fopen("./.getchan.tmp", "wb");

	CURL *curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if(curl) {

		//Download the the thread page
   		curl_easy_setopt(curl, CURLOPT_URL, argv[iuri]);// set the URL	
   		curl_easy_setopt(curl, CURLOPT_WRITEDATA, tfp);// set the URL
	
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		
		res = curl_easy_perform(curl);// get the file
		
		curl_easy_cleanup(curl);	
	
		fclose(tfp);
		//Crawl through the file to for each name

		tfp = fopen("./.getchan.tmp", "rb");

		char c = 0;
		char filelink[1024];
		char *fl = NULL;
		while(find_file_str("File: <a href=\"//", tfp)){
			memset(filelink, 0, 1024);
			fl = filelink;
			do{
				c = fgetc(tfp);
				if(c == '"'){ break; }
				//printf("\tAdding char to str"){
				*fl = c;
				fl++;
			}while(c != '"');
			if(strstr(filelink, ".org") == NULL){
				continue; } //Filter out non-links
			//printf("Found file link: %s\n", filelink);
			download_file(dirname, filelink);
		}//while not end of file

		remove("./.getchan.tmp");
	
	}

	fclose(tfp);

	return(0);
}



