#pragma once

namespace ShellObjects
{
       typedef struct __LARGE_INTEGER {
              LONGLONG QuadPart;
       } LARGE_INTEGER;
 
       typedef struct __ULARGE_INTEGER {
              ULONGLONG QuadPart;
       } ULARGE_INTEGER;
};
 
DEFINE_GUID(CLSID_TaskbarList,0x56FDF344,0xFD6D,0x11d0,0x95,0x8A,0x00,0x60,0x97,0xC9,0xA0,0x90);

#pragma warning(push)
#pragma warning(disable : 4192)
#pragma warning(disable : 4299)
#pragma warning(disable : 4298)
#import "ShObjIdl.tlb" auto_rename rename("_LARGE_INTEGER", "__LARGE_INTEGER") rename("_ULARGE_INTEGER", "__ULARGE_INTEGER")
#pragma warning(pop)
