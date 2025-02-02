/*

 This file is part of the Brother MFC/DCP backend for SANE.

 This program is free software; you can redistribute it and/or modify it
 under the terms of the GNU General Public License as published by the Free
 Software Foundation; either version 2 of the License, or (at your option)
 any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 more details.

 You should have received a copy of the GNU General Public License along with
 this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 Place, Suite 330, Boston, MA  02111-1307  USA

*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//	Source filename: brother_log.h
//
//	Copyright(c) 1997-2000 Brother Industries, Ltd.  All Rights Reserved.
//
//
//	Abstract:
//			���ե���������⥸�塼�롦�إå���
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#ifndef _BROTHER_LOG_H_
#define _BROTHER_LOG_H_

#include "brother.h"

#define LOGFILENAME  "/BrMfc32.log"

#define LOGSTRMAXLEN  1024

//
// �ؿ��Υץ�ȥ��������
//
void  WriteLogFileString( LPSTR lpszLogStr );
void  WriteLog( LPSTR first, ... );
void  WriteLogScanCmd( LPSTR lpszId, LPSTR lpszCmd );
void  CloseLogFile( void );
void  OpenRawData( void );
void  CloseRawData( void );
void  SaveRawData( BYTE *lpRawData, int nDataSize );
void  GetLogSwitch( Brother_Scanner *this );

#endif //_BROTHER_LOG_H_

//////// end of brother_log.h ////////
