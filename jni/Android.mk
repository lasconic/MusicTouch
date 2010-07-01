LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := player.c

LOCAL_MODULE := libplayer-jni

LOCAL_C_INCLUDES := 			\
       $(JNI_H_INCLUDE) \
	$(LOCAL_PATH)			\
	$(GST_PLUGINS_BASE_TOP)		\
	$(GST_PLUGINS_BASE_TOP)/gst-libs\
	$(GST_PLUGINS_BASE_TOP)/gst-libs/gst/audio/android\
	$(GST_PLUGINS_BAD_TOP)		\
	$(GST_PLUGINS_BAD_TOP)/android	\
	/home/jer/cm4/external/gstreamer		\
	/home/jer/cm4/external/gstreamer/android 	\
	/home/jer/cm4/external/gstreamer/libs		\
	/home/jer/cm4/external/gstreamer/gst		\
	/home/jer/cm4/external/gstreamer/gst/android	\
	/home/jer/cm4/external/glib			\
	/home/jer/cm4/external/glib/glib		\
	/home/jer/cm4/external/glib/android	  	\
	/home/jer/cm4/external/glib/gmodule	  	\
	/home/jer/cm4/external/glib/gobject	  	\
	/home/jer/cm4/external/soundtouch/include	\
	/home/jer/cm4/external/glib/gthread

LOCAL_PRELINK_MODULE := false
LOCAL_LDLIBS := -L/home/jer/cm4/out/target/product/passion/system/lib -lglib-2.0 -lgstreamer-0.10 -lgstbase-0.10 -lgstcontroller-0.10 -lgthread-2.0 -lgmodule-2.0 -lgobject-2.0 -llog
#LOCAL_CFLAGS := 
#LOCAL_MODULE_TAGS :=
#LOCAL_SYSTEM_SHARED_LIBRARIES := libc
include $(BUILD_SHARED_LIBRARY)
