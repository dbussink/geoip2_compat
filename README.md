# Ruby GeoIP2 compatibility bindings

Implements the new Maxmind database format while keeping the output
complatible with https://github.com/mtodd/geoip

## Dependencies

[libmaxminddb](https://github.com/maxmind/libmaxminddb) is bundled with the gem.
This gem doesn't depend on the system library.

## Installation

`gem install geoip2_compat`

## Usage

```
geoip = GeoIP2Compat.new(dbfile)
geoip.lookup("24.24.24.24")
```
