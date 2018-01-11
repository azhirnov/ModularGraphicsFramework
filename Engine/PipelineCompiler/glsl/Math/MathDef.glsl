/*
	GLSL function restyling.

	license: free
*/

// functions redefinition
#define Sin							sin
#define Cos							cos
#define ASin						asin
#define ACos						acos
#define SinH						sinh
#define CosH						cosh
#define ASinH						asinh
#define ACosH						acosh
#define Tan							tan
#define ATan						atan
#define TanH						tanh
#define ATanH						atanh

#define Abs							abs
#define SignOrZero					sign
#define Clamp						clamp

#define Max							max
#define Min							min

#define Pow							pow
#define Ln							log
#define Log2						log2
#define Exp							exp
#define Exp2						exp2

#define Sqrt						sqrt
#define InvSqrt						inversesqrt

#define Mod							mod
#define Fract						fract
#define Floor						floor
#define Ceil						ceil
#define Trunc						trunc
#define Round						round

#define Reflect						reflect
#define Refract						refract

#define Cross						cross
#define Dot							dot
#define Distance					distance
#define Length						length
#define Normalize					normalize

#define IsNaN						isnan
#define IsInfinity					isinf
#define IsFinite( _x_ )				(not IsInfinity(_x_) and not IsNaN(_x_))

#define SmoothStep( _x_, _edge0_, _edge1_ )		smoothstep( _edge0_, _edge1_, _x_ )
#define Step( _x_, _edge_ )						step( _edge_, _x_ )

#define Pi							3.14159265358979323846
#define Lerp						mix
