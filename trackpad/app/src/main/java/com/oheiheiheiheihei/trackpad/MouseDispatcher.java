package com.oheiheiheiheihei.trackpad;

public class MouseDispatcher implements AbstractMouseDeviceClient.AMDClientEventListener {

    private AbstractMouseDeviceClient amdClient;

    public void start()
    {
        this.amdClient = new TCPMouseDeviceClient();
        this.amdClient.setListener(this);
        this.amdClient.initialize();
        this.amdClient.connect();
    }

    public void stop()
    {}

    public void sendMessage(String message)
    {
        this.amdClient.sendMessage(message);
    }

    @Override
    public void statusChanged(AbstractMouseDeviceClient.AMDClientStatus status) {

    }

    @Override
    public void messageReceived() {

    }
}
