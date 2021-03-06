
/*
 * Copyright 2006 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


// TODO: add unittests for all these operations

#ifndef SkOSFile_DEFINED
#define SkOSFile_DEFINED

#include "SkString.h"

#if defined(SK_BUILD_FOR_MAC) || defined(SK_BUILD_FOR_UNIX) || defined(SK_BUILD_FOR_ANDROID) || defined(SK_BUILD_FOR_IOS)
    #include <dirent.h>
#endif

struct SkFILE;

enum SkFILE_Flags {
    kRead_SkFILE_Flag   = 0x01,
    kWrite_SkFILE_Flag  = 0x02
};

#ifdef _WIN32
const static char SkPATH_SEPARATOR = '\\';
#else
const static char SkPATH_SEPARATOR = '/';
#endif

SkFILE* sk_fopen(const char path[], SkFILE_Flags);
void    sk_fclose(SkFILE*);

size_t  sk_fgetsize(SkFILE*);
/** Return true if the file could seek back to the beginning
*/
bool    sk_frewind(SkFILE*);

size_t  sk_fread(void* buffer, size_t byteCount, SkFILE*);
size_t  sk_fwrite(const void* buffer, size_t byteCount, SkFILE*);
void    sk_fflush(SkFILE*);

int     sk_fseek( SkFILE*, size_t, int );
size_t  sk_ftell( SkFILE* );

// Returns true if something (file, directory, ???) exists at this path.
bool    sk_exists(const char *path);

// Returns true if a directory exists at this path.
bool    sk_isdir(const char *path);

// Create a new directory at this path; returns true if successful.
// If the directory already existed, this will return true.
// Description of the error, if any, will be written to stderr.
bool    sk_mkdir(const char* path);

class SkOSFile {
public:
    class Iter {
    public:
        Iter();
        Iter(const char path[], const char suffix[] = NULL);
        ~Iter();

        void reset(const char path[], const char suffix[] = NULL);
        /** If getDir is true, only returns directories.
            Results are undefined if true and false calls are
            interleaved on a single iterator.
        */
        bool next(SkString* name, bool getDir = false);

    private:
#ifdef SK_BUILD_FOR_WIN
        HANDLE      fHandle;
        uint16_t*   fPath16;
#elif defined(SK_BUILD_FOR_MAC) || defined(SK_BUILD_FOR_UNIX) || defined(SK_BUILD_FOR_ANDROID) || defined(SK_BUILD_FOR_IOS)
        DIR*        fDIR;
        SkString    fPath, fSuffix;
#endif
    };
};

class SkUTF16_Str {
public:
    SkUTF16_Str(const char src[]);
    ~SkUTF16_Str()
    {
        sk_free(fStr);
    }
    const uint16_t* get() const { return fStr; }

private:
    uint16_t*   fStr;
};

#endif
