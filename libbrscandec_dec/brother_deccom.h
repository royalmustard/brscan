/*******************************************************************************
 *
 *	Scan Decode ���̥إå����ե�����
 *
 *	Copyright: 2000 brother Industries , Ltd.
 *
 *	ver 1.0.0 : 2000.04.13 : ���� : ��������
 ******************************************************************************/

/* ���顼���� */
#define	SC_BW		0x01
#define	SC_ED		0x02
#define	SC_DTH		0x04
#define	SC_TG		0x08
#define	SC_256		0x10
#define	SC_FUL		0x20
#define	SC_FULNOCM	0x40

#define	SC_2BIT		(0x01<<8)
#define	SC_8BIT		(0x02<<8)
#define	SC_24BIT	(0x04<<8)


#define	SCCLR_TYPE_BW		(SC_BW | SC_2BIT)		// Black & White
#define	SCCLR_TYPE_ED		(SC_ED | SC_2BIT)		// Error Diffusion Gray
#define	SCCLR_TYPE_DTH		(SC_DTH | SC_2BIT)		// Dithered Gray
#define	SCCLR_TYPE_TG		(SC_TG | SC_8BIT)		// True Gray
#define	SCCLR_TYPE_256		(SC_256 | SC_8BIT)		// 256 Color
#define	SCCLR_TYPE_FUL		(SC_FUL | SC_24BIT)		// 24bit Full Color
#define	SCCLR_TYPE_FULNOCM	(SC_FULNOCM | SC_24BIT)	// 24bit Full Color(do not colormatch)

/* ���ϥǡ������� */
#define	SCIDK_MONO		1	/* ��Υ��� */
#define	SCIDK_R			2	/* R�ǡ��� */
#define	SCIDK_G			3	/* G�ǡ��� */
#define	SCIDK_B			4	/* B�ǡ��� */
#define	SCIDK_RGB		5	/* RGB���ǽ� */
#define	SCIDK_BGR		6	/* BGR���ǽ� */
#define	SCIDK_256		7	/* 256�����顼 */

/* 24BitColor����RGB���Ϸ��� */
#define	SCODK_PIXEL_RGB		1	/* RGB RGB RGB���ǽ� */
#define	SCODK_PLANE			2	/* RRR GGG BBB���ǽ� */
#define	SCODK_PIXEL_BGR		3	/* BGR BGR BGR���ǽ� */

