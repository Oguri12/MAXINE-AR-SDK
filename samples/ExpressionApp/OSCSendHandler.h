#include <string>
#include <stdio.h>
#include <vector>

#include "OscReceivedElements.h"
#include "OscPacketListener.h"
#include "OscOutboundPacketStream.h"
#include "UdpSocket.h"
#include "IpEndpointName.h"

#define OUTPUT_BUFFER_SIZE 8192

class OSCSendHandler
{
public:
	OSCSendHandler();
	~OSCSendHandler();
	void connectSocket(std::string ip_address, int port);
	void send(std::vector<float> expressions);

private:
	static const char* _psNames[];
	static const int _psExprIndex[];
	
	UdpTransmitSocket* transmitSocket;
};

