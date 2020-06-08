#include "HitboxData.hpp"

bool operator== (const HitboxData& p1, const HitboxData& p2) {
	return p1.id == p2.id;
}

bool operator< (const HitboxData& p1, const HitboxData& p2) {
	return p1.id < p2.id;
}