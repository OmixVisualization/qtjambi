package io.qt.autotests;

import java.io.BufferedReader;
import java.io.InputStreamReader;

import org.junit.ClassRule;
import org.junit.internal.runners.statements.RunAfters;
import org.junit.rules.TestRule;
import org.junit.runner.Description;
import org.junit.runners.model.Statement;

public abstract class UnitTestInitializer {
	
	@ClassRule public static final TestRule rule = new TestRule(){
		@Override
		public Statement apply(Statement base, Description description) {
			if(base instanceof RunAfters) 
			{
				if(System.getenv().containsKey("QTJAMBI_TEST_ATTACH")) {
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
				}else if(System.getenv().containsKey("QTJAMBI_TEST_DELAY")) {
					try {
						int delay = Integer.parseInt(System.getenv("QTJAMBI_TEST_DELAY"));
						for (int i = delay; i > 0; --i) {
							System.out.println(description.getTestClass().getSimpleName()+" starts in "+i+"s");
							Thread.sleep(1000);
						}
					} catch (Exception e) {
						throw new ExceptionInInitializerError(e);
					}
				}
			}
			return base;
		}};
}
