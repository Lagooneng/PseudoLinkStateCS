#include<stdio.h>
#include<string.h>
#include<cstring>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<iostream>
#define MAXBUFFER 100
#define NODELENGTH 9
using namespace std;

const int INF = 2000000000;

int main() {
    int cs;
    char recvBuf[100], sendBuf[100];
    struct sockaddr_in csa;

    memset(&csa, 0, sizeof(csa));
    csa.sin_family = AF_INET;
    csa.sin_addr.s_addr = inet_addr("127.0.0.1");
    csa.sin_port = htons(12345);

    cs = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (cs < 0)
    {
        std::cout << "소켓 생성 실패" << std::endl;
        close(cs);
        return -1;
    }
    
    int n = connect(cs, (struct sockaddr *) &csa, sizeof(csa));
    if (n < 0)
    {
        std::cout << "연결 실패" << std::endl;
        close(cs);
        return -1;
    }

    // 본인의 인덱스
    int idx = 3;
    // 변경 사항이 담긴 배열
    int dist[NODELENGTH] = {INF, 0, 7, INF, 9, 14, 1, INF, INF};

    memcpy(sendBuf, &idx, sizeof(int));
    for (int i = 1; i <= NODELENGTH; i++)
    {
        memcpy(sendBuf + i * sizeof(int), &(dist[i - 1]), sizeof(int));
    }

    send( cs, sendBuf, MAXBUFFER, 0 );


    recv( cs, recvBuf, MAXBUFFER, 0);

    for (int i = 0; i < NODELENGTH; i++)
    {
        int weight;
        memcpy(&weight, recvBuf + i * sizeof(int), sizeof(int));
        if ( i == idx )
        {
            cout << "Node: " << i << " 본인" << endl;
        }
        else if ( weight == 0 ){
            cout << "Node: " << i << " 도달 불가" << endl;
        }
        else {
            cout << "Node: " << i << " Weight: " <<  weight << endl;
        }
    }
    

    close(cs);

    return 0;
}