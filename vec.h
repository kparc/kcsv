#define fill(n)n,1,n,1,n,1,n,1

#ifdef __aarch64__
#include<arm_neon.h>
#define ALIGN_STRUCT(x) __attribute__((aligned(x)))
typedef uint8x16_t V;
typedef int64x2_t __m128i;          //!< 128-bit int vector
#define vreinterpretq_m128i_s8(x)   vreinterpretq_s64_s8(x)
#define vreinterpretq_s8_m128i(x)   vreinterpretq_s8_s64(x)
#define vreinterpretq_m128i_s16(x)  vreinterpretq_s64_s16(x)
#define vreinterpretq_u8_m128i(x)   vreinterpretq_u8_s64(x)
#define vreinterpretq_s16_m128i(x)  vreinterpretq_s16_s64(x)
#define vreinterpretq_m128i_s32(x)  vreinterpretq_s64_s32(x)
#define vreinterpretq_m128i_u16(x)  vreinterpretq_s64_u16(x)
#define vreinterpretq_s32_m128i(x)  vreinterpretq_s32_s64(x)
#define vreinterpretq_m128i_u8(x)   vreinterpretq_s64_u8(x)

Zin __m128i _mm_set1_epi8(char w){R vreinterpretq_m128i_s8(vdupq_n_s8(w));}
Zin __m128i _mm_loadu_si128(const __m128i *p){R vreinterpretq_m128i_s32(vld1q_s32((const int32_t *) p));}

Zin __m128i _mm_setr_epi8(G b0,G b1,G b2,G b3,G b4,G b5,G b6,G b7,G b8,G b9,G b10,G b11,G b12,G b13,G b14,G b15) {
    int8_t ALIGN_STRUCT(16)
        data[16] = {(int8_t) b0,  (int8_t) b1,  (int8_t) b2,  (int8_t) b3,
                    (int8_t) b4,  (int8_t) b5,  (int8_t) b6,  (int8_t) b7,
                    (int8_t) b8,  (int8_t) b9,  (int8_t) b10, (int8_t) b11,
                    (int8_t) b12, (int8_t) b13, (int8_t) b14, (int8_t) b15};
    R (__m128i) vld1q_s8(data);}

Zin __m128i _mm_setr_epi16(H w0,H w1,H w2,H w3,H w4,H w5,H w6,H w7){int16_t ALIGN_STRUCT(16) data[8] = {w0, w1, w2, w3, w4, w5, w6, w7};R vreinterpretq_m128i_s16(vld1q_s16((int16_t *)data));}
Zin __m128i _mm_sub_epi8(__m128i a, __m128i b){R vreinterpretq_m128i_s8(vsubq_s8(vreinterpretq_s8_m128i(a), vreinterpretq_s8_m128i(b)));}

Zin __m128i _mm_maddubs_epi16(__m128i _a, __m128i _b) {
    uint8x16_t a = vreinterpretq_u8_m128i(_a);
    int8x16_t b = vreinterpretq_s8_m128i(_b);
    int16x8_t tl = vmulq_s16(vreinterpretq_s16_u16(vmovl_u8(vget_low_u8(a))),
                             vmovl_s8(vget_low_s8(b)));
    int16x8_t th = vmulq_s16(vreinterpretq_s16_u16(vmovl_u8(vget_high_u8(a))),
                             vmovl_s8(vget_high_s8(b)));
    R vreinterpretq_m128i_s16(vqaddq_s16(vuzp1q_s16(tl, th), vuzp2q_s16(tl, th)));}

Zin __m128i _mm_madd_epi16(__m128i a, __m128i b) {
    int32x4_t low = vmull_s16(vget_low_s16(vreinterpretq_s16_m128i(a)),
                              vget_low_s16(vreinterpretq_s16_m128i(b)));
    int32x4_t high = vmull_s16(vget_high_s16(vreinterpretq_s16_m128i(a)),
                               vget_high_s16(vreinterpretq_s16_m128i(b)));
    int32x2_t low_sum = vpadd_s32(vget_low_s32(low), vget_high_s32(low));
    int32x2_t high_sum = vpadd_s32(vget_low_s32(high), vget_high_s32(high));
    R vreinterpretq_m128i_s32(vcombine_s32(low_sum, high_sum));}

Zin __m128i _mm_packus_epi32(__m128i a, __m128i b) {R vreinterpretq_m128i_u16(vcombine_u16(vqmovun_s32(vreinterpretq_s32_m128i(a)),vqmovun_s32(vreinterpretq_s32_m128i(b))));}
Zin _ _mm_storeu_si128(__m128i *p, __m128i a){vst1q_s32((UI*)p, vreinterpretq_s32_m128i(a));}
Zin __m128i _mm_cmplt_epi8(__m128i a, __m128i b){R vreinterpretq_m128i_u8(vcltq_s8(vreinterpretq_s8_m128i(a), vreinterpretq_s8_m128i(b)))}
Zin __m128i _mm_andnot_si128(__m128i a, __m128i b){R vreinterpretq_m128i_s32(vbicq_s32(vreinterpretq_s32_m128i(b),vreinterpretq_s32_m128i(a)));}
Zin __m128i _mm_cmpeq_epi8(__m128i a, __m128i b){R vreinterpretq_m128i_u8(vceqq_s8(vreinterpretq_s8_m128i(a), vreinterpretq_s8_m128i(b)));}
Zin __m128i _mm_or_si128(__m128i a, __m128i b){R vreinterpretq_m128i_s32(vorrq_s32(vreinterpretq_s32_m128i(a), vreinterpretq_s32_m128i(b)));}
Zin __m128i _mm_subs_epu8(__m128i a, __m128i b){R vreinterpretq_m128i_u8(vqsubq_u8(vreinterpretq_u8_m128i(a), vreinterpretq_u8_m128i(b)));}
Zin _ _mm_store_si128(__m128i *p, __m128i a){vst1q_s32((UI*)p, vreinterpretq_s32_m128i(a));}

#else
#include<x86intrin.h>
#endif

//:~
