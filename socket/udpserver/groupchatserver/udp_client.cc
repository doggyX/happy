#include <iostream>
#include <memory>
#include "udp_client.hpp"

static void usage(const std::string &proc)
{
    std::cout << "\n"
              << "Usage: " << proc << " serverip serverport" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        usage(argv[0]);
        exit(1);
    }

    std::string serverip = argv[1];
    uint16_t serverport = atoi(argv[2]);

    std::unique_ptr<UdpClient> cli(new UdpClient(serverip, serverport));

    cli->initClient();
    cli->run();
}