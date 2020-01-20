
#ifndef __HRD11_DRIVER_PROXY_HPP__
#define __HRD11_DRIVER_PROXY_HPP__

namespace hrd11
{

class Requset;
class Replay;

class DriverProxy
{

public:
    virtual void ReciveRequest(Requset& req) = 0;
    virtual void SendReplay(Replay& rep) = 0;
    virtual void Disconnect() = 0;
    int GetSocket();

private:
    int m_sock_recv;
    int m_sock_send;

};

}   // namespace hrd11

#endif // __HRD11_DRIVER_PROXY_HPP__
