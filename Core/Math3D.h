inline void FNegate(const float &a, float &b)
{
	b = -a;
}
inline void FNegate(float &a)
{
	a = -a;
}

inline bool IsNegative(float f)
{
	return f < 0;
}

inline float appRsqrt(float number)
{
	return 1.0f / sqrt(number);
}

template<class T> inline T Lerp(const T& A, const T& B, float Alpha)
{
	return A + Alpha * (B-A);
}


/*-----------------------------------------------------------------------------
	Vector
-----------------------------------------------------------------------------*/

struct CVec3
{
	float	v[3];
	// access to data
	inline float& operator[](int index)
	{
		return v[index];
	}
	inline const float& operator[](int index) const
	{
		return v[index];
	}
	// NOTE: for fnctions, which requires CVec3 -> float*, can easily do it using CVec3.v field
	// trivial setup functions
	inline void Zero()
	{
		memset(this, 0, sizeof(CVec3));
	}
	inline void Set(float x, float y, float z)
	{
		v[0] = x; v[1] = y; v[2] = z;
	}
	// simple math
	inline void Negate()
	{
		FNegate(v[0]);
		FNegate(v[1]);
		FNegate(v[2]);
	}
	//!! +NegateTo(dst)
	inline void Scale(float scale)
	{
		v[0] *= scale;
		v[1] *= scale;
		v[2] *= scale;
	}
	inline void Scale(const CVec3 &scale)
	{
		v[0] *= scale[0];
		v[1] *= scale[1];
		v[2] *= scale[2];
	}
	inline void Add(const CVec3 &a) //?? == "operator +=(CVec3&)"
	{
		v[0] += a.v[0];
		v[1] += a.v[1];
		v[2] += a.v[2];
	}
	inline void Sub(const CVec3 &a) //?? == "operator -=(CVec3&)"
	{
		v[0] -= a.v[0];
		v[1] -= a.v[1];
		v[2] -= a.v[2];
	}
	//!! +ScaleTo(dst)
	float GetLength() const;
	inline float GetLengthSq() const
	{
		return dot(*this, *this);
	}
	float Normalize();			// returns vector length
	float NormalizeFast();		//?? 2-arg version too?
	void FindAxisVectors(CVec3 &right, CVec3 &up) const;

	friend float dot(const CVec3&, const CVec3&);

	friend CArchive& operator<<(CArchive &Ar, CVec3 &V)
	{
		return Ar << V[0] << V[1] << V[2];
	}
};


inline bool operator==(const CVec3 &v1, const CVec3 &v2)
{
	return memcmp(&v1, &v2, sizeof(CVec3)) == 0;
}

inline bool operator!=(const CVec3 &v1, const CVec3 &v2)
{
	return memcmp(&v1, &v2, sizeof(CVec3)) != 0;
}

inline void Lerp(const CVec3 &A, const CVec3 &B, float Alpha, CVec3 &dst)
{
	dst[0] = A.v[0] + Alpha * (B.v[0]-A.v[0]);
	dst[1] = A.v[1] + Alpha * (B.v[1]-A.v[1]);
	dst[2] = A.v[2] + Alpha * (B.v[2]-A.v[2]);
}


inline float dot(const CVec3 &a, const CVec3 &b)
{
//	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
	return a.v[0]*b.v[0] + a.v[1]*b.v[1] + a.v[2]*b.v[2];
}

void cross(const CVec3 &v1, const CVec3 &v2, CVec3 &result);

//?? OLD-STYLE FUNCTIONS :

inline void VectorSubtract(const CVec3 &a, const CVec3 &b, CVec3 &d)
{
	d.v[0] = a.v[0] - b.v[0];
	d.v[1] = a.v[1] - b.v[1];
	d.v[2] = a.v[2] - b.v[2];
}

inline void VectorAdd(const CVec3 &a, const CVec3 &b, CVec3 &d)
{
	d.v[0] = a.v[0] + b.v[0];
	d.v[1] = a.v[1] + b.v[1];
	d.v[2] = a.v[2] + b.v[2];
}

inline void VectorNegate(const CVec3 &a, CVec3 &b)
{
	FNegate(a.v[0], b.v[0]);
	FNegate(a.v[1], b.v[1]);
	FNegate(a.v[2], b.v[2]);
}

inline void VectorScale(const CVec3 &a, float scale, CVec3 &b)
{
	b.v[0] = scale * a.v[0];
	b.v[1] = scale * a.v[1];
	b.v[2] = scale * a.v[2];
}

// d = a + scale * b
inline void VectorMA(const CVec3 &a, float scale, const CVec3 &b, CVec3 &d)
{
	d.v[0] = a.v[0] + scale * b.v[0];
	d.v[1] = a.v[1] + scale * b.v[1];
	d.v[2] = a.v[2] + scale * b.v[2];
}

// a += scale * b
inline void VectorMA(CVec3 &a, float scale, const CVec3 &b)	//!! method, rename?
{
	a.v[0] += scale * b.v[0];
	a.v[1] += scale * b.v[1];
	a.v[2] += scale * b.v[2];
}

float VectorDistance(const CVec3 &vec1, const CVec3 &vec2);
//?? VectorDistanceFast()
float VectorNormalize(const CVec3 &v, CVec3 &out);

/*-----------------------------------------------------------------------------
	Axis aligned box
-----------------------------------------------------------------------------*/

struct CBox
{
	// fields
	CVec3	mins, maxs;
	// methods
	void Clear();							// set mins>maxs (negative volume)
	void Expand(const CVec3 &p);			// expand mins/maxs by point p
	void Expand(const CBox &b);				// include box b into volume
	void GetCenter(CVec3 &p) const;			// compute center point
	bool Contains(const CVec3 &p) const;	// true, when point is inside box volume
	bool Intersects(const CBox &b) const;	// true, when boxes have common volume

	inline void GetVertex(int idx, CVec3& vec) const
	{
		vec[0] = (idx & 1) ? maxs[0] : mins[0];
		vec[1] = (idx & 2) ? maxs[1] : mins[1];
		vec[2] = (idx & 4) ? maxs[2] : mins[2];
	}
	inline void Inflate(float amount)
	{
		mins[0] -= amount;
		mins[1] -= amount;
		mins[2] -= amount;
		maxs[0] += amount;
		maxs[1] += amount;
		maxs[2] += amount;
	}
};

/*-----------------------------------------------------------------------------
	Axis
-----------------------------------------------------------------------------*/

struct CAxis
{
	// fields
	// NOTE: v[] can be private, but this will prevent from
	// initializing CAxis object with initializer list ( "= {n,n,n,n ...}" )
	CVec3	v[3];
	// methods
	void FromEuler(const CVec3 &angles);
	void TransformVector(const CVec3 &src, CVec3 &dst) const;		// orthonormal 'this'
	void TransformVectorSlow(const CVec3 &src, CVec3 &dst) const;	// any 'this'
	void UnTransformVector(const CVec3 &src, CVec3 &dst) const;
	void TransformAxis(const CAxis &src, CAxis &dst) const;			// orthonormal 'this'
	void TransformAxisSlow(const CAxis &src, CAxis &dst) const;		// any 'this'
	void UnTransformAxis(const CAxis &src, CAxis &dst) const;
	void PrescaleSource(const CVec3 &scale);						// scale vector before transformation by this axis
	// indexed access
	inline CVec3& operator[](int index)
	{
		return v[index];
	}
	inline const CVec3& operator[](int index) const
	{
		return v[index];
	}

	friend CArchive& operator<<(CArchive &Ar, CAxis &A)
	{
		return Ar << A.v[0] << A.v[1] << A.v[2];
	}
};


struct CCoords
{
	// fields
	CVec3	origin;
	CAxis	axis;
	// methods
	inline void Zero()
	{
		memset(this, 0, sizeof(CCoords));
	}
	void TransformPoint(const CVec3 &src, CVec3 &dst) const;		// orthonormal 'this'
	void TransformPointSlow(const CVec3 &src, CVec3 &dst) const;	// any 'this'
	void UnTransformPoint(const CVec3 &src, CVec3 &dst) const;
	void TransformCoords(const CCoords &src, CCoords &dst) const;	// orthonormal 'this'
	void TransformCoordsSlow(const CCoords &src, CCoords &dst) const; // any 'this'
	void UnTransformCoords(const CCoords &src, CCoords &dst) const;

	friend CArchive& operator<<(CArchive &Ar, CCoords &C)
	{
		return Ar << C.origin << C.axis;
	}
};

// Functions for work with coordinate systems, not combined into CCoords class

// global coordinate system -> local coordinate system (src -> dst) by origin/axis coords
void TransformPoint(const CVec3 &origin, const CAxis &axis, const CVec3 &src, CVec3 &dst);
// local coordinate system -> global coordinate system
void UnTransformPoint(const CVec3 &origin, const CAxis &axis, const CVec3 &src, CVec3 &dst);
// compute reverse transformation
void InvertCoords(const CCoords &S, CCoords &D);					// orthonormal S
void InvertCoordsSlow(const CCoords &S, CCoords &D);				// any S

inline void CoordsMA(CCoords &a, float scale, const CCoords &b)
{
	VectorMA(a.origin,  scale, b.origin );
	VectorMA(a.axis[0], scale, b.axis[0]);
	VectorMA(a.axis[1], scale, b.axis[1]);
	VectorMA(a.axis[2], scale, b.axis[2]);
}


/*-----------------------------------------------------------------------------
	Angle math
-----------------------------------------------------------------------------*/

// angle indexes
enum
{
	PITCH,							// looking up and down (0=Straight Ahead, +Up, -Down).
	YAW,							// rotating around (running in circles), 0=East, +North, -South.
	ROLL							// rotation about axis of screen, 0=Straight, +Clockwise, -CCW.
};

void  Euler2Vecs(const CVec3 &angles, CVec3 *forward, CVec3 *right, CVec3 *up);
void  Vec2Euler(const CVec3 &vec, CVec3 &angles);
float Vec2Yaw(const CVec3 &vec);


/*-----------------------------------------------------------------------------
	Some constants
-----------------------------------------------------------------------------*/

#define nullVec3	identCoords.origin
//extern const CBox  nullBox;
#define identAxis	identCoords.axis
extern const CCoords identCoords;


/*-----------------------------------------------------------------------------
	Quaternion
-----------------------------------------------------------------------------*/

struct CQuat
{
	float	x, y, z, w;

	void ToAxis(CAxis &dst) const;
	float GetLength() const;

	inline void Zero()
	{
		x = y = z = w = 0;
	}

	inline void Conjugate()
	{
		FNegate(x);
		FNegate(y);
		FNegate(z);
	}
	void Normalize();

	friend CArchive& operator<<(CArchive &Ar, CQuat &Q)
	{
		return Ar << Q.x << Q.y << Q.z << Q.w;
	}
};

void Slerp(const CQuat &A, const CQuat &B, float Alpha, CQuat &dst);


/*-----------------------------------------------------------------------------
	Rotator
-----------------------------------------------------------------------------*/

struct CRotator
{
	int		Yaw, Pitch, Roll;

	inline void Set(int _Yaw, int _Pitch, int _Roll)
	{
		Yaw = _Yaw; Pitch = _Pitch; Roll = _Roll;
	}

	friend CArchive& operator<<(CArchive &Ar, CRotator &R)
	{
		return Ar << R.Pitch << R.Yaw << R.Roll;
	}
};
