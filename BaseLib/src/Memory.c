#include "../Include/Memory.h"

VOID
CopyMem (
  VOID   *Destination,
  VOID   *Source,
  UINTN  Length
  )
{
  CHAR8 *Destination8;
  CHAR8 *Source8;

  Destination8  = Destination;
  Source8       = Source;
  while (Length--) {
    *(Destination8++) = *(Source8++);
  }
}


VOID ZeroMem (
  VOID   *Buffer,
  UINTN  Size
  )
{
  INT8  *Ptr;

  Ptr = Buffer;
  while (Size--) {
    *(Ptr++) = 0;
  }
}

VOID *
AllocatePool (
   UINTN   AllocationSize
  )
{
  VOID * Memory;

  Memory = malloc(AllocationSize);
  // ASSERT(Memory != NULL);
  return Memory;
}

VOID GetClock(char *s)
{
	printf("Function \"%s\": clock = %d \n", s, clock());
}