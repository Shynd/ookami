#include <ookami/constants.hpp>
#include <ookami/memory/memory.hpp>
#include <ookami/objmgr/objmgr.hpp>
#include <ookami/offsets.hpp>

// TODO: Actually do something with the result from the callback.
//       Right now it's just enumerating every object. Make some
//       classes for each object (like WoWUnit), and maybe store
//       everything in a list.

uintptr_t ObjectPtr(uint32_t filter, uint64_t guid)
{
  return ((uintptr_t(__fastcall*)(uint32_t, const char*, uint64_t,
                                  uint32_t))OBJMGR_OBJECTPTR)(filter, nullptr,
                                                              guid, 0);
}

int __fastcall EnumVisibleObjectCallback(uint64_t guid, int filter)
{
  int pointer = ObjectPtr(filter, guid);

  if (!guid) return 0;

  auto type_ptr = pointer + 0x14;
  uint8_t type = Read<uint8_t>(type_ptr);

  if (type & TYPEMASK_PLAYER) {
    // printf("PLAYER: 0x%p\n", pointer);
    if (guid == ObjMgr::GetActivePlayer()) {
      // printf("LOCALPLAYER: 0x%p\n", pointer);
    }
  } else if (type & TYPEMASK_UNIT) {
    // printf("UNIT: 0x%p\n", pointer);
  }

  return 1;
}

void ObjMgr::EnumVisibleObjects()
{
  ((int(__fastcall*)(int, int))OBJMGR_ENUMVISIBLEOBJECTS)(
    (int)EnumVisibleObjectCallback, 0xFFFFFFFF);
}

uint64_t ObjMgr::GetActivePlayer()
{
  return ((uint64_t(__stdcall*)())OBJMGR_GETACTIVEPLAYER)();
}