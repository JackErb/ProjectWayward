#ifndef PhysicsBitmask_h
#define PhysicsBitmask_h

namespace Bitmask {
enum Bitmask {
    None      = 0,
    Stage     = 1 << 1,
    Player    = 1 << 2,
    Explosive = 1 << 3,
    Water     = 1 << 4
};
}

#endif  /* PhysicsBitmask_h */
