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

task default: [:compile, :download, :test]
