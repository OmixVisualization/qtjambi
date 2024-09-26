package io.qt.autotests;

import org.junit.Assert;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.core.QList;
import io.qt.core.QMetaType;
import io.qt.core.QMultiHash;
import io.qt.core.QSet;
import io.qt.gui.QTextCursor;
import io.qt.gui.QTextDocument;

public class TestReferenceCountingQMultiHashNested extends ApplicationInitializer {
	
	public static final int COUNT = 10;
    
    @Test
    public void test_owner_QMultiHash_value_QSet_QList_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QMultiHash<String,QSet<QList<QTextCursor>>> container = QMultiHash.of("A", new QSet<>(QMetaType.fromType(QList.class, QMetaType.fromType(QTextCursor.class))), 
															      "Z", QSet.of(QList.of(new QTextCursor(owner))));
		Assert.assertEquals(2, container.size());
		Assert.assertEquals(owner, General.internalAccess.owner(container));
		container = null;
    }
}
