#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "vector.h"
#include "..\shared.h"

#define MATRIX_INVERSE_EPSILON		1e-14
#define MATRIX_EPSILON				1e-6

namespace CG {
	class Mat2 {
	private:
		
	};

	class Mat3 {
	};

	class Mat4 {
	public:
		Mat4();
		explicit Mat4(const Vec4 &x, const Vec4 &y, const Vec4 &z, const Vec4 &w);
		explicit Mat4(float xx, float xy, float xz, float xw,
					  float yx, float yy, float yz, float yw,
					  float zx, float zy, float zz, float zw,
					  float wx, float wy, float wz, float ww);

		const Vec4 &operator[](int index) const;
		Vec4 &operator[](int index);

		Mat4 operator*(const Mat4 &a) const;

		Mat4 Transport() const;
		Mat4 & TransportSelf();
		Mat4 Inverse() const;
		bool InverseSelf();
	private:
		Vec4 mat[4];
	};

	inline Mat4::Mat4() {
	}

	inline Mat4::Mat4(const Vec4 &x, const Vec4 &y, const Vec4 &z, const Vec4 &w) {
		mat[0] = x;
		mat[1] = y;
		mat[2] = z;
		mat[3] = w;
	}

	inline Mat4::Mat4(float xx, float xy, float xz, float xw,
		float yx, float yy, float yz, float yw,
		float zx, float zy, float zz, float zw,
		float wx, float wy, float wz, float ww) {
		mat[0][0] = xx; mat[0][1] = xy; mat[0][2] = xz; mat[0][3] = xw;
		mat[1][0] = yx; mat[1][1] = yy; mat[1][2] = yz; mat[1][3] = yw;
		mat[2][0] = zx; mat[2][1] = zy; mat[2][2] = zz; mat[2][3] = zw;
		mat[3][0] = wx; mat[3][1] = wy; mat[3][2] = wz; mat[3][3] = ww;
	}

	inline const Vec4 &Mat4::operator[](int index) const {
		assert(index >= 0 && index < 4);
		return mat[index];
	}

	inline Vec4 &Mat4::operator[](int index) {
		assert(index >= 0 && index < 4);
		return mat[index];
	}

	inline Mat4 Mat4::operator*(const Mat4 &a) const {
		Mat4 dst;
		const float *m1Ptr = reinterpret_cast<const float *>(this);
		const float *m2Ptr = reinterpret_cast<const float *>(&a);
		float *dstPtr = reinterpret_cast<float *>(&dst);

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				*dstPtr = m1Ptr[0] * m2Ptr[0 * 4 + j]
						+ m1Ptr[1] * m2Ptr[1 * 4 + j]
						+ m1Ptr[2] * m2Ptr[2 * 4 + j]
						+ m1Ptr[3] * m2Ptr[3 * 4 + j];

				dstPtr++;
			}

			m1Ptr += 4;
		}

		return dst;
	}

	inline Mat4 Mat4::Inverse() const {
		Mat4 invMat;

		invMat = *this;
		bool r = invMat.InverseSelf();

		return invMat;
	}
}

#endif