require "mkmf"
extension_name = "geoip2_compat"

$LDFLAGS << " #{ENV['LDFLAGS']}"
$CFLAGS << " #{ENV['CFLAGS']}"

RbConfig::MAKEFILE_CONFIG['CC'] = ENV['CC'] if ENV['CC']

ROOT = File.expand_path("../../..", __FILE__)

Dir.chdir("#{ROOT}/vendor/libmaxminddb-0.5.3") do
  system "./configure", "--prefix", "#{ROOT}/ext/libmaxminddb"
  system "make", "install"
end

$INCFLAGS << " -I#{ROOT}/ext/libmaxminddb/include "
$LDFLAGS  << " -L#{ROOT}/ext/libmaxminddb/lib "

if have_header('maxminddb.h') && have_library('maxminddb')
  create_makefile("#{extension_name}/#{extension_name}")
else
  puts "Couldn't find libmaxminddb"
  exit(1)
end
