# Ruby GeoIP2 compatibility bindings

Implements the new Maxmind database format while keeping the output
complatible with https://github.com/mtodd/geoip

## Installation

`gem install geoip2_compat`

## Usage

```
geoip = GeoIP2Compat.new(dbfile)
geoip.lookup("24.24.24.24")
```
