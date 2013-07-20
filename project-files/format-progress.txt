/* TODO: Josh, Review this file and move any info still valid or needing work into TODO comments in the actual
 *       libembroidery code. Many items in this list are out of date and do not reflect the current status of
 *       libembroidery. When finished, delete this file.
 */

- Test that all formats read data in correct scale (format details should match other programs)
- Add which formats to work with to preferences.
- Check for memory leaks
- Update all formats without color to check for edr or rgb files
- Fix issues with DST (VERY important that DST work well)

--------------------------------
libembroidery C formats
--------------------------------
FORMAT | READ  | WRITE | NOTES
--------------------------------
10o    | YES   |       | read (need to fix external color loading) (maybe find out what ctrl code flags of 0x10, 0x08, 0x04, and 0x02 mean)
100    |       |       | none (4 byte codes) 61 00 10 09 (type, type2, x, y ?) x & y (signed char)
art    |       |       | none
bro    | YES   |       | read (complete)(maybe figure out detail of header)
cnd    |       |       | none
col    |       |       | (color file no design) read(final) write(final)
csd    | YES   |       | read (complete)
dat    |       |       | read ()
dem    |       |       | none (looks like just encrypted cnd)
dsb    | YES   |       | read (unknown how well) (stitch data looks same as 10o)
dst    | YES   |       | read (complete) / write(unknown)
dsz    | YES   |       | read (unknown)
dxf    |       |       | read (Port to C. needs refactored)
edr    |       |       | read (C version is broken) / write (complete)
emd    |       |       | read (unknown)
exp    | YES   |       | read (unknown) / write(unknown)
exy    | YES   |       | read (need to fix external color loading)
fxy    | YES   |       | read (need to fix external color loading)
gnc    |       |       | none
gt     |       |       | read (need to fix external color loading)
hus    | YES   |       | read (unknown) / write (C version is broken)
inb    | YES   |       | read (buggy?)
jef    | YES   |       | write (need to fix the offsets when it is moving to another spot)
ksm    | YES   |       | read (unknown) / write (unknown)
pcd    |       |       | 
pcm    |       |       | 
pcq    |       |       | read (Port to C)
pcs    | BUGGY |       | read (buggy / colors are not correct / after reading, writing any other format is messed up)
pec    |       |       | read / write (without embedded images, sometimes overlooks some stitches leaving a gap)
pel    |       |       | none
pem    |       |       | none
pes    | YES   |       | 
phb    |       |       | 
phc    |       |       | 
rgb    |       |       | 
sew    | YES   |       | 
shv    |       |       | read (C version is broken)
sst    |       |       | none
svg    |       | YES   | 
tap    | YES   |       | read (unknown)
u01    |       |       | 
vip    | YES   |       | 
vp3    | YES   |       | 
xxx    | YES   |       | 
zsk    |       |       | read (complete)


Support for Singer FHE, CHE (Compucon) formats?



