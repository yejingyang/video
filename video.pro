TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    src/format_convert.cpp \
    src/h264_codec.c \
    src/rtp.c \
    src/video_capture.c \
    src/xcodec.c \
    src/xx_ortp.c \
    src/mfc-6410.c \
    src/output_format_conversion.c

HEADERS += \
    include/video_capture.h \
    include/h264_codec.h \
    include/rtp.h \
    include/format_convert.h \
    include/sdf.h \
    include/output_format_conversion.h

