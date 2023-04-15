########################
#    everything.pro    #
########################
#  This Qt4/5 project  #
#  file will build all #
#  applications        #
########################

TEMPLATE = subdirs
CONFIG += ordered

##########################
# Linux/Unix             #
##########################

unix:!macx {
SUBDIRS  = \
../../thumbnailer-kde4 \
../../embroidermodder2 \

}

##########################
# Windows                #
##########################

win32 {
SUBDIRS  = \
../../embroidermodder2 \

}

##########################
# Mac OSX                #
##########################

macx {
SUBDIRS  = \
../../embroidermodder2 \

}
