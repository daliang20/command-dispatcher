#include "command_dispatcher.hpp"
#include <stdio.h>

void foo(int i, float j) {
    printf("foo(%d, %g)\n", i, j);
    return;
}


int main()
{   
    CommandDispatcher dispatcher;

    dispatcher.AddHandler("foo", &foo);

    SerialPacket packet("foo 123 124");

    dispatcher.DispatchPacket(packet);

    return 0;
}