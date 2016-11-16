#include <string.h>

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define YOUR_ID 128
int main( )
{
    char str[10]="ON";  
    int sockfd,len,ret;

    if(0>(sockfd = socket(AF_INET, SOCK_STREAM, 0)))
    {   
        printf("socket create failed\n");
        close(sockfd);
        return -1; 
    }   
    printf("socket create success %s, %d\n", str, (int)strlen(str));
    ret = setsockopt(sockfd, IPPROTO_IP, YOUR_ID, str, strlen(str));
    if(0 != ret)
    {   
        printf("setsockopt error:%d, %s\n",errno,strerror(errno));
        return -2; 
    }   

    printf("setsockopt success: ret=%d\n", ret);

    len=10;
    ret = getsockopt(sockfd, IPPROTO_IP, YOUR_ID, str, &len);
    if(0 != ret)
    {   
        printf("setsockopt error: %d, %s\n", errno, strerror(errno));
        return -2; 
    }   
    printf("getsockopt success: got %s\n", str);

    close(sockfd);
    return 0;
}
