#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "..\shared.h"

namespace CG {
	class Material {
	public:
		int shaderID;

	public:
		Material();

		void SetRenderFlags(uint32 f);
		void UnseRenderFlags(uint32 f);
		bool HasRenderFlags(uint32 f) const;

	protected:
		uint32 flags;
	};

	inline Material::Material() : shaderID(0), flags(0)
	{
	}

	inline void Material::SetRenderFlags(uint32 f)
	{
		flags |= f;
	}

	inline void Material::UnseRenderFlags(uint32 f)
	{
		flags &= ~(f);
	}

	inline bool Material::HasRenderFlags(uint32 f) const
	{
		return (flags & f) != 0;
	}
}

#endif