/* 
 Ruby bindings for LZO.
 LZO is a portable lossless data compression library written in ANSI C.

 Copyright (C) 2008 SUGAWARA Genki <sgwr_dts@yahoo.co.jp>
*/

#ifdef _WIN32
__declspec(dllexport) void Init_lzoruby(void);
#endif

#include "lzo/lzoconf.h"
#include "lzo/lzo1x.h"
#include "ruby.h"

#ifndef RSTRING_PTR
#define RSTRING_PTR(s) (RSTRING(s)->ptr)
#endif

#ifndef RSTRING_LEN
#define RSTRING_LEN(s) (RSTRING(s)->len)
#endif

#define VERSION "0.1.3"

static VALUE LZO;
static VALUE LZO_Error;

/* */
static VALUE lzoruby_compress(int argc, const VALUE *argv, VALUE self) {
  const lzo_bytep in;
  lzo_bytep out;
  lzo_voidp wrkmem = NULL;
  lzo_uint in_len;
  lzo_uint out_len;
  lzo_uint new_len;
  VALUE v_in, v_out, v_level;
  int level, err;

  rb_scan_args(argc, argv, "11", &v_in, &v_level);
  Check_Type(v_in, T_STRING);
  level = NIL_P(v_level) ? 1 : NUM2INT(v_level);

  in = RSTRING_PTR(v_in);
  in_len = RSTRING_LEN(v_in);
  out_len = in_len + in_len / 64 + 16 + 3;
  out = xmalloc(5 + out_len);
  new_len = out_len;
  wrkmem = xmalloc((level == 1) ? LZO1X_1_MEM_COMPRESS : LZO1X_999_MEM_COMPRESS);

  if (level == 1) {
    out[0] = 0xf0;
    err = lzo1x_1_compress(in, in_len, out + 5, &new_len, wrkmem);
  } else {
    out[0] = 0xf1;
    err = lzo1x_999_compress(in, in_len, out + 5, &new_len, wrkmem);
  }

  xfree(wrkmem);

  if (err != LZO_E_OK || new_len > out_len) {
    xfree(out);
    rb_raise(LZO_Error, "Error %d while compressing data", err);
  }

  out[1] = (unsigned char) ((in_len >> 24) & 0xff);
  out[2] = (unsigned char) ((in_len >> 16) & 0xff);
  out[3] = (unsigned char) ((in_len >>  8) & 0xff);
  out[4] = (unsigned char) ((in_len >>  0) & 0xff);
  v_out = rb_str_new(out, 5 + new_len);
  xfree(out);

  return v_out;
}

/* */
static VALUE lzoruby_decompress(VALUE self, VALUE v_in) {
  lzo_bytep in;
  lzo_bytep out;
  lzo_uint in_len;
  lzo_uint out_len;
  lzo_uint new_len;
  VALUE v_out;
  int err;

  Check_Type(v_in, T_STRING);

  in = RSTRING_PTR(v_in);
  in_len = RSTRING_LEN(v_in);

  if (((int) in_len) < 5 + 3 || in[0] < 0xf0 || in[0] > 0xf1) {
    rb_raise(LZO_Error, "Header error - invalid compressed data");
  }

  in_len -= 5;
  out_len = (in[1] << 24) | (in[2] << 16) | (in[3] << 8) | in[4];

  if (((int) out_len) < 0 || in_len > out_len + out_len / 64 + 16 + 3) {
    rb_raise(LZO_Error, "Header error - invalid compressed data");
  }

  out = xmalloc(out_len);
  new_len = out_len;

  err = lzo1x_decompress_safe(in + 5, in_len, out, &new_len, NULL);

  if (err != LZO_E_OK || new_len != out_len) {
    xfree(out);
    rb_raise(LZO_Error, "Compressed data violation %d", err);
  }

  v_out = rb_str_new(out, new_len);
  xfree(out);

  return v_out;
}

/* */
static VALUE lzoruby_adler32(VALUE self, VALUE v_adler, VALUE v_buf) {
  lzo_uint32 adler;
  lzo_bytep buf;
  lzo_uint len;

  Check_Type(v_adler, T_FIXNUM);
  Check_Type(v_buf, T_STRING);
  adler = FIX2LONG(v_adler);
  buf = RSTRING_PTR(v_buf);
  len = RSTRING_LEN(v_buf);

  adler = lzo_adler32(adler, buf, len);

  return LONG2FIX(adler);
}

void Init_lzoruby() {
  if (lzo_init() != LZO_E_OK) {
    rb_warn("internal error - lzo_init() failed !!!");
    return;
  }

  LZO = rb_define_module("LZO");
  LZO_Error = rb_define_class_under(LZO, "Error", rb_eStandardError);

  rb_define_const(LZO, "VERSION", rb_str_new2(VERSION));
  rb_define_const(LZO, "LZO_VERSION", rb_str_new2(lzo_version_string()));
  rb_define_module_function(LZO, "compress", lzoruby_compress, -1);
  rb_define_module_function(LZO, "decompress", lzoruby_decompress, 1);
  rb_define_module_function(LZO, "adler32", lzoruby_adler32, 2);
}
