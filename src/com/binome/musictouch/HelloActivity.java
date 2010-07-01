/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.binome.musictouch;



import com.binome.musictouch.R;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import java.util.Random;



/**
 * A minimal "Hello, World!" application.
 */
public class HelloActivity extends Activity {
    public HelloActivity() {
    }
    
    /**
     * Called with the activity is first created. 
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Set the layout for this activity.  You can find it
        // in res/layout/hello_activity.xml
        setContentView(R.layout.hello_activity);
            
        System.loadLibrary("glib-2.0");
        System.loadLibrary("gthread-2.0");
        System.loadLibrary("gmodule-2.0");
        System.loadLibrary("gobject-2.0");
        System.loadLibrary("gstreamer-0.10");
        System.loadLibrary("gstbase-0.10");
        System.loadLibrary("gstaudio-0.10");
        System.loadLibrary("gstcontroller-0.10");
        System.loadLibrary("oil");
        System.loadLibrary("gstaudioconvert");
        System.loadLibrary("gsttag-0.10");
        System.loadLibrary("gstmad");
        System.loadLibrary("gstmpegaudioparse");
        System.loadLibrary("soundtouch");
        System.loadLibrary("gstsoundtouch");
        System.loadLibrary("player-jni"); 
		final Player player = new Player();
		
//		player.play("/sdcard/Music/mp3");
		
		new Thread(new Runnable() {

			public void run() {
					player.play("/sdcard/Music/mp3");
			}
		}).start();

		try {
			Thread.sleep(5000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		final Random rand = new Random();
		new Thread(new Runnable() {

			public void run() {
				try {
					while (true) {
						Thread.sleep(5000);
						float value = (rand.nextInt(50)+1) / 20f;
						player.setTempo(value);
						System.out.println("tempo = " + value);
					}
				} catch (InterruptedException e) {
				}
			}
		}).start();
    }
}

