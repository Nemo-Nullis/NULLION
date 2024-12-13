package org.nullioncore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class NullionQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File nullionDir = new File(getFilesDir().getAbsolutePath() + "/.nullion");
        if (!nullionDir.exists()) {
            nullionDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
