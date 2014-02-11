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
../../libembroidery-convert \
../../embroidermodder2 \

}

##########################
# Windows                #
##########################

win32 {
SUBDIRS  = \
../../libembroidery-convert \
../../embroidermodder2 \

}

##########################
# Mac OSX                #
##########################

macx {
SUBDIRS  = \
../../libembroidery-convert \
../../embroidermodder2 \

}
