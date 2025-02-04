#include "brother_dtype.h"
#include "brother_scandec.h"
#include "brother_bugchk.h"



static DWORD InLinePixelCnt;
static void* pix_buf;
static HANDLE HANDLE_1;
static HANDLE HANDLE_2;
static int some_flag_1;
static int some_flag_2;
static void* some_function;
BOOL ScanDecOpen(SCANDEC_OPEN *scandec_open)
{
  BOOL BVar1;
  BOOL ret_val;
  SCANDEC_OPEN scandec_open_2;
  SCANDEC_OPEN *scandec_open_local;
  
  if ((scandec_open->nColorType >> 8 & 1) == 0) {
    if ((scandec_open->nColorType >> 9 & 1) == 0) {
      InLinePixelCnt = scandec_open->dwInLinePixCnt * 3;
    }
    else {
      InLinePixelCnt = scandec_open->dwInLinePixCnt;
    }
  }
  else {
    InLinePixelCnt = scandec_open->dwInLinePixCnt;
  }
  scandec_open_local = scandec_open;
  pix_buf = bugchk_malloc(InLinePixelCnt,69,"brother_scandec.c");
  if (pix_buf == (void *)0x0) {
    ret_val = 0;
  }
  else {
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
      bugchk_free(pix_buf,0x59,-0x70);
      pix_buf = (void *)0x0;
      ret_val = 0;
    }
    else {
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

static void* pix_buf_2;
char* set_buffer_and_do_something(SCANDEC_WRITE *param_1,size_t *param_2)

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
  
  local_20 = (char*)param_1->pLineData;
  local_40 = param_1->dwLineDataSize;
  *param_2 = 0;
  pvVar4 = pix_buf_2;
  if (param_1->nInDataComp == 1) {
    memset(pix_buf_2,0,InLinePixelCnt);
    local_pix_buf_2 = (char *)pix_buf_2;
    *param_2 = InLinePixelCnt;
  }
  else if (param_1->nInDataComp == 3) {
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
        }
        else {
          local_32 = bVar3 + 1;
          local_40 = sVar1;
          local_20 = pbVar2;
          while (((sVar1 = local_40, pbVar2 = local_20, local_32 != '\0' && (local_48 != 0)) &&
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
    }
    else {
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
        }
        else {
          local_32 = bVar3 + 1;
          local_40 = sVar1;
          local_20 = pbVar2;
          while (((sVar1 = local_40, pbVar2 = local_20, local_32 != '\0' && (local_48 != 0)) &&
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
  }
  else if (some_flag_1 == 0) {
    *param_2 = local_40;
    local_pix_buf_2 = local_20;
  }
  else {
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