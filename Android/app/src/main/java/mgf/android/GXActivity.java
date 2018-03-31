// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
*/

package mgf.android;

import android.app.NativeActivity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;

public class GXActivity extends NativeActivity {

    static {
        System.loadLibrary( "ModularGraphicsFramework" );
    }


	public GXActivity() {
		super();
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	}

	@Override
	public void onStart() {
		super.onStart();
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
	}

	@Override
	public boolean dispatchKeyEvent(KeyEvent event) {

		return super.dispatchKeyEvent(event);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
	}

	@Override
	public void onStop() {
		super.onStop();
	}
}

