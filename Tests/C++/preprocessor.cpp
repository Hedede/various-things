#include "aw/meta/pp/foldl.h"
#include "aw/meta/pp/foldr.h"
#include "aw/meta/pp/macro.h"
#include <aw/meta/pp/tuple.h>
#include <aw/meta/pp/separators.h>
#define COMBINE(A,B,...) AW_DEFER(A) AW_DEFER(AW_DEFER) ( AW_DEFER(B) AW_DEFER(AW_DEFER) (__VA_ARGS__))

COMBINE(AW_EXPAND,AW_EXPAND,a,b,c)

#define LP() (
#define LEFT(...)  AW_DEFER(LP)() __VA_ARGS__,
#define RIGHT(...) __VA_ARGS__ )

#define OBSTRUCT(...) __VA_ARGS__ AW_DEFER(AW_EMPTY)()
#define COMBINE2(A,B) OBSTRUCT(AW_TUPLE) AW_DEFER(LEFT)(A,B) RIGHT

111

//AW_FOREACH( AW_SEP_SPACE, AW_DEFER(COMBINE2)(A,B) AW_DEFER, a,b,c )
//AW_APPLY(AW_FR_3, AW_SEP_SPACE, AW_DEFER(COMBINE2)(A,B) AW_DEFER, a,b,c)
//AW_FR_3(AW_SEP_SPACE, AW_DEFER(COMBINE2)(A,B), a,b,c)

//AW_APPLY( AW_FR_3, AW_SEP_SPACE, AW_DEFER(COMBINE2(A,B)) AW_DEFER(), a,b,c)
//AW_FR_3(AW_SEP_SPACE, AW_DEFER(COMBINE2(A,B)) AW_DEFER(), a,b,c)

//#define AW_FR_1(OP,  DO, X, ...)  DO(X)
//#define AW_FR_2(OP,  DO, X, ...)  OP(DO(X), AW_FR_1(OP,  DO, __VA_ARGS__))
//#define AW_FR_3(OP,  DO, X, ...)  OP(DO(X), AW_FR_2(OP,  DO, __VA_ARGS__))
//AW_EXPAND(AW_DEFER(COMBINE2(A,B)) AW_DEFER() (a))
//AW_EXPAND(AW_EXPAND(AW_DEFER(COMBINE2(A,B)) AW_DEFER() (a)))
//AW_SEP_SPACE(AW_DEFER(COMBINE2(A,B)) AW_DEFER(a),
//AW_SEP_SPACE(AW_DEFER(COMBINEx(A,B)) AW_DEFER(b),
//             AW_DEFER(COMBINEx(A,B)) AW_DEFER(c)))
//AW_FR_3(AW_SEP_SPACE, AW_DEFER(COMBINE2(A,B)) AW_DEFER(), a,b,c)

0000
COMBINE2(A,B) (c)
1111
AW_EXPAND(COMBINE2(A,B) (c))
2222
AW_EXPAND(AW_EXPAND(COMBINE2(A,B) (c)))

3333
AW_EXPAND(AW_DEFER(AW_TUPLE) (a,b,c))
4444
AW_EXPAND(AW_EXPAND(OBSTRUCT(AW_TUPLE) (a,b,c)))

