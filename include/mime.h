#ifndef _MIME_H
#define _MIME_H

#define  MIME_TYPE_INVALID      0
#define  MIME_TYPE_HTMLMSG      1
#define  MIME_TYPE_FILE         2
#define  MIME_TYPE_OCTET        3
#define  MIME_TYPE_XML          4
#define  MIME_TYPE_SOAP         5
#define  MIME_TYPE_DRM_CONTENT  6
#define  MIME_TYPE_MULTIPART    7

#define  MIME_MULTIPART_MIME    0
#define  MIME_MULTIPART_DIME    1

#define  MIME_SAVE_FILE_BIN     0
#define  MIME_SAVE_DIR          1
#define  MIME_SAVE_SINGLE_FILE  2

#define MAX_MIME_BUFFER_SIZE       4096000
#define MAX_MIME_HEADER_VALUE_SIZE   256

/**
 * >0: related method
 * 0: ok
 * <0: failure
 */
#define MIME_RETURN_CODE_OK               0
#define MIME_RETURN_CODE_HANDLE_NULL     -1
#define MIME_RETURN_CODE_DECODE_FAILURE  -2
#define MIME_RETURN_CODE_ENCODE_FAILURE  -3
#define MIME_RETURN_CODE_BUFFER_SHORT    -4
#define MIME_RETURN_CODE_INSERT          -5
#define MIME_RETURN_CODE_DELETE          -6
#define MIME_RETURN_CODE_READ_FILE       -7
#define MIME_RETURN_CODE_WRITE_FILE      -8

const char* matchExtToMimeType(const char* filename);
const char* matchMimeTypeToExt(const char* mimeType);


typedef void* TMimeHandle;

TMimeHandle newMime();
void deleteMime(TMimeHandle handle);

int getMimeType(TMimeHandle handle);
int setMimeMultiPartType(TMimeHandle handle, int type);

int mimeDecode(TMimeHandle handle, char* buffer, int len);
int mimeEncode(TMimeHandle handle, char* buffer, int maxlen=MAX_MIME_BUFFER_SIZE);

int mimeGetContentID(TMimeHandle handle, char* value, int maxlen=MAX_MIME_HEADER_VALUE_SIZE);

int mimeGetContentType(TMimeHandle handle, char* value, int maxlen=MAX_MIME_HEADER_VALUE_SIZE, int base=0);
int mimeSetContentType(TMimeHandle handle, char* value);

int mimeGetContentDisposition(TMimeHandle handle, char* value, int maxlen=MAX_MIME_HEADER_VALUE_SIZE);
int mimeSetContentDisposition(TMimeHandle handle, char* value);

int mimeGetEncodeStyle(TMimeHandle handle, char* value, int maxlen=MAX_MIME_HEADER_VALUE_SIZE);
int mimeSetEncodeStyle(TMimeHandle handle, char* value);

int mimeGetHeader(TMimeHandle handle, char* key, char* value, int maxlen=MAX_MIME_HEADER_VALUE_SIZE);
int mimeSetHeader(TMimeHandle handle, char* key, char* value);

int mimeInsertSubMime(TMimeHandle handle, TMimeHandle subHandle, int append = 1);
TMimeHandle mimeRemoveSubMime(TMimeHandle handle, int index);
int mimeGetSubNum(TMimeHandle handle);
TMimeHandle mimeGetSubMime(TMimeHandle handle, int index);

int mimeSetBody(TMimeHandle handle, char* buffer, int len);

int mimeWriteFile(TMimeHandle handle, char* fileName, int saveType);
int mimeReadFile(TMimeHandle handle, char* fileName, int saveType);

#endif
