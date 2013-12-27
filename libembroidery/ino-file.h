#ifndef INO_FILE_H
#define INO_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct InoFile_ InoFile;
typedef InoFile EmbFile;

InoFile* inoFile_open(const char* fileName, const char* mode);
int inoFile_close(InoFile* stream);
int inoFile_eof(InoFile* stream);
int inoFile_getc(InoFile* stream);
int inoFile_seek(InoFile* stream, long offset, int origin);
long inoFile_tell(InoFile* stream);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INO_FILE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
