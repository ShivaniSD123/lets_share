#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdint>
#include <filesystem>

namespace fs = std::filesystem;

// send-all to send the files
size_t size = 0;
ssize_t send_all(int sockfd, const void *w_buff, size_t n)
{
    const char *p = (const char *)w_buff;
    size_t left = n;
    while (left > 0)
    {
        ssize_t s = send(sockfd, p, left, 0);
        if (s < 0)
            return s;
        left -= s;
        p += s;
    }
    return n;
}

// reading the file into binary file
std::vector<char> readFiles(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
        std::cout << "No file found" << std::endl;
    file.seekg(0, std::ios::end);
    size = file.tellg();
    std::vector<char> buffer(size);
    file.seekg(0, std::ios::beg);
    if (file.read(buffer.data(), size))
        return buffer;
    std::cerr << "Could not read the whole file" << std::endl;
    return {};
}
// sending handler
int send_handler(int fd, std::vector<char> &file, const std::string &filename)
{

    uint32_t file_name_size = static_cast<uint32_t>(filename.size());
    uint64_t file_size = static_cast<uint64_t>(file.size());

    // total = 4 (filename length) + filename + 8 (file size) + file bytes
    size_t total = 4 + file_name_size + 8 + static_cast<size_t>(file_size);
    std::vector<char> buffer(total);

    memcpy(buffer.data(), &file_name_size, 4);
    memcpy(buffer.data() + 4, filename.data(), file_name_size);
    memcpy(buffer.data() + 4 + file_name_size, &file_size, 8);
    if (file_size > 0)
    {
        memcpy(buffer.data() + 12 + file_name_size, file.data(), static_cast<size_t>(file_size));
    }

    int err = send_all(fd, buffer.data(), buffer.size());
    std::cout << "Sent the whole file" << std::endl;
    (void)err;
    return 0;
}

int main()
{

    // obtaining socket handler
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "Socket created" << std::endl;
    if (fd < 0)
        abort();

    // binding the address
    std::cout << "Conntcted" << std::endl;
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.0.100");
    addr.sin_port = htons(1234);
    std::string filepath;
    std::getline(std::cin, filepath);
    std::string filename = fs::path(filepath).filename().string();

    int rv = connect(fd, (const sockaddr *)&addr, sizeof(addr));
    if (rv)
        abort();
    std::vector<char> buffer = readFiles(filepath);
    int sent = send_handler(fd, buffer, filename);
    close(fd);

    return 0;
}