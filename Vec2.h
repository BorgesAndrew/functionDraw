#ifndef VEC_2_H_
#define VEC_2_H_

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
	class Vec2
	{
		private:
			T x, y;
		public:
			Vec2():
				x(0.0), y(0.0){}
			Vec2(const T &n):
				x(n), y(n){}
			Vec2(const T &s, const T &t):
				x(s), y(t){}
			T const getX() const { return x; }
			T const getY() const { return y; }
			Vec2<T> operator + (const T &n) const
			{
				return Vec2<T>(x+n, y+n);
			}
			Vec2<T> operator + (const Vec2<T> &n) const
			{
				return Vec2<T>(x+n.x, y+n.y);
			}
			Vec2<T> operator - (const T &n) const
			{
				return Vec2<T>(x-n, y-n);
			}
			Vec2<T> operator - (const Vec2<T> &n) const
			{
				return Vec2<T>(x-n.x,y-n.y);
			}
			Vec2<T> operator * (const T &n) const
			{
				return Vec2<T>(x*n, y*n);
			}
			Vec2<T> operator * (const Vec2<T> &n) const
			{
				return Vec2<T>(x*n.x, y*n.y);
			}
			Vec2<T> operator / (const T &n) const
			{
				if(n < 0)
				{
					std::cerr << "Error: divide by zero." << std::endl;
					exit(101);
				}
				return Vec2<T>(x/n,y/n);
			}
			Vec2<T> operator / (const Vec2<T> &n) const
			{
				if(n.x < 0 || n.y < 0)
				{
					std::cerr << "Error: divide by zero." << std::endl;
					exit(101);
				}
				return Vec2<T>(x/n.x,y/n.y);
			}
			Vec2<T> operator += (const T &n)
			{
				return Vec2<T>(x += n, y += n);
			}
			Vec2<T> operator += (const Vec2<T> &n)
			{
				return Vec2<T>(x += n.x, y += n.y);
			}
                         bool operator == (const Vec2<T> &n)
			{
                                 return (x == n.x and y == n.y);
			}
                         bool operator != (const Vec2<T> &n)
			{
                                 return !(x == n.x and y == n.y);
			}
			friend std::ostream & operator << (std::ostream &os, const Vec2<T> &v)
			{
				os << +v.x << " " << +v.y;
				return os;
			}
			T dotProduct(const Vec2<T> &v) const
			{
				return (x*v.x + y*v.y);
			}
			Vec2<T> crossProduct(const Vec2<T> &v) const
			{
				return Vec2<T>(x*v.y - y*v.x);
			}
			T magnitude(void) const
			{
				return sqrt(x*x + y*y);
			}
			Vec2<T> &normalize(void)
			{
				T magInv = 1/magnitude();
				x*=magInv, y*=magInv;
				return *this;
			}
			T sum() const
			{
				return x+y;
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
	
	typedef Vec2<float> Vec2F;
	typedef Vec2<double> Vec2D;
	typedef Vec2<int> Vec2i;
	typedef Vec2<unsigned char> Vec2UC;
	typedef Vec2<unsigned> Vec2U;
}
	
	
#endif