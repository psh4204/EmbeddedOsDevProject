#ifndef INCLUDE_STDARG_H_
#define INCLUDE_STDARG_H_

// 가변인자를 위한 va_ 시리즈는 컴파일 별 빌트인 인자를 사용함.
typedef __builtin_va_list va_list;

#define va_start(v,l)   __builtin_va_start(v,l);
#define va_end(v)   __builtin_va_end(v);
#define va_arg(v,l)   __builtin_va_arg(v, l);

#endif /* INCLUDE_STDARG_H_ */