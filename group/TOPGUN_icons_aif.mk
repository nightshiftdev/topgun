#  Improved icon generating makefile, with dependencies and cleaning support
#  Copyright © 2008 Nokia.  All rights reserved.


ICONTARGETFILENAME=$(TARGETDIR)\TOPGUN_aif.mif

# This line can contain multiple icons, separated by spaces
ICONS = ..\images\icon_topgun.svg


#########

ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=$(EPOCROOT)epoc32\release\$(PLATFORM)\$(CFG)\Z
else
ZDIR=$(EPOCROOT)epoc32\data\z
endif

TARGETDIR=$(ZDIR)\resource\apps

# ----------------------------------------------------------------------------
# NOTE: if you have JUSTINTIME enabled for your S60 3rd FP1 or newer SDK
# and this command crashes, consider adding "/X" to the command line.
# See <http://forum.nokia.com/document/Forum_Nokia_Technical_Library_v1_35/contents/FNTL/Build_process_fails_at_mif_file_creation_in_S60_3rd_Ed_FP1_SDK.htm>
# ----------------------------------------------------------------------------

$(ICONTARGETFILENAME) : $(ICONS)
	mifconv $(ICONTARGETFILENAME) /c32 $(ICONS)

MAKMAKE : ;

BLD : ;

CLEAN : ;
	-del $(ICONTARGETFILENAME)

LIB : ;

CLEANLIB : ;

RESOURCE : $(ICONTARGETFILENAME)
				
FREEZE : ;

SAVESPACE : ;

RELEASABLES :
	@echo $(ICONTARGETFILENAME)

FINAL : ;

