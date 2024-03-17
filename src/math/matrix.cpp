#include "matrix.h"

namespace CG {

Mat4 Mat4::Transport() const {
    Mat4 transpose;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            transpose[i][j] = mat[j][i];
        }
    }

    return transpose;
}

Mat4 &Mat4::TransportSelf() {
    float temp;

    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            temp = mat[i][j];
            mat[i][j] = mat[j][i];
            mat[j][i] = temp;
        }
    }

    return *this;
}

bool Mat4::InverseSelf() {
    // 84+4+16 = 104 multiplications
    //			   1 division
    double det, invDet;

    // 2x2 sub-determinants required to calculate 4x4 determinant
    float det2_01_01 = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
    float det2_01_02 = mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0];
    float det2_01_03 = mat[0][0] * mat[1][3] - mat[0][3] * mat[1][0];
    float det2_01_12 = mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1];
    float det2_01_13 = mat[0][1] * mat[1][3] - mat[0][3] * mat[1][1];
    float det2_01_23 = mat[0][2] * mat[1][3] - mat[0][3] * mat[1][2];

    // 3x3 sub-determinants required to calculate 4x4 determinant
    float det3_201_012 = mat[2][0] * det2_01_12 - mat[2][1] * det2_01_02 + mat[2][2] * det2_01_01;
    float det3_201_013 = mat[2][0] * det2_01_13 - mat[2][1] * det2_01_03 + mat[2][3] * det2_01_01;
    float det3_201_023 = mat[2][0] * det2_01_23 - mat[2][2] * det2_01_03 + mat[2][3] * det2_01_02;
    float det3_201_123 = mat[2][1] * det2_01_23 - mat[2][2] * det2_01_13 + mat[2][3] * det2_01_12;

    det = (-det3_201_123 * mat[3][0] + det3_201_023 * mat[3][1] - det3_201_013 * mat[3][2] + det3_201_012 * mat[3][3]);

    if (fabs(det) < MATRIX_INVERSE_EPSILON) {
        return false;
    }

    invDet = 1.0f / det;

    // remaining 2x2 sub-determinants
    float det2_03_01 = mat[0][0] * mat[3][1] - mat[0][1] * mat[3][0];
    float det2_03_02 = mat[0][0] * mat[3][2] - mat[0][2] * mat[3][0];
    float det2_03_03 = mat[0][0] * mat[3][3] - mat[0][3] * mat[3][0];
    float det2_03_12 = mat[0][1] * mat[3][2] - mat[0][2] * mat[3][1];
    float det2_03_13 = mat[0][1] * mat[3][3] - mat[0][3] * mat[3][1];
    float det2_03_23 = mat[0][2] * mat[3][3] - mat[0][3] * mat[3][2];

    float det2_13_01 = mat[1][0] * mat[3][1] - mat[1][1] * mat[3][0];
    float det2_13_02 = mat[1][0] * mat[3][2] - mat[1][2] * mat[3][0];
    float det2_13_03 = mat[1][0] * mat[3][3] - mat[1][3] * mat[3][0];
    float det2_13_12 = mat[1][1] * mat[3][2] - mat[1][2] * mat[3][1];
    float det2_13_13 = mat[1][1] * mat[3][3] - mat[1][3] * mat[3][1];
    float det2_13_23 = mat[1][2] * mat[3][3] - mat[1][3] * mat[3][2];

    // remaining 3x3 sub-determinants
    float det3_203_012 = mat[2][0] * det2_03_12 - mat[2][1] * det2_03_02 + mat[2][2] * det2_03_01;
    float det3_203_013 = mat[2][0] * det2_03_13 - mat[2][1] * det2_03_03 + mat[2][3] * det2_03_01;
    float det3_203_023 = mat[2][0] * det2_03_23 - mat[2][2] * det2_03_03 + mat[2][3] * det2_03_02;
    float det3_203_123 = mat[2][1] * det2_03_23 - mat[2][2] * det2_03_13 + mat[2][3] * det2_03_12;

    float det3_213_012 = mat[2][0] * det2_13_12 - mat[2][1] * det2_13_02 + mat[2][2] * det2_13_01;
    float det3_213_013 = mat[2][0] * det2_13_13 - mat[2][1] * det2_13_03 + mat[2][3] * det2_13_01;
    float det3_213_023 = mat[2][0] * det2_13_23 - mat[2][2] * det2_13_03 + mat[2][3] * det2_13_02;
    float det3_213_123 = mat[2][1] * det2_13_23 - mat[2][2] * det2_13_13 + mat[2][3] * det2_13_12;

    float det3_301_012 = mat[3][0] * det2_01_12 - mat[3][1] * det2_01_02 + mat[3][2] * det2_01_01;
    float det3_301_013 = mat[3][0] * det2_01_13 - mat[3][1] * det2_01_03 + mat[3][3] * det2_01_01;
    float det3_301_023 = mat[3][0] * det2_01_23 - mat[3][2] * det2_01_03 + mat[3][3] * det2_01_02;
    float det3_301_123 = mat[3][1] * det2_01_23 - mat[3][2] * det2_01_13 + mat[3][3] * det2_01_12;

    mat[0][0] = -det3_213_123 * invDet;
    mat[1][0] = +det3_213_023 * invDet;
    mat[2][0] = -det3_213_013 * invDet;
    mat[3][0] = +det3_213_012 * invDet;

    mat[0][1] = +det3_203_123 * invDet;
    mat[1][1] = -det3_203_023 * invDet;
    mat[2][1] = +det3_203_013 * invDet;
    mat[3][1] = -det3_203_012 * invDet;

    mat[0][2] = +det3_301_123 * invDet;
    mat[1][2] = -det3_301_023 * invDet;
    mat[2][2] = +det3_301_013 * invDet;
    mat[3][2] = -det3_301_012 * invDet;

    mat[0][3] = -det3_201_123 * invDet;
    mat[1][3] = +det3_201_023 * invDet;
    mat[2][3] = -det3_201_013 * invDet;
    mat[3][3] = +det3_201_012 * invDet;

    return true;
}

/* transformation matrices */

/*
 * tx, ty, tz: the x, y, and z coordinates of a translation vector
 *
 *  1  0  0 tx
 *  0  1  0 ty
 *  0  0  1 tz
 *  0  0  0  1
 *
 * see http://docs.gl/gl2/glTranslate
 */
Mat4 Mat4::FromTranslate(float tx, float ty, float tz) {
    Mat4 m;
    m.Indentity();

    m.mat[0][3] = tx;
    m.mat[1][3] = ty;
    m.mat[2][3] = tz;

    return m;
}

/*
 * sx, sy, sz: scale factors along the x, y, and z axes, respectively
 *
 * sx  0  0  0
 *  0 sy  0  0
 *  0  0 sz  0
 *  0  0  0  1
 *
 * see http://docs.gl/gl2/glScale
 */
Mat4 Mat4::FromScale(float sx, float sy, float sz) {
    Mat4 m;
    m.Indentity();

    assert(sx != 0 && sy != 0 && sz != 0);
    m.mat[0][0] = sx;
    m.mat[1][1] = sy;
    m.mat[2][2] = sz;
    
    return m;
}

/*
 * angle: the angle of rotation, in radians
 * vx, vy, vz: the x, y, and z coordinates of a vector, respectively
 *
 * nx*nx*(1-c)+c     ny*nx*(1-c)-s*nz  nz*nx*(1-c)+s*ny  0
 * nx*ny*(1-c)+s*nz  ny*ny*(1-c)+c     nz*ny*(1-c)-s*nx  0
 * nx*nz*(1-c)-s*ny  ny*nz*(1-c)+s*nx  nz*nz*(1-c)+c     0
 * 0                 0                 0                 1
 *
 * nx, ny, nz: the normalized coordinates of the vector, respectively
 * s, c: sin(angle), cos(angle)
 *
 * see http://docs.gl/gl2/glRotate
 */
Mat4 Mat4::FromRotate(float angle, float vx, float vy, float vz) {
    Vec3 n(vx, vy, vz);
    float s, c;

    n.Normalize();
    Math::SinCos(angle, s, c);

    Mat4 mat;

    mat.Indentity();

    mat[0][0] = n.x * n.x * (1 - c) + c;
    mat[0][1] = n.y * n.x * (1 - c) - s * n.z;
    mat[0][2] = n.z * n.x * (1 - c) + s * n.y;

    mat[1][0] = n.x * n.y * (1 - c) + s * n.z;
    mat[1][1] = n.y * n.y * (1 - c) + c;
    mat[1][2] = n.z * n.y * (1 - c) - s * n.x;

    mat[2][0] = n.x * n.z * (1 - c) - s * n.y;
    mat[2][1] = n.y * n.z * (1 - c) + s * n.x;
    mat[2][2] = n.z * n.z * (1 - c) + c;

    return mat;
}

/*
 * angle: the angle of rotation, in radians
 *
 *  1  0  0  0
 *  0  c -s  0
 *  0  s  c  0
 *  0  0  0  1
 *
 * see http://www.songho.ca/opengl/gl_anglestoaxes.html
 */
Mat4 Mat4::FromRotateX(float angle) {
    float s, c;
    Math::SinCos(angle, s, c);

    Mat4 mat;

    mat.Indentity();
    mat[1][1] = c;
    mat[1][2] = -s;
    mat[2][1] = s;
    mat[2][2] = c;

    return mat;
}

/*
 * angle: the angle of rotation, in radians
 *
 *  c  0  s  0
 *  0  1  0  0
 * -s  0  c  0
 *  0  0  0  1
 *
 * see http://www.songho.ca/opengl/gl_anglestoaxes.html
 */
Mat4 Mat4::FromRotateY(float angle) {
    float s, c;
    Math::SinCos(angle, s, c);

    Mat4 mat;

    mat.Indentity();
    mat[0][0] = c;
    mat[0][2] = s;
    mat[2][0] = -s;
    mat[2][2] = c;

    return mat;
}

/*
 * angle: the angle of rotation, in radians
 *
 *  c -s  0  0
 *  s  c  0  0
 *  0  0  1  0
 *  0  0  0  1
 *
 * see http://www.songho.ca/opengl/gl_anglestoaxes.html
 */
Mat4 Mat4::FromRotateZ(float angle) {
    float s, c;
    Math::SinCos(angle, s, c);

    Mat4 mat;

    mat.Indentity();
    mat[0][0] = c;
    mat[0][1] = -s;
    mat[1][0] = s;
    mat[1][1] = c;

    return mat;
}

// Fundamentals of Computer Graphics, Fourth Edition, Chapter 8.7.3 
Mat4 Mat4::FromQuat(const Quat &rotation) {
    float xx = rotation.x * rotation.x;
    float xy = rotation.x * rotation.y;
    float xz = rotation.x * rotation.z;
    float xw = rotation.x * rotation.w;
    float yy = rotation.y * rotation.y;
    float yz = rotation.y * rotation.z;
    float yw = rotation.y * rotation.w;
    float zz = rotation.z * rotation.z;
    float zw = rotation.z * rotation.w;

    Mat4 mat(
        (1.0f - 2.0f * yy - 2.0f * zz), 2.0f * xy - 2.0f * zw, 2.0f * xz + 2.0f * yw, 0,
        2.0f * xy + 2.0f * zw, (1.0f - 2.0f * xx - 2.0f * zz), 2.0f * yz - 2.0f * xw, 0,
        2.0f * xz - 2.0f * yw, 2.0f * yz + 2.0f * xw, (1.0f - 2.0f * xx - 2.0f * yy), 0,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    return mat;
}

}