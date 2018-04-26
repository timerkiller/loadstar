//
// Created by timerkiller on 2018/4/14.
#include <zmq.hpp>
#include <iostream>
#include <unistd.h>

int main()
{
    //  Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.setsockopt(ZMQ_NOBLOCK, 1);
    socket.connect("tcp://localhost:5555");
    int count{0};
    while (true)
    {
        zmq::message_t request{3};

        memcpy((void *) request.data(), "Hello", 5);
        //  Wait for next request from client
        socket.send(request);

        zmq::message_t response;
        socket.recv(&response);
        std::cout<< response.data()<< std::endl;
        if (count++ > 5)
        {
            break;
        }
        sleep(3);
    }
    return 0;
}