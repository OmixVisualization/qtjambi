package io.qt.core;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * <p>Convenience class that simplifies QFuture synchronization</p>
 * <p>Java wrapper for Qt class <a href="http://doc.qt.io/qt/qfuturesynchronizer.html">QFutureSynchronizer</a></p>
 */
public final class QFutureSynchronizer<F extends QAbstractFuture>
{
    static {
        QtJambi_LibraryInitializer.init();
    }
    
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuturesynchronizer.html#QFutureSynchronizer-1">QFutureSynchronizer::</a></p>
     */
    public QFutureSynchronizer(){
        super();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuturesynchronizer.html#QFutureSynchronizer-2">QFutureSynchronizer::</a></p>
     */
    public QFutureSynchronizer(F future){
        super();
        addFuture(future);
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuturesynchronizer.html#addFuture">QFutureSynchronizer::</a></p>
     */
    @io.qt.QtUninvokable
    public final void addFuture(F future){
        m_futures.add(future);
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuturesynchronizer.html#cancelOnWait">QFutureSynchronizer::</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean cancelOnWait(){
        return m_cancelOnWait;
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuturesynchronizer.html#clearFutures">QFutureSynchronizer::</a></p>
     */
    @io.qt.QtUninvokable
    public final void clearFutures(){
    	m_futures.clear();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuturesynchronizer.html#setCancelOnWait">QFutureSynchronizer::</a></p>
     */
    @io.qt.QtUninvokable
    public final void setCancelOnWait(boolean enabled){
    	this.m_cancelOnWait = enabled;
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuturesynchronizer.html#setFuture">QFutureSynchronizer::</a></p>
     */
    @io.qt.QtUninvokable
    public final void setFuture(F future){
        waitForFinished();
        m_futures.clear();
        addFuture(future);
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuturesynchronizer.html#waitForFinished">QFutureSynchronizer::</a></p>
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
     * <p>See <a href="http://doc.qt.io/qt/qfuturesynchronizer.html#futures">QFutureSynchronizer::futures() const</a></p>
     */
    @io.qt.QtUninvokable
    public final List<F> futures() {
        return Collections.unmodifiableList(m_futures);
    }
    
    private final List<F> m_futures = new ArrayList<>();
    private boolean m_cancelOnWait;
}
