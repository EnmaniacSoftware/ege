#ifndef EGE_CORE_FLAGS_H
#define EGE_CORE_FLAGS_H

/** Class encpsulating type-safe OR-combinations of enum values.
 */

#include <EGETypes.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
class EGEFlags
{
	public:
	
		EGEFlags();
		EGEFlags(const EGEFlags& f);
		EGEFlags(Enum f);

		inline EGEFlags&  operator  = (const EGEFlags& f);
		inline EGEFlags&  operator &= (s32 mask);
		inline EGEFlags&  operator &= (u32 mask);
		inline EGEFlags&  operator |= (EGEFlags f);
		inline EGEFlags&  operator |= (Enum f);
		inline EGEFlags&  operator ^= (EGEFlags f);
		inline EGEFlags&  operator ^= (Enum f);
		inline EGEFlags   operator | (EGEFlags f) const;
		inline EGEFlags   operator | (Enum f) const;
		inline EGEFlags   operator ^ (EGEFlags f) const;
		inline EGEFlags   operator ^ (Enum f) const;
		inline EGEFlags   operator & (s32 mask) const;
		inline EGEFlags   operator & (u32 mask) const;
		inline EGEFlags   operator & (Enum f) const;
		inline EGEFlags   operator ~ () const;
		inline            operator s32 () const;
		inline bool       operator ! () const;

	private:

		/*! Internal data storing ORed flags. */
		s32 m_i;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum>::EGEFlags(const EGEFlags& f) : m_i(f.m_i)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum>::EGEFlags(Enum f) : m_i(f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum>::EGEFlags() : m_i(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum>& EGEFlags<Enum>::operator = (const EGEFlags& f)
{ 
  m_i = f.m_i; 
  return *this; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum>& EGEFlags<Enum>::operator &= (s32 mask) 
{ 
  m_i &= mask; return *this; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum>& EGEFlags<Enum>::operator &= (u32 mask) 
{ 
  m_i &= mask; 
  return *this; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum>& EGEFlags<Enum>::operator |= (EGEFlags f) 
{ 
  m_i |= f.m_i; 
  return *this; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum>& EGEFlags<Enum>::operator |= (Enum f) 
{ 
  m_i |= f; 
  return *this; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum>& EGEFlags<Enum>::operator ^= (EGEFlags f) 
{ 
  m_i ^= f.m_i; 
  return *this; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum>& EGEFlags<Enum>::operator ^= (Enum f) 
{ 
  m_i ^= f; 
  return *this; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum>::operator s32 () const 
{ 
  return m_i; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum> EGEFlags<Enum>::operator | (EGEFlags f) const 
{ 
  EGEFlags g; 
  g.m_i = m_i | f.m_i; 
  return g; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum> EGEFlags<Enum>::operator | (Enum f) const 
{ 
  EGEFlags g; 
  g.m_i = m_i | f; 
  return g; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum> EGEFlags<Enum>::operator ^ (EGEFlags f) const 
{ 
  EGEFlags g; 
  g.m_i = m_i ^ f.m_i; 
  return g; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum> EGEFlags<Enum>::operator ^ (Enum f) const 
{ 
  EGEFlags g; 
  g.m_i = m_i ^ f; 
  return g; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum> EGEFlags<Enum>::operator & (s32 mask) const 
{ 
  EGEFlags g; 
  g.m_i = m_i & mask; 
  return g; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum> EGEFlags<Enum>::operator & (u32 mask) const 
{ 
  EGEFlags g; 
  g.m_i = m_i & mask; 
  return g; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum> EGEFlags<Enum>::operator & (Enum f) const 
{ 
  EGEFlags g; 
  g.m_i = m_i & f; 
  return g; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
EGEFlags<Enum> EGEFlags<Enum>::operator ~ () const 
{ 
  EGEFlags g; 
  g.m_i = ~m_i; 
  return g; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
bool EGEFlags<Enum>::operator ! () const 
{ 
  return !m_i; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename Enum>
inline EGEFlags<Enum> operator | (Enum f1, Enum f2)
{ 
  return EGEFlags<Enum>(f1) | f2; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_FLAGS_H