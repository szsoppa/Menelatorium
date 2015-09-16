#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace ACTORS
{
    int const MENEL = 0;
    int const STAFF = 1;
    int const MANAGEMENT = 2;
}

namespace MENEL
{
    int const TIMESTAMP = 10;
    int const DRUNK = 11;
    int const NOT_DRUNK = 12;
    int const STATE = 13;
    int const OUT = 14;
}

namespace MANAGEMENT
{
    int const LAST_MENEL_IN = 20;
    int const OPEN_EXHIBITION = 21;
    int const END_OF_EXHIBITION = 22;
}

namespace STAFF
{
    int const HELP = 30;
    int const TIMESTAMP = 31;
    int const STATUS_CHANGE = 32; //TAGTYPE
    int const LEAVING_FOR = 61; //MESSAGE_TYPE
    int const REQUESTING_ADDITIONAL = 62; //czekam na dodatkowych
    int const CONSENSUS = 63;
    int const ACCEPTED = 64;
    int const NORMAL = 65;
    int const CONTACT = 66;
    int const PENDING = 67;

    int const WHITE = 70;
    int const GREEN = 71;
    int const BLACK = 72;
    int const RED = 73;

    int const FORCE = 74;
}

namespace CONSTANT
{
    int const NOT_PARTICIPATING = -100;
    int const NOT_IMPORTANT = -101;
}



#endif // CONSTANTS_H
