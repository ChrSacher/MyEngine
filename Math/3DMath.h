///////////////////////////////////////////////////////////////////////////////
// Matrice.h
// =========
// NxN Matrix Math classes
//
// The elements of the matrix are stored as column major order.
// | 0 2 |    | 0 3 6 |    |  0  4  8 12 |
// | 1 3 |    | 1 4 7 |    |  1  5  9 13 |
//            | 2 5 8 |    |  2  6 10 14 |


#pragma once

#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <algorithm>
///////////////////////////////////////////////////////////////////////////////
// 2D vector
///////////////////////////////////////////////////////////////////////////////
struct Vector2
{
    float x;
    float y;

    // ctors
    Vector2() : x(0), y(0) {};
    Vector2(float x, float y) : x(x), y(y) {};

    // utils functions
    void        set(float x, float y);
    float       length() const;                         //
    float       distance(const Vector2& vec) const;     // distance between two vectors
    Vector2&    normalize();                            //
    float       dot(const Vector2& vec) const;          // dot product
    bool        equal(const Vector2& vec, float e) const; // compare with epsilon

    // operators
    Vector2     operator-() const;                      // unary operator (negate)
    Vector2     operator+(const Vector2& rhs) const;    // add rhs
    Vector2     operator-(const Vector2& rhs) const;    // subtract rhs
    Vector2&    operator+=(const Vector2& rhs);         // add rhs and update this object
    Vector2&    operator-=(const Vector2& rhs);         // subtract rhs and update this object
    Vector2     operator*(const float scale) const;     // scale
    Vector2     operator*(const Vector2& rhs) const;
	Vector2     mul(const Vector2& rhs) const {return Vector2(*this * rhs);};// multiply each element
	Vector2		sub(const Vector2& rhs) const { return  Vector2(*this - rhs); };
    Vector2&    operator*=(const float scale);          // scale and update this object
    Vector2&    operator*=(const Vector2& rhs);         // multiply each element and update this object
    Vector2     operator/(const float scale) const;     // inverse scale
    Vector2&    operator/=(const float scale);          // scale and update this object
	std::string operator<<(const Vector2& rhs) const { return std::string(std::to_string(x) + " " + std::to_string(y)); }
    bool        operator==(const Vector2& rhs) const;   // exact compare, no epsilon
    bool        operator!=(const Vector2& rhs) const;   // exact compare, no epsilon
    bool        operator<(const Vector2& rhs) const;    // comparison for sort
    float       operator[](int index) const;            // subscript operator v[0], v[1]
    float&      operator[](int index);                  // subscript operator v[0], v[1]

    friend Vector2 operator*(const float a, const Vector2 vec);
    friend std::ostream& operator<<(std::ostream& os, const Vector2& vec);
};



///////////////////////////////////////////////////////////////////////////////
// 3D vector
///////////////////////////////////////////////////////////////////////////////
struct Vector3
{
    float x;
    float y;
    float z;

    // ctors
    Vector3() : x(0), y(0), z(0) {};
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {};

    // utils functions
    void        set(float x, float y, float z);
    float       length() const;                         //
    float       distance(const Vector3& vec) const;     // distance between two vectors
    Vector3&    normalize();                            //
    float       dot(const Vector3& vec) const;          // dot product
    Vector3     cross(const Vector3& vec) const;        // cross product
    bool        equal(const Vector3& vec, float e) const; // compare with epsilon
	void		conjugate(){x = -x; y = -y;z=-z;}
	Vector3     rotate(float angle,Vector3 axis);
    // operators
    Vector3     operator-() const;                      // unary operator (negate)
    Vector3     operator+(const Vector3& rhs) const;    // add rhs
    Vector3     operator-(const Vector3& rhs) const;    // subtract rhs
    Vector3&    operator+=(const Vector3& rhs);         // add rhs and update this object
    Vector3&    operator-=(const Vector3& rhs);  
	Vector3     mul(const Vector3& rhs) const { return Vector3(*this * rhs); };// multiply each element
	Vector3		sub(const Vector3& rhs) const { return Vector3(*this - rhs); };// subtract rhs and update this object
    Vector3     operator*(const float scale) const;     // scale
    Vector3     operator*(const Vector3& rhs) const;    // multiplay each element
    Vector3&    operator*=(const float scale);          // scale and update this object
    Vector3&    operator*=(const Vector3& rhs);         // product each element and update this object
    Vector3     operator/(const float scale) const;     // inverse scale
    Vector3&    operator/=(const float scale);          // scale and update this object
    bool        operator==(const Vector3& rhs) const;   // exact compare, no epsilon
    bool        operator!=(const Vector3& rhs) const;   // exact compare, no epsilon
    bool        operator<(const Vector3& rhs) const;    // comparison for sort
	std::string operator<<(const Vector3& rhs) const { return std::string(std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z));}
    float       operator[](int index) const;            // subscript operator v[0], v[1]
    float&      operator[](int index);                  // subscript operator v[0], v[1]
	
    friend Vector3 operator*(const float a, const Vector3 vec);
    friend std::ostream& operator<<(std::ostream& os, const Vector3& vec);
};

// 4D vector

struct Vector4
{
    float x;
    float y;
    float z;
    float w;

    // ctors
    Vector4() : x(0), y(0), z(0), w(0) {};
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

    // utils functions
    void        set(float x, float y, float z, float w);
    float       length() const;                         //
    float       distance(const Vector4& vec) const;     // distance between two vectors
    Vector4&    normalize();                            //
    float       dot(const Vector4& vec) const;          // dot product
    bool        equal(const Vector4& vec, float e) const; // compare with epsilon

    // operators
    Vector4     operator-() const;                      // unary operator (negate)
    Vector4     operator+(const Vector4& rhs) const;    // add rhs
    Vector4     operator-(const Vector4& rhs) const;    // subtract rhs
    Vector4&    operator+=(const Vector4& rhs);         // add rhs and update this object
    Vector4&    operator-=(const Vector4& rhs);         // subtract rhs and update this object
    Vector4     operator*(const float scale) const;     // scale
    Vector4     operator*(const Vector4& rhs) const;    // multiply each element
    Vector4&    operator*=(const float scale);          // scale and update this object
    Vector4&    operator*=(const Vector4& rhs);         // multiply each element and update this object
    Vector4     operator/(const float scale) const;     // inverse scale
    Vector4&    operator/=(const float scale);   
	Vector4     mul(const Vector4& rhs) const { return Vector4(*this * rhs); };// multiply each element
	Vector4		sub(const Vector4& rhs) const { return  Vector4(*this - rhs); };// scale and update this object
	std::string operator<<(const Vector4& rhs) const { return std::string(std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " " + std::to_string(w)); }
    bool        operator==(const Vector4& rhs) const;   // exact compare, no epsilon
    bool        operator!=(const Vector4& rhs) const;   // exact compare, no epsilon
    bool        operator<(const Vector4& rhs) const;    // comparison for sort
    float       operator[](int index) const;            // subscript operator v[0], v[1]
    float&      operator[](int index);                  // subscript operator v[0], v[1]
	
    friend Vector4 operator*(const float a, const Vector4 vec);
    friend std::ostream& operator<<(std::ostream& os, const Vector4& vec);
};






///////////////////////////////////////////////////////////////////////////
// 2x2 matrix
///////////////////////////////////////////////////////////////////////////
class Matrix2
{
public:
    // constructors
    Matrix2();  // init with identity
    Matrix2(const float src[4]);
    Matrix2(float m0, float m1, float m2, float m3);

    void        set(const float src[4]);
    void        set(float m0, float m1, float m2, float m3);
    void        setRow(int index, const float row[2]);
    void        setRow(int index, const Vector2& v);
    void        setColumn(int index, const float col[2]);
    void        setColumn(int index, const Vector2& v);

    const float* get() const;
    float       getDeterminant();

    Matrix2&    identity();
    Matrix2&    transpose();                            // transpose itself and return reference
    Matrix2&    invert();

    // operators
    Matrix2     operator+(const Matrix2& rhs) const;    // add rhs
    Matrix2     operator-(const Matrix2& rhs) const;    // subtract rhs
    Matrix2&    operator+=(const Matrix2& rhs);         // add rhs and update this object
    Matrix2&    operator-=(const Matrix2& rhs);         // subtract rhs and update this object
    Vector2     operator*(const Vector2& rhs) const;    // multiplication: v' = M * v
    Matrix2     operator*(const Matrix2& rhs) const;    // multiplication: M3 = M1 * M2
    Matrix2&    operator*=(const Matrix2& rhs);         // multiplication: M1' = M1 * M2
    bool        operator==(const Matrix2& rhs) const;   // exact compare, no epsilon
    bool        operator!=(const Matrix2& rhs) const;   // exact compare, no epsilon
    float       operator[](int index) const;            // subscript operator v[0], v[1]
    float&      operator[](int index);                  // subscript operator v[0], v[1]

    friend Matrix2 operator-(const Matrix2& m);                     // unary operator (-)
    friend Matrix2 operator*(float scalar, const Matrix2& m);       // pre-multiplication
    friend Vector2 operator*(const Vector2& vec, const Matrix2& m); // pre-multiplication
    friend std::ostream& operator<<(std::ostream& os, const Matrix2& m);

protected:

private:
    float m[4];

};



///////////////////////////////////////////////////////////////////////////
// 3x3 matrix
///////////////////////////////////////////////////////////////////////////
class Matrix3
{
public:
    // constructors
    Matrix3();  // init with identity
    Matrix3(const float src[9]);
    Matrix3(float m0, float m1, float m2,           // 1st column
            float m3, float m4, float m5,           // 2nd column
            float m6, float m7, float m8);          // 3rd column

    void        set(const float src[9]);
    void        set(float m0, float m1, float m2,   // 1st column
                    float m3, float m4, float m5,   // 2nd column
                    float m6, float m7, float m8);  // 3rd column
    void        setRow(int index, const float row[3]);
    void        setRow(int index, const Vector3& v);
    void        setColumn(int index, const float col[3]);
    void        setColumn(int index, const Vector3& v);

    const float* get() const;
    float       getDeterminant();

    Matrix3&    identity();
    Matrix3&    transpose();                            // transpose itself and return reference
    Matrix3&    invert();

    // operators
    Matrix3     operator+(const Matrix3& rhs) const;    // add rhs
    Matrix3     operator-(const Matrix3& rhs) const;    // subtract rhs
    Matrix3&    operator+=(const Matrix3& rhs);         // add rhs and update this object
    Matrix3&    operator-=(const Matrix3& rhs);         // subtract rhs and update this object
    Vector3     operator*(const Vector3& rhs) const;    // multiplication: v' = M * v
    Matrix3     operator*(const Matrix3& rhs) const;    // multiplication: M3 = M1 * M2
    Matrix3&    operator*=(const Matrix3& rhs);         // multiplication: M1' = M1 * M2
    bool        operator==(const Matrix3& rhs) const;   // exact compare, no epsilon
    bool        operator!=(const Matrix3& rhs) const;   // exact compare, no epsilon
    float       operator[](int index) const;            // subscript operator v[0], v[1]
    float&      operator[](int index);                  // subscript operator v[0], v[1]

    friend Matrix3 operator-(const Matrix3& m);                     // unary operator (-)
    friend Matrix3 operator*(float scalar, const Matrix3& m);       // pre-multiplication
    friend Vector3 operator*(const Vector3& vec, const Matrix3& m); // pre-multiplication
    friend std::ostream& operator<<(std::ostream& os, const Matrix3& m);

protected:

private:
    float m[9];

};



///////////////////////////////////////////////////////////////////////////
// 4x4 matrix
///////////////////////////////////////////////////////////////////////////
class Matrix4
{
public:
    // constructors
    Matrix4();  // init with identity
    Matrix4(const float src[16]);
    Matrix4(float m00, float m01, float m02, float m03, // 1st column
            float m04, float m05, float m06, float m07, // 2nd column
            float m08, float m09, float m10, float m11, // 3rd column
            float m12, float m13, float m14, float m15);// 4th column

    void        set(const float src[16]);
    void        set(float m00, float m01, float m02, float m03, // 1st column
                    float m04, float m05, float m06, float m07, // 2nd column
                    float m08, float m09, float m10, float m11, // 3rd column
                    float m12, float m13, float m14, float m15);// 4th column
    void        setRow(int index, const float row[4]);
    void        setRow(int index, const Vector4& v);
    void        setRow(int index, const Vector3& v);
    void        setColumn(int index, const float col[4]);
    void        setColumn(int index, const Vector4& v);
    void        setColumn(int index, const Vector3& v);

    const float* get() const;
    const float* getTranspose();                        // return transposed matrix
    float        getDeterminant();

    Matrix4&    identity();
    Matrix4&    transpose();                            // transpose itself and return reference
    Matrix4&    invert();                               // check best inverse method before inverse
    Matrix4&    invertEuclidean();                      // inverse of Euclidean transform matrix
    Matrix4&    invertAffine();                         // inverse of affine transform matrix
    Matrix4&    invertProjective();                     // inverse of projective matrix using partitioning
    Matrix4&    invertGeneral();                        // inverse of generic matrix
	//projection
	
    // transform matrix
    Matrix4&    translate(float x, float y, float z);   // translation by (x,y,z)
    Matrix4&    translate(const Vector3& v);            //
    Matrix4&    rotate(float angle, const Vector3& axis); // rotate angle(degree) along the given axix
    Matrix4&    rotate(float angle, float x, float y, float z);
    Matrix4&    rotateX(float angle);                   // rotate on X-axis with degree
    Matrix4&    rotateY(float angle);                   // rotate on Y-axis with degree
    Matrix4&    rotateZ(float angle);                   // rotate on Z-axis with degree
    Matrix4&    scale(float scale);   
	Matrix4&    scale(Vector3 s);// uniform scale
    Matrix4&    scale(float sx, float sy, float sz);    // scale by (sx, sy, sz) on each axis
    // operators
    Matrix4     operator+(const Matrix4& rhs) const;    // add rhs
    Matrix4     operator-(const Matrix4& rhs) const;    // subtract rhs
    Matrix4&    operator+=(const Matrix4& rhs);         // add rhs and update this object
    Matrix4&    operator-=(const Matrix4& rhs);         // subtract rhs and update this object
    Vector4     operator*(const Vector4& rhs) const;    // multiplication: v' = M * v
    Vector3     operator*(const Vector3& rhs) const;    // multiplication: v' = M * v
    Matrix4     operator*(const Matrix4& rhs) const;    // multiplication: M3 = M1 * M2
    Matrix4&    operator*=(const Matrix4& rhs);         // multiplication: M1' = M1 * M2
    bool        operator==(const Matrix4& rhs) const;   // exact compare, no epsilon
    bool        operator!=(const Matrix4& rhs) const;   // exact compare, no epsilon
    float       operator[](int index) const;            // subscript operator v[0], v[1]
    float&      operator[](int index);                  // subscript operator v[0], v[1]

    friend Matrix4 operator-(const Matrix4& m);                     // unary operator (-)
    friend Matrix4 operator*(float scalar, const Matrix4& m);       // pre-multiplication
    friend Vector3 operator*(const Vector3& vec, const Matrix4& m); // pre-multiplication
    friend Vector4 operator*(const Vector4& vec, const Matrix4& m); // pre-multiplication
    friend std::ostream& operator<<(std::ostream& os, const Matrix4& m);


	Matrix4& InitRotationFromVectors(const Vector3& n, const Vector3& v, const Vector3& u);
	Matrix4& perspective(float newfov,float aspect,float newznear,float newzfar);
	Matrix4& InitRotationFromDirection(Vector3& forward,Vector3& up);
	Matrix4& InitOrthographic(float left, float right, float bottom, float top, float near, float far);
	Matrix4 ToRotationMatrix() const;
	Matrix4 lookAt(Vector3 const &eye,Vector3 const &center,Vector3 const &up);
protected:

private:
    float       getCofactor(float m0, float m1, float m2,
                            float m3, float m4, float m5,
                            float m6, float m7, float m8);

    float m[16];
    float tm[16];                                       // transpose m

};

struct Quaternion
{
    float x, y, z, w;

    Quaternion(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _w = 1.0f);

	Quaternion(float real, const Vector3 &i);

	//! from 3 euler angles
	Quaternion(float theta_z, float theta_y, float theta_x);
	
	//! from 3 euler angles 
	Quaternion(const Vector3 &angles); 

	Quaternion(const Vector3& v, float W);

    Quaternion(const Vector4& v);

    Quaternion(const float* array);

    Vector3 complex() const ;
    void complex(const Vector3& c);

    float real() const;
    void real(float r);

    Quaternion conjugate(void) const ;

    /** 
     * @brief Computes the inverse of this quaternion.
     *
     * @note This is a general inverse.  If you know a priori
     * that you're using a unit quaternion (i.e., norm() == 1),
     * it will be significantly faster to use conjugate() instead.
     * 
     * @return The quaternion q such that q * (*this) == (*this) * q
     * == [ 0 0 0 1 ]<sup>T</sup>.
     */
    Quaternion inverse(void) const ;


    /** 
     * @brief Computes the product of this quaternion with the
     * quaternion 'rhs'.
     *
     * @param rhs The right-hand-side of the product operation.
     *
     * @return The quaternion product (*this) x @p rhs.
     */
    Quaternion product(const Quaternion& rhs) const ;

    /**
     * @brief Quaternion product operator.
     *
     * The result is a quaternion such that:
     *
     * result.real() = (*this).real() * rhs.real() -
     * (*this).complex().dot(rhs.complex());
     *
     * and:
     *
     * result.complex() = rhs.complex() * (*this).real
     * + (*this).complex() * rhs.real()
     * - (*this).complex().cross(rhs.complex());
     *
     * @return The quaternion product (*this) x rhs.
     */
    Quaternion operator*(const Quaternion& rhs) const ;

    /**
     * @brief Quaternion scalar product operator.
     * @param s A scalar by which to multiply all components
     * of this quaternion.
     * @return The quaternion (*this) * s.
     */
    Quaternion operator*(float s) const;

    /**
     * @brief Produces the sum of this quaternion and rhs.
     */
    Quaternion operator+(const Quaternion& rhs) const;

    /**
     * @brief Produces the difference of this quaternion and rhs.
     */
    Quaternion operator-(const Quaternion& rhs) const;

    /**
     * @brief Unary negation.
     */
    Quaternion operator-() const;

    /**
     * @brief Quaternion scalar division operator.
     * @param s A scalar by which to divide all components
     * of this quaternion.
     * @return The quaternion (*this) / s.
     */
    Quaternion operator/(float s) const;

    /**
     * @brief Returns a matrix representation of this
     * quaternion.
     *
     * Specifically this is the matrix such that:
     *
     * this->matrix() * q.vector() = (*this) * q for any quaternion q.
     *
     * Note that this is @e NOT the rotation matrix that may be
     * represented by a unit quaternion.
     */
    Matrix4 matrix() const;

    /**
     * @brief Returns a matrix representation of this
     * quaternion for right multiplication.
     *
     * Specifically this is the matrix such that:
     *
     * q.vector().transpose() * this->matrix() = (q *
     * (*this)).vector().transpose() for any quaternion q.
     *
     * Note that this is @e NOT the rotation matrix that may be
     * represented by a unit quaternion.
     */
    Matrix4 rightMatrix() const;

    /**
     * @brief Returns this quaternion as a 4-vector.
     *
     * This is simply the vector [x y z w]<sup>T</sup>
     */
    Vector4 vector() const;

    /**
     * @brief Returns the norm ("magnitude") of the quaternion.
     * @return The 2-norm of [ w(), x(), y(), z() ]<sup>T</sup>.
     */
    float norm() const;

    /**
     * @brief Computes the rotation matrix represented by a unit
     * quaternion.
     *
     * @note This does not check that this quaternion is normalized.
     * It formulaically returns the matrix, which will not be a
     * rotation if the quaternion is non-unit.
     */
    Matrix3 rotationMatrix() const;


    /**
     * @brief Returns a vector rotated by this quaternion.
     *
     * Functionally equivalent to:  (rotationMatrix() * v)
     * or (q * Quaternion(0, v) * q.inverse()).
     *
     * @warning conjugate() is used instead of inverse() for better
     * performance, when this quaternion must be normalized.
     */
    Vector3 rotatedVector(const Vector3& v) const ;



    /**
     * @brief Computes the quaternion that is equivalent to a given
     * euler angle rotation.
     * @param euler A 3-vector in order:  roll-pitch-yaw.
     */
    void euler(const Vector3& euler) ;

    /** @brief Returns an equivalent euler angle representation of
     * this quaternion.
     * @return Euler angles in roll-pitch-yaw order.
     */
    Vector3 euler(void) const ;

};



