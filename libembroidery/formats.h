#ifndef FORMATS_H
#define FORMATS_H

#ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */

#include "format-exp.h"

#else /* ARDUINO TODO: This is temporary. Remove when complete. */

#include "format-10o.h"
#include "format-100.h"
#include "format-art.h"
#include "format-bmc.h"
#include "format-bro.h"
#include "format-cnd.h"
#include "format-col.h"
#include "format-csd.h"
#include "format-csv.h"
#include "format-dat.h"
#include "format-dem.h"
#include "format-dsb.h"
#include "format-dst.h"
#include "format-dsz.h"
#include "format-dxf.h"
#include "format-edr.h"
#include "format-emd.h"
#include "format-exp.h"
#include "format-exy.h"
#include "format-eys.h"
#include "format-fxy.h"
#include "format-gnc.h"
#include "format-gt.h"
#include "format-hus.h"
#include "format-inb.h"
#include "format-inf.h"
#include "format-jef.h"
#include "format-ksm.h"
#include "format-max.h"
#include "format-mit.h"
#include "format-new.h"
#include "format-ofm.h"
#include "format-pcd.h"
#include "format-pcm.h"
#include "format-pcq.h"
#include "format-pcs.h"
#include "format-pec.h"
#include "format-pel.h"
#include "format-pem.h"
#include "format-pes.h"
#include "format-phb.h"
#include "format-phc.h"
#include "format-plt.h"
#include "format-rgb.h"
#include "format-sew.h"
#include "format-shv.h"
#include "format-sst.h"
#include "format-stx.h"
#include "format-svg.h"
#include "format-t09.h"
#include "format-tap.h"
#include "format-thr.h"
#include "format-txt.h"
#include "format-u00.h"
#include "format-u01.h"
#include "format-vip.h"
#include "format-vp3.h"
#include "format-xxx.h"
#include "format-zsk.h"

#endif /* ARDUINO TODO: This is temporary. Remove when complete. */

/*#include "api-start.h"*/
#include "emb-hash.h"
#ifdef __cplusplus
extern "C" {
#endif

#define EMBFORMAT_UNSUPPORTED 0
#define EMBFORMAT_STITCHONLY 1
#define EMBFORMAT_STCHANDOBJ 3 /* binary operation: 1+2=3 */
#define EMBFORMAT_OBJECTONLY 2
/* unassigned slot feature BIT 4,8,16,32,64,128,256,512  */
#define EMBFORMAT_HASREADER 1024
#define EMBFORMAT_HASWRITER 2048
#define EMBFORMAT_HASSUBWRITER 4096
/* unassigned slot feature  BIT  8192  */
#define EMBFORMAT_UNSTABLEREADER 16384
#define EMBFORMAT_UNSTABLEWRITER 32768

typedef struct EmbFormat_
{
    char* ext;
    unsigned long features;      /* combinations of EMBFORMAT_XXXX above. */
    char* smallInfo;    /* default decription such "Machine X Embroidery". */
    char* nextExt;      /* next format's key */
    struct EmbFormat_* next;
} EmbFormat;

extern EMB_PUBLIC EmbHash* EMB_CALL embFormatList_create();
extern EMB_PUBLIC void EMB_CALL embFormatList_free(EmbHash* hash);
extern EMB_PUBLIC EmbFormat* EMB_CALL embFormatList_first(EmbHash* hash);
extern EMB_PUBLIC int EMB_CALL embFormat_type(const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMATS_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
