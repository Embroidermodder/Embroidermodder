/* Tests for accuracy of the format table lookup and identification */

#include <string.h>

#include "../src/embroidery.h"

int
main(void)
{
    const char*  tName = "example.zsk";
    int format = emb_identify_format(tName);

    printf("Filename   : %s\n"
       "Extension  : %s\n"
       "Description: %s\n"
       "Reader     : %c\n"
       "Writer     : %c\n"
       "Type       : %d\n\n",
        tName,
        formatTable[format].extension,
        formatTable[format].description,
        formatTable[format].reader_state,
        formatTable[format].writer_state,
        formatTable[format].type);

    if (strncmp(formatTable[format].extension, ".zsk", 200)) {
        puts("In format table test the extension lookup failed.");
		return 20;
	}
    if (strncmp(formatTable[format].description, "ZSK USA Embroidery Format", 200)) {
        puts("In format table test the description lookup failed.");
        return 21;
    }
    if (formatTable[format].reader_state != 'U') {
        puts("In format table test the reader_state lookup failed.");
		return 22;
	}
    if (formatTable[format].writer_state != ' ') {
        puts("In format table test the write_state lookup failed.");
		return 23;
	}
    if (formatTable[format].type != 1) {
        puts("In format table test the type lookup failed.");
		return 24;
	}
    return 0;
}

