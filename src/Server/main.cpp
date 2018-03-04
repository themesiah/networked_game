#include "ServerEngine\ServerEngine.h"

class TestObject : public SerializableObject {
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
	enum { kClassId = 'TEST' };
	virtual uint32_t GetClassId() override {
		return kClassId;
	}
};

const float TestObject::defaultB = 5.0f;

class TestObject2 : public TestObject {
public:
	TestObject2() : TestObject() {

	}
	int f;
	static DataType* GetReflectionData() {
		return new DataType({
			MemberVariable("a", EPT_Int, OffsetOf(TestObject2, a), true, &defaultA),
			MemberVariable("b", OffsetOf(TestObject2, b), 0.0f, 0.1f, true, &defaultB),
			MemberVariable("c", EPT_Bool, OffsetOf(TestObject2, c)),
			MemberVariable("c2", EPT_Bool, OffsetOf(TestObject2, c2)),
			MemberVariable("c3", EPT_Bool, OffsetOf(TestObject2, c3)),
			MemberVariable("d", EPT_String, OffsetOf(TestObject2, d)),
			MemberVariable("e", EPT_MapIntInt, OffsetOf(TestObject2, e)),
			MemberVariable("f", EPT_Int, OffsetOf(TestObject2, f))
		}
		);
	}
	enum { kClassId = 'TES2' };
	virtual uint32_t GetClassId() override {
		return kClassId;
	}
};

int main()
{
	CServerEngine& lServer = CServerEngine::GetInstance();
	lServer.Init();

	while (1)
	{
		lServer.Update();
	}
	//return DoMoveTest();
	/*SET_REFLECTION_DATA(TestObject2);
	TestObject2 testObject1 = TestObject2();
	testObject1.a = 4;
	testObject1.b = 52.35f;
	testObject1.c = true;
	testObject1.c2 = false;
	testObject1.c3 = true;
	testObject1.d = std::string("Hello serializer!");
	testObject1.e = std::map<int, int>();
	testObject1.e[0] = 0; testObject1.e[1] = 10; testObject1.e[5] = 50; testObject1.e[70] = 700;
	testObject1.f = 56;
	OutputMemoryBitStream *output = new OutputMemoryBitStream();
	testObject1.Serialize(output);
	char* buffer = NULL;
	size_t size = (output->GetByteLength());
	buffer = static_cast<char*>(std::realloc(buffer, size));
	memcpy(buffer, output->GetBufferPtr(), size);
	delete output;
	InputMemoryBitStream *input = new InputMemoryBitStream(buffer, size);
	TestObject2 testObject2 = TestObject2();
	testObject2.Serialize(input);
	delete input;

	std::cout << "Finish" << std::endl;
	return 0;*/
}