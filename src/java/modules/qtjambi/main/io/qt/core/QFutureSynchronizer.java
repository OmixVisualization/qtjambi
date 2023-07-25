package io.qt.core;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * <p>Convenience class that simplifies QFuture synchronization</p>
 * <p>Java wrapper for Qt class <code><a href="http://doc.qt.io/qt/qfuturesynchronizer.html">QFutureSynchronizer</a></code></p>
 */
public final class QFutureSynchronizer<T>
{
    static {
        QtJambi_LibraryUtilities.initialize();
    }
    
    /**
     * <p>See <code><a href="http://doc.qt.io/qt/qfuturesynchronizer.html#QFutureSynchronizer-1">QFutureSynchronizer::<wbr>QFutureSynchronizer()</a></code></p>
     */
    public QFutureSynchronizer(){
        super();
    }
    
    /**
     * <p>See <code><a href="http://doc.qt.io/qt/qfuturesynchronizer.html#QFutureSynchronizer-2">QFutureSynchronizer::<wbr>QFutureSynchronizer(QFuture&lt;T>)</a></code></p>
     */
    public QFutureSynchronizer(QFuture<T> future){
        super();
        addFuture(future);
    }
    
    /**
     * <p>See <code><a href="http://doc.qt.io/qt/qfuturesynchronizer.html#addFuture">QFutureSynchronizer::<wbr>addFuture(QFuture&lt;T>)</a></code></p>
     */
    @io.qt.QtUninvokable
    public final void addFuture(QFuture<T> future){
        m_futures.add(future);
    }
    
    /**
     * <p>See <code><a href="http://doc.qt.io/qt/qfuturesynchronizer.html#cancelOnWait">QFutureSynchronizer::<wbr>cancelOnWait()const</a></code></p>
     */
    @io.qt.QtUninvokable
    public final boolean cancelOnWait(){
        return m_cancelOnWait;
    }
    
    /**
     * <p>See <code><a href="http://doc.qt.io/qt/qfuturesynchronizer.html#clearFutures">QFutureSynchronizer::<wbr>clearFutures()</a></code></p>
     */
    @io.qt.QtUninvokable
    public final void clearFutures(){
    	m_futures.clear();
    }
    
    /**
     * <p>See <code><a href="http://doc.qt.io/qt/qfuturesynchronizer.html#setCancelOnWait">QFutureSynchronizer::<wbr>setCancelOnWait(bool)</a></code></p>
     */
    @io.qt.QtUninvokable
    public final void setCancelOnWait(boolean enabled){
    	this.m_cancelOnWait = enabled;
    }
    
    /**
     * <p>See <code><a href="http://doc.qt.io/qt/qfuturesynchronizer.html#setFuture">QFutureSynchronizer::<wbr>setFuture(QFuture&lt;T>)</a></code></p>
     */
    @io.qt.QtUninvokable
    public final void setFuture(QFuture<T> future){
        waitForFinished();
        m_futures.clear();
        addFuture(future);
    }
    
    /**
     * <p>See <code><a href="http://doc.qt.io/qt/qfuturesynchronizer.html#waitForFinished">QFutureSynchronizer::<wbr>waitForFinished()const</a></code></p>
     */
    @io.qt.QtUninvokable
    public final void waitForFinished(){
        if (m_cancelOnWait) {
            for (int i = 0; i < m_futures.size(); ++i) {
                 m_futures.get(i).cancel();
            }
        }

        for (int i = 0; i < m_futures.size(); ++i) {
            m_futures.get(i).waitForFinished();
        }
    }
    
    /**
     * <p>See <code><a href="http://doc.qt.io/qt/qfuturesynchronizer.html#futures">QFutureSynchronizer::<wbr>futures()const</a></code></p>
     */
    @io.qt.QtUninvokable
    public final List<QFuture<T>> futures() {
        return Collections.unmodifiableList(m_futures);
    }
    
    private final List<QFuture<T>> m_futures = new ArrayList<>();
    private boolean m_cancelOnWait;
}
