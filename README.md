# Banned Lands Online
## About the project
I always wanted to make games. Also, I wanted to make online games. Getting experience from a job without having previous experience is difficult, so I decided to make a game engine (or maybe a game) that is networked and is up to the standards of the industry.

First, I want - need - to make a disclaimer: I'm not so used to C++. It's possible that I'm making a mess of C and C++ code and mixing programming styles. If you see something that burns your eyes and kill your soul, feel free to contact me and tell me how bad I'm doing.

Well, returning to the project: I always wanted to make an MMORPG. Yeah, yeah, I know, but let me explain it full. This is not __The MMORPG__. This will be __An MMORPG__. Little, simple, and maybe ugly. But it will __work__. It will be the game engine of a possible bigger MMORPG, but nothing more (for now).

At the time of writting this, I'm not sure of how the game will be. Right now I want a turn-based roguelike 2D MMORPG. Except for the roguelike thing (which I simply liked), there is a reason for choosing this genre.
* MMORPG: I want to learn how to manage multiple connections. A lot if possible. And how to optimise a server for a client-server architecture for a lot of players to connect at once.
* Turn-based: Well, there is no need to be an expert in networking to see that a turn-based game over the internet gives a lot less to think about than a real-time game. I wanted to keep it simple and focus myself on the task of replicating data and sending commands between client and server. If this game works, I'm planning on doing another and very different one with less players, real time and focusing on movement prediction.
* 2D: Not a network decision. I feel like working with 2D or 3D for the kind of game I want to make will be almost the same. I only wanted to make the graphical part easier.

This said, you can ask me any questions you like about the engine. I'm motivated and love this area of game programming, so I will be really pleased!

## Art disclaimer
I do not own any of the art used for this project. This is a project for learning and I don't plan on getting any money for this. If you see your art here and don't feel like I deserve to use it :disappointed: I will remove it quickly.

## Build
The project is made in Visual Studio 2013, and for now is Windows only. All the dependencies of the project are included in the lib and include folders and the project should be copy-and-compile as is.

To try it, you have to run the Server.exe first and then Client.exe. If you want to try with a remote machine, the file **_server.txt_** in the Data folder must be changed to __[SERVER_IP]:[PORT]__.

## Engine Explanation
I don't want to write a lot about it but... kidding, with how motivated I am I would write a book. But I don't have time to do so, if I want to keep coding. But there are some things that should really be explained. I will try to keep it high level and simple, but feel free to ask to explain something with more detail and I will comment it in the code. Yep. Right now, there are almost no comments.

- [ ] Write some comments

I say I would write a book, but in fact a lot (like a 80%) of the network code is based on a [very nice book](https://www.amazon.com/Multiplayer-Game-Programming-Architecting-Networked-ebook/dp/B0189RXWJQ) about multiplayer game programming that obviously I read to be able to do this. Right now the project is at a point where I do my own code, but some of the classes are (or were in a certain moment) a copy paste of the ones at the book.

The client side of the engine is made with SFML 2.0, but I will keep the explanations of this part to a minimum, as those are not the parts I'm interested in for this project. Again, feel free to ask.

### The Socket wrapper
All network directly related code is in the Network project. The sockets are in the Socket folder, along with the address factory and other utilities. Right now there are an UDP and a TCP socket wrapper that do nothing more than wrap the sockets in a C++ fashion. For this project I'm only using the TCP one.

### The Serializer
Almost a copy of the one in the book at some points, but still my little pride. It serializes any basic type, vectors, maps, strings and all is serialized __bit-wise__.

I made also a simple reflection system for serializing full classes in one function, a compression system that writes default values as a single bit and a float compressor. Now, I will explain what all of this means:

> Imagine we have an hypothetical 2D game with a transform class. A transform class have a 2-sized float vector for the position (4+4 bytes), a 2-sized float vector for the scale (4+4 bytes) and a float for the rotation. That's 20 bytes. Per player. In this hypothetical game we will have around 100 players per server. That's 2000 bytes. With the model I chose the server send the game state 10 times per second. 20000 bytes per second, or 19.5kb.
> But you know, in this game the people jump, and almost all the time they are on the ground. The Y position is 0.0 like 99% of the time. Also, the scale is 1.0 almost all the time. And our floats do not require more than a single decimal precision.

At this point we can see that there is a lot of useless data in our serializing, and we want a way to avoid this overhead in bytes.
- The serializer compresses floats (4 bytes) in uint16_t variables (2 bytes) losing precision. For a single decimal precision this is ok. So our floats are now only 2 bytes.
- The serializer can serialize a bool in 1 single bit. For every float that is usually the same value always (like 0.0 for the Y position) we serialize a bool first. If it's false, we serialize nothing more. The Y position is 0.0. If we serialize a true, we have to serialize the float, because it's not the default value. At worst, we have an overhead of 1 bit.

> Returing to our hypothethical 2D game, we have now big news for our transform class serialization. Our floats are now 2 bytes each one, and every variable in the transform is a float! Also, for 3 of our 5 variables we can use our little trick of default values. So let's do the math. 2 variables of 2 bytes are 4 bytes. The other three are 3 more bits (rounded up, it's 1 byte). At the worst case, those 3 variables are 6 bytes + 3 bits (again, rounded up one byte). But that's 1% of the time. More maths! 4 bytes + (7 bytes * 0.01) + (1 byte * 0.99). Is like 4 bytes + 1 byte. That means that our 20 byte transform is now a 5 bytes transform. We reduced the needed network throughput from 20000 bytes per second to 5000 bytes per second. Cool. Pretty cool.

And that's what the serializer does. There's some example code of how this hypothethical transform class would look in my engine.

```
// Transform2D.h

const float DEFAULT_Y = 0.0f;
const float DEFAULT_SCALE = 1.0f;
class Transform2D : public GameObject
{
public:
	Transform2D() : GameObject()
		, m_PosX(0.f)
		, m_PosY(0.f){}
	~Transform2D() {}
	static DataType* GetReflectionData() {
		return new DataType({
			MemberVariable("m_PosX", OffsetOf(Transform2D, m_PosX), -1000.0f, 0.1f),
			MemberVariable("m_PosY", OffsetOf(Transform2D, m_PosY), -100.0f, 0.1f, true, &DEFAULT_Y),
            MemberVariable("m_Direction", OffsetOf(Transform2D, m_Direction), 0.0f, 0.1f),
			MemberVariable("m_ScaleX", OffsetOf(Transform2D, m_ScaleX), 0.0f, 0.1f, true, &DEFAULT_SCALE),
            MemberVariable("m_ScaleY", OffsetOf(Transform2D, m_ScaleY), 0.0f, 0.1f, true, &DEFAULT_SCALE)
		}
		);
	}
    CLASS_IDENTIFICATION('TRNF', Transform2D);
private:
	float m_PosX;
	float m_PosY;
    float m_Direction;
    float m_ScaleX;
    float m_ScaleY;
};
```

The __MemberVariable__ lines are the most important ones. The parameters are the name, the byte offset of the variable in the class (that's what the __OffsetOf__ macro does!), the minimum float size of the variable (used for the compression), the compression precision and (optional) true for checking if is a default value and then a pointer to the default value. Again: Feel free to ask, but this is mostly covered in the book!

#### TCP Streams
TCP works in streams. I failed to notice this at first and didn't know how to solve it. But a little trick with the serializer and a helper class named __PacketStream__ helped me solve it. The __OutputMemoryBitStream__ (the serializer class to write from a class to the network) have a __Close__ function that writes the size of the stream in the header of the stream, using 2 bytes. This function should be called always before doing a TCPSocket.Send(), and will trigger an assert if you don't do it. When the destination machine receives the stream, it receives a int with the received data size, and a buffer with the data. I write this data in the __PacketStream__. Then I call __ReadPacket__ from the same class. This reads the first two bytes (the size of the packet) and returns the data if it have the full packet and does nothing otherwise. Cool. This way I can manage sending multiple packets in the same frame without fear!

#### Serializer limitations
Right now, the serializer is written for ___little endian___ machines. As I only want it for Windows right now I don't really have the reasons to change this. Change my mind.

__TODO__: Write about replication and network manager.