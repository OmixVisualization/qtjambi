/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
package io.qt.autotests;

import static org.junit.Assert.fail;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QThreadAffinityException;
import io.qt.core.QBuffer;
import io.qt.core.QByteArray;
import io.qt.core.QThread;
import io.qt.core.QUrl;
import io.qt.network.QHttpMultiPart;
import io.qt.network.QNetworkAccessManager;
import io.qt.network.QNetworkRequest;

public class TestNetworkThreadAffinity extends ApplicationInitializer{
	
	static {
		System.setProperty("io.qt.enable-thread-affinity-check", "true");
		System.setProperty("io.qt.enable-event-thread-affinity-check", "true");
	}
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithGui();
    }
	
	@Test
    public void test() throws Throwable{
		QNetworkAccessManager acc = new QNetworkAccessManager();
		QThread networkThread = new QThread();
		networkThread.start();
		acc.moveToThread(networkThread);
		QNetworkRequest request = new QNetworkRequest(new QUrl("http://www.qtjambi.io"));
		QBuffer buffer = new QBuffer();
		QHttpMultiPart multi = new QHttpMultiPart();
		try{
			try{
				acc.get(request);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {
			}
			try{
				acc.head(request);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {
			}
			try{
				acc.sendCustomRequest(request, new QByteArray(), new QByteArray());
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {
			}
			try{
				acc.sendCustomRequest(request, new QByteArray(), buffer);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {
			}
			try{
				acc.sendCustomRequest(request, new QByteArray(), multi);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {
			}
			try{
				acc.post(request, new QByteArray());
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {
			}
			try{
				acc.put(request, new QByteArray());
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {
			}
			try{
				acc.post(request, buffer);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {
			}
			try{
				acc.put(request, buffer);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {
			}
			try{
				acc.post(request, multi);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {
			}
			try{
				acc.put(request, multi);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {
			}
			try{
				acc.cookieJar();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {
			}
			try{
				acc.connectToHost("");
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {
			}
			try{
				acc.connectToHostEncrypted("");
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {
			}
			try{
				acc.deleteResource(request);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {
			}
			try{
				acc.setCache(null);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {
			}
		}finally {
			acc.disposeLater();
			networkThread.quit();
		}
	}
	
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestNetworkThreadAffinity.class.getName());
    }
}
