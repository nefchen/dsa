/*
 * Created on 06.03.2021 by nefchen.
 */

#ifndef TYPES_PHYSICS_HPP
#define TYPES_PHYSICS_HPP

#include <cmath>


namespace physics
{
    template <typename T>
    struct Vector3D
    {
        Vector3D(T x=0, T y=0, T z=0): m_x{x}, m_y{y}, m_z{z}
        {};

        inline T length() const
        {
            return (T) std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
        };

        template <typename To>
        inline Vector3D& operator+=(const Vector3D<To>& rhs)
        {
            m_x += rhs.m_x;
            m_y += rhs.m_y;
            m_z += rhs.m_z;

            return *this;
        };

        template <typename To>
        inline Vector3D& operator-=(const Vector3D<To>& rhs)
        {
            m_x -= rhs.m_x;
            m_y -= rhs.m_y;
            m_z -= rhs.m_z;

            return *this;
        };

        template <typename Scalar>
        inline Vector3D& operator*=(const Scalar& rhs)
        {
            m_x *= rhs;
            m_y *= rhs;
            m_z *= rhs;

            return *this;
        };

        T m_x{0};
        T m_y{0};
        T m_z{0};
    };

    template <typename T_lhs, typename T_rhs>
    inline Vector3D<T_lhs> operator+(const Vector3D<T_lhs>& lhs, const Vector3D<T_rhs>& rhs)
    {
        return {lhs.m_x + rhs.m_x, lhs.m_y + rhs.m_y, lhs.m_z + rhs.m_z};
    };

    template <typename T_lhs, typename T_rhs>
    inline Vector3D<T_lhs> operator-(const Vector3D<T_lhs>& lhs, const Vector3D<T_rhs>& rhs)
    {
        return {lhs.m_x - rhs.m_x, lhs.m_y - rhs.m_y, lhs.m_z - rhs.m_z};
    };

    template <typename T, typename Scalar>
    inline Vector3D<T> operator-(const Vector3D<T>& vec, const Scalar& scalar)
    {
        return {vec.m_x * scalar, vec.m_y * scalar, vec.m_z * scalar};
    };

    using Acceleration = Vector3D<double>;
    using Velocity = Vector3D<double>;
    using Position = Vector3D<double>;
    using Size = Vector3D<double>;
}

#endif  // TYPES_PHYSICS_HPP

