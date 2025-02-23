project('libsane-brother', 'c')
#sanedep = dependency('libsane')
incdir = include_directories('../include', '../libbrscandec_dec', '../libbrcolm')
usbdep = dependency('libusb', version: '>= 0.1.12')
add_global_arguments('-DBRSANESUFFIX=1', language : 'c')
add_global_arguments('-DV_MAJOR=1', language : 'c')
add_global_arguments('-DV_MINOR=0', language : 'c')

src = ['brother.c', 'brother_cmatch.c', 'brother_devaccs.c', 'brother_devinfo.c', 'brother_mfccmd.c',
	   'brother_misc.c', 'brother_modelinf.c', 'brother_scanner.c', 'brother_netdev.c', 'brother_advini.c', 'brother_log.c',
	   'brother_bugchk.c', 'sane_strstatus.c', 'sanei_config.c', 'sanei_constrain_value.c', 'sanei_init_debug.c',
       'bropen_com.c', 'bropen_sane.c', 'bropen_scan.c', 'bropen_types.c', 'bropen_yuv.c']
library('sane-brother', src, include_directories : incdir, dependencies : [usbdep])