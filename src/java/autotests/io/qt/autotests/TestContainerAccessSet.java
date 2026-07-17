package io.qt.autotests;

import static io.qt.core.QtGlobal.qHash;

import java.util.ArrayList;
import java.util.List;

import org.junit.Assert;
import org.junit.Test;

import io.qt.autotests.generated.ContainerTest;
import io.qt.core.QByteArray;
import io.qt.core.QDataStream;
import io.qt.core.QIODevice;
import io.qt.core.QList;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.QPair;
import io.qt.core.QSize;

public class TestContainerAccessSet extends ApplicationInitializer {
	
	static class SubObject extends QObject{
	}
    
	@Test
    public void testQListRemoveRange() {
		{
			QList<QObject> objectList = new QList<>(QObject.class);
			QObject o = new QObject();
			o.setObjectName("A");
			objectList.add(o);
			o = new QObject();
			o.setObjectName("B");
			objectList.add(o);
			Assert.assertEquals("A", objectList.takeAt(0).objectName());
			Assert.assertEquals("B", objectList.at(0).objectName());
		}
		{
			QList<SubObject> objectList = new QList<>(SubObject.class);
			SubObject o = new SubObject();
			o.setObjectName("A");
			objectList.add(o);
			o = new SubObject();
			o.setObjectName("B");
			objectList.add(o);
			Assert.assertEquals("A", objectList.takeAt(0).objectName());
			Assert.assertEquals("B", objectList.at(0).objectName());
		}
		{
			QList<QSize> list = QList.of(
					new QSize(0,0),
					new QSize(1,1),
					new QSize(2,2),
					new QSize(3,3),
					new QSize(4,4),
					new QSize(5,5),
					new QSize(6,6),
					new QSize(7,7),
					new QSize(8,8),
					new QSize(9,9)
					);
			QList<QSize> list2 = list.clone();
			list2.remove(2, 3);
			Assert.assertEquals(QList.of(
					new QSize(0,0),
					new QSize(1,1),
					new QSize(5,5),
					new QSize(6,6),
					new QSize(7,7),
					new QSize(8,8),
					new QSize(9,9)
				), list2);
			list2 = list.clone();
			list2.remove(6, 2);
			Assert.assertEquals(QList.of(
					new QSize(0,0),
					new QSize(1,1),
					new QSize(2,2),
					new QSize(3,3),
					new QSize(4,4),
					new QSize(5,5),
					new QSize(8,8),
					new QSize(9,9)
				), list2);
		}
	}
	
	@SuppressWarnings("unchecked")
	@Test
    public void testQListFloatDoublePair() {
    	QList<QPair<Float,Double>> container = QList.of(new QPair<>(1.0f, 1.1), new QPair<>(2.0f, 2.2), new QPair<>(3.0f, 3.3));
    	List<QPair<Float,Double>> javaContainer = new ArrayList<>();
    	ContainerTest.copyQListFDToJavaList(container, javaContainer);
    	Assert.assertEquals(container, javaContainer);
    	Object hash = ContainerTest.getQListFDHash(container);
    	Assert.assertEquals(hash, qHash(container));
    	QByteArray array = new QByteArray();
    	QDataStream s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	ContainerTest.writeQListFD(s, container);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	QList<QPair<Float,Double>> list2 = new QList<>(QMetaType.fromType(QPair.class, new QMetaType(QMetaType.Type.Float), new QMetaType(QMetaType.Type.Double)));
    	list2.readFrom(s);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	array = new QByteArray();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	container.writeTo(s);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	list2 = new QList<>(QMetaType.fromType(QPair.class, new QMetaType(QMetaType.Type.Float), new QMetaType(QMetaType.Type.Double)));
    	ContainerTest.readQListFD(s, list2);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	javaContainer.clear();
    	ContainerTest.copyFromIterable(list2, javaContainer);
    	Assert.assertEquals(list2, javaContainer);
    	QList<Object> variantList = ContainerTest.toQVariantList(list2);
    	Assert.assertEquals(variantList, container);
    	Assert.assertEquals(container.size(), ContainerTest.containerSize(container));
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
    public void testEmptyQListToVariantListNotCrashing() {
		ContainerTest.toQVariantList(new QList<>(String.class));
    }
}
