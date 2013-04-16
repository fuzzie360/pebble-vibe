# Vibe Pebble App
_by Fazli Sapuan_

Vibrator app with pulse controls.

[![ScreenShot](http://i.imgur.com/ttPB7VY.png)](http://i.imgur.com/ttPB7VY.png)

###Change log:

1.0

* Initial release

###[Download v1.0](https://www.dropbox.com/s/c4u7kx8b6zuorqk/pebble-vibe-1.0.pbw)

## Disclaimer

This is a joke. Please don't actually use this. You may damage your vibrating motor in your pebble.

## Build Instructions

Clone this repository in an appropriate directory:

	git clone https://github.com/Fuzzie360/pebble-vibe.git

Set up waf:

	python ~/pebble-dev/pebble-sdk-release-001/tools/create_pebble_project.py --symlink-only ~/pebble-dev/pebble-sdk-release-001/sdk/ pebble-noms
	cd pebble-vibe

Configure and build:

	./waf configure
	./waf build

Install pebble-noms.pbw in build directory

## License

Copyright (C) 2013  Fazli Sapuan

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

You may contact the author at fazli@sapuan.org