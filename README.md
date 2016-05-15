# pluto-lzo
C implementation to inflate lzo files found on the vera home controller.

A C# managed version is available on the `manged-pluto-lzo` branch.

##Usage 
```
Usage: pluto-lzo {c|d} source_file dest_file
       pluto-lzo compressed_file
```

##Build and install
```
make gcc
chmod a+rx pluto-lzo
mv pluto-lzo /usr/bin/pluto-lzo
```

### Attribution
 * Markus Franz Xaver Johannes Oberhumer
 * <markus@oberhumer.com>
 * http://www.oberhumer.com/opensource/lzo/
 * Version : 2.09


Note: this project is in no way associated with any commercial product 