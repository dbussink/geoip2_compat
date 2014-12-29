Gem::Specification.new do |s|
  s.name         = "geoip2_compat"
  s.version      = "0.0.3"

  s.licenses     = ["MIT"]

  s.authors      = ["Dirkjan Bussink"]
  s.email        = ["d.bussink@gmail.com"]

  s.summary      = "Bindings for libmaxminddb to access GeoIP2 database files"
  s.description  = "Lookup IPv4 and IPv6 addresses in Maxmind GeoIP2 database files"
  s.homepage     = "http://github.com/dbussink/geoip2_compat"

  s.files        = `git ls-files`.split("\n")
  s.extensions   = ['ext/geoip2_compat/extconf.rb']
  s.require_paths = ["lib"]

  s.add_development_dependency 'rake', '~>10.0'
  s.add_development_dependency 'rake-compiler', '~> 0'
  s.add_development_dependency 'test-unit', '~> 0'
end
