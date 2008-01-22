/*==============================================================================
	C++ class definitions exported from script.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .uc files instead!
==============================================================================*/


#ifndef __ANIMCLASSES_H__
#define __ANIMCLASSES_H__

/*------------------------------------------------------------------------------
	CAnimSet class (declared in AnimSet.uc)
------------------------------------------------------------------------------*/

#define MAX_BONE_NAME			32
#define MAX_ANIM_NAME			64

struct CAnimBone
{
	TString<MAX_BONE_NAME>		Name;

	friend CArchive& operator<<(CArchive &Ar, CAnimBone &B)
	{
		return Ar << B.Name;
	}
};


/**
 * Raw keyframe data for one track.  Each array will contain either NumKey elements or 1 element.
 * One element is used as a simple compression scheme where if all keys are the same, they'll be
 * reduced to 1 key that is constant over the entire sequence.
 */
struct CAnalogTrack
{
	/**
	 * Rotation keys
	 */
	TArray<CQuat>				KeyQuat;
	/**
	 * Position keys
	 */
	TArray<CVec3>				KeyPos;
	/**
	 * Key times, in seconds
	 */
	TArray<float>				KeyTime;

	friend CArchive& operator<<(CArchive &Ar, CAnalogTrack &T)
	{
		return Ar << T.KeyQuat << T.KeyPos << T.KeyTime;
	}
};


struct CMeshAnimSeq
{
	/**
	 * Name of the animation sequence
	 */
	TString<MAX_ANIM_NAME>		Name;
	/**
	 * Number for tweaking playback rate of this animation globally
	 */
	float						Rate;
	/**
	 * Length (in seconds) of this AnimSequence if played back with a speed of 1
	 */
	int							NumFrames;
	/**
	 * Raw keyframe data for this sequence
	 */
	TArray<CAnalogTrack>		Tracks;

	friend CArchive &operator<<(CArchive &Ar, CMeshAnimSeq &A)
	{
		return Ar << A.Name << A.Rate << A.NumFrames << A.Tracks;
	}
};


/**
 * This is a set of AnimSequences
 * All sequence have the same number of tracks, and they relate to the same bone names.
 */
class CAnimSet : public CObject
{
	DECLARE_CLASS(CAnimSet, CObject)
public:
	/**
	 * Bone name that each track relates to. TrackBoneName.Num() == Number of tracks.
	 */
	TArray<CAnimBone>			TrackBoneName;
	/**
	 * Actual animation sequence information
	 */
	TArray<CMeshAnimSeq>		Sequences;
	/**
	 * Indicates that only the rotation should be taken from the animation sequence and the translation
	 * should come from the SkeletalMesh ref pose. Note that the root bone always takes translation from
	 * the animation, even if this flag is set.
	 */
	bool						AnimRotationOnly;

	CAnimSet()
	:	AnimRotationOnly(true)
	{}

	virtual void Serialize(CArchive &Ar)
	{
		Super::Serialize(Ar);
		Ar << TrackBoneName << Sequences << AnimRotationOnly;
	}
};


/*------------------------------------------------------------------------------
	CSkeletalMesh class (declared in SkeletalMesh.uc)
------------------------------------------------------------------------------*/

#define MAX_MESH_BONES			256
#define MAX_MESH_MATERIALS		256
#define MAX_VERTEX_INFLUENCES	4
#define NO_INFLUENCE			-1

struct CPointWeight
{
	/**
	 * NO_INFLUENCE == entry not used
	 */
	short						BoneIndex;
	/**
	 * 0==0.0f, 65535==1.0f
	 */
	word						Weight;

	friend CArchive& operator<<(CArchive &Ar, CPointWeight &W)
	{
		return Ar << W.BoneIndex << W.Weight;
	}
};


struct CMeshPoint
{
	CVec3						Point;
	CVec3						Normal;
	float						U;
	float						V;
	CPointWeight				Influences[MAX_VERTEX_INFLUENCES];

	friend CArchive& operator<<(CArchive &Ar, CMeshPoint &P)
	{
		Ar << P.Point << P.Normal << P.U << P.V;
		for (int i = 0; i < MAX_VERTEX_INFLUENCES; i++)
			Ar << P.Influences[i];
		return Ar;
	}
};


struct CMeshBone
{
	TString<MAX_BONE_NAME>		Name;
	CVec3						Position;
	CQuat						Orientation;
	int							ParentIndex;
	/**
	 * following data generated after mesh loading
	 */
	CCoords						InvRefCoords;
	int							SubtreeSize;

	friend CArchive& operator<<(CArchive &Ar, CMeshBone &B)
	{
		return Ar << B.Name << B.Position << B.Orientation << AR_INDEX(B.ParentIndex);
	}
};


struct CMeshSection
{
	int							MaterialIndex;
	int							FirstIndex;
	int							NumIndices;

	friend CArchive& operator<<(CArchive &Ar, CMeshSection &S)
	{
		return Ar << AR_INDEX(S.MaterialIndex) << S.FirstIndex << S.NumIndices;
	}
};


struct CSkeletalMeshLod
{
	TArray<CMeshSection>		Sections;
	TArray<CMeshPoint>			Points;
	TArray<int>					Indices;

	friend CArchive& operator<<(CArchive &Ar, CSkeletalMeshLod &L)
	{
		return Ar << L.Sections << L.Points << L.Indices;
	}
};


struct CMeshMaterial
{

	friend class CRenderingMaterial;
	/**
	 * Name of the material file
	 */
	TString<64>					Filename;
	/**
	 * Material, used internally in renderer
	 */
	CRenderingMaterial*			RenMaterial;

	friend CArchive& operator<<(CArchive &Ar, CMeshMaterial &M)
	{
		return Ar << M.Filename;
	}
};


class CSkeletalMesh : public CObject
{
	DECLARE_CLASS(CSkeletalMesh, CObject)
public:
	/**
	 * Origin in original coordinate system
	 */
	CVec3						MeshOrigin;
	/**
	 * Amount to scale mesh when importing
	 */
	CVec3						MeshScale;
	/**
	 * Amount to rotate when importing
	 */
	CRotator					RotOrigin;
	/**
	 * Information for LOD levels
	 */
	TArray<CSkeletalMeshLod>	Lods;
	/**
	 * Skeleton bones
	 */
	TArray<CMeshBone>			Skeleton;
	TArray<CMeshMaterial>		Materials;
	/**
	 * Following data generated after loading
	 */
	CCoords						BaseTransform;
	CCoords						BaseTransformScaled;

	CSkeletalMesh();
	~CSkeletalMesh();
	virtual void PostLoad();
	virtual void PostEditChange();
	void DumpBones();
	bool BindMaterial(int index);

	virtual void Serialize(CArchive &Ar)
	{
		Super::Serialize(Ar);
		Ar << MeshOrigin << MeshScale << RotOrigin << Lods << Skeleton << Materials;
	}
};


#endif // __ANIMCLASSES_H__