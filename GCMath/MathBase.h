#pragma once
#include <stdlib.h>
#include<cmath>

#include "Type.h"
using namespace std;
namespace oocd
{
	class MathBase
	{
	public:

		//将浮点数转换为整数
		static   int TruncToInt(float F)
		{
			return (int)F;
		}

		//将浮点数取整
		static   float TruncToFloat(float F)
		{
			return (float)TruncToInt(F);
		}

		//向下取整
		static  int FloorToInt(float F)
		{
			return TruncToInt(floorf(F));
		}

		//将浮点数取整
		static  float FloorToFloat(float F)
		{
			return floorf(F);
		}

		static  double FloorToDouble(double F)
		{
			return floor(F);
		}

		//取最接近的
		static  int RoundToInt(float F)
		{
			return FloorToInt(F + 0.5f);
		}

		static  float RoundToFloat(float F)
		{
			return FloorToFloat(F + 0.5f);
		}

		static  double RoundToDouble(double F)
		{
			return FloorToDouble(F + 0.5);
		}

		//向上
		static  int CeilToInt(float F)
		{
			return TruncToInt(ceilf(F));
		}

		static  float CeilToFloat(float F)
		{
			return ceilf(F);
		}

		static  double CeilToDouble(double F)
		{
			return ceil(F);
		}

		//返回浮点数的有符号小数部分。
		static  float Fractional(float Value)
		{
			return Value - TruncToFloat(Value);
		}

		//小数部分
		static  float Frac(float Value)
		{
			return Value - FloorToFloat(Value);
		}

		//将给定值分解为整数和小数部分。
		static  float Modf(const float InValue, float* OutIntPart)
		{
			return modff(InValue, OutIntPart);
		}

		//将给定值分解为整数和小数部分。
		static  double Modf(const double InValue, double* OutIntPart)
		{
			return modf(InValue, OutIntPart);
		}

		// 指数函数
		static  float Exp(float Value) { return expf(Value); }

		// Returns 2^Value
		static  float Exp2(float Value) { return powf(2.f, Value); /*exp2f(Value);*/ }

		//log
		static  float Loge(float Value) { return logf(Value); }
		static  float LogX(float Base, float Value) { return Loge(Value) / Loge(Base); }

		static  float Log2(float Value) { return Loge(Value) * 1.4426950f; }

		//返回X / Y的浮点余数
		static  float Fmod(float X, float Y)
		{
			if (fabsf(Y) <= 1.e-8f)
			{
				return 0.f;
			}
			const float Quotient = TruncToFloat(X / Y);
			float IntPortion = Y * Quotient;

			// Rounding and imprecision could cause IntPortion to exceed X and cause the result to be outside the expected range.
			// For example Fmod(55.8, 9.3) would result in a very small negative value!
			if (fabsf(IntPortion) > fabsf(X))
			{
				IntPortion = X;
			}

			const float Result = X - IntPortion;
			return Result;
		}

		static  float Sin(float Value) { return sinf(Value); }
		static  float Asin(float Value) { return asinf((Value < -1.f) ? -1.f : ((Value < 1.f) ? Value : 1.f)); }
		static  float Sinh(float Value) { return sinhf(Value); }
		static  float Cos(float Value) { return cosf(Value); }
		static  float Acos(float Value) { return acosf((Value < -1.f) ? -1.f : ((Value < 1.f) ? Value : 1.f)); }
		static  float Tan(float Value) { return tanf(Value); }
		static  float Atan(float Value) { return atanf(Value); }
		static   float Atan2(float Y, float X);
		static  float Sqrt(float Value) { return sqrtf(Value); }
		static  float Pow(float A, float B) { return powf(A, B); }

		/** Computes a fully accurate inverse square root */
		static  float InvSqrt(float F)
		{
			return 1.0f / sqrtf(F);
		}

		/** Computes a faster but less accurate inverse square root */
		static  float InvSqrtEst(float F)
		{
			return InvSqrt(F);
		}

		/** Return true if value is NaN (not a number). */
		static  bool IsNaN(float A)
		{
			return ((*(uint32*)&A) & 0x7FFFFFFF) > 0x7F800000;
		}
		/** Return true if value is finite (not NaN and not Infinity). */
		static  bool IsFinite(float A)
		{
			return ((*(uint32*)&A) & 0x7F800000) != 0x7F800000;
		}
		static  bool IsNegativeFloat(const float& A)
		{
			return ((*(uint32*)&A) >= (uint32)0x80000000); // Detects sign bit.
		}

		static  bool IsNegativeDouble(const double& A)
		{
			return ((*(uint64*)&A) >= (uint64)0x8000000000000000); // Detects sign bit.
		}

		/** Returns a random integer between 0 and RAND_MAX, inclusive */
		static  int Rand() { return rand(); }

		/** Seeds global random number functions Rand() and FRand() */
		static  void RandInit(int Seed) { srand(Seed); }

		/** Returns a random float between 0 and 1, inclusive. */
		static  float FRand() { return Rand() / (float)RAND_MAX; }

		/** Seeds future calls to SRand() */
		static   void SRandInit(int Seed);

		/** Returns the current seed for SRand(). */
		static   int GetRandSeed();

		/** Returns a seeded random float in the range [0,1), using the seed from SRandInit(). */
		static   float SRand();

		/**
		 * Computes the base 2 logarithm for an integer value that is greater than 0.
		 * The result is rounded down to the nearest integer.
		 *
		 * @param Value		The value to compute the log of
		 * @return			Log2 of Value. 0 if Value is 0.
		 */
		static  uint32 FloorLog2(uint32 Value)
		{
			/*		// reference implementation

					// 1500ms on test data
					uint Bit = 32;
					for (; Bit > 0;)
					{
						Bit--;
						if (Value & (1<<Bit))
						{
							break;
						}
					}
					return Bit;
			*/

			// same output as reference

			// see http://codinggorilla.domemtech.com/?p=81 or http://en.wikipedia.org/wiki/Binary_logarithm but modified to return 0 for a input value of 0
			// 686ms on test data
			uint32 pos = 0;
			if (Value >= 1 << 16) { Value >>= 16; pos += 16; }
			if (Value >= 1 << 8) { Value >>= 8; pos += 8; }
			if (Value >= 1 << 4) { Value >>= 4; pos += 4; }
			if (Value >= 1 << 2) { Value >>= 2; pos += 2; }
			if (Value >= 1 << 1) { pos += 1; }
			return (Value == 0) ? 0 : pos;

			// even faster would be method3 but it can introduce more cache misses and it would need to store the table somewhere
			// 304ms in test data
			/*int LogTable256[256];

			void prep()
			{
				LogTable256[0] = LogTable256[1] = 0;
				for (int i = 2; i < 256; i++)
				{
					LogTable256[i] = 1 + LogTable256[i / 2];
				}
				LogTable256[0] = -1; // if you want log(0) to return -1
			}

			int _ method3(uint v)
			{
				int r;     // r will be lg(v)
				uint tt; // temporaries

				if ((tt = v >> 24) != 0)
				{
					r = (24 + LogTable256[tt]);
				}
				else if ((tt = v >> 16) != 0)
				{
					r = (16 + LogTable256[tt]);
				}
				else if ((tt = v >> 8 ) != 0)
				{
					r = (8 + LogTable256[tt]);
				}
				else
				{
					r = LogTable256[v];
				}
				return r;
			}*/
		}

		/**
		 * Computes the base 2 logarithm for a 64-bit value that is greater than 0.
		 * The result is rounded down to the nearest integer.
		 *
		 * @param Value		The value to compute the log of
		 * @return			Log2 of Value. 0 if Value is 0.
		 */
		static  uint64 FloorLog2_64(uint64 Value)
		{
			uint64 pos = 0;
			if (Value >= 1ull << 32) { Value >>= 32; pos += 32; }
			if (Value >= 1ull << 16) { Value >>= 16; pos += 16; }
			if (Value >= 1ull << 8) { Value >>= 8; pos += 8; }
			if (Value >= 1ull << 4) { Value >>= 4; pos += 4; }
			if (Value >= 1ull << 2) { Value >>= 2; pos += 2; }
			if (Value >= 1ull << 1) { pos += 1; }
			return (Value == 0) ? 0 : pos;
		}

		/**
		 * Counts the number of leading zeros in the bit representation of the value
		 *
		 * @param Value the value to determine the number of leading zeros for
		 *
		 * @return the number of zeros before the first "on" bit
		 */
		static  uint32 CountLeadingZeros(uint32 Value)
		{
			if (Value == 0) return 32;
			return 31 - FloorLog2(Value);
		}

		/**
		 * Counts the number of leading zeros in the bit representation of the 64-bit value
		 *
		 * @param Value the value to determine the number of leading zeros for
		 *
		 * @return the number of zeros before the first "on" bit
		 */
		static  uint64 CountLeadingZeros64(uint64 Value)
		{
			if (Value == 0) return 64;
			return 63 - FloorLog2_64(Value);
		}

		/**
		 * Counts the number of trailing zeros in the bit representation of the value
		 *
		 * @param Value the value to determine the number of trailing zeros for
		 *
		 * @return the number of zeros after the last "on" bit
		 */
		static  uint32 CountTrailingZeros(uint32 Value)
		{
			if (Value == 0)
			{
				return 32;
			}
			uint32 Result = 0;
			while ((Value & 1) == 0)
			{
				Value >>= 1;
				++Result;
			}
			return Result;
		}

		/**
		 * Counts the number of trailing zeros in the bit representation of the value
		 *
		 * @param Value the value to determine the number of trailing zeros for
		 *
		 * @return the number of zeros after the last "on" bit
		 */
		static  uint64 CountTrailingZeros64(uint64 Value)
		{
			if (Value == 0)
			{
				return 64;
			}
			uint64 Result = 0;
			while ((Value & 1) == 0)
			{
				Value >>= 1;
				++Result;
			}
			return Result;
		}

		/**
		 * Returns smallest N such that (1<<N)>=Arg.
		 * Note: CeilLogTwo(0)=0 because (1<<0)=1 >= 0.
		 */
		static  uint32 CeilLogTwo(uint32 Arg)
		{
			int Bitmask = ((int)(CountLeadingZeros(Arg) << 26)) >> 31;
			return (32 - CountLeadingZeros(Arg - 1)) & (~Bitmask);
		}

		static  uint64 CeilLogTwo64(uint64 Arg)
		{
			int64 Bitmask = ((int64)(CountLeadingZeros64(Arg) << 57)) >> 63;
			return (64 - CountLeadingZeros64(Arg - 1)) & (~Bitmask);
		}

		/** @return Rounds the given number up to the next highest power of two. */
		static  uint32 RoundUpToPowerOfTwo(uint32 Arg)
		{
			return 1 << CeilLogTwo(Arg);
		}

		static  uint64 RoundUpToPowerOfTwo64(uint64 V)
		{
			return uint64(1) << CeilLogTwo64(V);
		}

		/** Spreads bits to every other. */
		static  uint32 MortonCode2(uint32 x)
		{
			x &= 0x0000ffff;
			x = (x ^ (x << 8)) & 0x00ff00ff;
			x = (x ^ (x << 4)) & 0x0f0f0f0f;
			x = (x ^ (x << 2)) & 0x33333333;
			x = (x ^ (x << 1)) & 0x55555555;
			return x;
		}

		/** Reverses MortonCode2. Compacts every other bit to the right. */
		static  uint32 ReverseMortonCode2(uint32 x)
		{
			x &= 0x55555555;
			x = (x ^ (x >> 1)) & 0x33333333;
			x = (x ^ (x >> 2)) & 0x0f0f0f0f;
			x = (x ^ (x >> 4)) & 0x00ff00ff;
			x = (x ^ (x >> 8)) & 0x0000ffff;
			return x;
		}

		/**每三位传播一次。*/
		static  uint32 MortonCode3(uint32 x)
		{
			x &= 0x000003ff;
			x = (x ^ (x << 16)) & 0xff0000ff;
			x = (x ^ (x << 8)) & 0x0300f00f;
			x = (x ^ (x << 4)) & 0x030c30c3;
			x = (x ^ (x << 2)) & 0x09249249;
			return x;
		}

		/**反转MortonCode3。 每隔3位压缩一次。*/
		static  uint32 ReverseMortonCode3(uint32 x)
		{
			x &= 0x09249249;
			x = (x ^ (x >> 2)) & 0x030c30c3;
			x = (x ^ (x >> 4)) & 0x0300f00f;
			x = (x ^ (x >> 8)) & 0xff0000ff;
			x = (x ^ (x >> 16)) & 0x000003ff;
			return x;
		}

		static   float FloatSelect(float Comparand, float ValueGEZero, float ValueLTZero)
		{
			return Comparand >= 0.f ? ValueGEZero : ValueLTZero;
		}

		static   double FloatSelect(double Comparand, double ValueGEZero, double ValueLTZero)
		{
			return Comparand >= 0.f ? ValueGEZero : ValueLTZero;
		}

		template< class T >
		static   T Abs(const T A)
		{
			return (A >= (T)0) ? A : -A;
		}

		//大于0是1，小于0是-1，否则是0
		template< class T >
		static   T Sign(const T A)
		{
			return (A > (T)0) ? (T)1 : ((A < (T)0) ? (T)-1 : (T)0);
		}

		//选大的
		template< class T >
		static   T Max(const T A, const T B)
		{
			return (A >= B) ? A : B;
		}

		//选小的
		template< class T >
		static   T Min(const T A, const T B)
		{
			return (A <= B) ? A : B;
		}

		//计算比特数
		static  int CountBits(uint64 Bits)
		{
			// https://en.wikipedia.org/wiki/Hamming_weight
			Bits -= (Bits >> 1) & 0x5555555555555555ull;
			Bits = (Bits & 0x3333333333333333ull) + ((Bits >> 2) & 0x3333333333333333ull);
			Bits = (Bits + (Bits >> 4)) & 0x0f0f0f0f0f0f0f0full;
			return (Bits * 0x0101010101010101) >> 56;
		}
	};
}
