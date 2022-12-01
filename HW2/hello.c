#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

void pmessg(char *s){
	const char *end = s;
   	while (*end++) {};
   	size_t si = end-s-1;
	write(STDOUT_FILENO,s,si);
}


int main(void){
	
	int pid;
	int f_read,f_write;
	f_read = open("file_to_read.txt",O_RDONLY);
	f_write = open("file_to_write.txt",O_WRONLY|O_APPEND|O_CREAT, 0644);
	if (f_read == -1) {
	  char message[25] = "problem in read file";	
	  pmessg(message);	
	  _exit(1);
	}
	if (f_write == -1) {
	  char message[25] = "problem in write file";	
	  pmessg(message);	
	  _exit(1);
	}
	pid = fork();
	if(pid < 0) {
	 char message[40] = "problem in allocating resources";
	 pmessg(message);
	 _exit(1);
	}
	else if(pid == 0){
		char c;
		int number_of_chars = 0;
		int t = lseek(f_read,-1,SEEK_END);
		if (t == -1){
		 char message[40] = "problem in pointer";
		 pmessg(message);
		 _exit(1);		
		}
		for(int i = t;i>=0;i--){
		int tlseek = lseek(f_read,i,SEEK_SET);
		if (tlseek == -1){
	            char message[40] = "problem in pointer";
		    pmessg(message);
		    _exit(1);		
		}
		int tread = read(f_read,&c,1);
		if (tread == -1){
		 char message[40] = "problem in read";
		 pmessg(message);
		 _exit(1);		
		}
		int twrite = write(f_write,&c,1);
		if (t == -1){
		 char message[40] = "problem in write";
		 pmessg(message);
		 _exit(1);		
		}
		
		}
		c = '\n';
		int twrite = write(f_write,&c,1);
		if (twrite == -1){
		 char message[40] = "problem in write";
		 pmessg(message);
		 _exit(1);		
		}
		int lseek_pointer = lseek(f_read,0,SEEK_SET);
		if (lseek_pointer == -1){
		 char message[40] ="problem in pointer";
		 pmessg(message);
		 _exit(1);		
		}
	}
	else {
		wait(NULL);
		char c;
		while(read(f_read,&c,1)){
		int t_write = write(f_write,&c,1);
		if (t_write == -1){
		 char message[40] = "problem in write";
		 pmessg(message);
		 _exit(1);		
		 }
		}
		close(f_write);
		close(f_read);	
	}
	return 0;
}
