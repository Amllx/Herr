#include <HILib/Intel/IO.h>

UInt32 IOCpuReadAPIC(VoidPtr ioApic, UInt32 reg)
{
   UInt32 volatile *ioApicArr = (UInt32 volatile *)ioApic;
   ioApicArr[0] = (reg & 0xFF);
   return ioApicArr[4];
   
}
 
void IOCpuWriteAPIC(VoidPtr ioApic, UInt32 reg, UInt32 value)
{
   UInt32 volatile *ioApicArr = (UInt32 volatile *)ioApic;

   ioApicArr[0] = (reg & 0xFF);
   ioApicArr[4] = value;

}
