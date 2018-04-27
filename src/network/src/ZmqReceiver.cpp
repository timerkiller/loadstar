#include "ZmqReceiver.hpp"
#include <boost/exception/diagnostic_information.hpp>
#include <gsl/gsl_assert>

namespace network
{
void ZmqReceiver::registerReceiver(zmq::socket_t&& socket, std::function<void(const zmq::message_t&)> messageHandler)
{
    zmqSockets.push_back(std::move(socket));
    auto& addedSocket = zmqSockets.back();

    std::function<void()> callback =
        [&addedSocket, handler = std::move(messageHandler), &received = this->received, &wrapper = this->wrapper ]()
    {
        wrapper.recv(addedSocket, received);
        handler(received);
    };

    registerReceiver(addedSocket, std::move(callback));
}

void ZmqReceiver::registerReceiver(zmq::socket_t& socket, std::function<void()> callback)
{
    zmq_pollitem_t pollItem{socket, 0, ZMQ_POLLIN, 0};
    insertPollItemAndCallback(pollItem, std::move(callback));
}

void ZmqReceiver::shutdown()
{
    stop = true;
}

void ZmqReceiver::registerReceiverForFileDescriptor(
    const ZmqReceiver::FileDescriptor descriptor,
    std::function<void()> callback)
{
    zmq_pollitem_t pollItem{nullptr, descriptor, ZMQ_POLLIN, 0};
    insertPollItemAndCallback(pollItem, std::move(callback));
}

void ZmqReceiver::removeReceiverForFileDescriptor(ZmqReceiver::FileDescriptor descriptor)
{
    for (auto i = 0u; i < updatedPollItems.size(); ++i)
    {
        if (updatedPollItems.at(i).fd == descriptor)
        {
            updatedPollItems.erase(updatedPollItems.begin() + i);
            updatedCallbacks.erase(updatedCallbacks.begin() + i);
            return;
        }
    }
    BOOST_THROW_EXCEPTION(std::out_of_range("Trying to deregister unknown file descriptor"));
}

void ZmqReceiver::receiveLoop() try
{
    if(pollItems.size() != callbacks.size())
    {
        std::terminate();
    }

    while (!stop)
    {
        pollItems = updatedPollItems;
        callbacks = updatedCallbacks;
        wrapper.poll(pollItems);
        for (auto i = 0u; i < pollItems.size(); ++i)
        {
            if (pollItems.at(i).revents & ZMQ_POLLIN)
            {
                callbacks.at(i)();
            }
        }
    }
}
catch (...)
{
    throw;
}

void ZmqReceiver::removeReceiver(const zmq::socket_t &socket)
{
    for (std::size_t i = 0; i < updatedPollItems.size(); i++)
    {
        if (updatedPollItems[i].socket == socket)
        {
            updatedPollItems.erase(updatedPollItems.begin() + i);
            updatedCallbacks.erase(updatedCallbacks.begin() + i);
            return;
        }
    }
    BOOST_THROW_EXCEPTION(std::out_of_range("Trying to deregister unknown socket"));
}

void ZmqReceiver::insertPollItemAndCallback(const zmq_pollitem_t pollItem, std::function<void()> callback)
{
    updatedPollItems.push_back(pollItem);
    updatedCallbacks.push_back(std::move(callback));
}
}
