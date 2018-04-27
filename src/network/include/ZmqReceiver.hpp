#pragma once

#include <functional>
#include <list>
#include <utility>
#include <zmq.hpp>

namespace network
{
class ZmqReceiver
{
    using FileDescriptor = int;

public:
    struct ZmqWrapper
    {
        using PollType = std::function<void(std::vector<zmq_pollitem_t>&)>;
        using RecvType = std::function<void(zmq::socket_t&, zmq::message_t&)>;

        PollType poll = [](auto& items) { zmq::poll(items); };
        RecvType recv = [](auto& s, auto& m) { s.recv(&m); };
    };

    void registerReceiver(zmq::socket_t&&, std::function<void(const zmq::message_t&)> messageHandler);
    void registerReceiver(zmq::socket_t&, std::function<void()> callback);
    void registerReceiverForFileDescriptor(FileDescriptor, std::function<void()> callback);
    void removeReceiverForFileDescriptor(ZmqReceiver::FileDescriptor descriptor);
    void removeReceiver(const zmq::socket_t &);
    void receiveLoop();
    void shutdown();

    ZmqReceiver() = default;
    explicit ZmqReceiver(ZmqWrapper customWrapper) : wrapper(std::move(customWrapper)) {}

private:
    void insertPollItemAndCallback(zmq_pollitem_t, std::function<void()>);
    std::list<zmq::socket_t> zmqSockets;
    std::vector<zmq_pollitem_t> pollItems;
    std::vector<std::function<void()>> callbacks;

    std::vector<zmq_pollitem_t> updatedPollItems;
    std::vector<std::function<void()>> updatedCallbacks;

    zmq::message_t received;

    bool stop{false};
    ZmqWrapper wrapper;
};
}
