// This is generated file, don't change anything!

#include "glsl_source_vfs.h"
#include "Engine/STL/Containers/HashMap.h"

namespace glsl_vfs
{
	extern void VFS_Common_Cmp (OUT String &src);
	extern void VFS_Common_Defines (OUT String &src);
	extern void VFS_Common_GlobalIndex (OUT String &src);
	extern void VFS_Common_TypeInfo (OUT String &src);
	extern void VFS_Effects_Blur (OUT String &src);
	extern void VFS_Hash_DHash (OUT String &src);
	extern void VFS_Hash_Hash (OUT String &src);
	extern void VFS_Hash_Hash2 (OUT String &src);
	extern void VFS_Hash_IHash (OUT String &src);
	extern void VFS_Math_BitMath (OUT String &src);
	extern void VFS_Math_Color (OUT String &src);
	extern void VFS_Math_Math (OUT String &src);
	extern void VFS_Math_MathDef (OUT String &src);
	extern void VFS_Math_Matrix (OUT String &src);
	extern void VFS_Math_Quaternion (OUT String &src);
	extern void VFS_Math_Utils (OUT String &src);
	extern void VFS_Math2D_Line2 (OUT String &src);
	extern void VFS_Math2D_Rect (OUT String &src);
	extern void VFS_Math3D_AABBox (OUT String &src);
	extern void VFS_Math3D_Line3 (OUT String &src);
	extern void VFS_Math3D_Plane (OUT String &src);
	extern void VFS_Math3D_Ray (OUT String &src);
	extern void VFS_Noise_Billow (OUT String &src);
	extern void VFS_Noise_Cellular (OUT String &src);
	extern void VFS_Noise_FBM (OUT String &src);
	extern void VFS_Noise_Gabor (OUT String &src);
	extern void VFS_Noise_Perlin (OUT String &src);
	extern void VFS_Noise_Simplex (OUT String &src);
	extern void VFS_Noise_Turbulence (OUT String &src);
	extern void VFS_Noise_Voronoi (OUT String &src);
	extern void VFS_Noise_VoronoiLines (OUT String &src);
	extern void VFS_Noise_VoronoiNoise (OUT String &src);
	extern void VFS_Noise__NoiseUtils (OUT String &src);
	extern void VFS_Particles_Colors (OUT String &src);
	extern void VFS_Particles_Emitters (OUT String &src);
	extern void VFS_Physics_Gravity (OUT String &src);
	extern void VFS_Physics_Magnetism (OUT String &src);
	extern void VFS_SDF_DefaultSDF (OUT String &src);

	bool LoadFile (StringCRef filename, OUT String &src)
	{
		static const HashMap< String, void (*) (OUT String &) > vfs = {
			{ "Common/Cmp.glsl", &VFS_Common_Cmp },
			{ "Common/Defines.glsl", &VFS_Common_Defines },
			{ "Common/GlobalIndex.glsl", &VFS_Common_GlobalIndex },
			{ "Common/TypeInfo.glsl", &VFS_Common_TypeInfo },
			{ "Effects/Blur.glsl", &VFS_Effects_Blur },
			{ "Hash/DHash.glsl", &VFS_Hash_DHash },
			{ "Hash/Hash.glsl", &VFS_Hash_Hash },
			{ "Hash/Hash2.glsl", &VFS_Hash_Hash2 },
			{ "Hash/IHash.glsl", &VFS_Hash_IHash },
			{ "Math/BitMath.glsl", &VFS_Math_BitMath },
			{ "Math/Color.glsl", &VFS_Math_Color },
			{ "Math/Math.glsl", &VFS_Math_Math },
			{ "Math/MathDef.glsl", &VFS_Math_MathDef },
			{ "Math/Matrix.glsl", &VFS_Math_Matrix },
			{ "Math/Quaternion.glsl", &VFS_Math_Quaternion },
			{ "Math/Utils.glsl", &VFS_Math_Utils },
			{ "Math2D/Line2.glsl", &VFS_Math2D_Line2 },
			{ "Math2D/Rect.glsl", &VFS_Math2D_Rect },
			{ "Math3D/AABBox.glsl", &VFS_Math3D_AABBox },
			{ "Math3D/Line3.glsl", &VFS_Math3D_Line3 },
			{ "Math3D/Plane.glsl", &VFS_Math3D_Plane },
			{ "Math3D/Ray.glsl", &VFS_Math3D_Ray },
			{ "Noise/Billow.glsl", &VFS_Noise_Billow },
			{ "Noise/Cellular.glsl", &VFS_Noise_Cellular },
			{ "Noise/FBM.glsl", &VFS_Noise_FBM },
			{ "Noise/Gabor.glsl", &VFS_Noise_Gabor },
			{ "Noise/Perlin.glsl", &VFS_Noise_Perlin },
			{ "Noise/Simplex.glsl", &VFS_Noise_Simplex },
			{ "Noise/Turbulence.glsl", &VFS_Noise_Turbulence },
			{ "Noise/Voronoi.glsl", &VFS_Noise_Voronoi },
			{ "Noise/VoronoiLines.glsl", &VFS_Noise_VoronoiLines },
			{ "Noise/VoronoiNoise.glsl", &VFS_Noise_VoronoiNoise },
			{ "Noise/_NoiseUtils.glsl", &VFS_Noise__NoiseUtils },
			{ "Particles/Colors.glsl", &VFS_Particles_Colors },
			{ "Particles/Emitters.glsl", &VFS_Particles_Emitters },
			{ "Physics/Gravity.glsl", &VFS_Physics_Gravity },
			{ "Physics/Magnetism.glsl", &VFS_Physics_Magnetism },
			{ "SDF/DefaultSDF.glsl", &VFS_SDF_DefaultSDF }
		};

		usize idx = UMax;
		if ( vfs.FindIndex( filename, OUT idx ) ) {
			vfs[idx].second( OUT src );
			return true;
		}
		return false;
	}
}	// glsl_vfs
