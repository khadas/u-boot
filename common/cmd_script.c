
// ## hyphop ## for khadas

/*

## script

simple plain script run from mem without mkimage wrappers

`script` is cool alternative for `autoscript` and `source`
simle usage without mkimage

+ https://github.com/hyphop/uboot-extra
+ wget https://raw.githubusercontent.com/hyphop/uboot-extra/master/cmd_script.c

# sintax and parsing

```
#!script - script must begin from this marker
##END##  - its end marker - after this lines all strings ignored
'\0'     - its same end marker
```

script parsed by run_command_list

# features

we can use same `script` cmd for wrapped scripts with 72 bytes mkimage header
which parsed by `source` or `autoscript`

# how to install it

just add next line to Makefile

    obj-y += cmd_script.o

# uboot usage

    script [addr] [bytes] [nochk] [silent] - run script starting at addr
        bytes - read bytes (hex) limit
        nochk - no check #!script header
        silent - be silent

# uboot usage  examples

    script 0x1000000				- simple run from addr 0x1000000
    script 0x1000000 32 			- same but only fist 32 bytes
    script 0x1000000 $filesize 			- same but limited by file size value
    script 0x1000000 $filesize nochk 		- same but without header check
    script 0x1000000 $filesize nochk silent	- same but silent

    # tftp script usage
    ADDR=0x1000000; tftp $ADDR test.script && script $ADDR $filesize

    # usage as files
    ADDR=100000; ext4load mmc 1:5 $ADDR dhcp.cmd_test.script; script $ADDR $filesize

    # spi flash usage
    ADDR=100000; sf read $ADDR $SCRIPT_OFFSET $SCRIPT_BYTES; script $ADDR

*/


#include <common.h>
#include <command.h>
#include <image.h>
#include <malloc.h>
#include <asm/byteorder.h>

int
script (ulong addr , unsigned leng,  unsigned hdr_chk , unsigned silent)
{
	unsigned  len = leng;
	void *buf;

	buf = map_sysmem(addr, 0);

	char *data;
	char *n;

	data = (char *)buf;
	n = data;

// simple mkimage header parser
	if ( *(n+0) == 0x27 &&
	     *(n+1) == 0x05 &&
	     *(n+2) == 0x19 &&
	     *(n+3) == 0x56
	     ) {

//	size calculate
	    unsigned int l = (*(n+14))*256 + *(n+15);

	    if ( l > 8 ) l-=8;
//	fix offest
	    data+=72;
	    hdr_chk=0;

	    if (!silent)
		printf ("[w] mkimage Script a:%08lx l:%u - s:%u\n", addr, len, l) ;

//	fix len
	    len=l;

	}

// simple script header parser
	if ( hdr_chk ) {
	if ( *n++ == '#' &&
	     *n++ == '!' &&
	     *n++ == 's' &&
	     *n++ == 'c' &&
	     *n++ == 'r' &&
	     *n++ == 'i' &&
	     *n++ == 'p' &&
	     *n++ == 't' ) {
	} else {
		if (!silent)
		    printf ("[w] Script a:%08lx l:%u c:%u - wrong header!\n", addr, len, hdr_chk ) ;

		return -1;

	}
	}

	if (len < 1) {
	    len = 1024*32;
	}

	char a = *(data + len);

	*(data + len) = '\0';

	char *p = strstr( data , "\n##END##" );

	if (p != NULL) {
	    *(data + len) = a;
	    len = p - data;
	} else {
	    int i = 0;
	    while ( *n++ != '\0' ) {
		if ( i++ >= len ) break;
	    }
	    *(data + len) = a;
	    len = n - data; //len = i;
	}

	len--;

	if (!silent)
	    printf ("[#] Script a:%08lx l:%u c:%u s:%u - run\n", addr, len, hdr_chk, silent ) ;

	return run_command_list(data, len, 0);

//	unmap_sysmem(buf);

	return 0;


}

/**************************************************/

int do_script (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	ulong addr;
	int rcode;

	if ( argc < 2 )  {
	    printf ("[e] not defined addr\n");
	    return -1;
	}

	unsigned leng;

	addr = simple_strtoul(argv[1], NULL, 16);
	if ( argc < 3 )  {
	    leng = 65536/2;
	} else {
	    leng = simple_strtoul(argv[2], NULL, 16);
	}

	rcode = script (addr , leng ,
		argc > 3 ? 0 : 1,
		argc > 4 ? 1 : 0
	);


	return rcode;
}

U_BOOT_CMD(
	script, 5, 0,	do_script,
	"run plain script from memory\n",
	"[addr] [bytes] [nochk] [silent] - run script starting at addr\n"
	"	bytes - read bytes (hex) limit\n"
	"	nochk - no check #!script header\n"
	"	silent - be silent\n"
);
