/* 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's 
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok. 
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2011 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement. 
 */


#if !defined(HK_REAL_IS_DOUBLE)
#error This implementation is for 64-Bit double
#endif

HK_FORCE_INLINE /*static*/ const hkSimdReal HK_CALL hkSimdReal::convert(const hkSingleReal& x)
{
	hkSimdReal sr;
	sr.m_real = x;
	return sr;
	//HK_ASSERT(0x7c3a4d98, *(hkUint64*)&(x.m128d_f64[0]) == *(hkUint64*)&(x.m128d_f64[1]) );
}

template<int vectorConstant>
HK_FORCE_INLINE /*static*/ const hkSimdReal HK_CALL hkSimdReal::getConstant()
{
	HK_COMPILE_TIME_ASSERT2( 
		(vectorConstant!=HK_QUADREAL_1000) && (vectorConstant!=HK_QUADREAL_0100) && (vectorConstant!=HK_QUADREAL_0010) && (vectorConstant!=HK_QUADREAL_0001) &&
		(vectorConstant!=HK_QUADREAL_m11m11) && (vectorConstant!=HK_QUADREAL_1248) && (vectorConstant!=HK_QUADREAL_8421)
		, HK_SIMDREAL_ILLEGAL_CONSTANT_REQUEST);
#if HK_SSE_VERSION >= 0x50
	return convert(*(const hkSingleReal*)(g_vectorConstants + vectorConstant));
#else
	return convert((*(g_vectorConstants + vectorConstant)).xy);
#endif
}

HK_FORCE_INLINE /*static*/ const hkSimdReal HK_CALL hkSimdReal::getConstant(hkVectorConstant constant)
{
	HK_ASSERT2( 0x909ff234,
		(constant!=HK_QUADREAL_1000) && (constant!=HK_QUADREAL_0100) && (constant!=HK_QUADREAL_0010) && (constant!=HK_QUADREAL_0001) &&
		(constant!=HK_QUADREAL_m11m11) && (constant!=HK_QUADREAL_1248) && (constant!=HK_QUADREAL_8421)
		, "not a simdreal constant");
#if HK_SSE_VERSION >= 0x50
	return convert(*(const hkSingleReal*)(g_vectorConstants + constant));
#else
	return convert((*(g_vectorConstants + constant)).xy);
#endif
}

#ifndef HK_DISABLE_IMPLICIT_SIMDREAL_FLOAT_CONVERSION
HK_FORCE_INLINE hkSimdReal::hkSimdReal(const hkReal& x)
{
	m_real = _mm_set1_pd(x);
}

HK_FORCE_INLINE hkSimdReal::operator hkReal() const
{
	hkReal s;
	_mm_store_sd(&s, m_real);
	return s;
}
#endif

HK_FORCE_INLINE hkReal hkSimdReal::getReal() const
{
	hkReal s;
	_mm_store_sd(&s, m_real);
	return s;
}

HK_FORCE_INLINE void hkSimdReal::setFromFloat(const hkReal& x)
{
	m_real = _mm_set1_pd(x);
}

HK_FORCE_INLINE void hkSimdReal::setFromHalf(const hkHalf& h)
{
	float x = h;
	m_real = _mm_set1_pd(double(x));
}

HK_FORCE_INLINE void hkSimdReal::setFromInt32(const hkInt32&  x)
{
	m_real = _mm_cvtepi32_pd( _mm_set1_epi32(x) );
}


HK_FORCE_INLINE void hkSimdReal::setFromUint16(const hkUint16& x)
{
	int i = x;
	setFromInt32(i);
}

HK_FORCE_INLINE void hkSimdReal::setFromUint8(const hkUint8& x)
{
	int i = x;
	setFromInt32(i);
}

HK_FORCE_INLINE void hkSimdReal::setZero()
{
	m_real = _mm_setzero_pd();
}


HK_FORCE_INLINE void hkSimdReal::storeSaturateInt32(hkInt32* HK_RESTRICT result) const
{
	*result = _mm_cvtsi128_si32(_mm_cvttpd_epi32(m_real));
}


HK_FORCE_INLINE void hkSimdReal::storeSaturateUint16(hkUint16* HK_RESTRICT result) const
{
	hkSingleReal clamped   = _mm_max_pd(m_real,_mm_setzero_pd());	
	clamped = _mm_min_pd(clamped,_mm_set1_pd(65535.0));
	*result = hkUint16(_mm_cvtsi128_si32(_mm_cvttpd_epi32(clamped)));
}

HK_FORCE_INLINE const hkSimdReal hkSimdReal::operator+ (hkSimdRealParameter r) const
{
	return hkSimdReal::convert(_mm_add_pd(m_real,r.m_real));
}

HK_FORCE_INLINE const hkSimdReal hkSimdReal::operator- (hkSimdRealParameter r) const
{
	return hkSimdReal::convert(_mm_sub_pd(m_real,r.m_real));
}

HK_FORCE_INLINE const hkSimdReal hkSimdReal::operator* (hkSimdRealParameter r) const
{
	return hkSimdReal::convert(_mm_mul_pd(m_real,r.m_real));
}

#ifdef HK_DISABLE_OLD_VECTOR4_INTERFACE

HK_FORCE_INLINE bool hkSimdReal::operator< (hkSimdRealParameter r) const
{
	return _mm_ucomilt_sd(m_real, r.m_real) ? true : false;
}

HK_FORCE_INLINE bool hkSimdReal::operator<= (hkSimdRealParameter r) const
{
	return _mm_ucomile_sd(m_real, r.m_real) ? true : false;
}

HK_FORCE_INLINE bool hkSimdReal::operator> (hkSimdRealParameter r) const
{
	return _mm_ucomigt_sd(m_real, r.m_real) ? true : false;
}

HK_FORCE_INLINE bool hkSimdReal::operator>= (hkSimdRealParameter r) const
{
	return _mm_ucomige_sd(m_real, r.m_real) ? true : false;
}

HK_FORCE_INLINE bool hkSimdReal::operator== (hkSimdRealParameter r) const
{
	return _mm_ucomieq_sd(m_real, r.m_real) ? true : false;
}

HK_FORCE_INLINE bool hkSimdReal::operator!= (hkSimdRealParameter r) const
{
	return _mm_ucomineq_sd(m_real, r.m_real) ? true : false;
}

#endif

HK_FORCE_INLINE const hkSimdReal hkSimdReal::operator-() const
{
	return hkSimdReal::convert(_mm_sub_pd(_mm_setzero_pd(),m_real));
}

HK_FORCE_INLINE const hkVector4Comparison hkSimdReal::less(hkSimdRealParameter a) const
{
	hkVector4Comparison mask;
	const hkSingleReal m = _mm_cmplt_pd(m_real, a.m_real);
#if HK_SSE_VERSION >= 0x50
	mask.m_mask = _mm256_broadcast_pd(&m);
#else
	mask.m_mask.xy = m;
	mask.m_mask.zw = m;
#endif
	return mask;
}

HK_FORCE_INLINE const hkVector4Comparison hkSimdReal::greater(hkSimdRealParameter a) const
{
	hkVector4Comparison mask;
	const hkSingleReal m = _mm_cmpgt_pd(m_real, a.m_real);
#if HK_SSE_VERSION >= 0x50
	mask.m_mask = _mm256_broadcast_pd(&m);
#else
	mask.m_mask.xy = m;
	mask.m_mask.zw = m;
#endif
	return mask;
}

HK_FORCE_INLINE const hkVector4Comparison hkSimdReal::greaterEqual(hkSimdRealParameter a) const
{
	hkVector4Comparison mask;
	const hkSingleReal m = _mm_cmpge_pd(m_real, a.m_real);
#if HK_SSE_VERSION >= 0x50
	mask.m_mask = _mm256_broadcast_pd(&m);
#else
	mask.m_mask.xy = m;
	mask.m_mask.zw = m;
#endif
	return mask;
}

HK_FORCE_INLINE const hkVector4Comparison hkSimdReal::lessEqual(hkSimdRealParameter a) const
{
	hkVector4Comparison mask;
	const hkSingleReal m = _mm_cmple_pd(m_real, a.m_real);
#if HK_SSE_VERSION >= 0x50
	mask.m_mask = _mm256_broadcast_pd(&m);
#else
	mask.m_mask.xy = m;
	mask.m_mask.zw = m;
#endif
	return mask;
}

HK_FORCE_INLINE const hkVector4Comparison hkSimdReal::equal(hkSimdRealParameter a) const
{
	hkVector4Comparison mask;
	const hkSingleReal m = _mm_cmpeq_pd(m_real, a.m_real);
#if HK_SSE_VERSION >= 0x50
	mask.m_mask = _mm256_broadcast_pd(&m);
#else
	mask.m_mask.xy = m;
	mask.m_mask.zw = m;
#endif
	return mask;
}

HK_FORCE_INLINE const hkVector4Comparison hkSimdReal::notEqual(hkSimdRealParameter a) const
{
	hkVector4Comparison mask;
	const hkSingleReal m = _mm_cmpneq_pd(m_real, a.m_real);
#if HK_SSE_VERSION >= 0x50
	mask.m_mask = _mm256_broadcast_pd(&m);
#else
	mask.m_mask.xy = m;
	mask.m_mask.zw = m;
#endif
	return mask;
}

HK_FORCE_INLINE const hkVector4Comparison hkSimdReal::lessZero() const
{
	hkVector4Comparison mask;
	const hkSingleReal m = _mm_cmplt_pd(m_real, _mm_setzero_pd());
#if HK_SSE_VERSION >= 0x50
	mask.m_mask = _mm256_broadcast_pd(&m);
#else
	mask.m_mask.xy = m;
	mask.m_mask.zw = m;
#endif
	return mask;
}

HK_FORCE_INLINE const hkVector4Comparison hkSimdReal::lessEqualZero() const
{
	hkVector4Comparison mask;
	const hkSingleReal m = _mm_cmple_pd(m_real, _mm_setzero_pd());
#if HK_SSE_VERSION >= 0x50
	mask.m_mask = _mm256_broadcast_pd(&m);
#else
	mask.m_mask.xy = m;
	mask.m_mask.zw = m;
#endif
	return mask;
}

HK_FORCE_INLINE const hkVector4Comparison hkSimdReal::greaterZero() const
{
	hkVector4Comparison mask;
	const hkSingleReal m = _mm_cmpgt_pd(m_real, _mm_setzero_pd());
#if HK_SSE_VERSION >= 0x50
	mask.m_mask = _mm256_broadcast_pd(&m);
#else
	mask.m_mask.xy = m;
	mask.m_mask.zw = m;
#endif
	return mask;
}

HK_FORCE_INLINE const hkVector4Comparison hkSimdReal::greaterEqualZero() const
{
	hkVector4Comparison mask;
	const hkSingleReal m = _mm_cmpge_pd(m_real, _mm_setzero_pd());
#if HK_SSE_VERSION >= 0x50
	mask.m_mask = _mm256_broadcast_pd(&m);
#else
	mask.m_mask.xy = m;
	mask.m_mask.zw = m;
#endif
	return mask;
}

HK_FORCE_INLINE const hkVector4Comparison hkSimdReal::equalZero() const
{
	hkVector4Comparison mask;
	const hkSingleReal m = _mm_cmpeq_pd(m_real, _mm_setzero_pd());
#if HK_SSE_VERSION >= 0x50
	mask.m_mask = _mm256_broadcast_pd(&m);
#else
	mask.m_mask.xy = m;
	mask.m_mask.zw = m;
#endif
	return mask;
}

HK_FORCE_INLINE const hkVector4Comparison hkSimdReal::notEqualZero() const
{
	hkVector4Comparison mask;
	const hkSingleReal m = _mm_cmpneq_pd(m_real, _mm_setzero_pd());
#if HK_SSE_VERSION >= 0x50
	mask.m_mask = _mm256_broadcast_pd(&m);
#else
	mask.m_mask.xy = m;
	mask.m_mask.zw = m;
#endif
	return mask;
}

HK_FORCE_INLINE hkBool32 hkSimdReal::isSignBitSet() const
{
	return _mm_movemask_pd(m_real);
}

HK_FORCE_INLINE hkBool32 hkSimdReal::isSignBitClear() const
{
	return !_mm_movemask_pd(m_real);
}

HK_FORCE_INLINE const hkVector4Comparison hkSimdReal::signBitSet() const
{
	hkVector4Comparison mask;
	static HK_ALIGN16( const hkUint64 signmask[2] ) = { 0x8000000000000000, 0x8000000000000000 };
	const hkSingleReal c = _mm_and_pd(m_real, *(const hkSingleReal*)&signmask);
#if HK_SSE_VERSION >= 0x41
	const hkSingleReal m = _mm_castsi128_pd(_mm_cmpeq_epi64(_mm_castpd_si128(c), *(const __m128i*)&signmask));
#else
	const __m128i mm = _mm_cmpeq_epi32(_mm_castpd_si128(c), *(const __m128i*)&signmask);
	const hkSingleReal m = _mm_castsi128_pd(_mm_shuffle_epi32(mm, _MM_SHUFFLE(2,2,0,0)));
#endif
#if HK_SSE_VERSION >= 0x50
	mask.m_mask = _mm256_broadcast_pd(&m);
#else
	mask.m_mask.xy = m;
	mask.m_mask.zw = m;
#endif
	return mask;
}

HK_FORCE_INLINE const hkVector4Comparison hkSimdReal::signBitClear() const
{
	hkVector4Comparison mask;
	static HK_ALIGN16( const hkUint64 signmask[2] ) = { 0x7fffffffffffffff, 0x7fffffffffffffff };
	const hkSingleReal c = _mm_and_pd(m_real, *(const hkSingleReal*)&signmask);
	const hkSingleReal m = _mm_cmpeq_pd(c, m_real);
#if HK_SSE_VERSION >= 0x50
	mask.m_mask = _mm256_broadcast_pd(&m);
#else
	mask.m_mask.xy = m;
	mask.m_mask.zw = m;
#endif
	return mask;
}

HK_FORCE_INLINE hkBool32 hkSimdReal::isLess(hkSimdRealParameter a) const
{
	return _mm_ucomilt_sd(m_real, a.m_real);
}

HK_FORCE_INLINE hkBool32 hkSimdReal::isLessEqual(hkSimdRealParameter a) const
{
	return _mm_ucomile_sd(m_real, a.m_real);
}

HK_FORCE_INLINE hkBool32 hkSimdReal::isGreater(hkSimdRealParameter a) const
{
	return _mm_ucomigt_sd(m_real, a.m_real);
}

HK_FORCE_INLINE hkBool32 hkSimdReal::isGreaterEqual(hkSimdRealParameter a) const
{
	return _mm_ucomige_sd(m_real, a.m_real);
}

HK_FORCE_INLINE hkBool32 hkSimdReal::isEqual(hkSimdRealParameter a) const
{
	return _mm_ucomieq_sd(m_real, a.m_real);
}

HK_FORCE_INLINE hkBool32 hkSimdReal::isNotEqual(hkSimdRealParameter a) const
{
	return _mm_ucomineq_sd(m_real, a.m_real);
}

HK_FORCE_INLINE hkBool32 hkSimdReal::isLessZero() const
{
	return _mm_ucomilt_sd(m_real, _mm_setzero_pd());
}

HK_FORCE_INLINE hkBool32 hkSimdReal::isLessEqualZero() const
{
	return _mm_ucomile_sd(m_real, _mm_setzero_pd());
}

HK_FORCE_INLINE hkBool32 hkSimdReal::isGreaterZero() const
{
	return _mm_ucomigt_sd(m_real, _mm_setzero_pd());
}

HK_FORCE_INLINE hkBool32 hkSimdReal::isGreaterEqualZero() const
{
	return _mm_ucomige_sd(m_real, _mm_setzero_pd());
}

HK_FORCE_INLINE hkBool32 hkSimdReal::isEqualZero() const
{
	return _mm_ucomieq_sd(m_real, _mm_setzero_pd());
}

HK_FORCE_INLINE hkBool32 hkSimdReal::isNotEqualZero() const
{
	return _mm_ucomineq_sd(m_real, _mm_setzero_pd());
}

HK_FORCE_INLINE hkBool32 hkSimdReal::isOk() const
{
	const hkSingleReal nanMask = _mm_cmpunord_pd(m_real, _mm_setzero_pd());
	return !_mm_movemask_pd(nanMask);
}

HK_FORCE_INLINE void hkSimdReal::setSelect( hkVector4ComparisonParameter comp, hkSimdRealParameter trueValue, hkSimdRealParameter falseValue )
{
	HK_ASSERT2(0xc78f08e, comp.allAreSet() || (comp.getMask() == hkVector4Comparison::MASK_NONE), "illegal compare mask");
#if HK_SSE_VERSION >= 0x50
	m_real = _mm_blendv_pd(falseValue.m_real, trueValue.m_real, _mm256_extractf128_pd(comp.m_mask,0));
#elif HK_SSE_VERSION >= 0x41
	m_real = _mm_blendv_pd(falseValue.m_real, trueValue.m_real, comp.m_mask.xy);
#else
	m_real = _mm_or_pd( _mm_and_pd(comp.m_mask.xy, trueValue.m_real), _mm_andnot_pd(comp.m_mask.xy, falseValue.m_real) );
#endif
}

HK_FORCE_INLINE void hkSimdReal::setMin(  hkSimdRealParameter a, hkSimdRealParameter b ) 
{
	m_real = _mm_min_pd( a.m_real, b.m_real );
}

HK_FORCE_INLINE void hkSimdReal::setMax(  hkSimdRealParameter a, hkSimdRealParameter b ) 
{
	m_real = _mm_max_pd( a.m_real, b.m_real );
}

HK_FORCE_INLINE void hkSimdReal::setAbs(  hkSimdRealParameter a )
{
	static HK_ALIGN16( const hkUint64 absMask[2] ) = { 0x7fffffffffffffff, 0x7fffffffffffffff }; // todo constant
	m_real = _mm_and_pd(a.m_real, *(const hkSingleReal*)&absMask);
}

HK_FORCE_INLINE void hkSimdReal::setFlipSign(hkSimdRealParameter v, hkSimdRealParameter sSign)
{
	static HK_ALIGN16( const hkUint64 signmask[2] ) = { 0x8000000000000000, 0x8000000000000000 };
	m_real = _mm_xor_pd(v.m_real, _mm_and_pd(sSign.m_real, *(const hkSingleReal*)&signmask));
}

HK_FORCE_INLINE void hkSimdReal::setFlipSign(hkSimdRealParameter v, hkVector4ComparisonParameter mask)
{
	HK_ASSERT2(0x5831c04, mask.allAreSet() || (mask.getMask() == hkVector4Comparison::MASK_NONE), "illegal compare mask");
	static HK_ALIGN16( const hkUint64 signmask[2] ) = { 0x8000000000000000, 0x8000000000000000 };
#if HK_SSE_VERSION >= 0x50
	m_real = _mm_xor_pd(v.m_real, _mm_and_pd(_mm256_extractf128_pd(mask.m_mask,0), *(const hkSingleReal*)&signmask));
#else
	m_real = _mm_xor_pd(v.m_real, _mm_and_pd(mask.m_mask.xy, *(const hkSingleReal*)&signmask));
#endif
}

template <int N>
HK_FORCE_INLINE void hkSimdReal::store(  hkReal *f ) const
{
	HK_SIMDREAL_DIMENSION_CHECK;
	_mm_store_sd( f, m_real );
}

HK_FORCE_INLINE void hkSimdReal::storeToHalf(  hkHalf *f ) const
{
	hkReal r;
	_mm_store_sd( &r, m_real );
	*f = float(r);
}

template <int N>
HK_FORCE_INLINE void hkSimdReal::load(const hkReal *f )
{
	HK_SIMDREAL_DIMENSION_CHECK;
	m_real = _mm_load1_pd(f);
}


//
// advanced interface
//

namespace hkSimdReal_AdvancedInterface
{

template <hkMathAccuracyMode A, hkMathDivByZeroMode D>
struct unroll_setReciprocal { HK_FORCE_INLINE static void apply(hkSingleReal& self, hkSimdRealParameter a)
{
	HK_SIMDREAL_TEMPLATE_CONFIG_NOT_IMPLEMENTED;
} };
template <hkMathAccuracyMode A>
struct unroll_setReciprocal<A, HK_DIV_IGNORE> { HK_FORCE_INLINE static void apply(hkSingleReal& self, hkSimdRealParameter a)
{
	switch (A)
	{
		case HK_ACC_23_BIT: 
			{
				const __m128 fr = _mm_cvtpd_ps(a.m_real);
				const __m128 r = _mm_rcp_ps(fr);
				const hkSingleReal rb = _mm_cvtps_pd(r);
				// One Newton-Raphson refinement iteration
				const hkSingleReal rbr = _mm_mul_pd(a.m_real, rb);
				const hkSingleReal d = _mm_sub_pd(*(const hkSingleReal*)(g_vectorConstants + HK_QUADREAL_2), rbr);
				self = _mm_mul_pd(rb, d);
			}
			break;
		case HK_ACC_12_BIT: 
			{
				const __m128 fr = _mm_cvtpd_ps(a.m_real);
				const __m128 r = _mm_rcp_ps(fr);
				self = _mm_cvtps_pd(r);
			}
			break;
		default:
			{
				self = _mm_div_pd(*(const hkSingleReal*)(g_vectorConstants + HK_QUADREAL_1),a.m_real); 
			}
			break; // HK_ACC_FULL
	}
} };
template <hkMathAccuracyMode A>
struct unroll_setReciprocal<A, HK_DIV_SET_ZERO> { HK_FORCE_INLINE static void apply(hkSingleReal& self, hkSimdRealParameter a)
{
	const hkSingleReal equalsZero = _mm_cmpeq_pd(a.m_real, _mm_setzero_pd());
	hkSingleReal e; unroll_setReciprocal<A, HK_DIV_IGNORE>::apply(e, a);
	self = _mm_andnot_pd(equalsZero, e);
} };
template <hkMathAccuracyMode A>
struct unroll_setReciprocal<A, HK_DIV_SET_HIGH> { HK_FORCE_INLINE static void apply(hkSingleReal& self, hkSimdRealParameter a)
{
	const hkSingleReal equalsZero = _mm_cmpeq_pd(a.m_real, _mm_setzero_pd());
	hkSingleReal e; unroll_setReciprocal<A, HK_DIV_IGNORE>::apply(e, a);
	const hkSingleReal huge = _mm_set1_pd(HK_REAL_HIGH);
#if HK_SSE_VERSION >= 0x41
	self = _mm_blendv_pd(e, huge, equalsZero);
#else
	self = _mm_or_pd( _mm_and_pd(equalsZero, huge), _mm_andnot_pd(equalsZero, e) );
#endif
} };
template <hkMathAccuracyMode A>
struct unroll_setReciprocal<A, HK_DIV_SET_MAX> { HK_FORCE_INLINE static void apply(hkSingleReal& self, hkSimdRealParameter a)
{
	const hkSingleReal equalsZero = _mm_cmpeq_pd(a.m_real, _mm_setzero_pd());
	hkSingleReal e; unroll_setReciprocal<A, HK_DIV_IGNORE>::apply(e, a);
	const hkSingleReal huge = _mm_set1_pd(HK_REAL_MAX);
#if HK_SSE_VERSION >= 0x41
	self = _mm_blendv_pd(e, huge, equalsZero);
#else
	self = _mm_or_pd( _mm_and_pd(equalsZero, huge), _mm_andnot_pd(equalsZero, e) );
#endif
} };
template <hkMathAccuracyMode A>
struct unroll_setReciprocal<A, HK_DIV_SET_ZERO_AND_ONE> { HK_FORCE_INLINE static void apply(hkSingleReal& self, hkSimdRealParameter a)
{
	static HK_ALIGN16( const hkUint64 absMask[2] ) = { 0x7fffffffffffffff, 0x7fffffffffffffff }; // todo constant
	unroll_setReciprocal<A, HK_DIV_SET_ZERO>::apply(self, a);
	const hkSingleReal one = g_vectorConstants[HK_QUADREAL_1].xy;
	const hkSingleReal absVal = _mm_and_pd(self, *(const hkSingleReal*)&absMask);
	const hkSingleReal absValLessOne = _mm_sub_pd(absVal, one);
	const hkSingleReal lessEqualEps = _mm_cmple_pd(absValLessOne, g_vectorConstants[HK_QUADREAL_EPS].xy);
#if HK_SSE_VERSION >= 0x41
	self = _mm_blendv_pd(self, one, lessEqualEps);
#else
	self = _mm_or_pd( _mm_and_pd(lessEqualEps, one), _mm_andnot_pd(lessEqualEps, self) );
#endif
} };

} // namespace 

template <hkMathAccuracyMode A, hkMathDivByZeroMode D> 
HK_FORCE_INLINE void hkSimdReal::setReciprocal(hkSimdRealParameter a)
{
	hkSimdReal_AdvancedInterface::unroll_setReciprocal<A,D>::apply(m_real,a);
}

HK_FORCE_INLINE void hkSimdReal::setReciprocal(hkSimdRealParameter a)
{
	hkSimdReal_AdvancedInterface::unroll_setReciprocal<HK_ACC_23_BIT,HK_DIV_IGNORE>::apply(m_real,a);
}



namespace hkSimdReal_AdvancedInterface
{

template <hkMathAccuracyMode A, hkMathDivByZeroMode D>
struct unroll_setDiv { HK_FORCE_INLINE static void apply(hkSimdReal& self, hkSimdRealParameter a, hkSimdRealParameter b)
{
	hkSingleReal t;
	unroll_setReciprocal<A,D>::apply(t,b);
	self.m_real = _mm_mul_pd(a.m_real,t);
} };
template <hkMathAccuracyMode A>
struct unroll_setDiv<A, HK_DIV_IGNORE> { HK_FORCE_INLINE static void apply(hkSimdReal& self, hkSimdRealParameter a, hkSimdRealParameter b)
{
	if (A == HK_ACC_FULL) 
	{ 
		self.m_real = _mm_div_pd(a.m_real, b.m_real); 
	}
	else
	{
		hkSingleReal t;
		unroll_setReciprocal<A,HK_DIV_IGNORE>::apply(t,b);
		self.m_real = _mm_mul_pd(a.m_real,t);
	}
} };

} // namespace 

template <hkMathAccuracyMode A, hkMathDivByZeroMode D> 
HK_FORCE_INLINE void hkSimdReal::setDiv(hkSimdRealParameter a, hkSimdRealParameter b)
{
	hkSimdReal_AdvancedInterface::unroll_setDiv<A,D>::apply(*this,a,b);
}

HK_FORCE_INLINE void hkSimdReal::setDiv(hkSimdRealParameter a, hkSimdRealParameter b)
{
	hkSimdReal_AdvancedInterface::unroll_setDiv<HK_ACC_23_BIT,HK_DIV_IGNORE>::apply(*this,a,b);
}

template <hkMathAccuracyMode A, hkMathDivByZeroMode D> 
HK_FORCE_INLINE void hkSimdReal::div(hkSimdRealParameter a)
{
	setDiv<A,D>( *this, a );
}

HK_FORCE_INLINE void hkSimdReal::div(hkSimdRealParameter a)
{
	setDiv( *this, a );
}

HK_FORCE_INLINE const hkSimdReal hkSimdReal::operator/ (hkSimdRealParameter r) const
{
	hkSimdReal q;
	q.setDiv(*this,r);
	return q;
}


namespace hkSimdReal_AdvancedInterface
{

template <hkMathAccuracyMode A, hkMathNegSqrtMode S>
struct unroll_sqrt { HK_FORCE_INLINE static hkSingleReal apply(hkSimdRealParameter self)
{
	HK_SIMDREAL_TEMPLATE_CONFIG_NOT_IMPLEMENTED;
	return _mm_setzero_pd();
} };
template <hkMathAccuracyMode A>
struct unroll_sqrt<A, HK_SQRT_IGNORE> { HK_FORCE_INLINE static hkSingleReal apply(hkSimdRealParameter self)
{
	switch (A)
	{
		case HK_ACC_23_BIT: 
			{
				const __m128 fr = _mm_cvtpd_ps(self.m_real);
				const __m128 re = _mm_rsqrt_ps(fr);
				const hkSingleReal e = _mm_cvtps_pd(re);
				// One Newton-Raphson refinement iteration
				const hkSingleReal he = _mm_mul_pd(*(const hkSingleReal*)(g_vectorConstants + HK_QUADREAL_INV_2),e);
				const hkSingleReal ree = _mm_mul_pd(_mm_mul_pd(self.m_real,e),e);
				hkSingleReal refined = _mm_mul_pd(he, _mm_sub_pd(*(const hkSingleReal*)(g_vectorConstants + HK_QUADREAL_3), ree) );
				return _mm_mul_pd(self.m_real, refined);
			}
			break;
		case HK_ACC_12_BIT: 
			{
				const __m128 fr = _mm_cvtpd_ps(self.m_real);
				const __m128 re = _mm_rsqrt_ps(fr);
				hkSingleReal e = _mm_cvtps_pd(re);
				return _mm_mul_pd(self.m_real, e);
			}
			break;
		default:
			{
				return _mm_sqrt_pd(self.m_real); 
			}
			break; // HK_ACC_FULL
	}
} };
template <hkMathAccuracyMode A>
struct unroll_sqrt<A, HK_SQRT_SET_ZERO> { HK_FORCE_INLINE static hkSingleReal apply(hkSimdRealParameter self)
{
	const hkSingleReal equalsZero = _mm_cmple_pd(self.m_real, _mm_setzero_pd());
	const hkSingleReal e = unroll_sqrt<A, HK_SQRT_IGNORE>::apply(self);
	return _mm_andnot_pd(equalsZero, e);
} };

} // namespace 

template <hkMathAccuracyMode A, hkMathNegSqrtMode S> 
HK_FORCE_INLINE const hkSimdReal hkSimdReal::sqrt() const
{
	return hkSimdReal::convert(hkSimdReal_AdvancedInterface::unroll_sqrt<A,S>::apply(*this));
}

HK_FORCE_INLINE const hkSimdReal hkSimdReal::sqrt() const
{
	return hkSimdReal::convert(hkSimdReal_AdvancedInterface::unroll_sqrt<HK_ACC_23_BIT,HK_SQRT_SET_ZERO>::apply(*this));
}


namespace hkSimdReal_AdvancedInterface
{

template <hkMathAccuracyMode A, hkMathNegSqrtMode S>
struct unroll_sqrtInverse { HK_FORCE_INLINE static hkSingleReal apply(hkSimdRealParameter self)
{
	HK_SIMDREAL_TEMPLATE_CONFIG_NOT_IMPLEMENTED;
	return _mm_setzero_pd();
} };
template <hkMathAccuracyMode A>
struct unroll_sqrtInverse<A, HK_SQRT_IGNORE> { HK_FORCE_INLINE static hkSingleReal apply(hkSimdRealParameter self)
{
	switch (A)
	{
		case HK_ACC_23_BIT: 
			{
				const __m128 fr = _mm_cvtpd_ps(self.m_real);
				const __m128 re = _mm_rsqrt_ps(fr);
				const hkSingleReal e = _mm_cvtps_pd(re);
				// One Newton-Raphson refinement iteration
				const hkSingleReal he = _mm_mul_pd(*(const hkSingleReal*)(g_vectorConstants + HK_QUADREAL_INV_2),e);
				const hkSingleReal ree = _mm_mul_pd(_mm_mul_pd(self.m_real,e),e);
				return _mm_mul_pd(he, _mm_sub_pd(*(const hkSingleReal*)(g_vectorConstants + HK_QUADREAL_3), ree) );
			}
			break;
		case HK_ACC_12_BIT: 
			{
				const __m128 fr = _mm_cvtpd_ps(self.m_real);
				const __m128 re = _mm_rsqrt_ps(fr);
				return _mm_cvtps_pd(re);
			}
			break;
		default:         
			{
				return _mm_div_pd(*(const hkSingleReal*)(g_vectorConstants + HK_QUADREAL_1), _mm_sqrt_pd(self.m_real));
			}
			break; // HK_ACC_FULL
	}
} };
template <hkMathAccuracyMode A>
struct unroll_sqrtInverse<A, HK_SQRT_SET_ZERO> { HK_FORCE_INLINE static hkSingleReal apply(hkSimdRealParameter self)
{
	const hkSingleReal equalsZero = _mm_cmple_pd(self.m_real, _mm_setzero_pd());
	const hkSingleReal e = unroll_sqrtInverse<A, HK_SQRT_IGNORE>::apply(self);
	return _mm_andnot_pd(equalsZero, e);
} };
} // namespace 

template <hkMathAccuracyMode A, hkMathNegSqrtMode S> 
HK_FORCE_INLINE const hkSimdReal hkSimdReal::sqrtInverse() const
{
	return hkSimdReal::convert(hkSimdReal_AdvancedInterface::unroll_sqrtInverse<A,S>::apply(*this));
}

HK_FORCE_INLINE const hkSimdReal hkSimdReal::sqrtInverse() const
{
	return hkSimdReal::convert(hkSimdReal_AdvancedInterface::unroll_sqrtInverse<HK_ACC_23_BIT,HK_SQRT_SET_ZERO>::apply(*this));
}

/* 
 * Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20110913) 
 *  
 * Confidential Information of Havok.  (C) Copyright 1999-2011 
 * Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok 
 * Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership 
 * rights, and intellectual property rights in the Havok software remain in 
 * Havok and/or its suppliers. 
 *  
 * Use of this software for evaluation purposes is subject to and indicates 
 * acceptance of the End User licence Agreement for this product. A copy of 
 * the license is included with this software and is also available at www.havok.com/tryhavok. 
 *  
 */
