# Open source Brother USB scanner driver and frontend

## Goals
* scan on x86, x86_64, arm and arm64
* fully open source

## Status
* `bropen-scan`: **fully open source** scan frontend
* `libsane-brother`: libsane driver, **partially open source**, still depends on *libbrcolm* and *libbrscandec* even if new hardware support don't use them

## Roadmap / TODOs
* Get rid of *libbrcolm* and *libbrscandec* to become fully open source, i.e. drop support of old scanners as not testable due to lack of hardware.
* Fix installation paths at least on 64-bit systems.

## Add/Test new hardware
* You can use the `bropen-scan` frontend to test your scanner.
* To add a new scanner to the list of supported devices by the driver you must add a line to `data/Brsane.ini` in the *[Support Model]* section.  
 The line must be of the form:  
 ```
product id,function type,model type,model name,read endpoint (dth),write endpoint (htd)
```
with:  
`product id`: the USB product id of your scanner, hexadecimal format  
`function type`: must be 214  
`model type`: must be 1  
`model name`: the name you want
`read endpoint (dth)`: USB device to host endpoint, decimal format  
`write endpoint (htd)`: USB host to device endpoint, decimal format  
If you don't know your `product id`, `read endpoint (dth)`, `write endpoint (htd)`, you can ask your system (`lsub -v` or other) or run `bropen-scan` and take note of its output.  
for example a new line should looks like that:  
`0x035b,214,1,"DCP-1610W",133,4`

## Limitations
* Only scan in 24bit color mode

## Caveats and Gotchas
* `make install` is outdated and may be broken on your system, see [Manual Installation](##Manual Installation) for the prefered way to install the driver for now.
* `bropen-scan` uses libusb-1 and `libsane-brother` uses libusb-0.1
* Scanning with `bropen-scan` you may notice a slight difference in color tints between the bmp and jpeg format, it's probably due to the yuv to rbg conversion, the jpeg format is more 'native' and should probably be the prefered format.

## Manual Installation
* Install the configuration file to `/usr/share/sane/brother/`.
```
mkdir -p /usr/share/sane/brother/
cp data/Brsane.ini /usr/share/sane/brother/
```
* Add the library to libsane, you must know the libsane library directory on your system
`e.g.` if it's `/usr/lib64/sane`:
```
 cp libsane-brother/.libs/libsane-brother.so* /usr/lib64/sane
```
* Make libsane load the library, you must know the sane's library configuration file location on your system
`e.g.` if it's `/etc/sane.d/dll.conf` :
```
echo brother >> /etc/sane.d/dll.conf
```
* ***Optional***, copy the library blobs, do it if you are on **x86_64** and want to support hardware that don't use the new part on the driver (i.e.: scanners that don't have their `function type` set to `214`)
```
cp libbrscandec/libbrscandec.so.1.0.0 /usr/local/lib64/
cp libbrcolm/libbrcolm.so.1.0.1 /usr/local/lib64/
```
