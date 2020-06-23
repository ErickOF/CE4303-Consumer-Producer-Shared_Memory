#include <stdio.h>
#include <time.h>

void getDate(char date[25]){
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
    	strftime(date, 24, "%Y-%m-%d %H:%M:%S", tm);

}

void main(){
	char date[50];
	getDate(date);
	printf("%s\n", date);	
		
}


