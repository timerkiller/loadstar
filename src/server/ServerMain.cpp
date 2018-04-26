#include <zmq.hpp>
#include <iostream>
#include <unistd.h>
#include <boost/variant.hpp>

struct MyVisitor: public boost::static_visitor<>
{
    void operator()(int& i)
    {
        std::cout << "the i value: " << i << std::endl;
    }

    void operator()(const std::string& str)
    {
        std::cout << "the str :" <<str << std::endl;
    }
};

int main()
{
    //  Prepare our context and socket
    boost::variant<int, std::string> test{"hello"};
    MyVisitor visitor;
    boost::apply_visitor(visitor, test);

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");
    int count{0};
    while (true)
    {
        zmq::message_t request;

        //  Wait for next request from client
        socket.recv(&request);
        std::cout << "Received Hello" << std::string{(char*)request.data()} << std::endl;

        //  Do some 'work'
        sleep(1);

        //  Send reply back to client
        zmq::message_t reply(5);
        memcpy((void *) reply.data(), "World", 5);
        socket.send(reply);

        if (count++ > 10)
        {
            break;
        }
    }

    std::cout << "server exit(0)" << std::endl;
    return 0;
}