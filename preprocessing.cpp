#include <stdio.h>
#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <string>
#include "RPCServer.hpp"
#include "Config.hpp"

using namespace jsonrpc;

int main(int argc, const char *argv[])
{
    try
    {
        Config config;

        // Temporary
        // These values should be parsed from the main YAML config file
        config.original_dir = "/opt/er/images/original";
        config.preprocessed_dir = "/opt/er/images/preprocessed";

        HttpServer server(8383);
        RPCServer serv(server, config);
        if (serv.StartListening())
        {
            std::cout << "Server started successfully" << std::endl;
            getchar();
            serv.StopListening();
        }
        else
        {
            std::cout << "Error starting Server" << std::endl;
        }
    }
    catch (jsonrpc::JsonRpcException& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
