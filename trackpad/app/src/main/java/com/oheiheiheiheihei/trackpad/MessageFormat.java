package com.oheiheiheiheihei.trackpad;

public class MessageFormat {
    public final static String MouseMovingBegin = "{\"type\":" + MessageID.MouseLeftButtonDown + ", \"x\":%f, \"y\":%f}\n";
    public final static String MouseMovingEnd = "{\"type\":" + MessageID.MouseLeftButtonUp + ", \"x\":%f, \"y\":%f}\n";
    public final static String MouseMoving = "{\"type\":" + MessageID.MouseMoving + ", \"x\":%f, \"y\":%f}\n";
}
