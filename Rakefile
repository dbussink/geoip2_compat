require "rake/extensiontask"
require 'rake/testtask'

Rake::ExtensionTask.new "geoip2_compat" do |ext|
  ext.lib_dir = "lib/geoip2_compat"
end

Rake::TestTask.new do |t|
  t.libs << "test"
  t.test_files = FileList['test/test*.rb']
  t.verbose = true
end

file "test/GeoIP2-City-Test.mmdb" do |t|
  # Use the fixture from the vendored libmaxminddb 1.0.3 test suite.
  url = "https://raw.githubusercontent.com/maxmind/MaxMind-DB/f887fec99eabf6d68746e257b894bae854fefc27/test-data/GeoIP2-City-Test.mmdb"
  sh "curl", "--fail", "--location", "--remove-on-error", "--output", t.name, url
end

task :vendor do
  version = "1.0.3"
  mkdir_p "tmp/"
  dir = "tmp/libmaxminddb-#{version}"
  cd "tmp/" do
    sh "curl -L https://github.com/maxmind/libmaxminddb/releases/download/#{version}/libmaxminddb-#{version}.tar.gz | tar xz"
  end
  cp "#{dir}/src/maxminddb-compat-util.h", "ext/geoip2_compat/maxminddb-compat-util.h"
  cp "#{dir}/src/maxminddb.c", "ext/geoip2_compat/maxminddb.c"
  cp "#{dir}/include/maxminddb.h", "ext/geoip2_compat/maxminddb.h"
  cp "#{dir}/include/maxminddb_config.h.in", "ext/geoip2_compat/maxminddb_config.h"
end

task download: "test/GeoIP2-City-Test.mmdb"
task test: [:compile, :download]
task default: :test
