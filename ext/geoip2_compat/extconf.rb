require "mkmf"
extension_name = "geoip2_compat"

$LDFLAGS << " #{ENV['LDFLAGS']}"
$CFLAGS << " -std=c99 #{ENV['CFLAGS']}"

RbConfig::MAKEFILE_CONFIG['CC'] = ENV['CC'] if ENV['CC']

$defs << ' -DPACKAGE_VERSION=\"1.0.3\"'

create_makefile("#{extension_name}/#{extension_name}")
