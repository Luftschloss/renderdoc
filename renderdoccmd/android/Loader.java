package @UWACLIENT_ANDROID_PACKAGE_NAME@;
import android.os.Build;
import android.app.Activity;
import android.view.WindowManager;
import android.os.Environment;
import android.content.Intent;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

public class Loader extends android.app.NativeActivity
{

    public native void updateLogoData(byte[] data, int width, int height);
    
    /* load our native library */
    static {
        System.loadLibrary("uwaclient"); // this will load VkLayer_GLES_RenderDoc as well
    }

    public void LoadLogoAndUpdate()
    {
    try {
        InputStream inputStream = getAssets().open("textures/logo.png");
        Bitmap bitmap = BitmapFactory.decodeStream(inputStream);
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();
        int bytes = bitmap.getByteCount();
        ByteBuffer buffer = ByteBuffer.allocate(bytes);
        bitmap.copyPixelsToBuffer(buffer);
        updateLogoData(buffer.array(), width, height);
        inputStream.close();
    }catch(IOException e){

    }
    }

    @Override
    protected void onCreate(android.os.Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        LoadLogoAndUpdate();

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        // if we're running on something older than Android M (6.0), return now
        // before requesting permissions as it's not supported
        if(Build.VERSION.SDK_INT < Build.VERSION_CODES.M)
            return;

        String[] permission = new String[1];
        if (Build.VERSION.SDK_INT < 30 /*Build.VERSION_CODES.R*/) {
            permission[0] = android.Manifest.permission.WRITE_EXTERNAL_STORAGE;

            requestPermissions(permission, 1);

            // Request is asynchronous, so prevent connection to server until permissions granted.
            while(checkSelfPermission(permission[0])
                != android.content.pm.PackageManager.PERMISSION_GRANTED)
            {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    break;
                }
            }
        }
        else {
            permission[0] = "android.permission.MANAGE_EXTERNAL_STORAGE"; // android.Manifest.permission.MANAGE_EXTERNAL_STORAGE;

            try {
                java.lang.reflect.Method method = Environment.class.getMethod("isExternalStorageManager");

                Boolean result = (Boolean)method.invoke(null);

                // Popup a dialog if we haven't granted Android storage permissions.
                if (!result) {
                    Intent viewIntent = new Intent( "android.settings.MANAGE_ALL_FILES_ACCESS_PERMISSION"  /*android.provider.Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION*/);
                    startActivity(viewIntent);
                }
            } catch(Exception e) { }
        }
    }
}
