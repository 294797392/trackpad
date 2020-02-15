package com.oheiheiheiheihei.trackpad;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

public class TCPMouseDeviceClient extends AbstractMouseDeviceClient {

    private Socket tcpSocket;
    private InputStream inputStream;
    private OutputStream outputStream;

    @Override
    public boolean initialize() {
        this.tcpSocket = new Socket();
        return true;
    }

    @Override
    public boolean release() {
        return true;
    }

    @Override
    public boolean connect() {
        InetSocketAddress address = new InetSocketAddress("192.168.1.105", 8888);
        try {
            this.tcpSocket.connect(address);
            this.inputStream = this.tcpSocket.getInputStream();
            this.outputStream = this.tcpSocket.getOutputStream();
        } catch (IOException ex) {
            return false;
        }
        return true;
    }

    @Override
    public boolean disconnect() {
        return false;
    }

    @Override
    public boolean sendMessage(String message) {
        try {
            this.outputStream.write(message.getBytes());
        } catch (IOException ex) {
            return false;
        }
        return true;
    }
}
