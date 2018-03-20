#ifndef _SM_INETADDRESS_H__
#define _SM_INETADDRESS_H__

#include <netinet/in.h>
#include <string>

namespace SM
{
class InetAddress
{
public:
	InetAddress(short port);
	InetAddress(const char * pIp, short port);
	InetAddress(const struct sockaddr_in & addr);

	const struct sockaddr_in * getSockAddrPtr()const;
	std::string ip()const;
	unsigned short port()const;
private:
	struct sockaddr_in _addr;
};
}

#endif
