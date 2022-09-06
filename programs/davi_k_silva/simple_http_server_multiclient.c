/*
	Simple http server (with multiple connections)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <ctype.h>
 
#define BUFLEN	1024	//Max length of buffer
#define PORT	8000	//The port on which to listen for incoming data

char http_ok[] = "HTTP/1.0 200 OK\r\nContent-type: text/html\r\nServer: Test\r\n\r\n";
char http_error[] = "HTTP/1.0 400 Bad Request\r\nContent-type: text/html\r\nServer: Test\r\n\r\n";
char text[] = "<html> <head> <title>Machine info</title> </head> <body> <p>Machine info</p> <p>Date: %s <br> Uptime: %d seconds <br> Cpu Model: %s <br> Cpu Usage: %Lf %% <br> Memory : %d MB / %d MB <br> Avaible Memory : %d MB <br> Version: <br> %s <br> Process:<br> %s </p> </body></html>";
char page[10000];

char *strremove(char *str, const char *sub) {
    size_t len = strlen(sub);
    if (len > 0) {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}

char *extractStr(char *str, const char *p1, const char *p2){

    char *target = NULL;
    char *start, *end;

    if ( start = strstr( str, p1 ) )
    {
        start += strlen( p1 );
        if ( end = strstr( start, p2) )
        {
            target = ( char * )malloc( end - start + 1 );
            memcpy( target, start, end - start );
            target[end - start] = '\0';
        }
    }

	return target;
}

void die(char *s)
{
	perror(s);
	exit(1);
}
 
int main(void)
{	
	int w=0;
	
	// Date	 
	char date[80];
	time_t t = time(NULL);
  	struct tm tm = *localtime(&t);
	sprintf(date, "%d-%02d-%02d %02d:%02d:%02d\n",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	//printf("%s <-",date);

	// Uptime
	FILE *f = fopen("/proc/uptime", "r");
    int unused;
    int uptime;
    fscanf(f, "%d %f", &uptime ,&unused);
	//printf("%d %f", uptime, unused);
	fclose(f);

	// Processor info and velocity
	FILE *f2 = fopen("/proc/cpuinfo", "r");
    char model[200];
	char buf1[200]; 

    while (fgets (buf1, 200, f2)) {              
            char *cmpstr = "model name";                 
            size_t cmplen = strlen (cmpstr);        
            if (strncmp (buf1, cmpstr, cmplen) == 0) {
				//printf("%s", buf1);
				break;  
			}
        
    }
	strremove(buf1, "model name	:");
	//printf("%s<", strremove(buf1, "model name	:"));
    fclose (f2);

	//cpu usage
	char str[100];
	const char d[2] = " ";
	char* token;
	long int sum = 0, idle;
	long double idleFraction =2;
	FILE* fp3 = fopen("/proc/stat","r");
	                w = 0;
			fgets(str,100,fp3);
			fclose(fp3);
			token = strtok(str,d);
	                sum = 0;
			while(token!=NULL){
				token = strtok(NULL,d);
				if(token!=NULL){
					sum += atoi(token);
					//printf("%d sum\n",sum);
					if(w==3){
						idle = atoi(token);
						//printf("%d idle\n",idle);
					}
					w++;
			    }
		    }
	
	idleFraction = 100 - (idle)*100.0/(sum);
	//printf("%Lf , %di , %ds<<",idleFraction, idle, sum);

	// Memory Used and Total
	FILE *f4 = fopen("/proc/meminfo", "r");
    
	char buf2[200];  
	int memTotal;
	int memFree;
	int memAv;
    while (fgets (buf2, 200, f4)) {                
            char *cmpstr2 = "MemTotal";  
			char *cmpstr3 = "MemFree";   
			char *cmpstr4 = "MemAvaible";            
            size_t cmplen2 = strlen (cmpstr2);
			size_t cmplen3 = strlen (cmpstr3);
			size_t cmplen4 = strlen (cmpstr4);        
            if (strncmp (buf2, cmpstr2, cmplen2) == 0) {
				sscanf(buf2,"%*s %d",&memTotal);
				//printf("%s <", buf2); 
			}
			if (strncmp (buf2, cmpstr3, cmplen3) == 0) {
				sscanf(buf2,"%*s %d",&memFree);
				//printf("%s >", buf2); 
			}
			if (strncmp (buf2, cmpstr4, cmplen4) == 0) {
				sscanf(buf2,"%*s %d",&memAv);
				//printf("%s >", buf2);
				break;  
			}
        
    }
	//printf("%d<mem", memTotal);
    fclose (f4);

	// Version
	FILE *f5 = fopen("/proc/version", "r");
	char buf3[200];  
	fgets (buf3, 200, f5);
    fclose (f5);

	// Process pids and names
	char injection[2000];
	char buf4[300]; 
	struct dirent *files;
	DIR *dir = opendir("/proc/");
	if (dir == NULL){
		printf("Directory cannot be opened!" );
		return 0;
	}

	while ((files = readdir(dir)) != NULL){
		if (isdigit(files->d_name[0])){
			//printf("%s\n", files->d_name);
			char filename[40];
			sprintf(filename,"/proc/%s/stat",files->d_name);
			FILE *fpid = fopen(filename, "r");
			fgets (buf4, 300, fpid);
			char injectionModel[100];
			sprintf(injectionModel,"%s %s <br>",files->d_name, extractStr(buf4, "(", ")"));
			//printf("%s\n",injectionModel);
			strcat(injection, injectionModel);
			fclose(fpid);
		}
		//printf("%s\n", injection);
	}
	//printf("%s\n", injection);

	// Add values
	sprintf(page,text,date,uptime,buf1,idleFraction,(memTotal-memFree)/1000,memTotal/1000,memAv/1000,buf3,injection);

// ---------------------------------------------------------------------- //
	struct sockaddr_in si_me, si_other;

	int s, i, slen = sizeof(si_other) , recv_len, conn, child = 0;
	char buf[BUFLEN];
	pid_t pid;
     
	/* create a TCP socket */
	if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		die("socket");
    
	/* zero out the structure */
	memset((char *) &si_me, 0, sizeof(si_me));
     
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
	/* bind socket to port */
	if (bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1)
		die("bind");
	
	/* allow 10 requests to queue up */ 
	if (listen(s, 10) == -1)
		die("listen");
     
	/* keep listening for data */
	while (1) {
		memset(buf, 0, sizeof(buf));
		printf("Waiting a connection...");
		fflush(stdout);
		
		conn = accept(s, (struct sockaddr *) &si_other, &slen);
		if (conn < 0){
			die("accept");
		}

		if ((pid = fork()) < 0)
			die("fork");
		else if (pid == 0) {
			close(s);
			printf("Client connected: %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

			/* try to receive some data, this is a blocking call */
			recv_len = read(conn, buf, BUFLEN);
			if (recv_len < 0)
				die("read");

			//snapSystem();
			/* print details of the client/peer and the data received */
			printf("Data: %s\n" , buf);
		 
			if (strstr(buf, "GET")) {
				/* now reply the client with the same data */
				if (write(conn, http_ok, strlen(http_ok)) < 0)
					die("write");
				if (write(conn, page, strlen(page)-1) < 0){
					memset(page,0,strlen(page));
					die("write");
				}
					
			} else {
				if (write(conn, http_error, strlen(http_error)) < 0)
					die("write");
			}

			exit(0);
		}

		/* close the connection */
		close(conn);

		child++;
		while (child) {
			pid = waitpid((pid_t) -1, NULL, WNOHANG);
			if (pid < 0)
				die("?");
			else if (pid == 0) break;
			else child--;
		}
	}
	close(s);
	
	return 0;
}
