#include "command_dispatcher.hpp"
#include <stdio.h>

void foo(int i, float j) {
    printf("foo(%d, %g)\n", i, j);
    return;
}

void bar(int i, float j, int k) {
    printf("bar(%d, %g, %d)\n", i, j, k);
    return;
}

void foobar() {
    printf("foobar()\n");
    return;
}

void flash(uint32_t address, std::string hexString, uint32_t crc) {
    printf("flash(%x, %s, %x)\n", address, hexString.c_str(), crc);
    return;
}

struct TestParameter {
    std::vector<int> test;
};

template<> 
TestParameter Read<TestParameter>(Packet& packet) {
    TestParameter parameters; 

    parameters.test.push_back(packet.ReadInt32());
    parameters.test.push_back(packet.ReadInt32());
    parameters.test.push_back(packet.ReadInt32());
    parameters.test.push_back(packet.ReadInt32());

    return parameters;
}

void aaaa(TestParameter parameters) {
    for (auto t : parameters.test) {
        std::cout << t << " ";
    }

    std::cout << std::endl;
}


int main()
{   
    CommandDispatcher dispatcher;
    dispatcher.AddHandler("foo", &foo);
    dispatcher.AddHandler("bar", &bar);
    dispatcher.AddHandler("foobar", &foobar);
    dispatcher.AddHandler("flash", &flash);
    dispatcher.AddHandler("aaaa", &aaaa);

    dispatcher.AddHandler(0x42, &aaaa);

    SerialPacket packet("aaaa 100 69 420 00");
    dispatcher.DispatchPacket(packet);

    uint32_t data[] = { 0x42, 100, 69, 420, 00};

    BinaryPacket binaryPacket((const char *)data, sizeof(data)/sizeof(data[0]));
    dispatcher.DispatchPacket(binaryPacket);

    return 0;
}