#ifndef EGE_CORE_FLAGS_H
#define EGE_CORE_FLAGS_H

#include <EGETypes.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename Enum>
class EGEFlags
{
	//typedef void** Zero;

	public:
	
		//typedef Enum enum_type;
		
		inline EGEFlags(const EGEFlags &f) : m_i(f.m_i) {}
		inline EGEFlags(Enum f) : m_i(f) {}
		inline EGEFlags() : m_i(0) {}
		//inline EGEFlags(QFlag f) : m_i(f) {}

		inline EGEFlags &operator=(const EGEFlags &f) { m_i = f.m_i; return *this; }
		inline EGEFlags &operator&=(s32 mask) { m_i &= mask; return *this; }
		inline EGEFlags &operator&=(u32 mask) { m_i &= mask; return *this; }
		inline EGEFlags &operator|=(EGEFlags f) { m_i |= f.m_i; return *this; }
		inline EGEFlags &operator|=(Enum f) { m_i |= f; return *this; }
		inline EGEFlags &operator^=(EGEFlags f) { m_i ^= f.m_i; return *this; }
		inline EGEFlags &operator^=(Enum f) { m_i ^= f; return *this; }

		inline operator s32() const { return m_i; }

		inline EGEFlags operator|(EGEFlags f) const { EGEFlags g; g.m_i = m_i | f.m_i; return g; }
		inline EGEFlags operator|(Enum f) const { EGEFlags g; g.m_i = m_i | f; return g; }
		inline EGEFlags operator^(EGEFlags f) const { EGEFlags g; g.m_i = m_i ^ f.m_i; return g; }
		inline EGEFlags operator^(Enum f) const { EGEFlags g; g.m_i = m_i ^ f; return g; }
		inline EGEFlags operator&(s32 mask) const { EGEFlags g; g.m_i = m_i & mask; return g; }
		inline EGEFlags operator&(u32 mask) const { EGEFlags g; g.m_i = m_i & mask; return g; }
		inline EGEFlags operator&(Enum f) const { EGEFlags g; g.m_i = m_i & f; return g; }
		inline EGEFlags operator~() const { EGEFlags g; g.m_i = ~m_i; return g; }

		inline bool operator!() const { return !m_i; }

		//inline bool testFlag(Enum f) const { return (m_i & f) == f && (f != 0 || i == int(f) ); }

	private:

		/*! Internal data storing ORed flags. */
		s32 m_i;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define EGE_DECLARE_FLAGS(Flags, Enum) typedef EGEFlags<Enum> Flags;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_FLAGS_H