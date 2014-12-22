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

task :download do
  file = "test/GeoLite2-City.mmdb"
  unless File.exist?(file)
    `curl http://geolite.maxmind.com/download/geoip/database/GeoLite2-City.mmdb.gz | gzip -d > #{file}`
  end
end

task :vendor do
  sh "cd vendor && curl -L https://github.com/maxmind/libmaxminddb/releases/download/0.5.3/libmaxminddb-0.5.3.tar.gz | tar xz"
end

task default: [:compile, :download, :test]
