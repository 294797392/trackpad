package com.oheiheiheiheihei.trackpad;

public abstract class AbstractMouseDeviceClient {

    public enum AMDClientStatus {
        Connecting,
        Connected,
        Disconnected
    }

    public interface AMDClientEventListener {
        void statusChanged(AMDClientStatus status);

        void messageReceived();
    }

    private AMDClientEventListener listener;

    public abstract boolean initialize();

    public abstract boolean release();

    public abstract boolean connect();

    public abstract boolean disconnect();

    public void setListener(AMDClientEventListener listener) {
        this.listener = listener;
    }

    public abstract boolean sendMessage(String message);
}