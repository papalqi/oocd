#pragma once
#include "stdafx.h"
#include"VectorRegister.h"
class ScalarRegister
{
public:
	VectorRegister Value;

	/** default constructor */
	 ScalarRegister();

	/** Copy Constructor */
	 ScalarRegister(const ScalarRegister& VectorValue);

	/** Constructor using float value */
	explicit  ScalarRegister(const float& ScalarValue);

	/** Constructor
	 *
	 * @param VectorRegister float4 vector register type
	 */
	explicit  ScalarRegister(VectorRegister VectorValue);

	/**
	 * Gets the result of multiplying a scalar register to this.
	 *
	 * @param RHS The scalar register to multiply this by.
	 *
	 * @return The result of multiplication.
	 */
	 ScalarRegister operator*(const ScalarRegister& RHS) const;

	/**
	 * Gets the result of adding a scalar register to this.
	 *
	 * @param RHS The scalar register to add.
	 *
	 * @return The result of addition.
	 */
	 ScalarRegister operator+(const ScalarRegister& RHS) const;

	/**
	 * Adds to this scalar register.
	 *
	 * @param RHS The scalar register to add to this.
	 *
	 * @return Reference to this after addition.
	 */
	 ScalarRegister& operator+=(const ScalarRegister& RHS);

	/**
	 * Subtracts another scalar register from this.
	 *
	 * @param RHS The other scalar register.
	 *
	 * @return reference to this after subtraction.
	 */
	 ScalarRegister& operator-=(const ScalarRegister& RHS);

	/**
	 * Gets the result of subtracting a scalar register to this.
	 *
	 * @param RHS The scalar register to subtract.
	 *
	 * @return The result of subtraction.
	 */
	 ScalarRegister operator-(const ScalarRegister& RHS) const;


	/** assignment operator
	 *
	 * @param RHS a ScalarRegister
	 */
	 ScalarRegister& operator=(const ScalarRegister& RHS);

	/** assignment operator
	 *
	 * @param RHS a VectorRegister
	 */
	 ScalarRegister& operator=(const VectorRegister& RHS);

	/**
	 * ScalarRegister to VectorRegister conversion operator.
	 */
	 operator VectorRegister() const;
};

 ScalarRegister::ScalarRegister()
{
}

 ScalarRegister::ScalarRegister(const ScalarRegister& VectorValue)
{
	Value = VectorValue.Value;
}

 ScalarRegister::ScalarRegister(const float& ScalarValue)
{
	Value = VectorLoadFloat1(&ScalarValue);
}

 ScalarRegister::ScalarRegister(VectorRegister VectorValue)
{
	Value = VectorValue;
}

 ScalarRegister ScalarRegister::operator*(const ScalarRegister& RHS) const
{
	return ScalarRegister(VectorMultiply(Value, RHS.Value));
}

 ScalarRegister ScalarRegister::operator+(const ScalarRegister& RHS) const
{
	return ScalarRegister(VectorAdd(Value, RHS.Value));
}

 ScalarRegister& ScalarRegister::operator+=(const ScalarRegister& RHS)
{
	Value = VectorAdd(Value, RHS.Value);
	return *this;
}

 ScalarRegister& ScalarRegister::operator-=(const ScalarRegister& RHS)
{
	Value = VectorSubtract(Value, RHS.Value);
	return *this;
}

 ScalarRegister ScalarRegister::operator-(const ScalarRegister& RHS) const
{
	return ScalarRegister(VectorSubtract(Value, RHS.Value));
}


 ScalarRegister& ScalarRegister::operator=(const ScalarRegister& RHS)
{
	Value = RHS.Value;
	return *this;
}

 ScalarRegister& ScalarRegister::operator=(const VectorRegister& RHS)
{
	Value = RHS;
	return *this;
}

 ScalarRegister::operator VectorRegister() const
{
	return Value;
}