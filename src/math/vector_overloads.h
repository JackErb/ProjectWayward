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

inline Vector2D operator*(const Vector2D &v, const FixedPoint n) {
	return Vector2D(v.x * n, v.y * n);
}

inline Vector2D operator/(const Vector2D &v, const FixedPoint n) {
	return Vector2D(v.x / n, v.y / n);
}

#endif  /* vector_overloads_h */
