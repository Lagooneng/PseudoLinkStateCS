#include<string>
#include<cstring>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<iostream>
#include<cctype>
#include<vector>
using namespace std;
#define MAXBUFFER 100
#define NODELENGTH 9

const int INF = 2000000000;

int nextV(vector<int> &dist, vector<bool> &visited) {
    int idx = -1;
    int min = INF;
    
    for (int i = 0; i < NODELENGTH; ++i) {
        if (!visited[i] && dist[i] <= min) {
            min = dist[i];
            idx = i;
        }
    }
        
    return idx;
}

vector<int> dijkstra(int graph[NODELENGTH][NODELENGTH], int start) {
    vector<int> dist(NODELENGTH, INF); 
    vector<bool> visited(NODELENGTH, false); 

    dist[start] = 0; 

    for (int count = 0; count < NODELENGTH - 1; count++) {
        int now = nextV(dist, visited);

        visited[now] = true; 

        for (int v = 0; v < NODELENGTH; v++) {
            if (!visited[v] && graph[now][v] &&
                    dist[now] != INF && 
                    dist[now] + graph[now][v] < dist[v]) {
                dist[v] = dist[now] + graph[now][v];
            }
        }
    }

    return dist;
}

int main() {
    int ss, sa;
    char recvBuf[100], sendBuf[100];
    struct sockaddr_in ssa;

    memset(&ssa, 0, sizeof(ssa));
    ssa.sin_family = AF_INET;
    ssa.sin_addr.s_addr = htonl(INADDR_ANY);
    ssa.sin_port = htons(12345);

    ss = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if ( ss < 0 )
    {
        cout << "서버 오픈 실패" << endl;
        return -1;
    }
    
    bind(ss, (struct sockaddr *)&ssa, sizeof(ssa));
    int n = listen(ss, 10);
    if ( n < 0 )
    {
        cout << "listen() 실패" << endl;
        return -1;
    }

    cout << "Pseudo Link State Server Start" << endl;

    int graph[NODELENGTH][NODELENGTH] = {
        {0, 4, INF, INF, INF, INF, INF, 8, INF},
        {4, 0, 8, INF, INF, INF, INF, 11, INF},
        {INF, 8, 0, 7, INF, 4, INF, INF, 2},
        {INF, INF, 7, 0, 9, 14, INF, INF, INF},
        {INF, INF, INF, 9, 0, 10, INF, INF, INF},
        {INF, INF, 4, 14, 10, 0, 2, INF, INF},
        {INF, INF, INF, INF, INF, 2, 0, 1, 6},
        {8, 11, INF, INF, INF, INF, 1, 0, 7},
        {INF, INF, 2, INF, INF, INF, 6, 7, 0}
    };
    

    while (1)
    {
        sa = accept(ss, 0, 0);
        cout << "Client is detected-" << endl;
        recv(sa, recvBuf, 100, 0);  // 클라이언트한테 이웃 정보를 받음, 첫 바이트는 자신

        int idx;
        memcpy(&idx, recvBuf, sizeof(int));

        for (int i = 1; i <= NODELENGTH; ++i )
        {
            int weight;
            memcpy(&weight, recvBuf + i * sizeof(int), sizeof(int));
            graph[idx][i - 1] = weight;
        }

        vector<int> dist = dijkstra(graph, idx);
        
        for (int i = 0; i < NODELENGTH; i++)
        {
            memcpy(sendBuf + i * sizeof(int), &(dist[i]), sizeof(int));
        }
        
        send(sa, sendBuf, MAXBUFFER, 0);
        close(sa);
    }
    
    return 0;
}

