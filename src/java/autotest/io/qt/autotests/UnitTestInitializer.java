package io.qt.autotests;

import java.io.BufferedReader;
import java.io.InputStreamReader;

import org.junit.ClassRule;
import org.junit.rules.TestRule;
import org.junit.runner.Description;
import org.junit.runners.model.Statement;

public abstract class UnitTestInitializer {
	
	@ClassRule public static final TestRule rule = new TestRule(){
		@Override
		public Statement apply(Statement base, Description description) {
			if(System.getenv().containsKey("QTJAMBI_TEST_ATTACH") || Boolean.getBoolean("test.attach")) {
				System.out.println(description.getTestClass().getSimpleName()+" continues after input:");
				try (BufferedReader br = new BufferedReader(new InputStreamReader(System.in))){
					String l = br.readLine();
					while(l==null || l.isEmpty()){
						Thread.sleep(500);
						l = br.readLine();
					}
				} catch (Exception e) {
					throw new ExceptionInInitializerError(e);
				}
			}else if(System.getenv().containsKey("QTJAMBI_TEST_DELAY") || !"".equals(System.getProperty("test.delay", ""))) {
				try {
					int delay = Integer.parseInt(System.getenv().containsKey("QTJAMBI_TEST_DELAY") ? System.getenv("QTJAMBI_TEST_DELAY") : System.getProperty("test.delay", ""));
					for (int i = delay; i > 0; --i) {
						System.out.println(description.getTestClass().getSimpleName()+" starts in "+i+"s");
						Thread.sleep(1000);
					}
				} catch (Exception e) {
					throw new ExceptionInInitializerError(e);
				}
			}
			if(description.getTestClass().getSimpleName().startsWith("TestInitialization")) {
				System.clearProperty("io.qt.no-deployment-spec");
				System.clearProperty("io.qt.library-path-override");
				System.clearProperty("io.qt.qml-imports");
				System.clearProperty("io.qt.pluginpath");
			}
			return base;
		}};
}
