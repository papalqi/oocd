#pragma once
#define ScalarRegister float

#define ScalarOne 1.0f
#define ScalarZero 0.0f

#define ScalarMin Min
#define ScalarMax Max

#define ScalarReciprocal(A)  (1.0f / (A))

#define NonZeroAnimWeight(A) ((A) > ZERO_ANIMWEIGHT_THRESH)
#define NonOneAnimWeight(A) ((A) < 1.0f - ZERO_ANIMWEIGHT_THRESH)