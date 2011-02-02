#include "fireworks.h"

int Fw::go(Navigation nav, int size, int current, int previous)
{
    if(size > 0)
    {
        switch(nav)
        {
        case Fw::Go_Next:
            return (current == size - 1) ? 0 : current + 1;

        case Fw::Go_Previous:
            return (current == 0) ? size - 1 : current - 1;

        case Fw::Go_Back:
            if(previous >= 0 && previous < size)
            {
                return previous;
            }
            break;

        case Fw::Go_First:
            return 0;

        case Fw::Go_Last:
            return size -1;

        default:
            break;
        }
    }
    return -1;
}

