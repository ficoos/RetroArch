package com.retroarch;

import com.retroarch.R;
import com.retroarch.R.id;
import com.retroarch.R.menu;
import com.retroarch.fileio.FileChooser;

import android.app.Activity;
import android.content.Intent;
import android.content.Context;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.Toast;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

public class main extends Activity
{
	private GLSurfaceView ctx_gl;
	
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        
        this.setTitle("RetroArch | Main");
        ctx_gl = new rgl_context(this);
        setContentView(ctx_gl);
    }
    
    public boolean onCreateOptionsMenu(Menu menu)
    {
    	MenuInflater inflater = getMenuInflater();
    	inflater.inflate(R.menu.main_menu, menu);
    	return true;
    }
    
    public boolean onOptionsItemSelected(MenuItem item)
    {
    	switch (item.getItemId())
    	{
    	    case R.id.main:
    	    	this.finish();
    		    break;
    	    case R.id.open:
    	    	Toast.makeText(this, "Select a ROM image from the Filebrowser.", Toast.LENGTH_SHORT).show();
    	    	Intent myIntent = new Intent(main.this, FileChooser.class);
    	    	main.this.startActivity(myIntent);
    		    break;
    		default:
    	    	Toast.makeText(this, "MenuItem " + item.getTitle() + " selected.", Toast.LENGTH_SHORT).show();
    	    	break;
    	}
    	
    	return true;
    }
    
    @Override
    protected void onPause()
    {
    	super.onPause();
    	ctx_gl.onPause();
    }
    
    @Override
    protected void onResume()
    {
    	super.onResume();
    	ctx_gl.onResume();
    }
}

class rgl_context extends GLSurfaceView
{
	public rgl_context(Context context)
	{
		super(context);
		setEGLContextClientVersion(2);
		setRenderer(new rgl());
	}
}