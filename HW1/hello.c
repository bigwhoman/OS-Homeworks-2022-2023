#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void){
	int pid;
	int f_read,f_write;
	f_read = open("file_to_read.txt",O_RDONLY);
	f_write = open("file_to_write.txt",O_WRONLY|O_APPEND|O_CREAT, 0644);
	pid = fork();
	if(pid == 0){
		char c;
		int number_of_chars = 0;
		int t = lseek(f_read,-1,SEEK_END);
		for(int i = t;i>=0;i--){
			lseek(f_read,i,SEEK_SET);
			read(f_read,&c,1);
			write(f_write,&c,1);
		}
		c = '\n';
		write(f_write,&c,1);
		lseek(f_read,0,SEEK_SET);
	}
	else {
		wait(NULL);
		char c;
		while(read(f_read,&c,1)){
			write(f_write,&c,1);
		}
		close(f_write);
		close(f_read);	
	}
	return 0;
}
