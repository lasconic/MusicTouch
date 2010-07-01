#include <jni.h>
#include <gst/gst.h>
#include <glib.h>
#include <android/log.h>


static gboolean
bus_call (GstBus     *bus,
          GstMessage *msg,
          gpointer    data)
{
  GMainLoop *loop = (GMainLoop *) data;

  switch (GST_MESSAGE_TYPE (msg)) {

    case GST_MESSAGE_EOS:
      g_print ("End of stream\n");
      g_main_loop_quit (loop);
      break;

    case GST_MESSAGE_ERROR: {
      gchar  *debug;
      GError *error;

      gst_message_parse_error (msg, &error, &debug);
      g_free (debug);

      g_printerr ("Error: %s\n", error->message);
      g_error_free (error);

      g_main_loop_quit (loop);
      break;
    }
    default:
      break;
  }

  return TRUE;
}

GstElement *pipeline, *source, *demuxer, *decoder, *conv, *pitch, *conv2, *sink;

/*
 * Class:     Player
 * Method:    setTempo
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_binome_musictouch_Player_setTempo
  (JNIEnv * env, jobject jobj, jfloat tempo){
		
		 gfloat temp = (gfloat)tempo;
		 if(pitch)
		      g_object_set (G_OBJECT (pitch), "tempo", temp, NULL);
		
}


/*
 * Class:     Player
 * Method:    play
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_binome_musictouch_Player_play
  (JNIEnv * env, jobject jobj, jstring name){

jboolean iscopy;

const char *mfile = (*env)->GetStringUTFChars(
                env, name, &iscopy);

GMainLoop *loop;

  
  GstBus *bus;

  /* Initialisation */
  int   argc = 1;


  setenv("GST_PLUGIN_PATH", "/sd-ext/plugins");
  setenv("GST_REGISTRY_UPDATE", "1");
  setenv("GST_REGISTRY", "/sdcard/reg.bin");
  setenv("LD_LIBRARY_PATH", "/system/lib:/sd-ext/lib");
  system("/sd-ext/bin/gst-inspect-0.10 > /sdcard/debug");
  __android_log_write(ANDROID_LOG_DEBUG, "OKKKKKKKKKKKKKKK", "okkkkkkkk");
  //char *argv = mfile;	
  gst_init (NULL, NULL);

  loop = g_main_loop_new (NULL, FALSE);

  /* Create gstreamer elements */
  pipeline = gst_pipeline_new ("audio-player");
  if (!pipeline) {
  __android_log_write(ANDROID_LOG_DEBUG, "OKKKKKKKKKKKKKKK", "piepe : One element could not be created. Exiting.\n");
  }
  source   = gst_element_factory_make ("filesrc",       "file-source");
  if (!source) {
  __android_log_write(ANDROID_LOG_DEBUG, "OKKKKKKKKKKKKKKK", "source : One element could not be created. Exiting.\n");
  }
  demuxer  = gst_element_factory_make ("mp3parse",      "mp3parse");
  if (!demuxer) {
  __android_log_write(ANDROID_LOG_DEBUG, "OKKKKKKKKKKKKKKK", "demux One element could not be created. Exiting.\n");
  }
  decoder  = gst_element_factory_make ("mad",     			"mad");
  if (!decoder) {
  __android_log_write(ANDROID_LOG_DEBUG, "OKKKKKKKKKKKKKKK", "decod One element could not be created. Exiting.\n");
  }
  conv     = gst_element_factory_make ("audioconvert",  "audioconvert");
  if (!conv) {
  __android_log_write(ANDROID_LOG_DEBUG, "OKKKKKKKKKKKKKKK", "conv One element could not be created. Exiting.\n");
  }
	pitch    = gst_element_factory_make ("pitch",  			  "pitch");
  if (!pitch) {
  __android_log_write(ANDROID_LOG_DEBUG, "OKKKKKKKKKKKKKKK", "pitch One element could not be created. Exiting.\n");
  }
  conv2    = gst_element_factory_make ("audioconvert",  "audioconvert2");
  if (!conv2) {
  __android_log_write(ANDROID_LOG_DEBUG, "OKKKKKKKKKKKKKKK", "conv2 One element could not be created. Exiting.\n");
  }
  sink     = gst_element_factory_make ("audioflingersink", "audio-output");
  if (!sink) {
  __android_log_write(ANDROID_LOG_DEBUG, "OKKKKKKKKKKKKKKK", "sink One element could not be created. Exiting.\n");
  }

  if (!pipeline || !source || !demuxer || !decoder || !conv || !pitch || !conv2 || !sink) {
  __android_log_write(ANDROID_LOG_DEBUG, "OKKKKKKKKKKKKKKK", "Final : One element could not be created. Exiting.\n");
    return -1;
  }

  /* Set up the pipeline */

  /* we set the input filename to the source element */
  g_object_set (G_OBJECT (source), "location", mfile, NULL);
  gfloat temp = 1.5;
  g_object_set (G_OBJECT (pitch), "tempo", temp, NULL);


  /* we add a message handler */
  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  gst_bus_add_watch (bus, bus_call, loop);
  gst_object_unref (bus);

  /* we add all elements into the pipeline */
  /* file-source | ogg-demuxer | vorbis-decoder | converter | alsa-output */
  gst_bin_add_many (GST_BIN (pipeline),
                    source, demuxer, decoder, conv, pitch, conv2, sink, NULL);

  /* we link the elements together */
  /* file-source -> ogg-demuxer ~> vorbis-decoder -> converter -> alsa-output */
  gst_element_link (source, demuxer);
  gst_element_link (demuxer, decoder);
  gst_element_link (decoder, conv);
  gst_element_link (conv, pitch);
  gst_element_link (pitch, conv2);
  gst_element_link (conv2, sink);
  
  /* note that the demuxer will be linked to the decoder dynamically.
     The reason is that Ogg may contain various streams (for example
     audio and video). The source pad(s) will be created at run time,
     by the demuxer when it detects the amount and nature of streams.
     Therefore we connect a callback function which will be executed
     when the "pad-added" is emitted.*/


  /* Set the pipeline to "playing" state*/
  __android_log_print(ANDROID_LOG_DEBUG, "OKKKKKKKKKKKKKKK", "Now playing: %s\n", mfile);
  gst_element_set_state (pipeline, GST_STATE_PLAYING);


  /* Iterate */
  g_print ("Running...\n");
  g_main_loop_run (loop);


  /* Out of the main loop, clean up nicely */
  g_print ("Returned, stopping playback\n");
  gst_element_set_state (pipeline, GST_STATE_NULL);

  g_print ("Deleting pipeline\n");
  gst_object_unref (GST_OBJECT (pipeline));

  return 0;

}
