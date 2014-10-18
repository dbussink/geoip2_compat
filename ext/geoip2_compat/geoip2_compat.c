#include <ruby.h>
#include <ruby/encoding.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#include <maxminddb.h>

VALUE cgeoip2_compat;
VALUE egeoip2_compat_Exception;

ID geoip2_compat_COUNTRY_CODE;
ID geoip2_compat_COUNTRY_NAME;
ID geoip2_compat_REGION;
ID geoip2_compat_REGION_NAME;
ID geoip2_compat_CITY;
ID geoip2_compat_POSTAL_CODE;
ID geoip2_compat_LATITUDE;
ID geoip2_compat_LONGITUDE;

static void geoip2_compat_internal_close(MMDB_s* mmdb)
{
  if(mmdb->file_content) {
    MMDB_close(mmdb);
  }
  mmdb->file_content = NULL;
}

static VALUE geoip2_compat_close(VALUE self)
{
  MMDB_s *mmdb;
  Data_Get_Struct(self, MMDB_s, mmdb);
  geoip2_compat_internal_close(mmdb);
  return self;
}

static VALUE geoip2_compat_path(VALUE self)
{
  MMDB_s *mmdb;
  Data_Get_Struct(self, MMDB_s, mmdb);

  return rb_str_new2(mmdb->filename);
}

static void geoip2_compat_lookup_internal(MMDB_lookup_result_s* result, VALUE hash, ID name, ...)
{
  MMDB_entry_data_s entry_data;

  va_list path;
  va_start(path, name);
  int status = MMDB_vget_value(&result->entry, &entry_data, path);
  va_end(path);

  if (MMDB_SUCCESS != status) return;

  if (entry_data.has_data) {
    VALUE val = Qnil;
    switch (entry_data.type) {
      case MMDB_DATA_TYPE_UTF8_STRING:
        val = rb_enc_str_new(entry_data.utf8_string, entry_data.data_size, rb_utf8_encoding());
        break;
      case MMDB_DATA_TYPE_BYTES:
        val = rb_str_new((const char*)entry_data.bytes, entry_data.data_size);
        break;
      case MMDB_DATA_TYPE_DOUBLE:
        val = rb_float_new(entry_data.double_value);
        break;
      case MMDB_DATA_TYPE_UINT16:
        val = UINT2NUM(entry_data.uint16);
        break;
      case MMDB_DATA_TYPE_UINT32:
        val = UINT2NUM(entry_data.uint32);
        break;
      case MMDB_DATA_TYPE_INT32:
        val = INT2NUM(entry_data.int32);
        break;
      case MMDB_DATA_TYPE_UINT64:
        val = UINT2NUM(entry_data.uint64);
        break;
      case MMDB_DATA_TYPE_BOOLEAN:
        val = entry_data.boolean ? Qtrue : Qfalse;
        break;
      case MMDB_DATA_TYPE_FLOAT:
        val = rb_float_new(entry_data.float_value);
        break;
    }
    rb_hash_aset(hash, ID2SYM(name), val);
  }
}

static VALUE geoip2_compat_lookup(VALUE self, VALUE ip)
{
  int status, gai_error, mmdb_error;
  Check_Type(ip, T_STRING);

  char* ip_addr = StringValuePtr(ip);
  MMDB_s *mmdb;
  Data_Get_Struct(self, MMDB_s, mmdb);

  MMDB_lookup_result_s result =
    MMDB_lookup_string(mmdb, ip_addr, &gai_error, &mmdb_error);

  if (mmdb_error != MMDB_SUCCESS) {
    rb_raise(egeoip2_compat_Exception,
      "geoip2_compat - lookup failed: %s", MMDB_strerror(mmdb_error)
    );
  }

  if (gai_error != 0) {
    rb_raise(egeoip2_compat_Exception,
      "geoip2_compat - getaddrinfo failed: %s", gai_strerror(gai_error)
    );
  }

  if (!result.found_entry) return Qnil;

  VALUE hash = rb_hash_new();

  geoip2_compat_lookup_internal(&result, hash, geoip2_compat_COUNTRY_CODE, "country", "iso_code", NULL);
  geoip2_compat_lookup_internal(&result, hash, geoip2_compat_COUNTRY_NAME, "country", "names", "en", NULL);
  geoip2_compat_lookup_internal(&result, hash, geoip2_compat_REGION, "subdivisions", "0", "iso_code", NULL);
  geoip2_compat_lookup_internal(&result, hash, geoip2_compat_REGION_NAME, "subdivisions", "0", "names", "en", NULL);
  geoip2_compat_lookup_internal(&result, hash, geoip2_compat_CITY, "city", "names", "en", NULL);
  geoip2_compat_lookup_internal(&result, hash, geoip2_compat_POSTAL_CODE, "postal", "code", NULL);
  geoip2_compat_lookup_internal(&result, hash, geoip2_compat_LATITUDE, "location", "latitude", NULL);
  geoip2_compat_lookup_internal(&result, hash, geoip2_compat_LONGITUDE, "location", "longitude", NULL);

  return hash;
}

static void geoip2_compat_free(MMDB_s *mmdb)
{
  geoip2_compat_internal_close(mmdb);
  ruby_xfree(mmdb);
}

static VALUE geoip2_compat_allocate(VALUE klass)
{
  MMDB_s* mmdb = ruby_xmalloc(sizeof(MMDB_s));
  mmdb->file_content = NULL;
  return Data_Wrap_Struct(klass, NULL, geoip2_compat_free, mmdb);
}

static VALUE geoip2_compat_initialize(VALUE self, VALUE path)
{
  Check_Type(path, T_STRING);
  char* db = StringValuePtr(path);
  MMDB_s *mmdb;
  Data_Get_Struct(self, MMDB_s, mmdb);

  int status = MMDB_open(db, MMDB_MODE_MMAP, mmdb);

  if (status != MMDB_SUCCESS) {
    rb_raise(egeoip2_compat_Exception, "GeoIP2Compat - %s: %s",
      MMDB_strerror(status), db
    );
  }
  return self;
}

void Init_geoip2_compat()
{
  cgeoip2_compat = rb_define_class("GeoIP2Compat", rb_cObject);
  egeoip2_compat_Exception = rb_define_class_under(cgeoip2_compat, "Error", rb_eStandardError);

  rb_global_variable(&cgeoip2_compat);
  rb_global_variable(&egeoip2_compat_Exception);

  rb_define_alloc_func(cgeoip2_compat, geoip2_compat_allocate);
  rb_define_method(cgeoip2_compat, "initialize", geoip2_compat_initialize, 1);
  rb_define_method(cgeoip2_compat, "path", geoip2_compat_path, 0);
  rb_define_method(cgeoip2_compat, "close", geoip2_compat_close, 0);
  rb_define_method(cgeoip2_compat, "lookup", geoip2_compat_lookup, 1);

  geoip2_compat_COUNTRY_CODE  = rb_intern("country_code");
  geoip2_compat_COUNTRY_NAME  = rb_intern("country_name");
  geoip2_compat_REGION        = rb_intern("region");
  geoip2_compat_REGION_NAME   = rb_intern("region_name");
  geoip2_compat_CITY          = rb_intern("city");
  geoip2_compat_POSTAL_CODE   = rb_intern("postal_code");
  geoip2_compat_LATITUDE      = rb_intern("latitude");
  geoip2_compat_LONGITUDE     = rb_intern("longitude");
}
