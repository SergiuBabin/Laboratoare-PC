/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	mini-server de backup fisiere
*/

#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "helpers.h"


void usage(char*file)
{
	fprintf(stderr,"Usage: %s server_port file\n",file);
	exit(0);
}

/*
*	Utilizare: ./server server_port nume_fisier
*/
int main(int argc,char**argv)
{
	int fd;

	if (argc!=3)
		usage(argv[0]);
	
	struct sockaddr_in my_sockaddr, from_station ;
	char buf[BUFLEN];


	/*Deschidere socket*/
	int sockid = socket(PF_INET,SOCK_DGRAM,0);
	if(sockid == -1){
		DIE((sockid == -1), "error socket");
	}
	//struct id_addr in;
	//in.s_addr = INADDR_ANY;
	my_sockaddr.sin_family = AF_INET;
	my_sockaddr.sin_addr.s_addr = INADDR_ANY;
	my_sockaddr.sin_port = htons(atoi(argv[1]));

	
	/*Setare struct sockaddr_in pentru a asculta pe portul respectiv */

	
	/* Legare proprietati de socket */
	int p = bind(sockid,(struct sockaddr *)&my_sockaddr,sizeof(my_sockaddr));

	if(p == -1){
		DIE((p == -1), "error legare");
	}
	
	/* Deschidere fisier pentru scriere */
	DIE((fd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644))==-1,"open file");
	
	/*
	*  cat_timp  mai_pot_citi
	*		citeste din socket
	*		pune in fisier
	*/
	socklen_t addrlen;
	int nr ;
	while((nr = recvfrom(sockid,buf,BUFLEN,0,(struct sockaddr *) &from_station,&addrlen))> 0 ){
		printf("%d \n",nr);
		
		write(fd,buf,nr);
	}


	/*Inchidere socket*/	

	close(sockid);
	close(fd);
	/*Inchidere fisier*/

	return 0;
}
