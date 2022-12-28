package io.qt.autotests;

import org.junit.Assert;
import org.junit.Test;

import io.qt.autotests.generated.ContainerTest;
import io.qt.core.QHash;
import io.qt.core.QList;
import io.qt.core.QMap;
import io.qt.core.QMultiHash;
import io.qt.core.QMultiMap;
import io.qt.core.QPair;
import io.qt.core.QPoint;
import io.qt.core.QSet;

@SuppressWarnings("unchecked")
public class TestContainerAccessQt6 extends ApplicationInitializer {
	@Test
    public void testQListFloatDoublePair() {
    	QList<QPair<Float,Double>> container = QList.of(new QPair<>(1.0f, 1.1), new QPair<>(2.0f, 2.2), new QPair<>(3.0f, 3.3));
    	QList<QPair<Float,Double>> changedContainer = (QList<QPair<Float,Double>>)ContainerTest.sequentialAppend(container, new QPair<>(4.0f, 4.4));
    	Assert.assertEquals(QList.of(new QPair<>(1.0f, 1.1), new QPair<>(2.0f, 2.2), new QPair<>(3.0f, 3.3), new QPair<>(4.0f, 4.4)), changedContainer);
    	changedContainer = (QList<QPair<Float,Double>>)ContainerTest.sequentialPrepend(container, new QPair<>(4.0f, 4.4));
    	Assert.assertEquals(QList.of(new QPair<>(4.0f, 4.4), new QPair<>(1.0f, 1.1), new QPair<>(2.0f, 2.2), new QPair<>(3.0f, 3.3)), changedContainer);
    	changedContainer = (QList<QPair<Float,Double>>)ContainerTest.sequentialRemoveFirst(container);
    	Assert.assertEquals(QList.of(new QPair<>(2.0f, 2.2), new QPair<>(3.0f, 3.3)), changedContainer);
    	changedContainer = (QList<QPair<Float,Double>>)ContainerTest.sequentialRemoveLast(container);
    	Assert.assertEquals(QList.of(new QPair<>(1.0f, 1.1), new QPair<>(2.0f, 2.2)), changedContainer);
    	Assert.assertEquals(new QPair<>(2.0f, 2.2), ContainerTest.sequentialAt(container, 1));
    	changedContainer = (QList<QPair<Float,Double>>)ContainerTest.sequentialSetAt(container, 1, new QPair<>(6.0f, 6.6));
    	Assert.assertEquals(QList.of(new QPair<>(1.0f, 1.1), new QPair<>(6.0f, 6.6), new QPair<>(3.0f, 3.3)), changedContainer);
    }
    
    @Test
    public void testQSetQPoint() {
    	QSet<QPoint> container = QSet.of(new QPoint(1, 1), new QPoint(2, 2), new QPoint(3, 3));
    	QSet<QPoint> changedContainer = (QSet<QPoint>)ContainerTest.sequentialAppend(container, new QPoint(4, 4));
    	Assert.assertEquals(QSet.of(new QPoint(1, 1), new QPoint(2, 2), new QPoint(3, 3), new QPoint(4, 4)), changedContainer);
    	changedContainer = (QSet<QPoint>)ContainerTest.sequentialPrepend(container, new QPoint(4, 4));
    	Assert.assertEquals(QSet.of(new QPoint(4, 4), new QPoint(1, 1), new QPoint(2, 2), new QPoint(3, 3)), changedContainer);
    	changedContainer = (QSet<QPoint>)ContainerTest.sequentialRemoveFirst(container);
    	Assert.assertEquals(3, changedContainer.size());
    	changedContainer = (QSet<QPoint>)ContainerTest.sequentialRemoveLast(container);
    	Assert.assertEquals(3, changedContainer.size());
    	Assert.assertFalse(new QPoint(0, 0).equals(ContainerTest.sequentialAt(container, 1)));
    	changedContainer = (QSet<QPoint>)ContainerTest.sequentialSetAt(container, 1, new QPoint(6, 6)); // this is not possible for sets!
    	Assert.assertFalse(changedContainer.contains(new QPoint(6, 6)));
    }
    
    @Test
    public void testQHashShortDouble() {
    	QHash<Short,Double> container = QHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3);
    	Assert.assertEquals(2.2, ContainerTest.associativeValue(container, (short)2));
    	QHash<Short,Double> changedContainer = (QHash<Short,Double>)ContainerTest.associativeSetValue(container, (short)1, 11.1);
    	Assert.assertEquals(QHash.of((short)1, 11.1, (short)2, 2.2, (short)3, 3.3), changedContainer);
    	QPair<Object,Object> result = ContainerTest.associativeFindAndReplace(container, (short)2, 22.2);
    	Assert.assertEquals(Double.valueOf(2.2), result.second);
    	Assert.assertEquals(QHash.of((short)1, 1.1, (short)2, 22.2, (short)3, 3.3), result.first);
    	changedContainer = (QHash<Short,Double>)ContainerTest.associativeInsertKey(container, (short)1);
    	Assert.assertEquals(QHash.of((short)1, 0.0, (short)2, 2.2, (short)3, 3.3), changedContainer);
    	changedContainer = (QHash<Short,Double>)ContainerTest.associativeInsertKey(container, (short)128);
    	Assert.assertEquals(QHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)128, 0.0), changedContainer);
    	changedContainer = (QHash<Short,Double>)ContainerTest.associativeSetValue(container, (short)128, 128.128);
    	Assert.assertEquals(QHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)128, 128.128), changedContainer);
    }
    
    @Test
    public void testQMapShortDouble() {
    	QMap<Short,Double> container = QMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3);
    	Assert.assertEquals(2.2, ContainerTest.associativeValue(container, (short)2));
    	QMap<Short,Double> changedContainer = (QMap<Short,Double>)ContainerTest.associativeSetValue(container, (short)1, 11.1);
    	Assert.assertEquals(QMap.of((short)1, 11.1, (short)2, 2.2, (short)3, 3.3), changedContainer);
    	QPair<Object,Object> result = ContainerTest.associativeFindAndReplace(container, (short)2, 22.2);
    	Assert.assertEquals(Double.valueOf(2.2), result.second);
    	Assert.assertEquals(QMap.of((short)1, 1.1, (short)2, 22.2, (short)3, 3.3), result.first);
    	changedContainer = (QMap<Short,Double>)ContainerTest.associativeInsertKey(container, (short)1);
    	Assert.assertEquals(QMap.of((short)1, 0.0, (short)2, 2.2, (short)3, 3.3), changedContainer);
    	changedContainer = (QMap<Short,Double>)ContainerTest.associativeInsertKey(container, (short)128);
    	Assert.assertEquals(QMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)128, 0.0), changedContainer);
    	changedContainer = (QMap<Short,Double>)ContainerTest.associativeSetValue(container, (short)128, 128.128);
    	Assert.assertEquals(QMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)128, 128.128), changedContainer);
    }
    
    @Test
    public void testQMultiHashShortDouble() {
    	QMultiHash<Short,Double> container = QMultiHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4);
    	Assert.assertEquals(0.0, ContainerTest.associativeValue(container, (short)2));
    	QMultiHash<Short,Double> changedContainer = (QMultiHash<Short,Double>)ContainerTest.associativeSetValue(container, (short)1, 11.1);
    	Assert.assertEquals(QMultiHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3), changedContainer);
    	QPair<Object,Object> result = ContainerTest.associativeFindAndReplace(container, (short)2, 22.2);
    	Assert.assertEquals(Double.valueOf(2.2), result.second);
//    	Assert.assertEquals(QMultiHash.of((short)1, 1.1, (short)2, 22.2, (short)3, 3.3, (short)1, 4.4), result.first);
    	changedContainer = (QMultiHash<Short,Double>)ContainerTest.associativeInsertKey(container, (short)1);
    	Assert.assertEquals(QMultiHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4), changedContainer);
    	changedContainer = (QMultiHash<Short,Double>)ContainerTest.associativeInsertKey(container, (short)128);
    	Assert.assertEquals(QMultiHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4, (short)128, 0.0), changedContainer);
    	changedContainer = (QMultiHash<Short,Double>)ContainerTest.associativeSetValue(container, (short)128, 128.128);
    	Assert.assertEquals(QMultiHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4), changedContainer);
    }
    
    @Test
    public void testQMultiMapShortDouble() {
    	QMultiMap<Short,Double> container = QMultiMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4);
    	Assert.assertEquals(0.0, ContainerTest.associativeValue(container, (short)2));
    	QMultiMap<Short,Double> changedContainer = (QMultiMap<Short,Double>)ContainerTest.associativeSetValue(container, (short)1, 11.1);
    	Assert.assertEquals(QMultiMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4), changedContainer);
    	QPair<Object,Object> result = ContainerTest.associativeFindAndReplace(container, (short)2, 22.2);
    	Assert.assertEquals(Double.valueOf(2.2), result.second);
    	Assert.assertEquals(QMultiMap.of((short)1, 1.1, (short)2, 22.2, (short)3, 3.3, (short)1, 4.4), result.first);
    	changedContainer = (QMultiMap<Short,Double>)ContainerTest.associativeInsertKey(container, (short)1);
    	Assert.assertEquals(QMultiMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4, (short)1, 0.0), changedContainer);
    	changedContainer = (QMultiMap<Short,Double>)ContainerTest.associativeInsertKey(container, (short)128);
    	Assert.assertEquals(QMultiMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4, (short)128, 0.0), changedContainer);
    	changedContainer = (QMultiMap<Short,Double>)ContainerTest.associativeSetValue(container, (short)128, 128.128);
    	Assert.assertEquals(QMultiMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4), changedContainer);
    }
}
