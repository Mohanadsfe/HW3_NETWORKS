#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#define SERVER_PORT 5346
#define SERVER_IP_ADDRESS "127.0.0.1"
#define SIZEFILE 1048576
#define HALF_SIZEFILE 524288
#define M_Authentication_SIZE 21
#define FIlE_NAME "dataset.txt"
#define XOR "0101 0001 1100 0000"
int setSock();
void printTimes(double *timePart1, double *timePart2, int len,double sum_Cubic,double sum_Reno);


int main() {


    int size2=0;
    // on linux to prevent crash on closing socket.
    signal(SIGPIPE, SIG_IGN);
    char buffer[HALF_SIZEFILE];
    char User_Decision_MESSAGE[1];
    char Auth_Message[M_Authentication_SIZE]="0101 0001 1100 0000";
    char Check_DECISION[1];// create the message exit that the receiver will send if the user want to exit.
    Check_DECISION[0]='0';


    //Accept and incoming connection
    printf("Waiting for incoming connections\n");

    struct sockaddr_in client_addr;
    socklen_t client_addr_length = sizeof(client_addr);

    double sum_for_average_cubic = 0.0;
    double sum_for_average_reno = 0.0;
    // allocating memory
    double *arrTimePart1 = NULL;
    double *arrTimePart2 = NULL;
    arrTimePart1 = (double *) malloc(1);
    arrTimePart2 = (double *) malloc(1);

    char Name_CC_Algo[250]; // to the name of the cc algorithm
    socklen_t length;

    //Check allocate



    User_Decision_MESSAGE[0];
    int sockfd = -1;
    int countbytes = 0;

    int checkPart = 0; // to check in which part we ( part 1 of the file or part 2)
    int rcv=0; // save in the capacity the can rcv
    int Bole=1;
    int t=1; // only to design , to printf that after the do{}
    int c1=0; // increment the point of the array part1
    int c2=0; // increment the point of the array part2

    sockfd = setSock(); //Create the socket




    do {

        printf("Receive %d\n",t);

        countbytes = 0;

        memset(&client_addr, 0, sizeof(client_addr)); // to update (0000000....)all bytes in.
        client_addr_length = sizeof(client_addr);

        // accept a connection from the sender
        int client_socket = accept(sockfd, (struct sockaddr *) &client_addr, &client_addr_length);
        if (client_socket == -1) {
            printf("listen failed with error code : \n");

            return -1;
        } else {
            printf("\nclient number connection accepted\n");
        }


        while (1) {
            struct timeval start, end;

            // rcv the file until we get the half of it, then go in the if.
            // the size of the buffer is half-size of the file.
            gettimeofday(&start, 0); // begin to measure the time of rcv
            rcv = recv(client_socket, &buffer, sizeof(buffer), 0);

            countbytes += rcv;
            printf("2\t%d\n",countbytes);

            if (countbytes == HALF_SIZEFILE) {
                gettimeofday(&end, 0); // end measure


                if (checkPart == 0) { //To handle with  the first recv part


                    //printf("**in*\n");
                    printf("Sending the Authentication\n");
                    send(client_socket,Auth_Message, sizeof(M_Authentication_SIZE),0);

                    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1e6;

                    //sum the all times of the part1
                    sum_for_average_cubic += time_spent;
                    //save each time of the second part inside the array (pointer)

                    *(arrTimePart1 + c1) = time_spent;
                    c1++;
                    printf("Time in microseconds: %f microseconds\n", time_spent);
                    checkPart = 1; // that help us to go inside the handle part2 ,in the second iteration to
                    // recv the part2 of the file
                    size2+=countbytes;
                    countbytes=0;



                } else { // To handle with  the second recv part


                    //gettimeofday(&end, 0);
                    bzero(Name_CC_Algo, sizeof(Name_CC_Algo)); // update
                    printf("\nSwitching to Reno algorithm\n");
                    strcpy(Name_CC_Algo, "reno"); // put the name of the cc algorithm inside the name algo
                    length = strlen(Name_CC_Algo);
                    //change the cc algorithm
                    if ((setsockopt(client_socket, IPPROTO_TCP, TCP_CONGESTION, Name_CC_Algo, length)) != 0) {

                        printf("setsockopt_receiver\n");

                        return -1;

                    }

                    //measure the time
                    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1e6;

                    //sum the all times of the part2
                    sum_for_average_reno += time_spent;

                    //save each time of the second part inside the array (pointer)
                    *(arrTimePart2 + c2) = time_spent;
                    c2++;
                    printf("Time in microseconds: %f microseconds\n", time_spent);
                    checkPart = 0;
                    size2+=countbytes;
                    countbytes=0;


                }

                arrTimePart1 = (double *) realloc(arrTimePart1, sizeof(double) * (c1 + 1));
                arrTimePart2 = (double *) realloc(arrTimePart2, sizeof(double) * (c2 + 1));


                printf("\nsize2==%d\n\n",size2);
                if (size2 == SIZEFILE) { // if we go inside then we rcv all the file

                    printf("All the file is recv done\n");
                    printf("\n**********\n");
                    // To get the decision of the user
                    bzero(User_Decision_MESSAGE, sizeof(User_Decision_MESSAGE));// update
                    printf("\n\n");
                    // get the message option from the user(sender)
                    recv(client_socket, &User_Decision_MESSAGE, sizeof(User_Decision_MESSAGE), 0);

                    if (User_Decision_MESSAGE[0] == Check_DECISION[0]) { // if it's equal to exit-message then stop


                    Bole =0;

                    break;
                    }
                    else{ //else continue
                        Bole =1;
                        size2=0;
                        printf("Receive %d\n",++t);
                    }




                }



            }




        }







    }while(Bole);

    // if we here that mean the user stop send file,then call the function to print all times and avg.
    printTimes(arrTimePart1, arrTimePart2, c1, sum_for_average_cubic, sum_for_average_reno);



    close(sockfd);
    return 0;
}






void printTimes(double *timePart1, double *timePart2, int len,double sum_Cubic,double sum_Reno) {



    printf("Times Summary:\n");
    printf("Part1:-\n");
    for (int j = 0; j < len; j++) {
        printf("%f\n", *(timePart1 + j));
    }

    printf("Part2:-\n");
    for (int j = 0; j < len; j++) {
        printf("%f\n", *(timePart2 + j));
    }

    double avg_cubic = sum_Cubic / len;
    double avg_reno = sum_Reno / len;

    printf("\n************\n");

    printf("Averages:\n");
    printf("Part1:\t%f\n", avg_cubic);

    printf("Averages:\n");
    printf("Part2:\t%f\n", avg_reno);

    printf("\nEND\n");

    free(timePart1);
    free(timePart2);


}






int setSock(){


    struct sockaddr_in server_addr;
    char namealgo[250];
    socklen_t length;
    // The  memset()  function  fills  the  first  n  bytes of the memory area
    //       pointed to by s with the constant byte c.
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT); // short, network byte order

    /*socket()  creates  an endpoint for communication and returns a file de‐
    scriptor that refers to that endpoint.  The file descriptor returned by
    a  successful call will be the lowest-numbered file descriptor not cur‐
    rently open for the process.*/
    // create a socket lisener.
    int sockfd = -1;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Couldn't create a socket listener : %d\n",errno);
        perror("socket\n");
        return -1;
    }

    /* getsockopt()  and  setsockopt()  manipulate  options for the socket re‐
       ferred to by the file descriptor sockfd.  Options may exist at multiple
       protocol levels; they are always present at the uppermost socket level.*/
    strcpy(namealgo,"cubic");
    length = sizeof(namealgo);
    if (getsockopt(sockfd,IPPROTO_TCP,TCP_CONGESTION,namealgo,&length)!=0){

        perror("getsockopt\n");
        return -1;
    }

    /*When a socket is created with socket(2), it exists in a name space (ad‐
       dress family) but has no address assigned to it.   bind()  assigns  the
       address  specified  by  addr  to the socket referred to by the file de‐
       scriptor sockfd.  addrlen specifies the size, in bytes, of the  address
       structure  pointed to by addr.  Traditionally, this operation is called
       “assigning a name to a socket”.*/
    // connect the server to a port which can read and write on.
    if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Bind failed with error code : %d" , errno);
        return -1;
    }

    printf("the server is ready!\n\n");

    /*listen()  marks  the  socket referred to by sockfd as a passive socket,
       that is, as a socket that will be used to  accept  incoming  connection
       requests using accept(2).

       The  sockfd  argument  is  a file descriptor that refers to a socket of
       type SOCK_STREAM or SOCK_SEQPACKET.*/
    if(listen(sockfd, 500) == -1) {
        printf("listen() failed with error code : %d",errno);
        return -1;
    }

    return sockfd; // return the socket that we create.
}



