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

//M-LNX-38  cannot scan with scanimage/scanadf


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//	Source filename: brother_devaccs.c
//
//		Copyright(c) 1997-2000 Brother Industries, Ltd.  All Rights Reserved.
//
//
//	Abstract:
//			Module to access the MFC
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>

#include <usb.h>

#include "brother.h"

#include "brother_mfccmd.h"
#include "brother_misc.h"
#include "brother_log.h"
#include "brother_bugchk.h"

#include "brother_devaccs.h"
//
// buffer size to transfer
//
WORD  gwInBuffSize;

//
// time out value of device accsess
//
UINT  gnQueryTimeout;	// time out value of query command
UINT  gnScanTimeout;	// time out of scan start / timeout of scan

//
// handle of the buffer to receive
//
static HANDLE  hReceiveBuffer  = NULL;

BOOL timeout_flg;

static int iReadStatus;			// the status when ZERO is read
static struct timeval save_tv;		// the valiables of the information of time (sec,msec)
static struct timezone save_tz;		// the valiables of the information of time (min)

#define   TIMEOUTREADWRITE   2

int init_usb_criticalsection(void);
int discard_usb_criticalsection(void);
int enter_usb_criticalsection(void);
int release_usb_criticalsection(void);

#define SKEY_USBSEM

//-----------------------------------------------------------------------------
//
//	Function name:	GetDeviceAccessParam
//
//
//	Abstract:
//		Set the parameters to access the device
//
//
//	Parameters:
//		None
//
//
//	Return values:
//		None
//
//-----------------------------------------------------------------------------
//
void
GetDeviceAccessParam( Brother_Scanner *this )
{
	//
	// Get the transfer buffer size
	//
	gwInBuffSize  = this->modelConfig.wInBuffSize;

	//
	// Get the timeout value if "Query" commands
	//
	gnQueryTimeout = TIMEOUT_QUERYRES;

	//
	// Get the the valiables of scan
	//
	gnScanTimeout  = TIMEOUT_SCANNING * 1000;
}

//-----------------------------------------------------------------------------
//
//	Function name:	OpenDevice
//
//
//	Abstract:
//		Open the device
//
//
//	Parameters:
//              None
//
//	Return values:
//		TRUE  success (normal end)
//		FALSE error
//
//-----------------------------------------------------------------------------
//
int
OpenDevice(usb_dev_handle *hScanner, int seriesNo)
{
	int rc, nValue;
	int i;
	int nEndPoint;
	char data[BREQ_GET_LENGTH];

	rc = 0;
	if (IFTYPE_NET == hScanner->device){
	  //int scan_socket = -1;
	  if ((hScanner->net = open_device_net(hScanner->net_device_index,
					       NULL,ADRTYPE_DEPENDONINI)) == NULL ){
#if 0   //M-LNX-38  cannot scan with scanimage/scanadf
	    WriteLog("OpenDevice  ERROR at open_device_net (%x)",
		     hScanner->net);
	    return FALSE;
#else   //M-LNX-38  cannot scan with scanimage/scanadf
	    WriteLog("OpenDevice  ERROR at open_device_net (%x) retry 1",
		     hScanner->net);
	    usleep(300 * 1000); // wait for  300ms
	    if ((hScanner->net = open_device_net(hScanner->net_device_index,
					       NULL,ADRTYPE_DEPENDONINI)) == NULL ){
	      int retry;
	      WriteLog("OpenDevice  ERROR at open_device_net (%x) retry 2",
		       hScanner->net);
	      usleep(700 * 1000); // wait for  700ms
	      retry = 0;
	      while((hScanner->net = open_device_net(hScanner->net_device_index,
					       NULL,ADRTYPE_DEPENDONINI)) == NULL ){
		WriteLog("OpenDevice  ERROR at open_device_net (%x) give up ",
			 hScanner->net);
		retry ++;
		if(retry > 16)return FALSE;
		usleep(1000*1000);
	      }
	    }
#endif   //M-LNX-38  cannot scan with scanimage/scanadf
	  }
	  nEndPoint = 0;    //not be used
	  goto OPEN_POST_PROC;
	  //return TRUE;
	}
	//2005/11/11 Add SeriesNnumber information for L4CFB and later
	nEndPoint = hScanner->usb_r_ep;
	//printf("OpenDevice : endpoint %x\n",nEndPoint);
	if(nEndPoint <  0x80 || nEndPoint > 0xff){
	  nEndPoint = 0x84;
	  for(i=0; i < ANOTHERENDPOINT; i++){
	    if(seriesNo == ChangeEndpoint[i]){
	      nEndPoint = 0x85;
	      break;
	    }
	  }
	}
	//printf("OpenDevice : endpoint %x\n",nEndPoint);

	WriteLog( "Set EndPoint = %d", nEndPoint) ;
	  //if(seriesNo == L4CFB ||seriesNo == AL_FB_DCP )
	  //nEndPoint = 0x85;
	  //else
	  //nEndPoint = 0x84;

	WriteLog( "<<< OpenDevice start <<<\n" );
#ifdef SKEY_USBSEM
	init_usb_criticalsection();
	enter_usb_criticalsection();
#endif
	for (i = 0; i < RETRY_CNT;i++) {
	    rc = usb_control_msg(hScanner->usb,       /* handle */
				 BREQ_TYPE,           /* request type */
				 BREQ_GET_OPEN,  /* request */   /* GET_OPEN */
				 BCOMMAND_SCANNER,/* value */    /* scanner */
				 0,              /* index */
				 data, BREQ_GET_LENGTH,   /* bytes, size */
				 2000            /* Timeout */
		);
		if (rc >= 0) {
				break;
		}
	}
	if (rc < 0)
		return FALSE;

	// check the size of discriptor
	nValue = (int) data[0];
	if (nValue != BREQ_GET_LENGTH)
		return FALSE;

	// check the type of discriptor
	nValue = (int)data[1];
	if (nValue != BDESC_TYPE)
		return FALSE;

	// check the command ID
	nValue = (int)data[2];
	if (nValue != BREQ_GET_OPEN)
		return FALSE;

	// check the command parameters
	nValue = (int)*((WORD *)&data[3]);
	if (nValue & BCOMMAND_RETURN)
		return FALSE;

	if (nValue != BCOMMAND_SCANNER)
		return FALSE;

	OPEN_POST_PROC:

	// recovery
	{
	CHAR *lpBrBuff;
	int nResultSize;
	int iFirstData = 1;

	lpBrBuff = MALLOC( 32000 );
	if (!lpBrBuff)
		return FALSE;

	do {
		WriteLog( "OpenDevice Recovery Read start" );

		// discard the read data
		if (IFTYPE_NET == hScanner->device){
		  struct timeval net_timeout = NETTIMEOUTST;
		  // (sec, micro sec)
		  nResultSize = 0;
		  read_device_net(hScanner->net,
				  lpBrBuff,
				  32000,
				  &nResultSize,
				  &net_timeout);

		}
		else{
		  nResultSize = usb_bulk_read(hScanner->usb,
						  nEndPoint,
						  lpBrBuff,
						  32000,
						  2000
						  );
		}

		WriteLog( "TEST OpenDevice Recovery Read end nResultSize = %d %d", nResultSize,nEndPoint) ;

		// reset the timer if the data exists
		if (nResultSize > 0) { // the case the data exists

			// send the Q command at the first time
			if (iFirstData){
				WriteLog( "OpenDevice Recovery Q Command" );
				// send the Q command
				WriteDeviceData( hScanner, MFCMD_QUERYDEVINFO, strlen( MFCMD_QUERYDEVINFO ), seriesNo );
				iFirstData = 0;
			}
		}
		usleep(30 * 1000); // wait for  30ms
	} while (nResultSize > 0);
	FREE(lpBrBuff);

	} //  end of recovery proccess
	WriteLog("OpenDevice return");
	return TRUE;
}


//-----------------------------------------------------------------------------
//
//	Function name:	CloseDevice
//
//
//	Abstract:
//		Close the device
//
//
//	Parameters:
//		Nome
//
//
//	Return values:
//		Nome
//
//-----------------------------------------------------------------------------
//
void
CloseDevice( usb_dev_handle *hScanner )
{
    int rc;
    int i;
    char data[BREQ_GET_LENGTH];

    if (IFTYPE_NET == hScanner->device){
	close_device_net(hScanner->net);
	return ;
    }
    for (i = 0; i < RETRY_CNT;i++) {
	rc = usb_control_msg(hScanner->usb,       /* handle */
			     BREQ_TYPE,           /* request type */
			     BREQ_GET_CLOSE,  /* request */    /* GET_OPEN */
			     BCOMMAND_SCANNER,/* value */      /* scanner  */
			     0,              /* index */
			     data, BREQ_GET_LENGTH,        /* bytes, size */
			     2000            /* Timeout */
	    );
	if (rc >= 0) break;
    }
#ifdef SKEY_USBSEM
    release_usb_criticalsection();
    discard_usb_criticalsection();
#endif
    return;
}


//-----------------------------------------------------------------------------
//
//	Function name:	ReadDeviceData
//
//
//	Abstract:
//		Read from device
//
//
//	Parameters:
//		lpRxBuffer
//			the pointer ti the read buffer
//
//		nReadSize
//			the size to read (read buffer size)
//
//
//	Return values:
//		0 >  this function ends successfully . the return value is the size of read data
//		0 <= this function failed , the return value is error code
//
//-----------------------------------------------------------------------------
//
int
ReadDeviceData( usb_dev_handle *hScanner, LPSTR lpRxBuffer, int nReadSize, int seriesNo )
{
	int  nResultSize = 0;
	int  nTimeOut = 20000;
	int  nEndPoint,i;

	WriteLog( "ReadDeviceData Start nReadSize =%d\n", nReadSize ) ;

	if (IFTYPE_NET != hScanner->device){
	  nEndPoint = hScanner->usb_r_ep;
	  //printf("ReadDeviceData : endpoint %x\n",nEndPoint);
	  if(nEndPoint <  0x80 || nEndPoint > 0xff){
	    nEndPoint = 0x84;
	    for(i=0; i < ANOTHERENDPOINT; i++){
	      if(seriesNo == ChangeEndpoint[i]){
		nEndPoint = 0x85;
		break;
	      }
	    }
	  }
	  /*
	      if(seriesNo == L4CFB || seriesNo == AL_FB_DCP )
	      nEndPoint = 0x85;
	      else
	      nEndPoint = 0x84;
	  */
	}
	else{
	  nEndPoint = 0;    //not be used
	}


	if (iReadStatus > 0) { // The case  the status of "zero byte read" is available
		struct timeval tv;
		struct timezone tz;
		long   nSec, nUsec;


		if (gettimeofday(&tv, &tz) == 0) {

			if (tv.tv_usec < save_tv.tv_usec) {
				tv.tv_usec += 1000 * 1000 ;
				tv.tv_sec-- ;
			}
			nUsec = tv.tv_usec - save_tv.tv_usec;
			nSec = tv.tv_sec - save_tv.tv_sec;

			WriteLog( "ReadDeviceData iReadStatus = %d nSec = %d Usec = %d\n",iReadStatus, nSec, nUsec ) ;

			if (iReadStatus == 1) { // wait for 1s if it is the first "zero byte read"
				if (nSec == 0) { // check the order of second, if it is the same the diference is less than 1s
					if (nUsec < 1000) // check whether the wait time is less then 1ms or not 
						usleep( 1000 - nUsec );
				}
			}
			else if (iReadStatus == 2) { // wait for 200ms if the 2nd or later "zero byte read"
				if (nSec == 0) { // check the order of second, if it is the same the diference is less than 1s
					if (nUsec < 200 * 1000) // check whether the wait time is less then 200ms or not 
						usleep( 200 * 1000 - nUsec );
				}

			}
		}
	}

	if (IFTYPE_NET != hScanner->device){
	  nResultSize = usb_bulk_read(hScanner->usb,
				      nEndPoint,
				      lpRxBuffer,
				      nReadSize,
				      nTimeOut
				      );
	}
	else{
	  struct timeval net_timeout = {nTimeOut/1000,(nTimeOut%1000)*1000};    // (sec, micro sec)
	  read_device_net(hScanner->net,
			  lpRxBuffer,
			  nReadSize,
			  &nResultSize,
			  &net_timeout);
	}

	WriteLog( " ReadDeviceData ReadEnd nResultSize = %d\n", nResultSize ) ;

	if (nResultSize == 0) {
		if (iReadStatus == 0) {
			iReadStatus = 1;
			gettimeofday(&save_tv, &save_tz);
		}
		else {
			iReadStatus = 2;
		}

	} else {
		iReadStatus = 0;
		return nResultSize;
	}

	return nResultSize;
}
//-----------------------------------------------------------------------------
//
//	Function name:	ReadNonFixedData
//
//
//	Abstract:
//		read from device (with timeout process)
//
//
//	Parameters:
//		lpBuffer
//			the pointer to the read buffer
//
//		wReadSize
//			the max size to read
//
//		dwTimeOut
//			timeout value (mS)
//
//
//	Return values:
//		0 >  the function end successfully , the return value is the read data size
//		0 =  time out occured
//		-1 = read error
//
//	Note:
//		the function terminates even if the data size is 1 byte.
//		the function terminate if no data is read in a interbal of timeout
//-----------------------------------------------------------------------------
//
int
ReadNonFixedData( usb_dev_handle *hScanner, LPSTR lpBuffer, WORD wReadSize, DWORD dwTimeOutMsec ,int seriesNo)
{
	int   nReadDataSize = 0;

	struct timeval start_tv, tv;
	struct timezone tz;
	long   nSec, nUsec;
	long   nTimeOutSec, nTimeOutUsec;

	iReadStatus = 0;

	if (gettimeofday(&start_tv, &tz) == -1)
		return FALSE;

	// calculate the scond-order of time out value
	nTimeOutSec = dwTimeOutMsec / 1000;
	// calculate the micro-seconds-order of time out value
	nTimeOutUsec = (dwTimeOutMsec - (1000 * nTimeOutSec)) * 1000;

	while(1){

		if (gettimeofday(&tv, &tz) == 0) {
			if (tv.tv_usec < start_tv.tv_usec) {
				tv.tv_usec += 1000 * 1000 ;
				tv.tv_sec-- ;
			}
			nSec = tv.tv_sec - start_tv.tv_sec;
			nUsec = tv.tv_usec - start_tv.tv_usec;

			if (nSec > nTimeOutSec) { // break if nSec is larger than timeout value
				break;
			}
			else if( nSec == nTimeOutSec) {      // if the second-order is same 
				if (nUsec >= nTimeOutUsec) { //   check the micro-sec-order
					break;
				}
			}
		}
		else {
			break;
		}

		//
		// read data
		//
		nReadDataSize = ReadDeviceData( hScanner, lpBuffer, wReadSize, seriesNo );
		if( nReadDataSize > 0 ){
			break;
		}
		else if (nReadDataSize < 0) {
			break;
		}

		usleep(20 * 1000); // wait for 20mS
	}

	return nReadDataSize;
}

//-----------------------------------------------------------------------------
//
//	Function name:	ReadFixedData
//
//
//	Abstract:
//		Read data on a specified size from device
//
//
//	Parameters:
//		lpBuffer
//			the pointer to the buffer the read data stored
//
//		wReadSize
//			read data size
//
//		dwTimeOut
//			timeout value (mS)
//
//
//	Return values:
//		TRUE  = Function end successfully
//		FALSE = timeout occured (FAILED)
//
//
//	Note:
//
//-----------------------------------------------------------------------------
//	ReadBidiFixedData�ʵ�ReadBidiComm32_q��
BOOL
ReadFixedData( usb_dev_handle *hScanner, LPSTR lpBuffer, WORD wReadSize, DWORD dwTimeOutMsec, int seriesNo )
{
	BOOL  bResult = TRUE;
	WORD  wReadCount = 0;
	int   nReadDataSize;

	struct timeval start_tv, tv;
	struct timezone tz;
	long   nSec, nUsec;
	long   nTimeOutSec, nTimeOutUsec;

	if (gettimeofday(&start_tv, &tz) == -1)
		return FALSE;

	// calculate the second-order of the timeout value
	nTimeOutSec = dwTimeOutMsec / 1000;
	// calculate the micro-second-order of the timeout value
	nTimeOutUsec = (dwTimeOutMsec - (1000 * nTimeOutSec)) * 1000;

	while( wReadCount < wReadSize ){

		if (gettimeofday(&tv, &tz) == 0) {
			if (tv.tv_usec < start_tv.tv_usec) {
				tv.tv_usec += 1000 * 1000 ;
				tv.tv_sec-- ;
			}
			nSec = tv.tv_sec - start_tv.tv_sec;
			nUsec = tv.tv_usec - start_tv.tv_usec;

			if (nSec > nTimeOutSec) { // break if nSec is larger than timeout value
				break;
			}
			else if( nSec == nTimeOutSec) {      // if the econd-order is same
				if (nUsec >= nTimeOutUsec) { //     check the micro-sec-order
					break;
				}
			}
		}
		else {
			bResult = FALSE;
		}

		//
		// read the data
		//
		nReadDataSize = ReadDeviceData( hScanner, &lpBuffer[ wReadCount ], wReadSize - wReadCount, seriesNo );
		if( nReadDataSize > 0 ){
			wReadCount += nReadDataSize;
		}

		if( wReadCount >= wReadSize ) break;	// terminate if reading data is completed

		usleep(20 * 1000); // 20ms�Ԥ�
	}

	return bResult;
}

//-----------------------------------------------------------------------------
//
//	Function name:	ReadDeviceCommand
//
//
//	Abstract:
//		read the command from the device
//
//
//	Parameters:
//		lpRxBuffer
//			the pointer to the read buffer
//
//		nReadSize
//			the size to read
//
//
//	Return values:
//		0 >  the function terminates successfully ,the return value is the read data size
//		0 <= the function is failed : the return value is error code
//
//-----------------------------------------------------------------------------
//
int
ReadDeviceCommand( usb_dev_handle *hScanner, LPSTR lpRxBuffer, int nReadSize, int seriesNo )
{
	int  nResultSize;


	nResultSize = ReadDeviceData( hScanner, lpRxBuffer, nReadSize, seriesNo );

	return nResultSize;
}


//-----------------------------------------------------------------------------
//
//	Function name:	WriteDeviceData
//
//
//	Abstract:
//		Write to the device
//
//
//	Parameters:
//		lpTxBuffer
//			the pointer to the data to write to the device
//
//		nWriteSize
//			size of data
//
//
//	Return values:
//		0 >  this function terminate successfully : the return value is size of data  written to the device (byte)
//		0 <= the function is failed : the return value is error code
//
//-----------------------------------------------------------------------------
//
int
WriteDeviceData( usb_dev_handle *hScanner, LPSTR lpTxBuffer, int nWriteSize, int seriesNo)
{
	int i;
	int  nResultSize = 0;
	int  nEndPoint;

	if (IFTYPE_NET == hScanner->device){
	  int iWritten,rc;
	  for (i = 0; i < RETRY_CNT;i++) {
	    struct timeval net_timeout = NETTIMEOUTST;
	    rc = write_device_net(hScanner->net,
			   lpTxBuffer,
			   nWriteSize ,
			   &iWritten,
			   &net_timeout);
	    if ( rc >= 0)
			break;
	  }
	  return iWritten;
	}
	//2005/11/11 Add SeriesNnumber information for L4CFB and later
	nEndPoint = hScanner->usb_w_ep;
	//printf("WriteDeviceData : endpoint %x\n",nEndPoint);
	if(nEndPoint <  0x1 || nEndPoint > 0x7f){
	  //2005/11/11 Add SeriesNnumber information for L4CFB and later
	  nEndPoint = 0x03;
	  for(i=0; i < ANOTHERENDPOINT; i++){
	    if(seriesNo == ChangeEndpoint[i]){
	      nEndPoint = 0x04;
	      break;
	    }
	  }
	}

	  /*
	if(seriesNo == L4CFB || seriesNo == AL_FB_DCP)
	  nEndPoint = 0x04;
	else
	  nEndPoint = 0x03;
	  */

	for (i = 0; i < RETRY_CNT;i++) {
	    nResultSize = usb_bulk_write(hScanner->usb,
					 nEndPoint,
					 lpTxBuffer,
					 nWriteSize,
					 2000);
	    if ( nResultSize >= 0) break;
	}

	return nResultSize;
}


//-----------------------------------------------------------------------------
//
//	Function name:	WriteDeviceCommand
//
//
//	Abstract:
//		write the commands to the device
//
//
//	Parameters:
//		lpTxBuffer
//			the pointer to the command
//
//		nWriteSize
//			the size of command (byte)
//
//
//	Return values:
//		TRUE on success
//		FALSE on error
//
//-----------------------------------------------------------------------------
//
BOOL
WriteDeviceCommand( usb_dev_handle *hScanner, LPSTR lpTxBuffer, int nWriteSize, int seriesNo)
{
    int  nResultSize;
    if (IFTYPE_NET == hScanner->device){
	int iWritten;
	struct timeval net_timeout = NETTIMEOUTST;
	write_device_net(hScanner->net,
			 lpTxBuffer,
			 nWriteSize ,
			 &iWritten,
			 &net_timeout);
	return iWritten;
    }

    nResultSize = WriteDeviceData(hScanner, lpTxBuffer, nWriteSize, seriesNo);

    return nResultSize == nWriteSize ? TRUE : FALSE;
}


//-----------------------------------------------------------------------------
//
//	Function name:	AllocReceiveBuffer
//
//
//	Abstract:
//		Allocate the buffer the received data is stored to
//
//
//	Parameters:
//		hWndDlg
//			Window handle of the dialog
//
//
//	Return values:
//		the pointer to the buffer the recieved data is stored in
//
//-----------------------------------------------------------------------------
//	AllocReceiveBuffer   (The parts of ScanStar of old version)
HANDLE
AllocReceiveBuffer( DWORD  dwBuffSize )
{
	if( hReceiveBuffer == NULL ){
		//
		// Allocate the buffer the data is stored in
		//
		hReceiveBuffer = MALLOC( dwBuffSize );

		WriteLog( "ReceiveBuffer = %X, size = %d", hReceiveBuffer, dwBuffSize );
	}
	return hReceiveBuffer;
}


//-----------------------------------------------------------------------------
//
//	Function name:	FreeReceiveBuffer
//
//
//	Abstract:
//		Discard the buffer kept by AllocReceiveBuffer
//
//
//	Parameters:
//              None
//
//
//	Return values:
//		None
//
//-----------------------------------------------------------------------------
//	FreeReceiveBuffer  (The parts of DRV_PROC/WM_DESTROY of the previous version)
void
FreeReceiveBuffer( void )
{
	if( hReceiveBuffer != NULL ){
		FREE( hReceiveBuffer );
		hReceiveBuffer = NULL;

		WriteLog( "free ReceiveBuffer" );
	}
}



//
//    Try to set configuration
//
//    if false, send the CLEAR_FEATURE request
//       in order to reset the data toggle statement.
//
//    M-LNX-24   2006/04/12 kado

int  usb_set_configuration_or_reset_toggle(
		   Brother_Scanner *this,
		   int configuration){
  int errornum,nEndPoint,i;

  errornum = usb_set_configuration(this->hScanner->usb, configuration);
  if(errornum){
    nEndPoint = this->hScanner->usb_w_ep;
    if(nEndPoint <  0x1 || nEndPoint > 0x7f){
      nEndPoint = 0x03;
      for(i=0; i < ANOTHERENDPOINT; i++){
	if(this->modelInf.seriesNo  == ChangeEndpoint[i]){
	  nEndPoint = 0x04;
	  break;
	}
      }
    }
    usb_clear_halt(this->hScanner->usb, nEndPoint);
  }
  return errornum;
}

#ifdef SKEY_USBSEM

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <usb.h>
#include <stdio.h>
#include <string.h>

#if 0
#define ERRPRINT printf
#define DBGPRINT printf
#else
int ERRPRINT(const char *format,...){return 0;}
int DBGPRINT(const char *format,...){return 0;}
#endif

int   sem_id = -1;
int   semaphore_owner = 0;


#define SKEY   "brscan-skey -h query-semid"
#define SKEYC  "/usr/bin/brscan-skey"

key_t get_semkey(){
  FILE *fp_skey;
  int    semid;
  size_t n_read;
  char buffer[64] = {0};

  fp_skey = fopen(SKEYC,"r");
  if(fp_skey){
    fclose(fp_skey);
  }
  else{
    ERRPRINT("get_semkey open pipe error , file doesn't exist");
    semid = -1;
    return semid;
  }
  fp_skey = popen(SKEY,"r");
  semid = -1;
  if(fp_skey){
    memset(buffer,0,sizeof(buffer));
    n_read = fread(buffer,1,sizeof(buffer)-1,fp_skey);
    if((n_read >0) && (n_read < (sizeof(buffer)-1))){
      semid = atoi(buffer);
    }
    else{
      ERRPRINT("get_semkey n_read = %zd\n",n_read);
    }
    pclose(fp_skey);
  }
  else{
    ERRPRINT("get_semkey open pipe error");
  }
  return semid;
}

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};

int init_usb_criticalsection(void){
  union semun semuni;
  key_t key;
  int ret;
  DBGPRINT("init_usb_criticalsection()\n");
  key = get_semkey();

  sem_id = semget(key,1,IPC_CREAT | IPC_EXCL | 0666);
  if (sem_id != -1){
    semuni.val = 1;
    if((ret = semctl(sem_id,0,SETVAL,semuni)) == -1){
      ERRPRINT("ERROR : Semaphore error 13 (%s)\n",strerror(errno));
      return -1;
    }
    semaphore_owner = 1;
  }
  else{
    semaphore_owner = 0;
    sem_id = semget(key,1,IPC_CREAT | 0666);
    //    sem_id = semget(key,1,IPC_CREAT );
    if (sem_id == -1){
      ERRPRINT("ERROR : Semaphore error 12 (%s)\n",strerror(errno));
      return -1;
    }
  }
  return 0;
}

int discard_usb_criticalsection(void){
  union semun semuni;

  DBGPRINT("discard_usb_criticalsection()\n");
  if (sem_id == -1){
    ERRPRINT("ERROR : Semaphore error 21\n");
    return -1;
  }
  if(semaphore_owner == 1){
    semuni.val = 1;
    semctl(sem_id,0,IPC_RMID,semuni);
  }
  return 0;
}

int enter_usb_criticalsection(void){
  struct sembuf st_sem_op;

  DBGPRINT("enter_usb_criticalsection()\n");

  if (sem_id == -1){
    ERRPRINT("ERROR : Semaphore error 31\n");
    return -1;
  }
  st_sem_op.sem_num = 0;
  st_sem_op.sem_op = -1;
  st_sem_op.sem_flg = SEM_UNDO;

  if(semop(sem_id,&st_sem_op, 1) == -1){
    ERRPRINT("ERROR : Semaphore error 32 (%s)\n",strerror(errno));
    return -1;
  }
  return 0;
}

int release_usb_criticalsection(void){
  struct sembuf st_sem_op;

  DBGPRINT("release_usb_criticalsection()\n");

  if (sem_id == -1){
    ERRPRINT("ERROR : Semaphore error 40\n");
    return -1;
  }
  st_sem_op.sem_num = 0;
  st_sem_op.sem_op = 1;
  st_sem_op.sem_flg = SEM_UNDO;

  if(semop(sem_id,&st_sem_op, 1) == -1){
    ERRPRINT("ERROR : Semaphore error 41 (%s)\n",strerror(errno));
    return -1;
  }
  return 0;
}

#endif

//////// end of brother_devaccs.c ////////
