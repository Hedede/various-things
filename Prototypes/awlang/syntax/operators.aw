uint sizeof(type T);  // like C sizeof
uint alignof(type T); // like C++ alignof
const char[]& typename(type T)
type_info& typeid(type T)

T& operator++(T); //++v
T& operator--(T); //--v

// always defined in terms of ++v
T operator++(T); // v++
T operator--(T); // v--
