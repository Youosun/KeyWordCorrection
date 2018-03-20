#ifndef _SM_SOCKETIO_H__
#define _SM_SOCKETIO_H__

#include <stdio.h>

namespace SM
{

class SocketIO
{
public:
	SocketIO(int sockfd);

	size_t readn(char * buf, size_t count);
	size_t writen(const char * buf, size_t count);
	size_t readline(char * buf, size_t max_len);
private:
	size_t recv_peek(char * buf, size_t count);

private:
	int _sockfd;
};
}//end of namespace SM

#endif
