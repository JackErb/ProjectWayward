#ifndef vector_overloads_h
#define vector_overloads_h

inline Vector2D operator-(const Vector2D &vec) {
    return Vector2D(-vec.x, -vec.y);
}

inline Vector2D operator+(const Vector2D &v1, const Vector2D &v2) {
    return Vector2D(v1.x + v2.x, v1.y + v2.y);
}

inline Vector2D operator-(const Vector2D &v1, const Vector2D &v2) {
    return Vector2D(v1.x - v2.x, v1.y - v2.y);
}

inline Vector2D operator*(const Vector2D &v, const FixedPoint &n) {
    return Vector2D(v.x * n, v.y * n);
}

inline Vector2D operator/(const Vector2D &v, const FixedPoint &n) {
    return Vector2D(v.x / n, v.y / n);
}

inline Vector2D operator*(const FixedPoint &n, const Vector2D &v) {
    return v * n;
}

inline Vector2D &operator-=(Vector2D &v1, const Vector2D &v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}

inline Vector2D &operator+=(Vector2D &v1, const Vector2D &v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}

inline Vector2D &operator*=(Vector2D &v, const FixedPoint &n) {
    v.x *= n;
    v.y *= n;
    return v;
}

inline Vector2D orthogonal(const Vector2D &v) {
    return Vector2D(-v.y, v.x);
}

#endif  /* vector_overloads_h */
