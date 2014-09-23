/*
* compile and run:
* g++ -std=c++11 -o a server.cpp; time ./a
*
* requires gcc 4.8 or above
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

const uint64_t m1  = 0x5555555555555555; //binary: 0101...
const uint64_t m2  = 0x3333333333333333; //binary: 00110011..
const uint64_t m4  = 0x0f0f0f0f0f0f0f0f; //binary:  4 zeros,  4 ones ...
const uint64_t m8  = 0x00ff00ff00ff00ff; //binary:  8 zeros,  8 ones ...
const uint64_t m16 = 0x0000ffff0000ffff; //binary: 16 zeros, 16 ones ...
const uint64_t m32 = 0x00000000ffffffff; //binary: 32 zeros, 32 ones
const uint64_t hff = 0xffffffffffffffff; //binary: all ones
const uint64_t h01 = 0x0101010101010101; //the sum of 256 to the power of 0,1,2,3...

int main () {
  cout << "loading file..." << endl;
  string line;
  ifstream myfile ("/tmp/data");
  if (myfile.is_open()) {
    int cnt = 0, bad_cnt = 0, bit_count = 0;
    uint64_t inp = 10213218608714346503;
    uint64_t shifted_inp = inp >> 1;
    uint64_t num, x;
    int i = 20000000, total_count = 0;
    //uint64_t *lines = new uint64_t[i];
    vector<uint64_t> lines(20000000);
    while ( getline (myfile, line) ) {
      try {
        num = stoul(line, NULL, 0);
        //*lines = num;
        //lines++;
        lines[total_count] = num;
        total_count++;
      } catch (...) {
        bad_cnt++;
      }
    }
    cout << "total count: " << total_count << endl;
    cout << "bad count: " << bad_cnt << endl;
    myfile.close();


    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 11600;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
          sizeof(serv_addr)) < 0)
    {
      perror("ERROR on binding");
      exit(1);
    }
    /* Now start listening for the clients, here
     * process will go in sleep mode and will wait
     * for the incoming connection
     */
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    while (1) {
      newsockfd = accept(sockfd,
          (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
      if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(1);
      }
      /* Create child process */
      int pid = fork();
      if (pid < 0) {
        perror("ERROR on fork");
        exit(1);
      }
      if (pid == 0) {
        /* This is the client process */
        close(sockfd);

        // newsockfd
        // do I have a off-by-one error? big freaking deal, so what? wanna fight about it? @soheil
        for (int j = 0; j < total_count; j++) {
          //while (--lines) {
          try {
            num = lines[j];
            //num = *lines;
            x = inp ^ num;

            x -= (x >> 1) & m1;             //put count of each 2 bits into those 2 bits
            x = (x & m2) + ((x >> 2) & m2); //put count of each 4 bits into those 4 bits
            x = (x + (x >> 4)) & m4;        //put count of each 8 bits into those 8 bits
            bit_count = (x * h01)>>56;      //returns left 8 bits of x + (x<<8) + (x<<16) + (x<<24) + ...

            if (bit_count < 5) cout << j << ":" << num << endl;
          } catch (...) {

          }
        }
        exit(0);
      } else {
        close(newsockfd);
      }
    } /* end of while */

  }

  else cout << "Unable to open file";
  return 0;
}
