#include <iostream>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>

using namespace std;

// recieving_all
static int recv_all(int fd, void *buff, size_t n)
{
    char *p = (char *)buff;
    size_t left = n;
    while (left > 0)
    {
        size_t r = recv(fd, p, left, 0);
        if (r <= 0)
            return r;
        left -= r;
        p += r;
    }
    return n;
}

// recieving and saving in disk

void recvFile(int fd)
{
    char r_buff[8];
    int err = recv_all(fd, r_buff, 4);
    if (err <= 0)
    {
        cout << "Could not read file name size" << endl;
        return;
    }
    uint32_t len;
    memcpy(&len, r_buff, 4);
    vector<char> r_buff_name(len);
    err = recv_all(fd, r_buff_name.data(), len);
    string file_name = string(r_buff_name.data(), len);
    cout << "File Created-  " << file_name << endl
         << "Started recieving" << endl;
    // open or create file
    std::ofstream out(file_name, std::ios::binary);
    err = recv_all(fd, r_buff, 8);
    uint64_t file_len;
    memcpy(&file_len, r_buff, 8);
    if (file_len <= 0)
    {
        cout << "Corrupted file, can't download" << endl;
        return;
    }
    vector<char> file_buff(file_len);
    err = recv_all(fd, file_buff.data(), file_len);
    if (err <= 0)
    {
        cout << "Error occured while recieving file" << endl;
        return;
    }
    out.write(file_buff.data(), file_len);
    out.close();
    cout << "File saved successfully.." << endl;
}
int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        abort();
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int rv = ::bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv)
        abort();
    rv = listen(fd, SOMAXCONN);
    if (rv)
        abort();
    while (true)
    {
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
        if (connfd < 0)
            continue;
        recvFile(connfd);
        close(connfd);
    }
    return 0;
}
