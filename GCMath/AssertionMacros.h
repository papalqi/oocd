#pragma once
#define CA_ASSUME( Expr )
#define UNLIKELY(x)			(x)
#define checkSlow(expr)					{ CA_ASSUME(expr); }
#define checkfSlow(expr, format, ...)	{ CA_ASSUME(expr); }
#define verifySlow(expr)				{ if(UNLIKELY(!(expr))) { CA_ASSUME(false); } }
