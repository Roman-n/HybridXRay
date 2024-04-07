#pragma once

template<class T> struct _matrix
{
public:
    typedef T           TYPE;
    typedef _matrix<T>  Self;
    typedef Self&       SelfRef;
    typedef const Self& SelfCRef;
    typedef _vector3<T> Tvector;

public:
    union
    {
        struct
        {   // Direct definition
            T _11, _12, _13, _14;
            T _21, _22, _23, _24;
            T _31, _32, _33, _34;
            T _41, _42, _43, _44;
        };
        struct
        {
            Tvector i;
            T       _14_;
            Tvector j;
            T       _24_;
            Tvector k;
            T       _34_;
            Tvector c;
            T       _44_;
        };
        struct
        {
            __m128 R[4];
        };
        T m[4][4];   // Array
    };
    inline SelfRef& operator=(const SelfRef& right)
    {
        copy(right);
        return *this;
    }
    inline void copy(const Self& right)
    {
        R[0] = right.R[0];
        R[1] = right.R[1];
        R[2] = right.R[2];
        R[3] = right.R[3];
    }
    // Class members
    ICF SelfRef set(const Self& a)
    {
        R[0] = a.R[0];
        R[1] = a.R[1];
        R[2] = a.R[2];
        R[3] = a.R[3];
        return *this;
    }
    ICF SelfRef set(const Tvector& r, const Tvector& N, const Tvector& D, const Tvector& C)
    {
        R[0] = _mm_set_ps(r.x, r.y, r.z, 0);
        R[1] = _mm_set_ps(N.x, N.y, N.z, 0);
        R[2] = _mm_set_ps(D.x, D.y, D.z, 0);
        R[3] = _mm_set_ps(C.x, C.y, C.z, 1);
        return *this;
    }
    ICF SelfRef identity(void)
    {
        R[0] = _mm_set_ps(1, 0, 0, 0);
        R[1] = _mm_set_ps(0, 1, 0, 0);
        R[2] = _mm_set_ps(0, 0, 1, 0);
        R[3] = _mm_set_ps(0, 0, 0, 1);
        return *this;
    }
    IC SelfRef  rotation(const _quaternion<T>& Q);
    ICF SelfRef mk_xform(const _quaternion<T>& Q, const Tvector& V);

    // Multiply RES = A[4x4]*B[4x4] (WITH projection)
    ICF SelfRef mul(const Self& A, const Self& B)
    {
        VERIFY((this != &A) && (this != &B));
        R[0] = _mm_mul_ps(A.R[0], _mm_shuffle_ps(B.R[0], R[0], _MM_SHUFFLE(0, 0, 0, 0)));
        R[0] = _mm_add_ps(R[0], _mm_mul_ps(A.R[1], _mm_shuffle_ps(B.R[0], B.R[0], _MM_SHUFFLE(1, 1, 1, 1))));
        R[0] = _mm_add_ps(R[0], _mm_mul_ps(A.R[2], _mm_shuffle_ps(B.R[0], B.R[0], _MM_SHUFFLE(2, 2, 2, 2))));
        R[0] = _mm_add_ps(R[0], _mm_mul_ps(A.R[3], _mm_shuffle_ps(B.R[0], B.R[0], _MM_SHUFFLE(3, 3, 3, 3))));

        R[1] = _mm_mul_ps(A.R[0], _mm_shuffle_ps(B.R[1], B.R[1], _MM_SHUFFLE(0, 0, 0, 0)));
        R[1] = _mm_add_ps(R[1], _mm_mul_ps(A.R[1], _mm_shuffle_ps(B.R[1], B.R[1], _MM_SHUFFLE(1, 1, 1, 1))));
        R[1] = _mm_add_ps(R[1], _mm_mul_ps(A.R[2], _mm_shuffle_ps(B.R[1], B.R[1], _MM_SHUFFLE(2, 2, 2, 2))));
        R[1] = _mm_add_ps(R[1], _mm_mul_ps(A.R[3], _mm_shuffle_ps(B.R[1], B.R[1], _MM_SHUFFLE(3, 3, 3, 3))));

        R[2] = _mm_mul_ps(A.R[0], _mm_shuffle_ps(B.R[2], B.R[2], _MM_SHUFFLE(0, 0, 0, 0)));
        R[2] = _mm_add_ps(R[2], _mm_mul_ps(A.R[1], _mm_shuffle_ps(B.R[2], B.R[2], _MM_SHUFFLE(1, 1, 1, 1))));
        R[2] = _mm_add_ps(R[2], _mm_mul_ps(A.R[2], _mm_shuffle_ps(B.R[2], B.R[2], _MM_SHUFFLE(2, 2, 2, 2))));
        R[2] = _mm_add_ps(R[2], _mm_mul_ps(A.R[3], _mm_shuffle_ps(B.R[2], B.R[2], _MM_SHUFFLE(3, 3, 3, 3))));

        R[3] = _mm_mul_ps(A.R[0], _mm_shuffle_ps(B.R[3], B.R[3], _MM_SHUFFLE(0, 0, 0, 0)));
        R[3] = _mm_add_ps(R[3], _mm_mul_ps(A.R[1], _mm_shuffle_ps(B.R[3], B.R[3], _MM_SHUFFLE(1, 1, 1, 1))));
        R[3] = _mm_add_ps(R[3], _mm_mul_ps(A.R[2], _mm_shuffle_ps(B.R[3], B.R[3], _MM_SHUFFLE(2, 2, 2, 2))));
        R[3] = _mm_add_ps(R[3], _mm_mul_ps(A.R[3], _mm_shuffle_ps(B.R[3], B.R[3], _MM_SHUFFLE(3, 3, 3, 3))));

        return *this;
    }

    // Multiply RES = A[4x3]*B[4x3] (no projection), faster than ordinary multiply
    ICF SelfRef mul_43(const Self& A, const Self& B)
    {
        VERIFY((this != &A) && (this != &B));
        m[0][0] = A.m[0][0] * B.m[0][0] + A.m[1][0] * B.m[0][1] + A.m[2][0] * B.m[0][2];
        m[0][1] = A.m[0][1] * B.m[0][0] + A.m[1][1] * B.m[0][1] + A.m[2][1] * B.m[0][2];
        m[0][2] = A.m[0][2] * B.m[0][0] + A.m[1][2] * B.m[0][1] + A.m[2][2] * B.m[0][2];
        m[0][3] = 0;

        m[1][0] = A.m[0][0] * B.m[1][0] + A.m[1][0] * B.m[1][1] + A.m[2][0] * B.m[1][2];
        m[1][1] = A.m[0][1] * B.m[1][0] + A.m[1][1] * B.m[1][1] + A.m[2][1] * B.m[1][2];
        m[1][2] = A.m[0][2] * B.m[1][0] + A.m[1][2] * B.m[1][1] + A.m[2][2] * B.m[1][2];
        m[1][3] = 0;

        m[2][0] = A.m[0][0] * B.m[2][0] + A.m[1][0] * B.m[2][1] + A.m[2][0] * B.m[2][2];
        m[2][1] = A.m[0][1] * B.m[2][0] + A.m[1][1] * B.m[2][1] + A.m[2][1] * B.m[2][2];
        m[2][2] = A.m[0][2] * B.m[2][0] + A.m[1][2] * B.m[2][1] + A.m[2][2] * B.m[2][2];
        m[2][3] = 0;

        m[3][0] = A.m[0][0] * B.m[3][0] + A.m[1][0] * B.m[3][1] + A.m[2][0] * B.m[3][2] + A.m[3][0];
        m[3][1] = A.m[0][1] * B.m[3][0] + A.m[1][1] * B.m[3][1] + A.m[2][1] * B.m[3][2] + A.m[3][1];
        m[3][2] = A.m[0][2] * B.m[3][0] + A.m[1][2] * B.m[3][1] + A.m[2][2] * B.m[3][2] + A.m[3][2];
        m[3][3] = 1;
        return *this;
    }
    IC SelfRef mulA_44(const Self& A)   // mul after
    {
        Self B;
        B.set(*this);
        mul(A, B);
        return *this;
    };
    IC SelfRef mulB_44(const Self& B)   // mul before
    {
        Self A;
        A.set(*this);
        mul(A, B);
        return *this;
    };
    ICF SelfRef mulA_43(const Self& A)   // mul after (no projection)
    {
        Self B;
        B.set(*this);
        mul_43(A, B);
        return *this;
    };
    ICF SelfRef mulB_43(const Self& B)   // mul before (no projection)
    {
        Self A;
        A.set(*this);
        mul_43(A, B);
        return *this;
    };
    IC SelfRef invert(const Self& a)
    {   // important: this is 4x3 invert, not the 4x4 one
        // faster than self-invert

        /*__m128 Mask1 = _mm_set_ps(1, -1, 1, 0);
        __m128 Mask2 = _mm_set_ps(-1, 1, -1,0);
        __m128 Mask3 = _mm_set_ps(-1,-1,-1, 0);
        __m128 Mask4 = _mm_set_ps(0, 0, 0,  1);

        __m128 RDetInv;
        {
            __m128 RTemp1 = _mm_mul_ps(_mm_shuffle_ps(B.R[1], R[1], _MM_SHUFFLE(1, 0, 0, 0)), _mm_shuffle_ps(B.R[2],
        R[2], _MM_SHUFFLE(2, 2, 1, 0));
            __m128 RTemp2 = _mm_mul_ps(_mm_shuffle_ps(B.R[1], R[1], _MM_SHUFFLE(2, 2, 1, 0)), _mm_shuffle_ps(B.R[2],
        R[2], _MM_SHUFFLE(1, 0, 0, 0));
            __m128 RTemp3 = _mm_mul_ps(R[0], _mm_sub_ps(RTemp1, RTemp2));
            float Temp3[4];
            _mm_store_ps(Temp3, RTemp3);
            float fDetInv = Temp3[0] - Temp3[1] + Temp3[2];
            VERIFY(_abs(fDetInv) > flt_zero);
            fDetInv = 1.0f / fDetInv;
            RDetInv = _mm_load1_ps(&fDetInv);
        }
        __m128 RTemp1 = _mm_unpacklo_ps(a.R[0], a.R[1]);
         RTemp1 = _mm_shuffle_ps(RTemp1, RTemp1, _MM_SHUFFLE(4, 0, 0, 0));

         __m128 RTemp2 = _mm_unpacklo_ps(a.R[0], a.R[1]);
         RTemp2 = _mm_shuffle_ps(RTemp2, RTemp2, _MM_SHUFFLE(4, 0, 0, 0));


        R[0] = _mm_mul_ps(RDetInv,)
        _11 = fDetInv * (a.m[1][1] * a.m[2][2] - a.m[1][2] * a.m[2][1]);
        _12 = -fDetInv * (a.m[0][1] * a.m[2][2] - a.m[0][2] * a.m[2][1]);
        _13 = fDetInv * (a.m[0][1] * a.m[1][2] - a.m[0][2] * a.m[1][1]);
        _14 = 0.0f;

        _21 = -fDetInv * (a.m[1][0] * a.m[2][2] - a.m[1][2] * a.m[2][0]);
        _22 = fDetInv * (a.m[0][0] * a.m[2][2] - a.m[0][2] * a.m[2][0]);
        _23 = -fDetInv * (a.m[0][0] * a.m[1][2] - a.m[0][2] * a.m[1][0]);
        _24 = 0.0f;

        _31 = fDetInv * (a.m[1][0] * a.m[2][1] - a.m[1][1] * a.m[2][0]);
        _32 = -fDetInv * (a.m[0][0] * a.m[2][1] - a.m[0][1] * a.m[2][0]);
        _33 = fDetInv * (a.m[0][0] * a.m[1][1] - a.m[0][1] * a.m[1][0]);
        _34 = 0.0f;

        _41 = -(a._41 * _11 + a._42 * _21 + a._43 * _31);
        _42 = -(a._41 * _12 + a._42 * _22 + a._43 * _32);
        _43 = -(a._41 * _13 + a._42 * _23 + a._43 * _33);
        _44 = 1.0f;
        return *this;*/
    }

    IC bool invert_b(const Self& a)
    {   // important: this is 4x3 invert, not the 4x4 one
        // faster than self-invert
        /*/T fDetInv = (a._11 * (a._22 * a._33 - a._23 * a._32) -
            a._12 * (a._21 * a._33 - a._23 * a._31) +
            a._13 * (a._21 * a._32 - a._22 * a._31));

        if (_abs(fDetInv) <= flt_zero)	return	false;
        fDetInv = 1.0f / fDetInv;

        _11 = fDetInv * (a._22 * a._33 - a._23 * a._32);
        _12 = -fDetInv * (a._12 * a._33 - a._13 * a._32);
        _13 = fDetInv * (a._12 * a._23 - a._13 * a._22);
        _14 = 0.0f;

        _21 = -fDetInv * (a._21 * a._33 - a._23 * a._31);
        _22 = fDetInv * (a._11 * a._33 - a._13 * a._31);
        _23 = -fDetInv * (a._11 * a._23 - a._13 * a._21);
        _24 = 0.0f;

        _31 = fDetInv * (a._21 * a._32 - a._22 * a._31);
        _32 = -fDetInv * (a._11 * a._32 - a._12 * a._31);
        _33 = fDetInv * (a._11 * a._22 - a._12 * a._21);
        _34 = 0.0f;

        _41 = -(a._41 * _11 + a._42 * _21 + a._43 * _31);
        _42 = -(a._41 * _12 + a._42 * _22 + a._43 * _32);
        _43 = -(a._41 * _13 + a._42 * _23 + a._43 * _33);
        _44 = 1.0f;
        return true;*/
    }

    IC SelfRef invert()   // slower than invert other matrix
    {
        Self a;
        a.set(*this);
        invert(a);
        return *this;
    }
    IC SelfRef transpose(const Self& matSource)   // faster version of transpose
    {
        copy(matSource);
        _MM_TRANSPOSE4_PS(R[0], R[1], R[2], R[3]);
        return *this;
    }
    IC SelfRef transpose()   // self transpose - slower
    {
        _MM_TRANSPOSE4_PS(R[0], R[1], R[2], R[3]);
        return *this;
    }
    IC SelfRef translate(const Tvector& Loc)   // setup translation matrix
    {
        R[0] = _mm_set_ps(1, 0, 0, 0);
        R[1] = _mm_set_ps(0, 1, 0, 0);
        R[2] = _mm_set_ps(0, 0, 1, 0);
        R[3] = _mm_set_ps(Loc.x, Loc.y, Loc.z, 1);
        return *this;
    }
    IC SelfRef translate(T _x, T _y, T _z)   // setup translation matrix
    {
        R[0] = _mm_set_ps(1, 0, 0, 0);
        R[1] = _mm_set_ps(0, 1, 0, 0);
        R[2] = _mm_set_ps(0, 0, 1, 0);
        R[3] = _mm_set_ps(_x, _y, _z, 1);
        return *this;
    }
    IC SelfRef translate_over(const Tvector& Loc)   // modify only translation
    {
        R[3] = _mm_set_ps(Loc.x, Loc.y, Loc.z, R[3].m128_f32[3]);
        return *this;
    }
    IC SelfRef translate_over(T _x, T _y, T _z)   // modify only translation
    {
        R[3] = _mm_set_ps(_x, _y, _z, R[3].m128_f32[3]);
        return *this;
    }
    IC SelfRef translate_add(const Tvector& Loc)   // combine translation
    {
        R[3] = _mm_add_ps(R[3], _mm_set_ps(Loc.x, Loc.y, Loc.z, 0));
        return *this;
    }
    IC SelfRef scale(T x, T y, T z)   // setup scale matrix
    {
        R[0] = _mm_set_ps(x, 0, 0, 0);
        R[1] = _mm_set_ps(0, y, 0, 0);
        R[2] = _mm_set_ps(0, 0, z, 0);
        R[3] = _mm_set_ps(0, 0, 0, 1);
        return *this;
    }
    IC SelfRef scale(const Tvector& v)   // setup scale matrix
    {
        return scale(v.x, v.y, v.z);
    }

    IC SelfRef rotateX(T Angle)   // rotation about X axis
    {
        T cosa = _cos(Angle);
        T sina = _sin(Angle);

        R[0]   = _mm_set_ps(1, 0, 0, 0);
        R[1]   = _mm_set_ps(0, cosa, sina, 0);
        R[2]   = _mm_set_ps(0, -sina, cosa, 0);
        R[3]   = _mm_set_ps(0, 0, 0, 1);
        return *this;
    }
    IC SelfRef rotateY(T Angle)   // rotation about Y axis
    {
        T cosa = _cos(Angle);
        T sina = _sin(Angle);

        R[0]   = _mm_set_ps(cosa, 0, -sina, 0);
        R[1]   = _mm_set_ps(0, 1, 0, 0);
        R[2]   = _mm_set_ps(sina, 0, cosa, 0);
        R[3]   = _mm_set_ps(0, 0, 0, 1);

        return *this;
    }
    IC SelfRef rotateZ(T Angle)   // rotation about Z axis
    {
        T cosa = _cos(Angle);
        T sina = _sin(Angle);

        R[0]   = _mm_set_ps(cosa, sina, 0, 0);
        R[1]   = _mm_set_ps(-sina, cosa, 0, 0);
        R[2]   = _mm_set_ps(0, 0, 1, 0);
        R[3]   = _mm_set_ps(0, 0, 0, 1);

        return *this;
    }

    IC SelfRef rotation(const Tvector& vdir, const Tvector& vnorm)
    {
        Tvector vright;
        vright.crossproduct(vnorm, vdir).normalize();
        R[0] = _mm_set_ps(vright.x, vright.y, vright.z, 0);
        R[1] = _mm_set_ps(vnorm.x, vnorm.y, vnorm.z, 0);
        R[2] = _mm_set_ps(vdir.x, vdir.y, vdir.z, 0);
        R[3] = _mm_set_ps(0, 0, 0, 1);
        return *this;
    }

    IC SelfRef mapXYZ()
    {
        R[0] = _mm_set_ps(1, 0, 0, 0);
        R[1] = _mm_set_ps(0, 1, 0, 0);
        R[2] = _mm_set_ps(0, 0, 1, 0);
        R[3] = _mm_set_ps(0, 0, 0, 1);
        return *this;
    }
    IC SelfRef mapXZY()
    {
        R[0] = _mm_set_ps(1, 0, 0, 0);
        R[1] = _mm_set_ps(0, 0, 1, 0);
        R[2] = _mm_set_ps(0, 1, 0, 0);
        R[3] = _mm_set_ps(0, 0, 0, 1);
        return *this;
    }
    IC SelfRef mapYXZ()
    {
        R[0] = _mm_set_ps(0, 1, 0, 0);
        R[1] = _mm_set_ps(1, 0, 0, 0);
        R[2] = _mm_set_ps(0, 0, 1, 0);
        R[3] = _mm_set_ps(0, 0, 0, 1);
        return *this;
    }
    IC SelfRef mapYZX()
    {
        R[0] = _mm_set_ps(0, 1, 0, 0);
        R[1] = _mm_set_ps(0, 0, 1, 0);
        R[2] = _mm_set_ps(1, 0, 0, 0);
        R[3] = _mm_set_ps(0, 0, 0, 1);
        return *this;
    }
    IC SelfRef mapZXY()
    {
        R[0] = _mm_set_ps(0, 0, 1, 0);
        R[1] = _mm_set_ps(1, 0, 0, 0);
        R[2] = _mm_set_ps(0, 1, 0, 0);
        R[3] = _mm_set_ps(0, 0, 0, 1);
        return *this;
    }
    IC SelfRef mapZYX()
    {
        R[0] = _mm_set_ps(0, 0, 1, 0);
        R[1] = _mm_set_ps(0, 1, 0, 0);
        R[2] = _mm_set_ps(1, 0, 0, 0);
        R[3] = _mm_set_ps(0, 0, 0, 1);
        return *this;
    }

    IC SelfRef rotation(const Tvector& axis, T Angle)
    {
        T     Cosine = _cos(Angle);
        T     Sine   = _sin(Angle);
        float M[4];
        M[0] = axis.x * axis.x + (1 - axis.x * axis.x) * Cosine;
        M[1] = axis.x * axis.y * (1 - Cosine) + axis.z * Sine;
        M[2] = axis.x * axis.z * (1 - Cosine) - axis.y * Sine;
        M[3] = 0;

        R[0] = _mm_load_ps(M);
        M[0] = axis.x * axis.y * (1 - Cosine) - axis.z * Sine;
        M[1] = axis.y * axis.y + (1 - axis.y * axis.y) * Cosine;
        M[2] = axis.y * axis.z * (1 - Cosine) + axis.x * Sine;
        M[3] = 0;
        R[1] = _mm_load_ps(M);

        M[0] = axis.x * axis.z * (1 - Cosine) + axis.y * Sine;
        M[1] = axis.y * axis.z * (1 - Cosine) - axis.x * Sine;
        M[2] = axis.z * axis.z + (1 - axis.z * axis.z) * Cosine;
        M[3] = 0;
        R[2] = _mm_load_ps(M);

        R[3] = _mm_set_ps(0, 0, 0, 1);
        return *this;
    }

    // mirror X
    IC SelfRef mirrorX()
    {
        identity();
        m[0][0] = -1;
        return *this;
    }
    IC SelfRef mirrorX_over()
    {
        m[0][0] = -1;
        return *this;
    }
    IC SelfRef mirrorX_add()
    {
        m[0][0] *= -1;
        return *this;
    }

    // mirror Y
    IC SelfRef mirrorY()
    {
        identity();
        m[1][1] = -1;
        return *this;
    }
    IC SelfRef mirrorY_over()
    {
        m[1][1] = -1;
        return *this;
    }
    IC SelfRef mirrorY_add()
    {
        m[1][1] *= -1;
        return *this;
    }

    // mirror Z
    IC SelfRef mirrorZ()
    {
        identity();
        m[2][2] = -1;
        return *this;
    }
    IC SelfRef mirrorZ_over()
    {
        m[2][2] = -1;
        return *this;
    }
    IC SelfRef mirrorZ_add()
    {
        m[2][2] *= -1;
        return *this;
    }
    IC SelfRef mul(const Self& A, T v)
    {
        __m128 V = _mm_set_ps(v, v, v, v);

        R[0]     = _mm_mul_ps(A.R[0], V);
        R[1]     = _mm_mul_ps(A.R[1], V);
        R[2]     = _mm_mul_ps(A.R[2], V);
        R[3]     = _mm_mul_ps(A.R[3], V);
        return *this;
    }
    IC SelfRef mul(T v)
    {
        __m128 V = _mm_set_ps(v, v, v, v);

        R[0]     = _mm_mul_ps(R[0], V);
        R[1]     = _mm_mul_ps(R[1], V);
        R[2]     = _mm_mul_ps(R[2], V);
        R[3]     = _mm_mul_ps(R[3], V);
        return *this;
    }
    IC SelfRef div(const Self& A, T v)
    {
        VERIFY(_abs(v) > 0.000001f);
        return mul(A, 1.0f / v);
    }
    IC SelfRef div(T v)
    {
        VERIFY(_abs(v) > 0.000001f);
        return mul(1.0f / v);
    }
    // fov
    IC SelfRef build_projection(T fFOV, T fAspect, T fNearPlane, T fFarPlane)
    {
        return build_projection_HAT(tanf(fFOV / 2.f), fAspect, fNearPlane, fFarPlane);
    }
    // half_fov-angle-tangent
    IC SelfRef build_projection_HAT(T HAT, T fAspect, T fNearPlane, T fFarPlane)
    {
        VERIFY(_abs(fFarPlane - fNearPlane) > EPS_S);
        VERIFY(_abs(HAT) > EPS_S);

        T cot = T(1) / HAT;
        T w   = fAspect * cot;
        T h   = T(1) * cot;
        T Q   = fFarPlane / (fFarPlane - fNearPlane);
        R[0]  = _mm_set_ps(w, 0, 0, 0);
        R[1]  = _mm_set_ps(0, h, 0, 0);
        R[2]  = _mm_set_ps(0, 0, Q, 1.f);
        R[3]  = _mm_set_ps(0, 0, -Q * fNearPlane, 0);
        return *this;
    }
    IC SelfRef build_projection_ortho(T w, T h, T zn, T zf)
    {
        R[0] = _mm_set_ps(T(2) / w, 0, 0, 0);
        R[1] = _mm_set_ps(0, T(2) / h, 0, 0);
        R[2] = _mm_set_ps(0, 0, T(1) / (zf - zn), 0);
        R[3] = _mm_set_ps(0, 0, zn / (zn - zf), 1);
        return *this;
    }
    IC SelfRef build_camera(const Tvector& vFrom, const Tvector& vAt, const Tvector& vWorldUp)
    {
        // Get the z basis vector3, which points straight ahead. This is the
        // difference from the eyepoint to the lookat point.
        Tvector vView;
        vView.sub(vAt, vFrom).normalize();

        // Get the dot product, and calculate the projection of the z basis
        // vector3 onto the up vector3. The projection is the y basis vector3.
        T       fDotProduct = vWorldUp.dotproduct(vView);

        Tvector vUp;
        vUp.mul(vView, -fDotProduct).add(vWorldUp).normalize();

        // The x basis vector3 is found simply with the cross product of the y
        // and z basis vectors
        Tvector vRight;
        vRight.crossproduct(vUp, vView);

        // Start building the Device.mView. The first three rows contains the basis
        // vectors used to rotate the view to point at the lookat point
        _11 = vRight.x;
        _12 = vUp.x;
        _13 = vView.x;
        _14 = 0.0f;
        _21 = vRight.y;
        _22 = vUp.y;
        _23 = vView.y;
        _24 = 0.0f;
        _31 = vRight.z;
        _32 = vUp.z;
        _33 = vView.z;
        _34 = 0.0f;

        // Do the translation values (rotations are still about the eyepoint)
        _41 = -vFrom.dotproduct(vRight);
        _42 = -vFrom.dotproduct(vUp);
        _43 = -vFrom.dotproduct(vView);
        _44 = 1.0f;
        return *this;
    }
    IC SelfRef build_camera_dir(const Tvector& vFrom, const Tvector& vView, const Tvector& vWorldUp)
    {
        // Get the dot product, and calculate the projection of the z basis
        // vector3 onto the up vector3. The projection is the y basis vector3.
        T       fDotProduct = vWorldUp.dotproduct(vView);

        Tvector vUp;
        vUp.mul(vView, -fDotProduct).add(vWorldUp).normalize();

        // The x basis vector3 is found simply with the cross product of the y
        // and z basis vectors
        Tvector vRight;
        vRight.crossproduct(vUp, vView);

        // Start building the Device.mView. The first three rows contains the basis
        // vectors used to rotate the view to point at the lookat point
        _11 = vRight.x;
        _12 = vUp.x;
        _13 = vView.x;
        _14 = 0.0f;
        _21 = vRight.y;
        _22 = vUp.y;
        _23 = vView.y;
        _24 = 0.0f;
        _31 = vRight.z;
        _32 = vUp.z;
        _33 = vView.z;
        _34 = 0.0f;

        // Do the translation values (rotations are still about the eyepoint)
        _41 = -vFrom.dotproduct(vRight);
        _42 = -vFrom.dotproduct(vUp);
        _43 = -vFrom.dotproduct(vView);
        _44 = 1.0f;
        return *this;
    }

    IC SelfRef inertion(const Self& mat, T v)
    {
        T iv = 1.f - v;
        for (int i = 0; i < 4; i++)
        {
            m[i][0] = m[i][0] * v + mat.m[i][0] * iv;
            m[i][1] = m[i][1] * v + mat.m[i][1] * iv;
            m[i][2] = m[i][2] * v + mat.m[i][2] * iv;
            m[i][3] = m[i][3] * v + mat.m[i][3] * iv;
        }
        return *this;
    }
    ICF void transform_tiny(Tvector& dest, const Tvector& v) const   // preferred to use
    {
        dest.x = v.x * _11 + v.y * _21 + v.z * _31 + _41;
        dest.y = v.x * _12 + v.y * _22 + v.z * _32 + _42;
        dest.z = v.x * _13 + v.y * _23 + v.z * _33 + _43;
    }
    ICF void transform_tiny32(Fvector2& dest, const Tvector& v) const   // preferred to use
    {
        dest.x = v.x * _11 + v.y * _21 + v.z * _31 + _41;
        dest.y = v.x * _12 + v.y * _22 + v.z * _32 + _42;
    }
    ICF void transform_tiny23(Tvector& dest, const Fvector2& v) const   // preferred to use
    {
        dest.x = v.x * _11 + v.y * _21 + _41;
        dest.y = v.x * _12 + v.y * _22 + _42;
        dest.z = v.x * _13 + v.y * _23 + _43;
    }
    ICF void transform_dir(Tvector& dest, const Tvector& v) const   // preferred to use
    {
        dest.x = v.x * _11 + v.y * _21 + v.z * _31;
        dest.y = v.x * _12 + v.y * _22 + v.z * _32;
        dest.z = v.x * _13 + v.y * _23 + v.z * _33;
    }
    IC void transform(Fvector4& dest, const Tvector& v) const   // preferred to use
    {
        dest.w = v.x * _14 + v.y * _24 + v.z * _34 + _44;
        dest.x = (v.x * _11 + v.y * _21 + v.z * _31 + _41) / dest.w;
        dest.y = (v.x * _12 + v.y * _22 + v.z * _32 + _42) / dest.w;
        dest.z = (v.x * _13 + v.y * _23 + v.z * _33 + _43) / dest.w;
    }
    IC void transform(Tvector& dest, const Tvector& v) const   // preferred to use
    {
        T iw   = 1.f / (v.x * _14 + v.y * _24 + v.z * _34 + _44);
        dest.x = (v.x * _11 + v.y * _21 + v.z * _31 + _41) * iw;
        dest.y = (v.x * _12 + v.y * _22 + v.z * _32 + _42) * iw;
        dest.z = (v.x * _13 + v.y * _23 + v.z * _33 + _43) * iw;
    }

    IC void transform(Fvector4& dest, const Fvector4& v) const   // preferred to use
    {
        dest.w = v.x * _14 + v.y * _24 + v.z * _34 + v.w * _44;
        dest.x = v.x * _11 + v.y * _21 + v.z * _31 + v.w * _41;
        dest.y = v.x * _12 + v.y * _22 + v.z * _32 + v.w * _42;
        dest.z = v.x * _13 + v.y * _23 + v.z * _33 + v.w * _43;
    }

    ICF void transform_tiny(Tvector& v) const
    {
        Tvector res;
        transform_tiny(res, v);
        v.set(res);
    }
    IC void transform(Tvector& v) const
    {
        Tvector res;
        transform(res, v);
        v.set(res);
    }
    ICF void transform_dir(Tvector& v) const
    {
        Tvector res;
        transform_dir(res, v);
        v.set(res);
    }
    ICF SelfRef setHPB(T h, T p, T b)
    {
        T _ch, _cp, _cb, _sh, _sp, _sb, _cc, _cs, _sc, _ss;

        _sh = _sin(h);
        _ch = _cos(h);
        _sp = _sin(p);
        _cp = _cos(p);
        _sb = _sin(b);
        _cb = _cos(b);
        _cc = _ch * _cb;
        _cs = _ch * _sb;
        _sc = _sh * _cb;
        _ss = _sh * _sb;

        i.set(_cc - _sp * _ss, -_cp * _sb, _sp * _cs + _sc);
        _14_ = 0;
        j.set(_sp * _sc + _cs, _cp * _cb, _ss - _sp * _cc);
        _24_ = 0;
        k.set(-_cp * _sh, _sp, _cp * _ch);
        _34_ = 0;
        c.set(0, 0, 0);
        _44_ = 1;
        return *this;
    }
    IC SelfRef setXYZ(T x, T y, T z)
    {
        return setHPB(y, x, z);
    }
    IC SelfRef setXYZ(Tvector const& xyz)
    {
        return setHPB(xyz.y, xyz.x, xyz.z);
    }
    IC SelfRef setXYZi(T x, T y, T z)
    {
        return setHPB(-y, -x, -z);
    }
    IC SelfRef setXYZi(Tvector const& xyz)
    {
        return setHPB(-xyz.y, -xyz.x, -xyz.z);
    }
    //
    IC void getHPB(T& h, T& p, T& b) const
    {
        T cy = _sqrt(j.y * j.y + i.y * i.y);
        if (cy > 16.0f * type_epsilon(T))
        {
            h = (T)-atan2(k.x, k.z);
            p = (T)-atan2(-k.y, cy);
            b = (T)-atan2(i.y, j.y);
        }
        else
        {
            h = (T)-atan2(-i.z, i.x);
            p = (T)-atan2(-k.y, cy);
            b = 0;
        }
    }
    IC void getHPB(Tvector& hpb) const
    {
        getHPB(hpb.x, hpb.y, hpb.z);
    }
    IC void getXYZ(T& x, T& y, T& z) const
    {
        getHPB(y, x, z);
    }
    IC void getXYZ(Tvector& xyz) const
    {
        getXYZ(xyz.x, xyz.y, xyz.z);
    }
    IC void getXYZi(T& x, T& y, T& z) const
    {
        getHPB(y, x, z);
        x *= -1.f;
        y *= -1.f;
        z *= -1.f;
    }
    IC void getXYZi(Tvector& xyz) const
    {
        getXYZ(xyz.x, xyz.y, xyz.z);
        xyz.mul(-1.f);
    }
};

typedef _matrix<float>  Fmatrix;
typedef _matrix<double> Dmatrix;

template<class T> BOOL  _valid(const _matrix<T>& m)
{
    return _valid(m.i) && _valid(m._14_) && _valid(m.j) && _valid(m._24_) && _valid(m.k) && _valid(m._34_) && _valid(m.c) && _valid(m._44_);
}

extern XRCORE_API Fmatrix Fidentity;
extern XRCORE_API Dmatrix Didentity;
