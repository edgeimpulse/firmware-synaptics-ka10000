/*------------------------------------------------------------------------------
  INTERNAL USE ONLY

  Unpublished Work Copyright (C) 2010-2020 Synaptics Incorporated.

  All rights reserved.

  This file contains information that is proprietary to Synaptics Incorporated
  ("Synaptics"). The holder of this file shall treat all information contained
  herein as confidential, shall use the information only for its intended
  purpose, and shall not duplicate, disclose, or disseminate any of this
  information in any manner unless Synaptics has otherwise provided express,
  written permission.
  
  Use of the materials may require a license of intellectual property from a
  third party or from Synaptics. This file conveys no express or implied
  licenses to any intellectual property rights belonging to Synaptics.
--------------------------------------------------------------------------------

  File Name: cape_types.h

  Description: Special data types that are used in CAPE

------------------------------------------------------------------------------*/

#ifndef AUDIODSP_CAF_INCLUDE_CAPE_TYPES_H_
#define AUDIODSP_CAF_INCLUDE_CAPE_TYPES_H_

#ifdef __cplusplus

#if !defined(CONFIG_CAF_FLAVOR_CAPE2) 

#if !defined CONFIG_NO_FLOATING_POINT

#if defined _MSC_VER
#pragma warning (push)
#pragma warning (disable:4201) /* nonstandard extension used : nameless struct/union */
#endif

typedef struct
{
  union {
    int64_t  b64;
    struct {
      float   re;
      float   im;
    };
  };
} cfloat;

#if defined _MSC_VER
#pragma warning (pop)
#endif

#endif // !defined CONFIG_NO_FLOATING_POINT

class q23_t;
class q8_23_t;
class q31_t;
class q17_46_t;
class q9_54_t;
class q1_62_t;

class pow2_t {
public:
  int32_t   val;

  pow2_t(void) : val(0) {} // default constructor

  pow2_t(const int a) : val(a) {}
};

class q23_t {
public:
  int32_t   val;

  q23_t(void) : val(0) {} // default constructor

  q23_t(const float a) : val(((a<=-1.f)?(int32_t)(-1LL<<23):((a>=1.f)?(int32_t)((1LL<<23)-1):(int32_t)(a * (float)(1LL<<23)))) << 8) {}

  operator float() const {return ((float)val * (1.f/(1LL<<31)));}

  q23_t & operator = (const q23_t & a) { // assignment operator
    val = a.val;
    return (*this);
  }

  q23_t & operator+=(const q23_t & b);
  q23_t & operator-=(const q23_t & b);
  q23_t & operator*=(const pow2_t& b);
};

class q8_23_t {
public:
  int32_t   val;

  q8_23_t(void) : val(0) {} // default constructor

  q8_23_t(const float   a) : val((a<=-256.f)?(int32_t)(-1LL<<31):((a>=256.f)?(int32_t)((1LL<<31)-1):(int32_t)(a * (float)(1LL<<23)))) {}
  q8_23_t(const q9_54_t a);

  operator float() const {return ((float)val * (1.f/(1LL<<23)));}

  q8_23_t & operator = (const q8_23_t & a) { // assignment operator
    val = a.val;
    return (*this);
  }

  q8_23_t & operator+=(const q8_23_t & b);
  q8_23_t & operator-=(const q8_23_t & b);
  q8_23_t & operator*=(const pow2_t  & b);
};

class q31_t {
public:
  int32_t   val;

  q31_t(void) : val(0) {} // default constructor

  q31_t(const float   a) : val((a<=-1.f)?(int32_t)(-1LL<<31):((a>=1.f)?(int32_t)((1LL<<31)-1):(int32_t)(a * (float)(1LL<<31)))) {}
  q31_t(const q9_54_t a);
  q31_t(const q1_62_t a);

  operator float() const {return ((float)val * (1.f/(1LL<<31)));}

  q31_t & operator = (const q31_t & a) { // assignment operator
    val = a.val;
    return (*this);
  }

  q31_t & operator+=(const q31_t & b);
  q31_t & operator-=(const q31_t & b);
  q31_t & operator*=(const pow2_t& b);
};

class q9_54_t {
public:
  int64_t   val;

  q9_54_t(void) : val(0) {} // default constructor

  q9_54_t(const float a) : val((a<=-512.f)?(int64_t)(-1LL<<63):((a>=512.f)?(int64_t)(((1LL<<62)-1)+(1LL<<62)):(int64_t)(a * (float)(1LL<<54)))) {}
  q9_54_t(const q8_23_t    a);
  
  operator float() const {return ((float)val * (1.f/(1LL<<54)));}

  q9_54_t & operator = (const q9_54_t & a) { // assignment operator
    val = a.val;
    return (*this);
  }
};

class q1_62_t {
public:
  int64_t   val;

  q1_62_t(void) : val(0) {} // default constructor

  q1_62_t(const float a) : val((a<=-2.f)?(int64_t)(-1LL<<63):((a>=2.f)?(int64_t)(((1LL<<62)-1)+(1LL<<62)):(int64_t)(a * (float)(1LL<<62)))) {}

  operator float() const {return ((float)val * (1.f/(1LL<<62)));}

  q1_62_t & operator = (const q1_62_t & a) { // assignment operator
    val = a.val;
    return (*this);
  }
};

inline q8_23_t::q8_23_t(const q9_54_t a) : val((int32_t)((a.val + (1LL<<30)) >> 31)) {}

inline q31_t::q31_t    (const q9_54_t a) : val((int32_t)((a.val + (1LL<<22)) >> 23)) {}
inline q31_t::q31_t    (const q1_62_t a) : val((int32_t)((a.val + (1LL<<30)) >> 31)) {}
inline q9_54_t::q9_54_t(const q8_23_t a) : val(((int64_t)(a.val )) << 31) {}


inline pow2_t  pow2      (const int32_t a) {pow2_t  x; x.val = a; return (x);}

inline q23_t   as_q23_t  (const int32_t a) {q23_t x; x.val = a & 0xFFFFFF00; return (x);}
inline q8_23_t as_q8_23_t(const int32_t a) {q8_23_t x; x.val = a; return (x);}
inline q31_t   as_q31_t  (const int32_t a) {q31_t   x; x.val = a; return (x);}
inline q9_54_t as_q9_54_t(const int64_t a) {q9_54_t x; x.val = a; return (x);}
inline q1_62_t as_q1_62_t(const int64_t a) {q1_62_t x; x.val = a; return (x);}

inline int32_t as_int(const q23_t   & a) {return (a.val);}
inline int32_t as_int(const q8_23_t & a) {return (a.val);}
inline int32_t as_int(const q31_t   & a) {return (a.val);}

inline q23_t operator+(const q23_t & a, const q23_t & b) {
  q23_t x;
  int64_t r = (int64_t)(a.val) + (int64_t)(b.val);
  x.val = (int32_t)r;
  if ((int64_t)(x.val) != r) {
    x.val = (r < 0) ? 0x80000000 : 0x7FFFFF00;
  }
  return (x);
}

inline q23_t operator-(const q23_t & a, const q23_t & b) {
  q23_t x;
  int64_t r = (int64_t)(a.val) - (int64_t)(b.val);
  x.val = (int32_t)r;
  if ((int64_t)(x.val) != r) {
    x.val = (r < 0) ? 0x80000000 : 0x7FFFFF00;
  }
  return (x);
}

inline q8_23_t operator+(const q8_23_t & a, const q8_23_t & b) {
  q8_23_t x;
  int64_t r = (int64_t)(a.val) + (int64_t)(b.val);
  x.val = (int32_t)r;
  if ((int64_t)(x.val) != r) {
    x.val = (r < 0) ? 0x80000000 : 0x7FFFFFFF;
  }
  return (x);
}

inline q8_23_t operator-(const q8_23_t & a, const q8_23_t & b) {
  q8_23_t x;
  int64_t r = (int64_t)(a.val) - (int64_t)(b.val);
  x.val = (int32_t)r;
  if ((int64_t)(x.val) != r) {
    x.val = (r < 0) ? 0x80000000 : 0x7FFFFFFF;
  }
  return (x);
}

inline q31_t operator+(const q31_t & a, const q31_t & b) {
  q31_t x;
  int64_t r = (int64_t)(a.val) + (int64_t)(b.val);
  x.val = (int32_t)r;
  if ((int64_t)(x.val) != r) {
    x.val = (r < 0) ? 0x80000000 : 0x7FFFFFFF;
  }
  return (x);
}

inline q31_t operator-(const q31_t & a, const q31_t & b) {
  q31_t x;
  int64_t r = (int64_t)(a.val) - (int64_t)(b.val);
  x.val = (int32_t)r;
  if ((int64_t)(x.val) != r) {
    x.val = (r < 0) ? 0x80000000 : 0x7FFFFFFF;
  }
  return (x);
}

inline q9_54_t operator+(const q9_54_t & a, const q9_54_t & b) {
  q9_54_t x;
  int64_t r = (int64_t)(a.val) + (int64_t)(b.val);
  x.val = r;
  return (x);
}


inline q9_54_t operator-(const q9_54_t & a, const q9_54_t & b) {
  q9_54_t x;
  int64_t r = (int64_t)(a.val) - (int64_t)(b.val);
  x.val = r;
  return (x);
}


inline q1_62_t operator+(const q1_62_t & a, const q1_62_t & b) {
  q1_62_t x;
  int64_t r = (int64_t)(a.val) + (int64_t)(b.val);
  x.val = r;
  return (x);
}


inline q1_62_t operator-(const q1_62_t & a, const q1_62_t & b) {
  q1_62_t x;
  int64_t r = (int64_t)(a.val) - (int64_t)(b.val);
  x.val = r;
  return (x);
}


inline q23_t operator*(const q23_t & a, const pow2_t & p) {
  q23_t x;
  if (p.val < 0) {
    x.val = (a.val >> (-p.val))&0xFFFFFF00;
  } else {
    x.val =  a.val <<   p.val;
    if ((x.val >> p.val) != a.val) {
      x.val = (a.val < 0) ? 0x80000000 : 0x7FFFFF00;
    }
  }
  return (x);
}

inline q8_23_t operator*(const q8_23_t & a, const pow2_t & p) {
  q8_23_t x;
  if (p.val < 0) {
    x.val = a.val >> (-p.val);
  } else {
    x.val = a.val <<   p.val;
    if ((x.val >> p.val) != a.val) {
      x.val = (a.val < 0) ? 0x80000000 : 0x7FFFFFFF;
    }
  }
  return (x);
}

inline q31_t operator*(const q31_t & a, const pow2_t & p) {
  q31_t x;
  if (p.val < 0) {
    x.val = a.val >> (-p.val);
  } else {
    x.val = a.val <<   p.val;
    if ((x.val >> p.val) != a.val) {
      x.val = (a.val < 0) ? 0x80000000 : 0x7FFFFFFF;
    }
  }
  return (x);
}

inline q9_54_t operator*(const q9_54_t & a, const pow2_t & p) {
  q9_54_t x;
  if (p.val < 0) {
    x.val = a.val >> (-p.val);
  } else {
    x.val = a.val <<   p.val;
    if ((x.val >> p.val) != a.val) {
      x.val = (a.val < 0) ? (-1LL<<63) : (((1LL<<62)-1)+(1LL<<62));
    }
  }
  return (x);
}

inline q1_62_t operator*(const q1_62_t & a, const pow2_t & p) {
  q1_62_t x;
  if (p.val < 0) {
    x.val = a.val >> (-p.val);
  } else {
    x.val = a.val <<   p.val;
    if ((x.val >> p.val) != a.val) {
      x.val = (a.val < 0) ? (-1LL<<63) : (((1LL<<62)-1)+(1LL<<62));
    }
  }
  return (x);
}

inline q23_t   operator*(const pow2_t & q, const q23_t   & b) {return (b*q);}
inline q8_23_t operator*(const pow2_t & q, const q8_23_t & b) {return (b*q);}
inline q31_t   operator*(const pow2_t & q, const q31_t   & b) {return (b*q);}
inline q9_54_t operator*(const pow2_t & q, const q9_54_t & b) {return (b*q);}
inline q1_62_t operator*(const pow2_t & q, const q1_62_t & b) {return (b*q);}

inline q23_t   operator/(const q23_t   & b, const pow2_t & q) {return (b*pow2(-q.val));}
inline q8_23_t operator/(const q8_23_t & b, const pow2_t & q) {return (b*pow2(-q.val));}
inline q31_t   operator/(const q31_t   & b, const pow2_t & q) {return (b*pow2(-q.val));}
inline q9_54_t operator/(const q9_54_t & b, const pow2_t & q) {return (b*pow2(-q.val));}
inline q1_62_t operator/(const q1_62_t & b, const pow2_t & q) {return (b*pow2(-q.val));}

#define SELF_OPERATOR(T)                                                       \
inline T & T::operator+=(const T     & b) {*this = *this + b; return (*this);} \
inline T & T::operator-=(const T     & b) {*this = *this - b; return (*this);} \
inline T & T::operator*=(const pow2_t& b) {*this = *this * b; return (*this);} \
inline T      operator- (const T     & b) {T z,m; z.val=0; m = z - b; return (m);}

SELF_OPERATOR(q23_t  )
SELF_OPERATOR(q8_23_t)
SELF_OPERATOR(q31_t  )

#define MIN_MAX_ABS(T)                                                                             \
inline T cx_min(const T & a, const T & b) {T x; x.val = (a.val < b.val) ? a.val : b.val; return (x);} \
inline T cx_max(const T & a, const T & b) {T x; x.val = (a.val > b.val) ? a.val : b.val; return (x);} \
inline T cx_abs(const T & a) {T x; x.val = (a.val < 0) ? -a.val : a.val; return (x);}

MIN_MAX_ABS(q23_t  )
MIN_MAX_ABS(q8_23_t)
MIN_MAX_ABS(q31_t  )

#define COMPARISON_OPERATIONS(T)                                           \
inline int operator==(const T & a, const T & b) {return (a.val == b.val);} \
inline int operator!=(const T & a, const T & b) {return (a.val != b.val);} \
inline int operator< (const T & a, const T & b) {return (a.val <  b.val);} \
inline int operator<=(const T & a, const T & b) {return (a.val <= b.val);} \
inline int operator> (const T & a, const T & b) {return (a.val >  b.val);} \
inline int operator>=(const T & a, const T & b) {return (a.val >= b.val);}

COMPARISON_OPERATIONS(q23_t  )
COMPARISON_OPERATIONS(q8_23_t)
COMPARISON_OPERATIONS(q31_t  )

inline q9_54_t operator*(const q31_t & a, const q23_t & b) {
  q9_54_t x = as_q9_54_t( ((int64_t)as_int(a) * (int64_t)as_int(b)) >> 8 );
  return (x);
}

inline q9_54_t operator*(const q31_t & a, const q8_23_t & b) {
  q9_54_t x = as_q9_54_t(  (int64_t)as_int(a) * (int64_t)as_int(b) );
  return (x);
}

inline q1_62_t operator*(const q31_t & a, const q31_t & b) {
  q1_62_t x = as_q1_62_t(  (int64_t)as_int(a) * (int64_t)as_int(b) );
  return (x);
}

inline q9_54_t operator*(const q23_t   & c, const q31_t & d) { return (d*c); }
inline q9_54_t operator*(const q8_23_t & c, const q31_t & d) { return (d*c); }

#endif  /* CONFIG_CAF_FLAVOR_CAPE2 */

#endif /* __cplusplus */

#endif  /* AUDIODSP_CAF_INCLUDE_CAPE_TYPES_H_ */
