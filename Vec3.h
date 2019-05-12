#ifndef VEC_3_H_
#define VEC_3_H_
 
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdint>
#include <limits>
 
//IMPLEMENTAR OPERATOR ==
namespace art
{
    template<typename T>
    class Vec3
    {
        private:
            T x, y, z;
        public:
            Vec3():
                x(0.0), y(0.0), z(0.0){}
            Vec3(const T &n):
                x(n), y(n), z(n){}
            Vec3(const T &s, const T &t, const T &u):
                x(s), y(t), z(u){}
            T const getX() const { return x; }
            T const getY() const { return y; }
            T const getZ() const { return z; }
            Vec3<T> operator + (const T &n) const
            {
                return Vec3<T>(x+n, y+n, z+n);
            }
            Vec3<T> operator + (const Vec3<T> &n) const
            {
                return Vec3<T>(x+n.x, y+n.y, z+n.z);
            }
            Vec3<T> operator - (const T &n) const
            {
                return Vec3<T>(x-n, y-n, z-n);
            }
            Vec3<T> operator - (const Vec3<T> &n) const
            {
                return Vec3<T>(x-n.x,y-n.y,z-n.z);
            }
            Vec3<T> operator * (const T &n) const
            {
                return Vec3<T>(x*n, y*n, z*n);
            }
            Vec3<T> operator * (const Vec3<T> &n) const
            {
                return Vec3<T>(x*n.x, y*n.y, z*n.z);
            }
            Vec3<T> operator / (const T &n) const
            {
                if(n < 0)
                {
                    std::cerr << "Error: divide by zero." << std::endl;
                    exit(101);
                }
                return Vec3<T>(x/n,y/n,z/n);
            }
            Vec3<T> operator / (const Vec3<T> &n) const
            {
                if(n.x < 0 || n.y < 0 || n.z < 0)
                {
                    std::cerr << "Error: divide by zero." << std::endl;
                    exit(101);
                }
                return Vec3<T>(x/n.x,y/n.y,z/n.z);
            }
            Vec3<T> operator += (const T &n)
            {
                return Vec3<T>(x += n, y += n,z += n);
            }
            Vec3<T> operator += (const Vec3<T> &n)
            {
                return Vec3<T>(x += n.x, y += n.y, z += n.z);
            }
            Vec3<T> operator ^= (const Vec3<T> &n)
			{
                return Vec3<T>(x^=n.x,y^=n.y,z^=n.z);
			}
            bool operator == (const Vec3<T> &n)
			{
                return (x == n.x and y == n.y and z == n.z);
			}
            bool operator != (const Vec3<T> &n)
			{
                return !(x == n.x and y == n.y and z == n.z);
			}
            

            T operator [](unsigned i)
            {
                switch(i)
                {
                    case 0: return x;
                    case 1: return y;
                    case 2: return z;
                    default: return x;
                }
                
            }
            friend std::ostream & operator << (std::ostream &os, const Vec3<T> &v)
            {
                os << +v.x << " " << +v.y << " " << +v.z;
                return os;
            }
            T dotProduct(const Vec3<T> &v) const
            {
                return (x*v.x + y*v.y + z*v.z);
            }
            Vec3<T> crossProduct(const Vec3<T> &v) const
            {
                return Vec3<T>((y*v.z) - (z*v.y),
                                 (z*v.x) - (x*v.z),
                                 (x*v.y) - (y*v.x));
            }
            T magnitude(void) const
            {
                return sqrt(x*x + y*y + z*z);
            }
            Vec3<T> &normalize(void)
            {
                T magInv = 1/magnitude();
                x*=magInv, y*=magInv, z*=magInv;
                return *this;
            }
            T sum() const
            {
                return x+y+z;
            }

            T maxValue()
            {
                return std::numeric_limits<T>::max();
            }

            T minValue()
            {
                return std::numeric_limits<T>::min();
            }
            unsigned const sizeOfTypeInBytes() const
            {
                return sizeof(T);
            }
           
    };
   
    typedef Vec3<float> Vec3F;
    typedef Vec3<double> Vec3D;
    typedef Vec3<int> Vec3i;
    typedef Vec3<unsigned char> Vec3UC;
    typedef Vec3<unsigned> Vec3U;
}
   
   
#endif