package com.oheiheiheiheihei.trackpad;

import android.app.Activity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends Activity implements View.OnTouchListener, Runnable {

    private View viewTrackpad;
    private TextView textViewTouchPosition;
    private MouseDispatcher mouseDispatcher;
    private Thread mouseDispatcherThread;
    private Thread sendMessageThread;
    private float startX = 0;
    private float startY = 0;
    private BufferQueue messageQueue;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        this.textViewTouchPosition = ((TextView) this.findViewById(R.id.TextViewTouchPosition));
        this.viewTrackpad = this.findViewById(R.id.LinearLayoutTrackpad);
        this.viewTrackpad.setOnTouchListener(this);
        this.viewTrackpad.setLongClickable(true);

        this.messageQueue = new BufferQueue();
        this.sendMessageThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (true)
                {
                    String message = messageQueue.dequeue().toString();
                    mouseDispatcher.sendMessage(message);
                }
            }
        });

        this.mouseDispatcherThread = new Thread(this);
        this.mouseDispatcherThread.start();
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {

        int pointerCount = event.getPointerCount();

        if (pointerCount == 1) {
            float x = event.getX();
            float y = event.getY();

            StringBuilder builder = new StringBuilder();
            builder.append(String.format("%d, %f, %f\n", pointerCount, x, y));

            switch (event.getAction()) {
                case MotionEvent.ACTION_DOWN: {
                    builder.append("ACTION_DOWN\n");
                    this.startX = x;
                    this.startY = y;
                    this.messageQueue.enqueue(String.format(MessageFormat.MouseMovingBegin, this.startX, this.startY));
                }
                break;

                case MotionEvent.ACTION_MOVE: {
                    builder.append("ACTION_MOVE\n");
                    double offsetX = x - this.startX;
                    double offsetY = y - this.startY;
                    String message = String.format(MessageFormat.MouseMoving, offsetX, offsetY);
                    builder.append(message);
                    this.messageQueue.enqueue(message);
                }
                break;

                case MotionEvent.ACTION_UP: {
                    builder.append("ACTION_UP\n");
                    this.messageQueue.enqueue(String.format(MessageFormat.MouseMovingEnd, x, y));
                }
                break;
            }
            this.textViewTouchPosition.setText(builder.toString());
        } else if (pointerCount == 2) {
            switch (event.getAction())
            {
                case MotionEvent.ACTION_POINTER_DOWN
            }

        }

        return true;
    }

    @Override
    public void run() {
        this.mouseDispatcher = new MouseDispatcher();
        this.mouseDispatcher.start();
        this.sendMessageThread.start();
    }
}
