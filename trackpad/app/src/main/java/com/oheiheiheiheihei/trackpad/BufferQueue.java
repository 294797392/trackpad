package com.oheiheiheiheihei.trackpad;

import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.Semaphore;

public class BufferQueue {

    private Semaphore semaphore;
    private ConcurrentLinkedQueue<Object> queue;
    private Object queueLock;

    public BufferQueue() {
        this.queueLock = new Object();
        this.semaphore = new Semaphore(0);
        this.queue = new ConcurrentLinkedQueue<>();
    }

    public void enqueue(Object o) {
        try {
            this.queue.offer(o);
            this.semaphore.release();
        } catch (Exception ex) {

        }
    }

    public Object dequeue() {
        try {
            this.semaphore.acquire();
            return this.queue.poll();
        } catch (Exception ex) {
            return null;
        }
    }
}