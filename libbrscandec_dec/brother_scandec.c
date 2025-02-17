#include "brother_scandec.h"
#include "brother_dtype.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
// #include "third_fn_impls.h"
int NAN(float x) { return x != x; }
static void *RESO_BUFFR;
static void *RESO_BUFFER_2;
static int extra_bytes;
static int inResX;
static int outResX;
static int inResY;
static int outResY;
static int inResoY;
static int outResoY;
static int X_scaling_factor;
static int Y_scaling_factor;
static DWORD dwInLinePixCnt;
static DWORD dwOutLineByte;
static INT inResoX;
static INT unique0x0000c304;
static INT outResoX;
static INT nColorType;
static DWORD _dwInLinePixCnt;
static INT nOutDatKind;
static DWORD dwOutLinePixCnt;
static DWORD _dwOutLineByte;
static DWORD _dwOutWriteMaxSize;
static void *pix_buf_2;
static DWORD InLinePixelCnt;
static void *pix_buf;
static HANDLE HANDLE_1;
static HANDLE HANDLE_2;
static int some_flag_1;
static int some_flag_2;
static void *some_function;
static void *some_fn_write;
static THIRD_FN third_fn;
static FOURTH_FN fourth_fn;
static FIFTH_FN fifth_fn;
static int counter_2;
static int counter_3;
static int some_counter;
static long *some_buf;
static int DAT_00208c40;
static long DAT_002089c4;
static long DAT_00208c64;

void FUN_00105173(ulong param_1, int param_2, char *param_3, char *param_4,
                  char *param_5)

{
  long *plVar1;
  int local_34;
  ulong local_10;

  local_10 = param_1;
  if (dwInLinePixCnt / 3 <= param_1) {
    local_10 = dwInLinePixCnt / 3 - 1;
  }
  local_34 = param_2 + some_counter;
  if (extra_bytes <= local_34) {
    local_34 = local_34 - extra_bytes;
  }
  plVar1 = (&some_buf)[local_34];
  *param_3 = *(char *)(local_10 + (long)plVar1);
  *param_4 = *(char *)((long)plVar1 + local_10 + dwInLinePixCnt / 3);
  *param_5 = *(char *)((long)plVar1 + local_10 + (dwInLinePixCnt * 2) / 3);
  return;
}

uint FUN_00105cd3(float param_1, float param_2, float param_3, float param_4,
                  CHAR *param_5, CHAR *param_6, CHAR *param_7)

{
  uint local_74;
  char local_6f;
  char local_6e;
  char local_6d;
  float local_6c;
  float local_68;
  float local_64;
  float local_60;
  float local_5c;
  float local_58;
  float local_54;
  float local_50;
  float local_4c;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  CHAR *local_30;
  CHAR *local_28;
  CHAR *local_20;
  float local_18;
  float local_14;
  float local_10;
  float local_c;

  local_34 = (int)param_1;
  local_48 = (int)param_2;
  local_3c = (int)param_3;
  local_40 = (int)param_4;
  if (((float)local_3c != param_3) || (NAN((float)local_3c) || NAN(param_3))) {
    local_3c = local_3c + 1;
  }
  if (((float)local_40 != param_4) || (NAN((float)local_40) || NAN(param_4))) {
    local_40 = local_40 + 1;
  }
  local_68 = 0.0;
  local_64 = 0.0;
  local_60 = 0.0;
  local_5c = 0.0;
  local_38 = local_48;
  local_30 = param_7;
  local_28 = param_6;
  local_20 = param_5;
  local_18 = param_4;
  local_14 = param_3;
  local_10 = param_2;
  local_c = param_1;
  for (; local_48 < local_40; local_48 = local_48 + 1) {
    local_4c = (float)local_48;
    local_50 = (float)(local_48 + 1);
    if (local_4c < local_10) {
      local_4c = local_10;
    }
    if (local_18 < local_50) {
      local_50 = local_18;
    }
    local_54 = local_50 - local_4c;
    for (local_44 = local_34; local_44 < local_3c; local_44 = local_44 + 1) {
      FUN_00105173((long)local_44, local_48, &local_6d, &local_6e, &local_6f);
      local_4c = (float)local_44;
      local_50 = (float)(local_44 + 1);
      if (local_4c < local_c) {
        local_4c = local_c;
      }
      if (local_14 < local_50) {
        local_50 = local_14;
      }
      local_58 = local_50 - local_4c;
      local_6c = local_58 * local_54;
      local_68 = local_68 + local_6c;
      local_5c = local_5c + (float)(int)(short)(ushort)local_6d * local_6c;
      local_60 = local_60 + (float)(int)(short)(ushort)local_6e * local_6c;
      local_64 = local_64 + (float)(int)(short)(ushort)local_6f * local_6c;
    }
  }
  if (local_68 != 0.0) {
    *local_20 = (char)(int)(local_5c / local_68 + 0.5);
    *local_28 = (char)(int)(local_60 / local_68 + 0.5);
    *local_30 = (char)(int)(local_64 / local_68 + 0.5);
  }
  local_74 = (uint)(local_68 != 0.0);
  return local_74;
}

DWORD FUN_00103de8(float param_1, CHAR *param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float local_2c;
  float local_24;
  ulong local_20;
  CHAR *local_18;

  fVar2 = (float)inResX / (float)outResX;
  fVar3 = (float)inResY;
  fVar1 = (float)outResY;
  local_24 = 0.0;
  local_2c = fVar2 + 0.0;
  local_18 = param_2;
  for (local_20 = 0; local_20 < dwOutLinePixCnt; local_20 = local_20 + 1) {
    FUN_00105cd3(local_24, param_1, local_2c, param_1 + fVar3 / fVar1,
                 local_18 + 2, local_18 + 1, local_18);
    local_24 = local_24 + fVar2;
    local_2c = local_2c + fVar2;
    local_18 = local_18 + 3;
  }
  return dwOutLineByte;
}

ulong FUN_00103f01(float param_1, CHAR *param_2)

{
  ulong uVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float local_2c;
  float local_24;
  ulong local_20;
  CHAR *local_18;

  fVar3 = (float)inResX / (float)outResX;
  fVar4 = (float)inResY;
  fVar2 = (float)outResY;
  uVar1 = dwOutLineByte / 3;
  local_24 = 0.0;
  local_2c = fVar3 + 0.0;
  local_18 = param_2;
  for (local_20 = 0; local_20 < dwOutLinePixCnt; local_20 = local_20 + 1) {
    FUN_00105cd3(local_24, param_1, local_2c, param_1 + fVar4 / fVar2, local_18,
                 local_18 + uVar1, local_18 + uVar1 * 2);
    local_24 = local_24 + fVar3;
    local_2c = local_2c + fVar3;
    local_18 = local_18 + 1;
  }
  return dwOutLineByte;
}

DWORD FUN_00103ccf(float param_1, char *param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float local_2c;
  float local_24;
  ulong i;
  char *local_18;

  fVar2 = (float)inResX / (float)outResX;
  fVar3 = (float)inResY;
  fVar1 = (float)outResY;
  local_24 = 0.0;
  local_2c = fVar2 + 0.0;
  local_18 = param_2;
  for (i = 0; i < dwOutLinePixCnt; i = i + 1) {
    FUN_00105cd3(local_24, param_1, local_2c, param_1 + fVar3 / fVar1, local_18,
                 local_18 + 1, local_18 + 2);
    local_24 = local_24 + fVar2;
    local_2c = local_2c + fVar2;
    local_18 = local_18 + 3;
  }
  return dwOutLineByte;
}

void FUN_00105279(ulong param_1, int param_2, float *param_3, float *param_4,
                  float *param_5)

{
  long *plVar1;
  int local_34;
  ulong local_10;

  local_10 = param_1;
  if (dwInLinePixCnt / 3 <= param_1) {
    local_10 = dwInLinePixCnt / 3 - 1;
  }
  local_34 = param_2 + some_counter;
  if (extra_bytes <= local_34) {
    local_34 = local_34 - extra_bytes;
  }
  plVar1 = (&some_buf)[local_34];
  *param_3 = (float)(int)(short)(ushort) * (char *)(local_10 + (long)plVar1);
  *param_4 = (float)(int)(short)(ushort) *
             (char *)((long)plVar1 + local_10 + dwInLinePixCnt / 3);
  *param_5 = (float)(int)(short)(ushort) *
             (char *)((long)plVar1 + local_10 + (dwInLinePixCnt * 2) / 3);
  return;
}

float FUN_001053a0(long param_1, long param_2)

{
  float local_20;
  int local_1c;

  local_20 = 0.0;
  for (local_1c = 0; local_1c < 4; local_1c = local_1c + 1) {
    local_20 = local_20 + *(float *)((long)local_1c * 4 + param_1) *
                              *(float *)((long)local_1c * 4 + param_2);
  }
  return local_20;
}

int FUN_0010553d(long param_1)

{
  long lVar1;
  long lVar2;
  long lVar3;
  float fVar4;
  int local_4c;
  float local_38[5];
  int local_24;
  long local_20;

  lVar1 = (long)DAT_00208c40;
  lVar2 = (long)counter_3;
  local_20 = param_1;
  for (local_24 = 0; local_24 < 4; local_24 = local_24 + 1) {
    lVar3 = (long)local_24;
    fVar4 = FUN_001053a0((long)(&DAT_00208c64 + lVar2 * 0x14),
                         (long)local_24 * 0x10 + local_20);
    local_38[lVar3] = fVar4;
  }
  fVar4 = FUN_001053a0((long)local_38, (long)(&DAT_002089c4 + lVar1 * 0x14));
  local_4c = (int)fVar4;
  if (local_4c < 0) {
    local_4c = 0;
  } else if (0xff < local_4c) {
    local_4c = 0xff;
  }
  return local_4c;
}

void FUN_00105771(float param_1, float param_2, char *param_3, char *param_4,
                  char *param_5)

{
  char *pcVar1;
  char *pcVar2;
  int iVar3;
  float local_118[16];
  float local_d8[16];
  float local_98[19];
  int local_4c;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  char *local_38;
  char *local_30;
  char *local_28;
  float local_20;
  float local_1c;

  local_3c = (int)param_1;
  local_40 = (int)param_2;
  local_44 = 0;
  local_38 = param_5;
  local_30 = param_4;
  local_28 = param_3;
  local_20 = param_2;
  local_1c = param_1;
  for (local_48 = 0; pcVar1 = local_28, local_48 < 4; local_48 = local_48 + 1) {
    for (local_4c = 0; local_4c < 4; local_4c = local_4c + 1) {
      FUN_00105279((long)(local_48 + local_3c + -1), local_4c,
                   local_98 + local_44, local_d8 + local_44,
                   local_118 + local_44);
      local_44 = local_44 + 1;
    }
  }
  iVar3 = FUN_0010553d((long)local_98);
  pcVar2 = local_30;
  *pcVar1 = (char)iVar3;
  iVar3 = FUN_0010553d((long)local_d8);
  pcVar1 = local_38;
  *pcVar2 = (char)iVar3;
  iVar3 = FUN_0010553d((long)local_118);
  *pcVar1 = (char)iVar3;
  return;
}

DWORD FUN_00103059(float param_1, CHAR *param_2)

{
  char *pcVar1;
  int iVar2;
  int iVar3;
  float fVar4;
  ulong local_58;
  int local_50;
  float local_2c;
  float local_28;
  int local_20;
  int local_1c;
  CHAR *local_18;

  fVar4 = (float)inResX / (float)outResX;
  iVar2 = (int)(dwOutLinePixCnt / (ulong)(long)outResX);
  local_18 = param_2;
  if ((float)(int)param_1 == param_1) {
    local_28 = 0.0;
    for (local_1c = 0; local_1c < iVar2; local_1c = local_1c + 1) {
      if (9.223372e+18 <= param_1) {
        local_50 = (int)(long)(param_1 - 9.223372e+18);
      } else {
        local_50 = (int)(long)param_1;
      }
      if (9.223372e+18 <= local_28) {
        local_58 = (long)(local_28 - 9.223372e+18) ^ 0x8000000000000000;
      } else {
        local_58 = (ulong)local_28;
      }
      FUN_00105173(local_58, local_50, local_18, local_18 + 1, local_18 + 2);
      local_20 = 1;
      local_2c = local_28;
      pcVar1 = local_18;
      while (1) {
        local_18 = pcVar1 + 3;
        local_2c = local_2c + fVar4;
        if (outResX <= local_20)
          break;
        DAT_00208c40 = local_20;
        FUN_00105771(local_2c, param_1, local_18, pcVar1 + 4, pcVar1 + 5);
        local_20 = local_20 + 1;
        pcVar1 = local_18;
      }
      local_28 = local_28 + (float)inResX;
    }
  } else {
    local_28 = 0.0;
    for (local_1c = 0; local_1c < iVar2; local_1c = local_1c + 1) {
      local_2c = local_28;
      for (local_20 = 0; local_20 < outResX; local_20 = local_20 + 1) {
        DAT_00208c40 = local_20;
        FUN_00105771(local_2c, param_1, local_18, local_18 + 1, local_18 + 2);
        local_18 = local_18 + 3;
        local_2c = local_2c + fVar4;
      }
      local_28 = local_28 + (float)inResX;
    }
  }
  local_28 = (float)(inResX * iVar2);
  iVar2 = iVar2 * outResX;
  iVar3 = (int)dwOutLinePixCnt;
  for (local_1c = 0; local_1c < iVar3 - iVar2; local_1c = local_1c + 1) {
    DAT_00208c40 = local_1c;
    FUN_00105771(local_28, param_1, local_18, local_18 + 1, local_18 + 2);
    local_18 = local_18 + 3;
    local_28 = local_28 + fVar4;
  }
  return dwOutLineByte;
}

BOOL FUN_00104ed0(CHAR *src, int data_comp, CHAR *dest)

{
  BOOL bVar1;
  ulong __n;
  ulong i;

  __n = dwInLinePixCnt / 3;
  if (data_comp == 3) {
    counter_2 = counter_2 + 1;
    memcpy(dest + __n, src, __n);
    goto LAB_001050d9;
  }
  if (data_comp < 4) {
    if (data_comp == 2) {
      counter_2 = counter_2 + 1;
      memcpy(dest, src, __n);
      goto LAB_001050d9;
    }
  } else {
    if (data_comp == 4) {
      counter_2 = counter_2 + 1;
      memcpy(dest + __n * 2, src, __n);
      goto LAB_001050d9;
    }
    if (data_comp == 5) {
      counter_2 = counter_2 + 3;
      for (i = 0; i < __n; i = i + 1) {
        dest[i] = src[i * 3];
        dest[i + __n] = src[i * 3 + 1];
        dest[i + __n * 2] = src[i * 3 + 2];
      }
      goto LAB_001050d9;
    }
  }
  counter_2 = counter_2 + 3;
  for (i = 0; i < __n; i = i + 1) {
    dest[i] = src[i * 3 + 2];
    dest[i + __n] = src[i * 3 + 1];
    dest[i + __n * 2] = src[i * 3];
  }
LAB_001050d9:
  bVar1 = 2 < counter_2;
  if (bVar1) {
    counter_2 = 0;
    counter_3 = counter_3 + 1;
  }
  return bVar1;
}

CHAR *invert_buffer_offset(BOOL reverWrite, CHAR *write_buf, int param_3)

{
  long lVar1;
  CHAR *ret_val;

  if (reverWrite == 0) {
    lVar1 = (long)param_3 * dwOutLineByte;
  } else {
    lVar1 = -((long)(param_3 + 1) * dwOutLineByte);
  }
  ret_val = write_buf + lVar1;
  return ret_val;
}

size_t copy_buffer_and_decomp(SCANDEC_WRITE *scandec_write, int *param_2)

{
  BOOL bVar1;
  CHAR *__dest;
  CHAR *dest;
  DWORD local_20;

  local_20 = 0;
  if (((uint)nColorType >> 10 & 1) == 0) {
    __dest = invert_buffer_offset(scandec_write->bReverWrite,
                                  scandec_write->pWriteBuff, 0);
    memcpy(__dest, scandec_write->pLineData, dwOutLineByte);
    local_20 = dwOutLineByte;
    *param_2 = 1;
  } else {
    dest = invert_buffer_offset(scandec_write->bReverWrite,
                                scandec_write->pWriteBuff, 0);
    bVar1 = FUN_00104ed0(scandec_write->pLineData, scandec_write->nInDataComp,
                         dest);
    if (bVar1 != 0) {
      local_20 = dwOutLineByte;
      *param_2 = 1;
    }
  }
  return local_20;
}

BOOL decomp_line_data(CHAR *line_buf, int param_2, void *dest)

{
  BOOL bVar1;
  ulong uVar2;
  ulong i;

  uVar2 = dwInLinePixCnt / 3;
  if (param_2 == 3) {
    counter_2 = counter_2 + 1;
    for (i = 0; i < uVar2; i = i + 1) {
      *(CHAR *)((long)dest + i * 3 + 1) = line_buf[i];
    }
    goto LAB_00104eaa;
  }
  if (param_2 < 4) {
    if (param_2 == 2) {
      counter_2 = counter_2 + 1;
      for (i = 0; i < uVar2; i = i + 1) {
        *(CHAR *)((long)dest + i * 3 + 2) = line_buf[i];
      }
      goto LAB_00104eaa;
    }
  } else {
    if (param_2 == 4) {
      counter_2 = counter_2 + 1;
      for (i = 0; i < uVar2; i = i + 1) {
        *(CHAR *)(i * 3 + (long)dest) = line_buf[i];
      }
      goto LAB_00104eaa;
    }
    if (param_2 == 5) {
      counter_2 = counter_2 + 3;
      for (i = 0; i < uVar2; i = i + 1) {
        *(CHAR *)(i * 3 + (long)dest) = line_buf[i * 3 + 2];
        *(CHAR *)((long)dest + i * 3 + 1) = line_buf[i * 3 + 1];
        *(CHAR *)((long)dest + i * 3 + 2) = line_buf[i * 3];
      }
      goto LAB_00104eaa;
    }
  }
  counter_2 = counter_2 + 3;
  memcpy(dest, line_buf, dwInLinePixCnt);
LAB_00104eaa:
  bVar1 = 2 < counter_2;
  if (bVar1) {
    counter_2 = 0;
    counter_3 = counter_3 + 1;
  }
  return bVar1;
}

BOOL FUN_00104a67(CHAR *line_pointer, int data_comp, void *dest)

{
  BOOL bVar1;
  ulong uVar2;
  ulong i;

  uVar2 = dwInLinePixCnt / 3;
  if (data_comp == 3) {
    counter_2 = counter_2 + 1;
    for (i = 0; i < uVar2; i = i + 1) {
      *(CHAR *)((long)dest + i * 3 + 1) = line_pointer[i];
    }
    goto LAB_00104c78;
  }
  if (data_comp < 4) {
    if (data_comp == 2) {
      counter_2 = counter_2 + 1;
      for (i = 0; i < uVar2; i = i + 1) {
        *(CHAR *)(i * 3 + (long)dest) = line_pointer[i];
      }
      goto LAB_00104c78;
    }
  } else {
    if (data_comp == 4) {
      counter_2 = counter_2 + 1;
      for (i = 0; i < uVar2; i = i + 1) {
        *(CHAR *)((long)dest + i * 3 + 2) = line_pointer[i];
      }
      goto LAB_00104c78;
    }
    if (data_comp == 5) {
      counter_2 = counter_2 + 3;
      memcpy(dest, line_pointer, dwInLinePixCnt);
      goto LAB_00104c78;
    }
  }
  counter_2 = counter_2 + 3;
  for (i = 0; i < uVar2; i = i + 1) {
    *(CHAR *)(i * 3 + (long)dest) = line_pointer[i * 3 + 2];
    *(CHAR *)((long)dest + i * 3 + 1) = line_pointer[i * 3 + 1];
    *(CHAR *)((long)dest + i * 3 + 2) = line_pointer[i * 3];
  }
LAB_00104c78:
  bVar1 = 2 < counter_2;
  if (bVar1) {
    counter_2 = 0;
    counter_3 = counter_3 + 1;
  }
  return bVar1;
}

size_t write_sameRes_kind1(SCANDEC_WRITE *param_1, int *status)

{
  BOOL bVar1;
  CHAR *write_buffer_start;
  DWORD local_20;

  local_20 = 0;
  if (((uint)nColorType >> 10 & 1) == 0) {
    write_buffer_start =
        invert_buffer_offset(param_1->bReverWrite, param_1->pWriteBuff, 0);
    memcpy(write_buffer_start, param_1->pLineData, dwOutLineByte);
    local_20 = dwOutLineByte;
    *status = 1;
  } else {
    write_buffer_start =
        invert_buffer_offset(param_1->bReverWrite, param_1->pWriteBuff, 0);
    bVar1 = FUN_00104a67(param_1->pLineData, param_1->nInDataComp,
                         write_buffer_start);
    if (bVar1 != 0) {
      local_20 = dwOutLineByte;
      *status = 1;
    }
  }
  return local_20;
}

size_t write_sameRes_kind3(SCANDEC_WRITE *scandec_write, int *param_2)

{
  BOOL bVar1;
  CHAR *pvVar2;
  CHAR *dest;
  DWORD local_20;

  local_20 = 0;
  if (((uint)nColorType >> 10 & 1) == 0) {
    pvVar2 = invert_buffer_offset(scandec_write->bReverWrite,
                                  scandec_write->pWriteBuff, 0);
    memcpy(pvVar2, scandec_write->pLineData, dwOutLineByte);
    local_20 = dwOutLineByte;
    *param_2 = 1;
  } else {
    dest = invert_buffer_offset(scandec_write->bReverWrite,
                                scandec_write->pWriteBuff, 0);
    bVar1 = decomp_line_data(scandec_write->pLineData,
                             scandec_write->nInDataComp, dest);
    if (bVar1 != 0) {
      local_20 = dwOutLineByte;
      *param_2 = 1;
    }
  }
  return local_20;
}

int FUN_00104754(CHAR *param_1, int param_2)

{
  ulong __n;
  int local_34;
  int local_30;
  long *local_28;
  ulong local_20;

  local_34 = 0;
  __n = dwInLinePixCnt / 3;
  if (param_1 == (CHAR *)0x0) {
    local_30 = some_counter - 1;
    if (local_30 < 0) {
      local_30 = extra_bytes - 1;
    }
    memmove((&some_buf)[some_counter], (&some_buf)[local_30], dwInLinePixCnt);
    counter_2 = counter_2 + 3;
  } else {
    local_28 = (&some_buf)[some_counter];
    if (param_2 == 3) {
      counter_2 = counter_2 + 1;
      memmove((void *)((long)local_28 + __n), param_1, __n);
    } else {
      if (param_2 < 4) {
        if (param_2 == 2) {
          counter_2 = counter_2 + 1;
          memmove(local_28, param_1, __n);
          goto LAB_001049eb;
        }
      } else {
        if (param_2 == 4) {
          counter_2 = counter_2 + 1;
          memmove((void *)(__n * 2 + (long)local_28), param_1, __n);
          goto LAB_001049eb;
        }
        if (param_2 == 5) {
          counter_2 = counter_2 + 3;
          for (local_20 = 0; local_20 < __n; local_20 = local_20 + 1) {
            *(CHAR *)((long)local_28 + local_20) = param_1[local_20 * 3];
            *(CHAR *)((long)local_28 + local_20 + __n) =
                param_1[local_20 * 3 + 1];
            *(CHAR *)((long)local_28 + local_20 + __n * 2) =
                param_1[local_20 * 3 + 2];
          }
          goto LAB_001049eb;
        }
      }
      counter_2 = counter_2 + 3;
      for (local_20 = 0; local_20 < __n; local_20 = local_20 + 1) {
        *(CHAR *)((long)local_28 + local_20) = param_1[local_20 * 3 + 2];
        *(CHAR *)((long)local_28 + local_20 + __n) = param_1[local_20 * 3 + 1];
        *(CHAR *)((long)local_28 + local_20 + __n * 2) = param_1[local_20 * 3];
      }
    }
  }
LAB_001049eb:
  if (2 < counter_2) {
    counter_2 = 0;
    some_counter = some_counter + 1;
    counter_3 = counter_3 + 1;
    if (extra_bytes <= some_counter) {
      some_counter = 0;
    }
    if (counter_3 == 1) {
      memmove(*(void **)(&counter_2 + (long)extra_bytes * 2), local_28,
              dwInLinePixCnt);
    }
    local_34 = 1;
  }
  return local_34;
}

long FUN_00101eae(SCANDEC_WRITE *param_1, int *param_2)

{
  int iVar1;
  CHAR *pCVar2;
  long lVar3;
  float fVar4;
  float fVar5;
  int local_38;
  int local_34;
  float local_2c;
  long local_28;

  local_28 = 0;
  iVar1 = FUN_00104754(param_1->pLineData, param_1->nInDataComp);
  if ((iVar1 != 0) && (2 < counter_3)) {
    fVar5 = (float)inResY;
    fVar4 = (float)outResY;
    counter_3 = 0;
    if ((int)(counter_3 - 3) % inResY == 0) {
      local_2c = 1.0;
    } else {
      for (local_2c = 0.0; local_2c < (float)((int)(counter_3 - 3) % inResY);
           local_2c = local_2c + fVar5 / fVar4) {
        counter_3 = counter_3 + 1;
      }
      local_2c = (local_2c + 1.0) - (float)((int)(counter_3 - 3) % inResY);
    }
    local_38 = 0;
    for (local_34 = 0; local_34 < outResY; local_34 = local_34 + 1) {
      if (((int)(counter_3 - 3) % inResY <= (local_34 * inResY) / outResY) &&
          ((local_34 * inResY) / outResY < (int)(counter_3 - 3) % inResY + 1)) {
        local_38 = local_38 + 1;
      }
    }
    local_34 = 0;
    while (local_34 < local_38) {
      pCVar2 = invert_buffer_offset(param_1->bReverWrite, param_1->pWriteBuff,
                                    *param_2);
      lVar3 = third_fn(local_2c, pCVar2);
      local_28 = local_28 + lVar3;
      local_34 = local_34 + 1;
      local_2c = local_2c + fVar5 / fVar4;
      counter_3 = counter_3 + 1;
      *param_2 = *param_2 + 1;
    }
  }
  return local_28;
}

DWORD FUN_001033a4(float param_1, CHAR *param_2)

{
  char *pcVar1;
  int iVar2;
  int iVar3;
  float fVar4;
  ulong local_58;
  int local_50;
  float local_2c;
  float local_28;
  int local_20;
  int local_1c;
  char *local_18;

  fVar4 = (float)inResX / (float)outResX;
  iVar2 = (int)(dwOutLinePixCnt / (ulong)(long)outResX);
  local_18 = param_2;
  if ((float)(int)param_1 == param_1) {
    local_28 = 0.0;
    for (local_1c = 0; local_1c < iVar2; local_1c = local_1c + 1) {
      if (9.223372e+18 <= param_1) {
        local_50 = (int)(long)(param_1 - 9.223372e+18);
      } else {
        local_50 = (int)(long)param_1;
      }
      if (9.223372e+18 <= local_28) {
        local_58 = (long)(local_28 - 9.223372e+18) ^ 0x8000000000000000;
      } else {
        local_58 = (ulong)local_28;
      }
      FUN_00105173(local_58, local_50, local_18 + 2, local_18 + 1, local_18);
      local_20 = 1;
      local_2c = local_28;
      pcVar1 = local_18;
      while (1) {
        local_18 = pcVar1 + 3;
        local_2c = local_2c + fVar4;
        if (outResX <= local_20)
          break;
        DAT_00208c40 = local_20;
        FUN_00105771(local_2c, param_1, pcVar1 + 5, pcVar1 + 4, local_18);
        local_20 = local_20 + 1;
        pcVar1 = local_18;
      }
      local_28 = local_28 + (float)inResX;
    }
  } else {
    local_28 = 0.0;
    for (local_1c = 0; local_1c < iVar2; local_1c = local_1c + 1) {
      local_2c = local_28;
      for (local_20 = 0; local_20 < outResX; local_20 = local_20 + 1) {
        DAT_00208c40 = local_20;
        FUN_00105771(local_2c, param_1, local_18 + 2, local_18 + 1, local_18);
        local_18 = local_18 + 3;
        local_2c = local_2c + fVar4;
      }
      local_28 = local_28 + (float)inResX;
    }
  }
  local_28 = (float)(inResX * iVar2);
  iVar2 = iVar2 * outResX;
  iVar3 = (int)dwOutLinePixCnt;
  for (local_1c = 0; local_1c < iVar3 - iVar2; local_1c = local_1c + 1) {
    DAT_00208c40 = local_1c;
    FUN_00105771(local_28, param_1, local_18 + 2, local_18 + 1, local_18);
    local_18 = local_18 + 3;
    local_28 = local_28 + fVar4;
  }
  return dwOutLineByte;
}

ulong FUN_001036ef(float param_1, CHAR *param_2)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  int iVar4;
  float fVar5;
  ulong local_68;
  int local_60;
  float local_2c;
  float local_28;
  int local_20;
  int local_1c;
  char *local_18;

  fVar5 = (float)inResX / (float)outResX;
  uVar3 = dwOutLineByte / 3;
  lVar1 = uVar3 * 2;
  iVar2 = (int)(dwOutLinePixCnt / (ulong)(long)outResX);
  local_18 = param_2;
  if ((float)(int)param_1 == param_1) {
    local_28 = 0.0;
    for (local_1c = 0; local_1c < iVar2; local_1c = local_1c + 1) {
      if (9.223372e+18 <= param_1) {
        local_60 = (int)(long)(param_1 - 9.223372e+18);
      } else {
        local_60 = (int)(long)param_1;
      }
      if (9.223372e+18 <= local_28) {
        local_68 = (long)(local_28 - 9.223372e+18) ^ 0x8000000000000000;
      } else {
        local_68 = (ulong)local_28;
      }
      FUN_00105173(local_68, local_60, local_18, local_18 + uVar3,
                   local_18 + lVar1);
      local_20 = 1;
      local_2c = local_28;
      while (1) {
        local_18 = local_18 + 1;
        local_2c = local_2c + fVar5;
        if (outResX <= local_20)
          break;
        DAT_00208c40 = local_20;
        FUN_00105771(local_2c, param_1, local_18, local_18 + uVar3,
                     local_18 + lVar1);
        local_20 = local_20 + 1;
      }
      local_28 = local_28 + (float)inResX;
    }
  } else {
    local_28 = 0.0;
    for (local_1c = 0; local_1c < iVar2; local_1c = local_1c + 1) {
      local_2c = local_28;
      for (local_20 = 0; local_20 < outResX; local_20 = local_20 + 1) {
        DAT_00208c40 = local_20;
        FUN_00105771(local_2c, param_1, local_18, local_18 + uVar3,
                     local_18 + lVar1);
        local_18 = local_18 + 1;
        local_2c = local_2c + fVar5;
      }
      local_28 = local_28 + (float)inResX;
    }
  }
  local_28 = (float)(inResX * iVar2);
  iVar2 = iVar2 * outResX;
  iVar4 = (int)dwOutLinePixCnt;
  for (local_1c = 0; local_1c < iVar4 - iVar2; local_1c = local_1c + 1) {
    DAT_00208c40 = local_1c;
    FUN_00105771(local_28, param_1, local_18, local_18 + uVar3,
                 local_18 + lVar1);
    local_18 = local_18 + 1;
    local_28 = local_28 + fVar5;
  }
  return dwOutLineByte;
}

char FUN_001050ff(long param_1, int param_2)

{
  int local_1c;
  long local_10;

  local_10 = param_1;
  if (dwInLinePixCnt <= (ulong)param_1) {
    local_10 = dwInLinePixCnt - 1;
  }
  local_1c = param_2 + some_counter;
  if (extra_bytes <= local_1c) {
    local_1c = local_1c - extra_bytes;
  }
  return *(char *)(local_10 + (long)(&some_buf)[local_1c]);
}
uint FUN_00105aa6(float param_1, float param_2, float param_3, float param_4,
                  char *param_5)

{
  char bVar1;
  float fVar2;
  float fVar3;
  uint local_5c;
  float local_50;
  float local_4c;
  float local_40;
  float local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;

  local_38 = (int)param_2;
  local_2c = (int)param_3;
  local_30 = (int)param_4;
  if (((float)local_2c != param_3) || (NAN((float)local_2c) || NAN(param_3))) {
    local_2c = local_2c + 1;
  }
  if (((float)local_30 != param_4) || (NAN((float)local_30) || NAN(param_4))) {
    local_30 = local_30 + 1;
  }
  local_50 = 0.0;
  local_4c = 0.0;
  for (; local_38 < local_30; local_38 = local_38 + 1) {
    local_3c = param_2;
    if (param_2 <= (float)local_38) {
      local_3c = (float)local_38;
    }
    local_40 = param_4;
    if ((float)(local_38 + 1) <= param_4) {
      local_40 = (float)(local_38 + 1);
    }
    fVar2 = local_40 - local_3c;
    for (local_34 = (int)param_1; local_34 < local_2c;
         local_34 = local_34 + 1) {
      bVar1 = FUN_001050ff((long)local_34, (long)local_38);
      local_3c = param_1;
      if (param_1 <= (float)local_34) {
        local_3c = (float)local_34;
      }
      local_40 = param_3;
      if ((float)(local_34 + 1) <= param_3) {
        local_40 = (float)(local_34 + 1);
      }
      fVar3 = (local_40 - local_3c) * fVar2;
      local_50 = local_50 + fVar3;
      local_4c = local_4c + (float)(int)(short)(ushort)bVar1 * fVar3;
    }
  }
  if (local_50 != 0.0) {
    *param_5 = (char)(int)(local_4c / local_50 + 0.5);
  }
  local_5c = (uint)(local_50 != 0.0);
  return local_5c;
}

DWORD FUN_00103bca(float param_1, char *param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float local_2c;
  float local_24;
  ulong local_20;
  char *local_18;

  fVar2 = (float)inResX / (float)outResX;
  fVar3 = (float)inResY;
  fVar1 = (float)outResY;
  local_24 = 0.0;
  local_2c = fVar2 + 0.0;
  local_18 = param_2;
  for (local_20 = 0; local_20 < dwOutLinePixCnt; local_20 = local_20 + 1) {
    FUN_00105aa6(local_24, param_1, local_2c, param_1 + fVar3 / fVar1,
                 local_18);
    local_24 = local_24 + fVar2;
    local_2c = local_2c + fVar2;
    local_18 = local_18 + 1;
  }
  return dwOutLineByte;
}

void FUN_001056c7(float param_1, float param_2, char *param_3)

{
  char *pcVar1;
  char bVar2;
  int iVar3;
  long lVar4;
  float local_88[19];
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  char *l_param3;
  float local_20;
  float local_1c;

  local_2c = (int)param_1;
  local_30 = (int)param_2;
  local_34 = 0;
  l_param3 = param_3;
  local_20 = param_2;
  local_1c = param_1;
  for (local_38 = 0; pcVar1 = l_param3, local_38 < 4; local_38 = local_38 + 1) {
    for (local_3c = 0; local_3c < 4; local_3c = local_3c + 1) {
      lVar4 = (long)local_34;
      bVar2 = FUN_001050ff((long)(local_38 + local_2c + -1), local_3c);
      local_88[lVar4] = (float)(int)(short)(ushort)bVar2;
      local_34 = local_34 + 1;
    }
  }
  iVar3 = FUN_0010553d((long)local_88);
  *pcVar1 = (char)iVar3;
  return;
}

DWORD FUN_00102d59(float param_1, CHAR *param_2)

{
  char cVar1;
  int iVar2;
  int iVar3;
  float fVar4;
  ulong local_40;
  int local_38;
  float local_2c;
  float local_28;
  int local_20;
  int local_1c;
  char *l_param2;

  fVar4 = (float)inResX / (float)outResX;
  iVar2 = (int)(dwOutLinePixCnt / (ulong)(long)outResX);
  l_param2 = param_2;
  if ((float)(int)param_1 == param_1) {
    local_28 = 0.0;
    for (local_1c = 0; local_1c < iVar2; local_1c = local_1c + 1) {
      if (9.223372e+18 <= param_1) {
        local_38 = (int)(long)(param_1 - 9.223372e+18);
      } else {
        local_38 = (int)(long)param_1;
      }
      if (9.223372e+18 <= local_28) {
        local_40 = (long)(local_28 - 9.223372e+18) ^ 0x8000000000000000;
      } else {
        local_40 = (ulong)local_28;
      }
      cVar1 = FUN_001050ff(local_40, local_38);
      *l_param2 = cVar1;
      local_20 = 1;
      local_2c = local_28;
      while (1) {
        l_param2 = l_param2 + 1;
        local_2c = local_2c + fVar4;
        if (outResX <= local_20)
          break;
        DAT_00208c40 = local_20;
        FUN_001056c7(local_2c, param_1, l_param2);
        local_20 = local_20 + 1;
      }
      local_28 = local_28 + (float)inResX;
    }
  } else {
    local_28 = 0.0;
    for (local_1c = 0; local_1c < iVar2; local_1c = local_1c + 1) {
      local_2c = local_28;
      for (local_20 = 0; local_20 < outResX; local_20 = local_20 + 1) {
        DAT_00208c40 = local_20;
        FUN_001056c7(local_2c, param_1, l_param2);
        local_2c = local_2c + fVar4;
        l_param2 = l_param2 + 1;
      }
      local_28 = local_28 + (float)inResX;
    }
  }
  local_28 = (float)(inResX * iVar2);
  iVar2 = iVar2 * outResX;
  iVar3 = (int)dwOutLinePixCnt;
  for (local_1c = 0; local_1c < iVar3 - iVar2; local_1c = local_1c + 1) {
    DAT_00208c40 = local_1c;
    FUN_001056c7(local_28, param_1, l_param2);
    local_28 = local_28 + fVar4;
    l_param2 = l_param2 + 1;
  }
  return dwOutLineByte;
}

uint FUN_00105879(float param_1, float param_2, float param_3, float param_4,
                  char *param_5)

{
  char bVar1;
  float fVar2;
  float fVar3;
  uint local_5c;
  float local_50;
  float local_4c;
  float local_40;
  float local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;

  local_38 = (int)param_2;
  local_2c = (int)param_3;
  local_30 = (int)param_4;
  if (((float)local_2c != param_3) || (NAN((float)local_2c) || NAN(param_3))) {
    local_2c = local_2c + 1;
  }
  if (((float)local_30 != param_4) || (NAN((float)local_30) || NAN(param_4))) {
    local_30 = local_30 + 1;
  }
  local_50 = 0.0;
  local_4c = 0.0;
  for (; local_38 < local_30; local_38 = local_38 + 1) {
    local_3c = param_2;
    if (param_2 <= (float)local_38) {
      local_3c = (float)local_38;
    }
    local_40 = param_4;
    if ((float)(local_38 + 1) <= param_4) {
      local_40 = (float)(local_38 + 1);
    }
    fVar2 = local_40 - local_3c;
    for (local_34 = (int)param_1; local_34 < local_2c;
         local_34 = local_34 + 1) {
      bVar1 = FUN_001050ff((long)local_34, local_38);
      local_3c = param_1;
      if (param_1 <= (float)local_34) {
        local_3c = (float)local_34;
      }
      local_40 = param_3;
      if ((float)(local_34 + 1) <= param_3) {
        local_40 = (float)(local_34 + 1);
      }
      fVar3 = (local_40 - local_3c) * fVar2;
      local_50 = local_50 + fVar3;
      local_4c = local_4c + (float)(int)(short)(ushort)bVar1 * fVar3;
    }
  }
  if (local_50 != 0.0) {
    *param_5 = (char)(int)(local_4c / local_50 + 0.5);
  }
  local_5c = (uint)(local_50 != 0.0);
  return local_5c;
}

DWORD FUN_00103a7b(float param_1, CHAR *param_2)

{
  char local_39;
  float local_38;
  float local_34;
  float local_30;
  float local_2c;
  float local_28;
  float local_24;
  ulong local_20;
  char *local_18;
  float local_c;

  local_34 = (float)inResX / (float)outResX;
  local_38 = (float)inResY / (float)outResY;
  local_24 = 0.0;
  local_2c = local_34 + 0.0;
  local_30 = param_1 + local_38;
  local_18 = (char *)(param_2 + -1);
  local_28 = param_1;
  local_c = param_1;
  for (local_20 = 0; local_20 < dwOutLinePixCnt; local_20 = local_20 + 1) {
    FUN_00105879(local_24, local_28, local_2c, local_30, &local_39);
    if ((local_20 & 7) == 0) {
      local_18 = local_18 + 1;
      *local_18 = 0;
    }
    if (4 < local_39) {
      *local_18 = (char)(1 << (7 - ((char)local_20 & 7) & 0x1f)) | *local_18;
    }
    local_24 = local_24 + local_34;
    local_2c = local_2c + local_34;
  }
  return dwOutLineByte;
}

void FUN_0010561d(float param_1, float param_2, char *param_3)

{
  char *pcVar1;
  char bVar2;
  int iVar3;
  long lVar4;
  float local_88[19];
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  char *local_28;
  float local_20;
  float local_1c;

  local_2c = (int)param_1;
  local_30 = (int)param_2;
  local_34 = 0;
  local_28 = param_3;
  local_20 = param_2;
  local_1c = param_1;
  for (local_38 = 0; pcVar1 = local_28, local_38 < 4; local_38 = local_38 + 1) {
    for (local_3c = 0; local_3c < 4; local_3c = local_3c + 1) {
      lVar4 = (long)local_34;
      bVar2 = FUN_001050ff((long)(local_38 + local_2c + -1), local_3c);
      local_88[lVar4] = (float)(int)(short)(ushort)bVar2;
      local_34 = local_34 + 1;
    }
  }
  iVar3 = FUN_0010553d((long)local_88);
  *pcVar1 = (char)iVar3;
  return;
}

DWORD FUN_00102944(float param_1, CHAR *param_2)

{
  float fVar1;
  ulong local_50;
  int local_48;
  float local_38;
  float local_34;
  char local_2d;
  int local_2c;
  long local_28;
  int local_20;
  int local_1c;
  char *local_18;
  float local_c;

  fVar1 = (float)inResX / (float)outResX;
  local_18 = (char *)(param_2 + -1);
  local_2c = (int)(dwOutLinePixCnt / (ulong)(long)outResX);
  local_c = param_1;
  if ((float)(int)param_1 == param_1) {
    local_28 = 0;
    local_34 = 0.0;
    for (local_1c = 0; local_1c < local_2c; local_1c = local_1c + 1) {
      local_38 = local_34;
      for (local_20 = 0; local_20 < outResX; local_20 = local_20 + 1) {
        if (local_20 == 0) {
          if (9.223372e+18 <= local_c) {
            local_48 = (int)(long)(local_c - 9.223372e+18);
          } else {
            local_48 = (int)(long)local_c;
          }
          if (9.223372e+18 <= local_38) {
            local_50 = (long)(local_38 - 9.223372e+18) ^ 0x8000000000000000;
          } else {
            local_50 = (ulong)local_38;
          }
          local_2d = FUN_001050ff(local_50, local_48);
        } else {
          DAT_00208c40 = local_20;
          FUN_0010561d(local_38, local_c, &local_2d);
        }
        if ((local_20 + (int)local_28 & 7U) == 0) {
          local_18 = local_18 + 1;
          *local_18 = 0;
        }
        if (4 < local_2d) {
          *local_18 =
              (char)(1 << (7 - ((char)local_20 + (char)local_28 & 7U) & 0x1f)) |
              *local_18;
        }
        local_38 = local_38 + fVar1;
      }
      local_28 = local_28 + outResX;
      local_34 = local_34 + (float)inResX;
    }
  } else {
    local_28 = 0;
    local_34 = 0.0;
    for (local_1c = 0; local_1c < local_2c; local_1c = local_1c + 1) {
      local_38 = local_34;
      for (local_20 = 0; local_20 < outResX; local_20 = local_20 + 1) {
        DAT_00208c40 = local_20;
        FUN_0010561d(local_38, local_c, &local_2d);
        if ((local_20 + (int)local_28 & 7U) == 0) {
          local_18 = local_18 + 1;
          *local_18 = 0;
        }
        if (4 < local_2d) {
          *local_18 =
              (char)(1 << (7 - ((char)local_20 + (char)local_28 & 7U) & 0x1f)) |
              *local_18;
        }
        local_38 = local_38 + fVar1;
      }
      local_28 = local_28 + outResX;
      local_34 = local_34 + (float)inResX;
    }
  }
  local_34 = (float)(inResX * local_2c);
  local_2c = (int)dwOutLinePixCnt - local_2c * outResX;
  for (local_1c = 0; local_1c < local_2c; local_1c = local_1c + 1) {
    DAT_00208c40 = local_1c;
    FUN_0010561d(local_34, local_c, &local_2d);
    if ((local_1c + (int)local_28 & 7U) == 0) {
      local_18 = local_18 + 1;
      *local_18 = 0;
    }
    if (4 < local_2d) {
      *local_18 =
          (char)(1 << (7 - ((char)local_20 + (char)local_28 & 7U) & 0x1f)) |
          *local_18;
    }
    local_34 = local_34 + fVar1;
  }
  return dwOutLineByte;
}
//------------------------------End
//third_fn--------------------------------------------

//------------------------------start
//some_fn_write-------------------------------------
void copy_something_to_buf(char *src)

{
  int iVar1;
  int local_18;

  iVar1 = some_counter;
  some_counter = some_counter + 1;
  if (src == (char *)0x0) {
    local_18 = iVar1 + -1;
    if (local_18 < 0) {
      local_18 = extra_bytes + -1;
    }
    memcpy((&some_buf)[iVar1], (&some_buf)[local_18], dwInLinePixCnt);
  } else {
    memcpy((&some_buf)[iVar1], src, dwInLinePixCnt);
  }
  counter_3 = counter_3 + 1;
  if (counter_3 == 1) {
    memcpy(*(void **)(&counter_2 + (long)extra_bytes * 2), src, dwInLinePixCnt);
  }
  if (extra_bytes <= some_counter) {
    some_counter = 0;
  }
  return;
}

long FUN_00101cba(long buf, int *param_2)

{
  CHAR *pCVar1;
  long lVar2;
  float fVar3;
  float fVar4;
  int local_38;
  int i;
  float local_2c;
  long local_28;

  local_28 = 0;
  copy_something_to_buf(*(char **)(buf + 8));
  if (2 < counter_3) {
    fVar4 = (float)inResY;
    fVar3 = (float)outResY;
    counter_3 = 0;
    if ((int)(counter_3 - 3) % inResY == 0) {
      local_2c = 1.0;
    } else {
      for (local_2c = 0.0; local_2c < (float)((int)(counter_3 - 3) % inResY);
           local_2c = local_2c + fVar4 / fVar3) {
        counter_3 = counter_3 + 1;
      }
      local_2c = (local_2c + 1.0) - (float)((int)(counter_3 - 3) % inResY);
    }
    local_38 = 0;
    for (i = 0; i < outResY; i = i + 1) {
      if (((int)(counter_3 - 3) % inResY <= (i * inResY) / outResY) &&
          ((i * inResY) / outResY < (int)(counter_3 - 3) % inResY + 1)) {
        local_38 = local_38 + 1;
      }
    }
    i = 0;
    while (i < local_38) {
      pCVar1 = invert_buffer_offset(*(BOOL *)(buf + 0x28),
                                    *(CHAR **)(buf + 0x18), *param_2);
      lVar2 = (*third_fn)(local_2c, pCVar1);
      local_28 = local_28 + lVar2;
      i = i + 1;
      local_2c = local_2c + fVar4 / fVar3;
      counter_3 = counter_3 + 1;
      *param_2 = *param_2 + 1;
    }
  }
  return local_28;
}

long FUN_00101bd8(SCANDEC_WRITE *param_1, int *param_2)

{
  CHAR *pCVar1;
  long lVar2;
  long local_28;
  ushort flag;

  flag = counter_3;
  local_28 = 0;
  counter_3 = counter_3 + 1;
  if ((Y_scaling_factor != 5) || ((flag & 1) == 0)) {
    while (*param_2 < outResY) {
      pCVar1 = invert_buffer_offset(param_1->bReverWrite, param_1->pWriteBuff,
                                    *param_2);
      if (inResX == 1) {
        lVar2 = (*fourth_fn)(param_1->pLineData, param_1->dwLineDataSize,
                             pCVar1, outResX);
      } else {
        lVar2 = (*fourth_fn)(param_1->pLineData, param_1->dwLineDataSize,
                             pCVar1, inResX);
      }
      local_28 = local_28 + lVar2;
      *param_2 = *param_2 + 1;
    }
  }
  return local_28;
}

DWORD FUN_00104412(CHAR *param_1, DWORD param_2, CHAR *param_3, int param_4)

{
  ulong uVar1;
  ulong local_30;
  CHAR *local_20;
  char *local_10;

  uVar1 = dwOutLineByte / (ulong)(long)param_4;
  local_20 = param_3;
  local_10 = (char *)param_1;
  for (local_30 = 0; local_30 < uVar1; local_30 = local_30 + 1) {
    memset(local_20, (uint)*local_10, (long)param_4);
    local_10 = local_10 + 1;
    local_20 = local_20 + param_4;
  }
  if (dwOutLineByte % (ulong)(long)param_4 != 0) {
    memset(local_20, (uint)*local_10, dwOutLineByte % (ulong)(long)param_4);
  }
  return dwOutLineByte;
}

DWORD FUN_00104399(CHAR *param_1, DWORD param_2, CHAR *param_3, int param_4)

{
  ulong i;
  CHAR *local_20;
  CHAR *local_10;

  local_20 = param_3;
  local_10 = param_1;
  for (i = 0; (i < param_2 / (ulong)(long)param_4 && (i < dwOutLineByte));
       i = i + 1) {
    *local_20 = *local_10;
    local_20 = local_20 + 1;
    local_10 = local_10 + param_4;
  }
  return dwOutLineByte;
}
//--------------------------------------End fourth_fn impl---------------------------------------------

void FUN_001044ca(INT *param_1)

{
  int iVar1;
  ulong j;
  ulong i;
  int local_18;

  iVar1 = some_counter;
  some_counter = some_counter + 1;
  if (param_1 == (INT *)0x0) {
    local_18 = iVar1 + -1;
    if (local_18 < 0) {
      local_18 = extra_bytes + -1;
    }
    memcpy((&some_buf)[iVar1], (&some_buf)[local_18], dwInLinePixCnt);
  } else {
    for (i = 0; i < dwInLinePixCnt; i = i + 1) {
      for (j = 0; j < 8; j = j + 1) {
        if (((int)(uint) * (char *)(i + (long)param_1) >>
                 (7U - (char)j & 0x1f) &
             1U) == 0) {
          *(char *)((long)(&some_buf)[iVar1] + j + i * 8) = 0;
        } else {
          *(char *)((long)(&some_buf)[iVar1] + j + i * 8) = 10;
        }
      }
    }
  }
  counter_3 = counter_3 + 1;
  if (counter_3 == 1) {
    memcpy(*(void **)(&counter_2 + (long)extra_bytes * 2),param_1,dwInLinePixCnt);
  }
  if (extra_bytes <= some_counter) {
    some_counter = 0;
  }
  return;
}

long write_impl_2(INT *data_kind, int *ptr) {
  CHAR *pCVar1;
  long lVar2;
  float l_outResY;
  float l_inResY;
  int local_38;
  int local_34;
  float i;
  long local_28;

  local_28 = 0;
  FUN_001044ca(*(INT **)(data_kind + 2));
  if (2 < counter_3) {
    l_inResY = (float)inResY;
    l_outResY = (float)outResY;
    counter_3 = 0;
    if ((int)(counter_3 - 3) % inResY == 0) {
      i = 1.0;
    } else {
      for (i = 0.0; i < (float)((int)(counter_3 - 3) % inResY);
           i = i + l_inResY / l_outResY) {
        counter_3 = counter_3 + 1;
      }
      i = (i + 1.0) - (float)((int)(counter_3 - 3) % inResY);
    }
    local_38 = 0;
    for (local_34 = 0; local_34 < outResY; local_34 = local_34 + 1) {
      if (((int)(counter_3 - 3) % inResY <= (local_34 * inResY) / outResY) &&
          ((local_34 * inResY) / outResY < (int)(counter_3 - 3) % inResY + 1)) {
        local_38 = local_38 + 1;
      }
    }
    local_34 = 0;
    while (local_34 < local_38) {
      pCVar1 =
          invert_buffer_offset(data_kind[10], *(CHAR **)(data_kind + 6), *ptr);
      lVar2 = (*third_fn)(i, pCVar1);
      local_28 = local_28 + lVar2;
      local_34 = local_34 + 1;
      i = i + l_inResY / l_outResY;
      counter_3 = counter_3 + 1;
      *ptr = *ptr + 1;
    }
  }
  return local_28;
}


long write_impl_1(SCANDEC_WRITE *param_1,int *param_2)

{
  ushort uVar1;
  CHAR *pCVar2;
  long lVar3;
  long local_28;
  
  uVar1 = counter_3;
  local_28 = 0;
  counter_3 = counter_3 + 1;
  if ((Y_scaling_factor != 5) || ((uVar1 & 1) == 0)) {
    while (*param_2 < outResY) {
      pCVar2 = invert_buffer_offset(param_1->bReverWrite,param_1->pWriteBuff,*param_2);
      lVar3 = (*fifth_fn)(param_1->pLineData,param_1->dwLineDataSize,pCVar2);
      local_28 = local_28 + lVar3;
      *param_2 = *param_2 + 1;
    }
  }
  return local_28;
}

//-----------------------start fifth_fn impl-----------------------------------------------
DWORD FUN_00104153(CHAR *line_data,DWORD line_data_size,CHAR *buf)

{
  // ulong uVar1;
  // ulong local_28;
  // CHAR *local_20;
  // char *local_10;
  
  // uVar1 = dwOutLineByte / 3;
  // local_20 = buf;
  // local_10 = (char *)line_data;
  // for (local_28 = 0; local_28 < uVar1; local_28 = local_28 + 1) {
  //   *local_20 = (&DAT_002087a0)[*local_10];
  //   local_20[1] = (&DAT_002086a0)[*local_10];
  //   local_20[2] = (&DAT_002085a0)[*local_10];
  //   local_20 = local_20 + 3;
  //   local_10 = local_10 + 1;
  // }
  // uVar1 = dwOutLineByte % 3;
  // if (uVar1 != 0) {
  //   *local_20 = (&DAT_002087a0)[*local_10];
  //   if (1 < uVar1) {
  //     local_20[1] = (&DAT_002086a0)[*local_10];
  //   }
  // }
  return dwOutLineByte;
}


ulong FUN_001040d6(CHAR *param_1,DWORD param_2,CHAR *param_3)

{
  // CHAR *local_30;
  // ulong local_28;
  // char *local_10;
  
  // local_30 = param_3;
  // local_10 = (char *)param_1;
  // for (local_28 = 0; (local_28 < param_2 * 2 && (local_28 < dwOutLineByte)); local_28 = local_28 + 2
  //     ) {
  //   *local_30 = *(CHAR *)(&DAT_002083a0 + (ulong)*local_10 * 2);
  //   local_30 = local_30 + 1;
  //   local_10 = local_10 + 1;
  // }
  return dwOutLineByte;
}


ulong FUN_00104296(char *param_1,DWORD param_2,CHAR *param_3)
{
  // undefined2 uVar1;
  // undefined2 *local_30;
  // ulong local_28;
  // byte *local_10;
  
  // local_30 = (undefined2 *)param_3;
  // local_10 = (byte *)param_1;
  // for (local_28 = 0; (local_28 < param_2 << 2 && (local_28 < dwOutLineByte));
  //     local_28 = local_28 + 4) {
  //   uVar1 = *(undefined2 *)(&DAT_002083a0 + (ulong)*local_10 * 2);
  //   *local_30 = *(undefined2 *)(&DAT_002083a0 + (ulong)(byte)uVar1 * 2);
  //   local_30[1] = *(undefined2 *)(&DAT_002083a0 + (ulong)(byte)((ushort)uVar1 >> 8) * 2);
  //   local_30 = local_30 + 2;
  //   local_10 = local_10 + 1;
  // }
  return dwOutLineByte;
}


ulong FUN_0010404d(CHAR *param_1,DWORD param_2,char *param_3)

{
  ulong local_28;
  CHAR *local_20;
  CHAR *local_10;
  
  local_20 = param_3;
  local_10 = param_1;
  for (local_28 = 0; (local_28 < param_2 >> 1 && (local_28 < dwOutLineByte));
      local_28 = local_28 + 1) {
    //*local_20 = (&DAT_002082a0)[local_10[1]] | (&DAT_002082a0)[*local_10] << 4;
    local_20 = local_20 + 1;
    local_10 = local_10 + 2;
  }
  return local_28;
}

ulong FUN_00104365(CHAR *param_1,DWORD param_2,CHAR *param_3)

{
  memcpy(param_3,param_1,dwOutLineByte);
  return dwOutLineByte & 0xffff;
}



int assign_global_fn_pointers(int *assigned_type) {
  if ((inResoX == outResoX) && (inResoY == outResoY)) {
    if (nOutDatKind == 1) {
      some_fn_write = write_sameRes_kind1;
    } else if (nOutDatKind == 3) {
      some_fn_write = write_sameRes_kind3;
    } else {
      some_fn_write = copy_buffer_and_decomp;
    }
    *assigned_type = 1;
  } else if (((uint)nColorType >> 8 & 1) == 0) {
    if (((uint)nColorType >> 9 & 1) == 0) {
      if (outResY * 2 < inResY) {
        return 0;
      }
      some_fn_write = FUN_00101eae;
      if (outResX < inResX) {
        if (nOutDatKind == 1) {
          third_fn = FUN_00103ccf;
        } else if (nOutDatKind == 3) {
          third_fn = FUN_00103de8;
        } else {
          third_fn = FUN_00103f01;
        }
        *assigned_type = 4;
      } else {
        if (nOutDatKind == 1) {
          third_fn = FUN_00103059;
        } else if (nOutDatKind == 3) {
          third_fn = FUN_001033a4;
        } else {
          third_fn = FUN_001036ef;
        }
        *assigned_type = 3;
      }
    } else if (((uint)nColorType >> 4 & 1) == 0) {
      if (outResY * 2 < inResY) {
        return 0;
      }
      some_fn_write = FUN_00101cba;
      if (outResX < inResX) {
        third_fn = FUN_00103bca;
        *assigned_type = 4;
      } else {
        third_fn = FUN_00102d59;
        *assigned_type = 3;
      }
    } else {
      if ((X_scaling_factor == 0) || (Y_scaling_factor == 0)) {
        return 0;
      }
      some_fn_write = FUN_00101bd8;
      *assigned_type = 2;
      if (X_scaling_factor - 2U < 3) {
        fourth_fn = FUN_00104412;
      } else {
        fourth_fn = FUN_00104399;
      }
    }
  } else if ((X_scaling_factor == 0) || (Y_scaling_factor == 0)) {
    if (outResY * 2 < inResY) {
      return 0;
    }
    some_fn_write = write_impl_2;
    if (outResX < inResX) {
      third_fn = FUN_00103a7b;
      *assigned_type = 4;
    } else {
      third_fn = FUN_00102944;
      *assigned_type = 3;
    }
  } else {
    some_fn_write = write_impl_1;
    *assigned_type = 2;
    if (X_scaling_factor == 3) {
      fifth_fn = FUN_00104153;
    } else {
      if (X_scaling_factor < 4) {
        if (X_scaling_factor == 2) {
          fifth_fn = FUN_001040d6;
          return 1;
        }
      } else {
        if (X_scaling_factor == 4) {
          fifth_fn = FUN_00104296;
          return 1;
        }
        if (X_scaling_factor == 5) {
          fifth_fn = FUN_0010404d;
          return 1;
        }
      }
      fifth_fn = FUN_00104365;
    }
  }
  return 1;
}

void set_scaling_factor(int inRes, int outRes, int *scaling_factor)

{
  float ratio;

  ratio = (float)outRes / (float)inRes;
  if (ratio == 1.0) {
    *scaling_factor = 1;
  } else if (ratio == 2.0) {
    *scaling_factor = 2;
  } else if (ratio == 3.0) {
    *scaling_factor = 3;
  } else if (ratio == 4.0) {
    *scaling_factor = 4;
  } else if (ratio == 0.5) {
    *scaling_factor = 5;
  } else {
    *scaling_factor = 0;
  }
  return;
}

void find_int_ratio(int inResX, int outResX, int *global_in_res_X,
                    int *global_out_res_x) {
  int intRatio;
  float resXratio;
  int i;

  i = 1;
  while (1) {
    if (99 < i) {
      *global_in_res_X = inResX;
      *global_out_res_x = outResX;
      return;
    }
    resXratio = ((float)i * (float)outResX) / (float)inResX;
    intRatio = (int)resXratio;
    if ((float)intRatio == resXratio)
      break;
    i = i + 1;
  }
  *global_in_res_X = i;
  *global_out_res_x = intRatio;
  return;
}

float FUN_00105413(float param_1)

{
  float local_14;
  float local_10;
  
  local_10 = param_1;
  if (param_1 < 0.0) {
    local_10 = local_10 * -1.0;
  }
  if ((local_10 < 0.0) || (1.0 <= local_10)) {
    if ((local_10 < 1.0) || (2.0 <= local_10)) {
      local_14 = 0.0;
    }
    else {
      local_14 = ((4.0 - local_10 * 8.0) + local_10 * 5.0 * local_10) -
                 local_10 * local_10 * local_10;
    }
  }
  else {
    local_14 = local_10 * local_10 * local_10 + (1.0 - (local_10 + local_10) * local_10);
  }
  return local_14;
}


static float FLOAT_ARRAY_002089c0[32];
static float FLOAT_ARRAY_00208c60[32];
int check_res_ratios(void)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  int i;
  
  fVar5 = (float)inResX;
  fVar1 = (float)outResX;
  fVar6 = (float)inResY;
  fVar2 = (float)outResY;
  for (i = 0; i < outResX; i = i + 1) {
    fVar3 = (float)i * (fVar5 / fVar1);
    fVar3 = fVar3 - (float)(int)fVar3;
    if (31 < i) {
      return 0;
    }
    FLOAT_ARRAY_002089c0[(long)i * 5] = fVar3;
    fVar4 = FUN_00105413(fVar3 + 1.0);
    FLOAT_ARRAY_002089c0[(long)i * 5 + 1] = fVar4;
    fVar4 = FUN_00105413(fVar3);
    FLOAT_ARRAY_002089c0[(long)i * 5 + 2] = fVar4;
    fVar4 = FUN_00105413(1.0 - fVar3);
    FLOAT_ARRAY_002089c0[(long)i * 5 + 3] = fVar4;
    fVar3 = FUN_00105413(2.0 - fVar3);
    FLOAT_ARRAY_002089c0[(long)i * 5 + 4] = fVar3;
  }
  i = 0;
  while( 1 ) {
    if (outResY <= i) {
      return 1;
    }
    fVar1 = (float)i * (fVar6 / fVar2);
    fVar1 = fVar1 - (float)(int)fVar1;
    if (0x1f < i) break;
    FLOAT_ARRAY_00208c60[(long)i * 5] = fVar1;
    fVar5 = FUN_00105413(fVar1 + 1.0);
    FLOAT_ARRAY_00208c60[(long)i * 5 + 1] = fVar5;
    fVar5 = FUN_00105413(fVar1);
    FLOAT_ARRAY_00208c60[(long)i * 5 + 2] = fVar5;
    fVar5 = FUN_00105413(1.0 - fVar1);
    FLOAT_ARRAY_00208c60[(long)i * 5 + 3] = fVar5;
    fVar1 = FUN_00105413(2.0 - fVar1);
    FLOAT_ARRAY_00208c60[(long)i * 5 + 4] = fVar1;
    i = i + 1;
  }
  return 0;
}

BOOL ChangeResoInit(SCANDEC_OPEN *param_1)

{
  int rc;
  BOOL return_val;
  DWORD local_20;
  int assigned_fn_type;
  SCANDEC_OPEN *local_scandec_open;

  RESO_BUFFR = (void *)0x0;
  RESO_BUFFER_2 = 0;
  extra_bytes = 0;
  local_scandec_open = param_1;
  find_int_ratio(param_1->nInResoX, param_1->nOutResoX, &inResX, &outResX);
  find_int_ratio(local_scandec_open->nInResoY, local_scandec_open->nOutResoY,
                 &inResY, &outResY);
  set_scaling_factor(inResX, outResX, &X_scaling_factor);
  set_scaling_factor(inResY, outResY, &Y_scaling_factor);
  if (local_scandec_open->bLongBoundary == 0) {
    if (((unsigned int)local_scandec_open->nColorType >> 10 & 1) == 0) {
      if (((unsigned int)local_scandec_open->nColorType >> 9 & 1) == 0) {
        dwInLinePixCnt = (local_scandec_open->dwInLinePixCnt + 7) >> 3;
        dwOutLineByte =
            ((long)outResX * dwInLinePixCnt) / (unsigned long)(long)inResX;
        local_scandec_open->dwOutLineByte = dwOutLineByte;
        local_scandec_open->dwOutLinePixCnt = dwOutLineByte << 3;
      } else {
        dwInLinePixCnt = local_scandec_open->dwInLinePixCnt;
        dwOutLineByte =
            ((long)outResX * dwInLinePixCnt) / (unsigned long)(long)inResX;
        local_scandec_open->dwOutLineByte = dwOutLineByte;
        local_scandec_open->dwOutLinePixCnt = dwOutLineByte;
      }
    } else {
      dwInLinePixCnt = local_scandec_open->dwInLinePixCnt * 3;
      dwOutLineByte =
          ((long)outResX * dwInLinePixCnt) / (unsigned long)(long)inResX;
      local_scandec_open->dwOutLineByte = dwOutLineByte;
      local_scandec_open->dwOutLinePixCnt = dwOutLineByte / 3;
    }
  } else if (((unsigned int)local_scandec_open->nColorType >> 10 & 1) == 0) {
    if (((unsigned int)local_scandec_open->nColorType >> 9 & 1) == 0) {
      dwOutLineByte =
          (((local_scandec_open->dwInLinePixCnt + 7) >> 3) * (long)outResX) /
              (unsigned long)(long)inResX &
          0xfffffffffffffffc;
      dwInLinePixCnt =
          ((long)inResX * dwOutLineByte) / (unsigned long)(long)outResX;
      local_scandec_open->dwOutLineByte = dwOutLineByte;
      local_scandec_open->dwOutLinePixCnt = dwOutLineByte << 3;
    } else {
      dwOutLineByte = (local_scandec_open->dwInLinePixCnt * (long)outResX) /
                          (unsigned long)(long)inResX &
                      0xfffffffffffffffc;
      dwInLinePixCnt =
          ((long)inResX * dwOutLineByte) / (unsigned long)(long)outResX;
      local_scandec_open->dwOutLineByte = dwOutLineByte;
      local_scandec_open->dwOutLinePixCnt = dwOutLineByte;
    }
  } else {
    dwOutLineByte = ((((long)outResX * local_scandec_open->dwInLinePixCnt * 3) /
                      (unsigned long)(long)inResX) /
                     12) *
                    12;
    dwInLinePixCnt =
        ((long)inResX * dwOutLineByte) / (unsigned long)(long)outResX;
    local_scandec_open->dwOutLineByte = dwOutLineByte;
    local_scandec_open->dwOutLinePixCnt = dwOutLineByte / 3;
  }
  inResoX = local_scandec_open->nInResoX;
  // unique0x0000c304 = local_scandec_open->nInResoY;
  outResoX = local_scandec_open->nOutResoX;
  // unique0x0000c304 = local_scandec_open->nOutResoY;
  nColorType = local_scandec_open->nColorType;
  _dwInLinePixCnt = local_scandec_open->dwInLinePixCnt;
  nOutDatKind = local_scandec_open->nOutDataKind;
  // unique0x0000c304 = local_scandec_open->bLongBoundary;
  dwOutLinePixCnt = local_scandec_open->dwOutLinePixCnt;
  _dwOutLineByte = local_scandec_open->dwOutLineByte;
  _dwOutWriteMaxSize = local_scandec_open->dwOutWriteMaxSize;
  rc = assign_global_fn_pointers(&assigned_fn_type);
  if (rc == 0) {
    return_val = 0;
  } else {
    local_20 = (long)((outResY + inResY + -1) / inResY) * dwOutLineByte;
    if ((assigned_fn_type == 3) || (assigned_fn_type == 4)) {
      local_20 = local_20 * 2;
      extra_bytes = 4;
      if (((unsigned int)local_scandec_open->nColorType >> 8 & 1) == 0) {
        dwInLinePixCnt = dwInLinePixCnt;
      } else {
        dwInLinePixCnt = local_scandec_open->dwInLinePixCnt;
      }
    }
    if ((assigned_fn_type == 3) && (rc = check_res_ratios(), rc == 0)) {
      return_val = 0;
    } else if ((extra_bytes == 0) ||
               (RESO_BUFFR = malloc((long)extra_bytes * dwInLinePixCnt),
                RESO_BUFFR !=
                    (void *)0x0)) // bugchk_malloc((long)extra_bytes *
                                  // dwInLinePixCnt, 0xf2, "brother_chreso.c")
    {
      local_scandec_open->dwOutWriteMaxSize = local_20;
      inResoX = local_scandec_open->nInResoX;
      unique0x0000c304 = local_scandec_open->nInResoY;
      outResoX = local_scandec_open->nOutResoX;
      unique0x0000c304 = local_scandec_open->nOutResoY;
      nColorType = local_scandec_open->nColorType;
      _dwInLinePixCnt = local_scandec_open->dwInLinePixCnt;
      nOutDatKind = local_scandec_open->nOutDataKind;
      unique0x0000c304 = local_scandec_open->bLongBoundary;
      dwOutLinePixCnt = local_scandec_open->dwOutLinePixCnt;
      _dwOutLineByte = local_scandec_open->dwOutLineByte;
      _dwOutWriteMaxSize = local_scandec_open->dwOutWriteMaxSize;
      return_val = 1;
    } else {
      return_val = 0;
    }
  }
  return return_val;
}

char *set_buffer_and_do_something(SCANDEC_WRITE *param_1, size_t *param_2)

{
  size_t sVar1;
  char *pbVar2;
  char bVar3;
  void *pvVar4;
  size_t local_48;
  size_t local_40;
  char local_32;
  char *local_30;
  char *local_pix_buf_2;
  char *local_20;

  local_20 = (char *)param_1->pLineData;
  local_40 = param_1->dwLineDataSize;
  *param_2 = 0;
  pvVar4 = pix_buf_2;
  if (param_1->nInDataComp == 1) {
    memset(pix_buf_2, 0, InLinePixelCnt);
    local_pix_buf_2 = (char *)pix_buf_2;
    *param_2 = InLinePixelCnt;
  } else if (param_1->nInDataComp == 3) {
    local_30 = (char *)pix_buf_2;
    local_pix_buf_2 = (char *)pix_buf_2;
    local_48 = InLinePixelCnt;
    if (some_flag_1 == 0) {
      do {
        if (local_48 == 0) {
          return (char *)pvVar4;
        }
        sVar1 = local_40 - 1;
        if (sVar1 == 0) {
          return (char *)pvVar4;
        }
        bVar3 = *local_20;
        pbVar2 = local_20 + 1;
        if ((char)bVar3 < '\0') {
          if (bVar3 != 0x80) {
            local_32 = '\x01' - bVar3;
            bVar3 = *pbVar2;
            while ((local_32 != '\0' && (local_48 != 0))) {
              *local_30 = bVar3;
              local_30 = local_30 + 1;
              local_32 = local_32 + -1;
              local_48 = local_48 - 1;
              *param_2 = *param_2 + 1;
            }
            sVar1 = local_40 - 2;
            pbVar2 = local_20 + 2;
          }
        } else {
          local_32 = bVar3 + 1;
          local_40 = sVar1;
          local_20 = pbVar2;
          while (((sVar1 = local_40, pbVar2 = local_20,
                   local_32 != '\0' && (local_48 != 0)) &&
                  (local_40 != 0))) {
            *local_30 = *local_20;
            local_30 = local_30 + 1;
            local_32 = local_32 + -1;
            local_48 = local_48 - 1;
            local_40 = local_40 - 1;
            *param_2 = *param_2 + 1;
            local_20 = local_20 + 1;
          }
        }
        local_20 = pbVar2;
        local_40 = sVar1;
      } while (local_40 != 0);
    } else {
      do {
        if (local_48 == 0) {
          return (char *)pvVar4;
        }
        sVar1 = local_40 - 1;
        if (sVar1 == 0) {
          return (char *)pvVar4;
        }
        bVar3 = *local_20;
        pbVar2 = local_20 + 1;
        if ((char)bVar3 < '\0') {
          if (bVar3 != 0x80) {
            local_32 = '\x01' - bVar3;
            bVar3 = *pbVar2;
            while ((local_32 != '\0' && (local_48 != 0))) {
              *local_30 = *(char *)((unsigned long)bVar3 + some_flag_1);
              local_30 = local_30 + 1;
              local_32 = local_32 + -1;
              local_48 = local_48 - 1;
              *param_2 = *param_2 + 1;
            }
            sVar1 = local_40 - 2;
            pbVar2 = local_20 + 2;
          }
        } else {
          local_32 = bVar3 + 1;
          local_40 = sVar1;
          local_20 = pbVar2;
          while (((sVar1 = local_40, pbVar2 = local_20,
                   local_32 != '\0' && (local_48 != 0)) &&
                  (local_40 != 0))) {
            bVar3 = *local_20;
            local_20 = local_20 + 1;
            *local_30 = *(char *)((unsigned long)bVar3 + some_flag_1);
            local_30 = local_30 + 1;
            local_32 = local_32 + -1;
            local_48 = local_48 - 1;
            local_40 = local_40 - 1;
            *param_2 = *param_2 + 1;
          }
        }
        local_20 = pbVar2;
        local_40 = sVar1;
      } while (local_40 != 0);
    }
  } else if (some_flag_1 == 0) {
    *param_2 = local_40;
    local_pix_buf_2 = local_20;
  } else {
    local_30 = (char *)pix_buf_2;
    local_pix_buf_2 = (char *)pix_buf_2;
    local_48 = InLinePixelCnt;
    while ((local_48 != 0 && (local_40 != 0))) {
      bVar3 = *local_20;
      local_20 = local_20 + 1;
      *local_30 = *(char *)((unsigned long)bVar3 + some_flag_1);
      local_30 = local_30 + 1;
      local_48 = local_48 - 1;
      local_40 = local_40 - 1;
      *param_2 = *param_2 + 1;
    }
  }
  return local_pix_buf_2;
}

BOOL ScanDecOpen(SCANDEC_OPEN *scandec_open) {
  BOOL BVar1;
  BOOL ret_val;
  SCANDEC_OPEN scandec_open_2;
  SCANDEC_OPEN *scandec_open_local;

  if ((scandec_open->nColorType >> 8 & 1) == 0) {
    if ((scandec_open->nColorType >> 9 & 1) == 0) {
      InLinePixelCnt = scandec_open->dwInLinePixCnt * 3;
    } else {
      InLinePixelCnt = scandec_open->dwInLinePixCnt;
    }
  } else {
    InLinePixelCnt = scandec_open->dwInLinePixCnt;
  }
  scandec_open_local = scandec_open;
  pix_buf = malloc(
      InLinePixelCnt); // bugchk_malloc(InLinePixelCnt,69,"brother_scandec.c");
  if (pix_buf == (void *)0x0) {
    ret_val = 0;
  } else {
    scandec_open_2.nInResoX = scandec_open_local->nInResoX;
    scandec_open_2.nInResoY = scandec_open_local->nInResoY;
    scandec_open_2.nOutResoX = scandec_open_local->nOutResoX;
    scandec_open_2.nOutResoY = scandec_open_local->nOutResoY;
    scandec_open_2.nColorType = scandec_open_local->nColorType;
    scandec_open_2.dwInLinePixCnt = scandec_open_local->dwInLinePixCnt;
    scandec_open_2.nOutDataKind = scandec_open_local->nOutDataKind;
    scandec_open_2.bLongBoundary = scandec_open_local->bLongBoundary;
    BVar1 = ChangeResoInit(&scandec_open_2);
    if (BVar1 == 0) {
      free(pix_buf); // bugchk_free(pix_buf,0x59,-0x70);
      pix_buf = (void *)0x0;
      ret_val = 0;
    } else {
      scandec_open_local->dwOutLinePixCnt = scandec_open_2.dwOutLinePixCnt;
      scandec_open_local->dwOutLineByte = scandec_open_2.dwOutLineByte;
      scandec_open_local->dwOutWriteMaxSize = scandec_open_2.dwOutWriteMaxSize;
      HANDLE_1 = (HANDLE)0x0;
      some_flag_1 = 0;
      HANDLE_2 = (HANDLE)0x0;
      some_flag_2 = 0;
      some_function = set_buffer_and_do_something;
      ret_val = 1;
    }
  }
  return ret_val;
}
