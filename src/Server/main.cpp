#include <iostream>
#include <vector>

#include "Common\TCPSocket.h"
#include "Common\SocketUtil.h"
#include "Common\SocketAddressFactory.h"


#include "Serializer\InputMemoryBitStream.h"
#include "Serializer\MemoryStream.h"
#include "Serializer\SerializableObject.h"

#define GOOD_SEGMENT_SIZE 1024

void ProcessNewClient(TCPSocketPtr newSocket, SocketAddress newClientAddress)
{
	std::cout << "New connection" << std::endl;
}

class CPosition : public SerializableObject<CPosition> {
public:
	CPosition() {
	}
	float posx;
	float posy;
	static DataType* GetReflectionData() {
		return new DataType({
			MemberVariable("posx", OffsetOf(CPosition, posx), -1000.0f, 0.1f),
			MemberVariable("posy", OffsetOf(CPosition, posy), -1000.0f, 0.1f)
		}
		);
	}
};

void ProcessDataFromClientPos(const TCPSocketPtr& socket, char* segment, int dataReceived, CPosition* pos)
{
	socket->Send(segment, dataReceived);
	/*InputMemoryBitStream* input = new InputMemoryBitStream(segment, dataReceived);
	pos->Unserialize(input);
	MemoryStream* output = pos->Serialize();
	socket->Send(output->GetBufferPtr(), output->GetByteLength());*/
	//delete input;
	//delete segment;
}

void ProcessDataFromClient(const TCPSocketPtr& socket, char* segment, int dataReceived)
{
}

int DoMoveTest() {
	SET_REFLECTION_DATA(CPosition);
	CPosition* pos = new CPosition();
	pos->posx = 300.f;
	pos->posy = 300.f;
	SocketUtil::InitSockets();
	TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(INET);
	SocketAddress receivingAddress(INADDR_ANY, 48000);
	if (listenSocket->Bind(receivingAddress) != NO_ERROR)
	{
		return -1;
	}

	if (listenSocket->Listen() != NO_ERROR)
	{
		return -1;
	}
	std::vector<TCPSocketPtr> readBlockSockets;
	readBlockSockets.push_back(listenSocket);
	std::vector<TCPSocketPtr> readableSockets;

	std::vector<TCPSocketPtr> errorBlockSockets;
	//errorBlockSockets.push_back(listenSocket);
	std::vector<TCPSocketPtr> errorableSockets;

	bool gIsGameRunning = true;

	while (gIsGameRunning)
	{
		if (SocketUtil::Select(&readBlockSockets, &readableSockets,
			nullptr, nullptr,
			//&errorBlockSockets, &errorableSockets))
			nullptr, nullptr))
		{
			/*for (const TCPSocketPtr& socket : errorableSockets)
			{
			auto it = std::find(readableSockets.begin(), readableSockets.end(), socket);
			readableSockets.erase(it);
			it = std::find(errorableSockets.begin(), errorableSockets.end(), socket);
			errorableSockets.erase(it);
			}*/
			//we got a packet様oop through the set ones...
			for (const TCPSocketPtr& socket : readableSockets)
			{
				if (socket == listenSocket)
				{
					//it's the listen socket, accept a new connection
					SocketAddress newClientAddress;
					auto newSocket = listenSocket->Accept(newClientAddress);
					readBlockSockets.push_back(newSocket);
					//errorBlockSockets.push_back(newSocket);
					ProcessNewClient(newSocket, newClientAddress);
				}
				else
				{
					//it's a regular socket用rocess the data...
					char segment[GOOD_SEGMENT_SIZE];
					FD_ZERO(segment);
					int dataReceived = socket->Receive(segment, GOOD_SEGMENT_SIZE);
					if (dataReceived > 0)
					{
						if (strcmp(segment, "END") == 0)
						{
							gIsGameRunning = false;
						}
						else {
							ProcessDataFromClientPos(socket, segment, dataReceived, pos);
						}
					}
				}
			}
		}
	}
	return 0;
}

int DoChatLoop() {
	SocketUtil::InitSockets();
	TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(INET);
	SocketAddress receivingAddress(INADDR_ANY, 48000);
	if (listenSocket->Bind(receivingAddress) != NO_ERROR)
	{
		return -1;
	}

	if (listenSocket->Listen() != NO_ERROR)
	{
		return -1;
	}
	std::vector<TCPSocketPtr> readBlockSockets;
	readBlockSockets.push_back(listenSocket);
	std::vector<TCPSocketPtr> readableSockets;

	std::vector<TCPSocketPtr> errorBlockSockets;
	//errorBlockSockets.push_back(listenSocket);
	std::vector<TCPSocketPtr> errorableSockets;

	bool gIsGameRunning = true;
	while (gIsGameRunning)
	{
		if (SocketUtil::Select(&readBlockSockets, &readableSockets,
			nullptr, nullptr,
			//&errorBlockSockets, &errorableSockets))
			nullptr, nullptr))
		{
			/*for (const TCPSocketPtr& socket : errorableSockets)
			{
				auto it = std::find(readableSockets.begin(), readableSockets.end(), socket);
				readableSockets.erase(it);
				it = std::find(errorableSockets.begin(), errorableSockets.end(), socket);
				errorableSockets.erase(it);
			}*/
			//we got a packet様oop through the set ones...
			for (const TCPSocketPtr& socket : readableSockets)
			{
				if (socket == listenSocket)
				{
					//it's the listen socket, accept a new connection
					SocketAddress newClientAddress;
					auto newSocket = listenSocket->Accept(newClientAddress);
					readBlockSockets.push_back(newSocket);
					//errorBlockSockets.push_back(newSocket);
					ProcessNewClient(newSocket, newClientAddress);
				}
				else
				{
					//it's a regular socket用rocess the data...
					char segment[GOOD_SEGMENT_SIZE];
					FD_ZERO(segment);
					int dataReceived = socket->Receive(segment, GOOD_SEGMENT_SIZE);
					if (dataReceived > 0)
					{
						if (strcmp(segment, "END") == 0)
						{
							gIsGameRunning = false;
						}
						else {
							ProcessDataFromClient(socket, segment, dataReceived);
						}
					}
				}
			}
		}
	}
	return 0;
}

class TestObject : public SerializableObject<TestObject> {
public:
	TestObject() {
	}
	static const int defaultA = 4;
	static const float defaultB;
	int a;
	float b;
	bool c;
	bool c2;
	bool c3;
	std::string d;
	std::map<int, int> e;
	static DataType* GetReflectionData() {
		return new DataType({
			MemberVariable("a", EPT_Int, OffsetOf(TestObject, a), true, &defaultA),
			MemberVariable("b", OffsetOf(TestObject, b), 0.0f, 0.1f, true, &defaultB),
			MemberVariable("c", EPT_Bool, OffsetOf(TestObject, c)),
			MemberVariable("c2", EPT_Bool, OffsetOf(TestObject, c2)),
			MemberVariable("c3", EPT_Bool, OffsetOf(TestObject, c3)),
			MemberVariable("d", EPT_String, OffsetOf(TestObject, d)),
			MemberVariable("e", EPT_MapIntInt, OffsetOf(TestObject, e))
		}
		);
	}
};

const float TestObject::defaultB = 5.0f;

int main()
{
	//return DoChatLoop();
	return DoMoveTest();
	/*SET_REFLECTION_DATA(TestObject);
	TestObject testObject1 = TestObject();
	testObject1.a = 4;
	testObject1.b = 52.35f;
	testObject1.c = true;
	testObject1.c2 = false;
	testObject1.c3 = true;
	testObject1.d = std::string("Hello serializer!");
	testObject1.e = std::map<int, int>();
	testObject1.e[0] = 0; testObject1.e[1] = 10; testObject1.e[5] = 50; testObject1.e[70] = 700;
	MemoryStream *output = testObject1.Serialize();
	char* buffer = NULL;
	size_t size = (output->GetByteLength());
	buffer = static_cast<char*>(std::realloc(buffer, size));
	memcpy(buffer, output->GetBufferPtr(), size);
	delete output;
	MemoryStream *input = new InputMemoryBitStream(buffer, size);
	TestObject testObject2 = TestObject();
	testObject2.Unserialize(input);
	delete input;

	std::cout << "Finish" << std::endl;
	return 0;*/
}