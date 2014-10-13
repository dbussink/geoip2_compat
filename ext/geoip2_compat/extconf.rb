require "mkmf"
extension_name = "geoip2_compat"

$LDFLAGS << " #{ENV['LDFLAGS']}"
$CFLAGS << " #{ENV['CFLAGS']}"

RbConfig::MAKEFILE_CONFIG['CC'] = ENV['CC'] if ENV['CC']

if have_header('maxminddb.h') && have_library('maxminddb')
  create_makefile("#{extension_name}/#{extension_name}")
else
  puts "Couldn't find libmaxminddb"
  exit(1)
end
