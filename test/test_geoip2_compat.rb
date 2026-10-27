require 'geoip2_compat'
require 'test/unit'

class GeoIP2Compat
  class TestGeoIP2Compat < Test::Unit::TestCase

    def setup
      @db = GeoIP2Compat.new("test/GeoIP2-City-Test.mmdb")
    end

    def teardown
      @db.close if @db
    end

    def test_initialize_success
      assert_equal "test/GeoIP2-City-Test.mmdb", @db.path
    end

    def test_initialize_failure
      assert_raise ArgumentError do
        GeoIP2Compat.new {}
      end
    end

    def test_lookup_ipv4
      res = {
        :city=>"Milton",
        :country_code=>"US",
        :country_name=>"United States",
        :latitude=>47.2513,
        :longitude=>-122.3149,
        :postal_code=>"98354",
        :region=>"WA",
        :region_name=>"Washington",
      }
      assert_equal res, @db.lookup("216.160.83.56")
    end

    def test_lookup_ipv6
      res = {
        :country_code=>"JP",
        :country_name=>"Japan",
        :latitude=>35.68536,
        :longitude=>139.75309,
      }
      assert_equal res, @db.lookup("2001:218::")
    end
  end
end
