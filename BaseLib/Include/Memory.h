#include <stdlib.h>
#include "BaseLib.h"
#include <time.h>


VOID ZeroMem (
  VOID   *Buffer,
  UINTN  Size
  );

VOID CopyMem (
  VOID   *Destination,
  VOID   *Source,
  UINTN  Length
);

VOID *AllocatePool (
   UINTN   AllocationSize
  );

VOID DumpHex (
  UINTN        Indent,
  UINTN        Offset,
  UINTN        DataSize,
  VOID         *UserData
  );

  VOID GetClock(char *s);