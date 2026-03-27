package io.qt.autotests;

import org.junit.BeforeClass;

import io.qt.core.*;
import io.qt.gui.*;
import io.qt.statemachine.*;
import io.qt.widgets.*;

public class TestStartMachine extends ApplicationInitializer{
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	static class StateSwitchEvent extends QEvent {

	    public static final QEvent.Type StateSwitchType = QEvent.Type.resolve(QEvent.Type.User.value() + 256);

	    private int m_rand;

	    public StateSwitchEvent(int rand) {
	        super(StateSwitchType);
	        this.m_rand = rand;
	    }

	    public int rand() {
	        return m_rand;
	    }
	}


	static class QGraphicsRectWidget extends QGraphicsWidget {

	    @Override
	    public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
	        painter.fillRect(rect(), Qt.GlobalColor.blue);
	    }
	}


	static class StateSwitchTransition extends QAbstractTransition {

	    private int m_rand;

	    public StateSwitchTransition(int rand) {
	        super();
	        this.m_rand = rand;
	    }

	    @Override
	    protected boolean eventTest(QEvent event) {
	        return (event.type() == StateSwitchEvent.StateSwitchType)
	                && (((StateSwitchEvent) event).rand() == m_rand);
	    }

	    @Override
	    protected void onTransition(QEvent event) {
	    }
	}


	static class StateSwitcher extends QState {

	    private int m_stateCount = 0;
	    private int m_lastIndex = 0;

	    public StateSwitcher(QStateMachine machine) {
	        super(machine);
	    }

	    @Override
	    protected void onEntry(QEvent event) {
	        int n;
	        do {
	            n = QRandomGenerator.global().bounded(m_stateCount) + 1;
	        } while (n == m_lastIndex);

	        m_lastIndex = n;
	        machine().postEvent(new StateSwitchEvent(n));
	    }

	    @Override
	    protected void onExit(QEvent event) {
	    }

	    public void addState(QState state, QAbstractAnimation animation) {
	        StateSwitchTransition trans = new StateSwitchTransition(++m_stateCount);
	        trans.setTargetState(state);
	        addTransition(trans);
	        trans.addAnimation(animation);
	    }
	}


	static QState createGeometryState(QObject w1, QRect rect1,
	                                  QObject w2, QRect rect2,
	                                  QObject w3, QRect rect3,
	                                  QObject w4, QRect rect4,
	                                  QState parent) {

	    QState result = new QState(parent);
	    result.assignProperty(w1, "geometry", rect1);
	    result.assignProperty(w2, "geometry", rect2);
	    result.assignProperty(w3, "geometry", rect3);
	    result.assignProperty(w4, "geometry", rect4);

	    return result;
	}


	static class GraphicsView extends QGraphicsView {

	    // Q_OBJECT  // Nicht übertragbar

	    public GraphicsView(QGraphicsScene scene, QWidget parent) {
	        super(scene, parent);
	    }

	    @Override
	    protected void resizeEvent(QResizeEvent event) {
	        fitInView(scene().sceneRect());
	        super.resizeEvent(event);
	    }
	}

	
	@org.junit.Test
	public void test() {
		QGraphicsRectWidget button1 = new QGraphicsRectWidget();
		QGraphicsRectWidget button2 = new QGraphicsRectWidget();
		QGraphicsRectWidget button3 = new QGraphicsRectWidget();
		QGraphicsRectWidget button4 = new QGraphicsRectWidget();

		button2.setZValue(1);
		button3.setZValue(2);
		button4.setZValue(3);

		QGraphicsScene scene = new QGraphicsScene(0, 0, 300, 300);
		scene.setBackgroundBrush(Qt.GlobalColor.black);
		scene.addItem(button1);
		scene.addItem(button2);
		scene.addItem(button3);
		scene.addItem(button4);

		GraphicsView window = new GraphicsView(scene, null);
		window.setFrameStyle(0);
		window.setAlignment(Qt.AlignmentFlag.AlignLeft, Qt.AlignmentFlag.AlignTop);
		window.setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff);
		window.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff);

		QStateMachine machine = new QStateMachine();

		QState group = new QState();
		group.setObjectName("group");

		QTimer timer = new QTimer();
		timer.setInterval(1250);
		timer.setSingleShot(true);

		group.entered.connect(timer, QTimer::start);

		QState state1 = createGeometryState(button1, new QRect(100, 0, 50, 50),
		                                    button2, new QRect(150, 0, 50, 50),
		                                    button3, new QRect(200, 0, 50, 50),
		                                    button4, new QRect(250, 0, 50, 50),
		                                    group);

		QState state2 = createGeometryState(button1, new QRect(250, 100, 50, 50),
		                                    button2, new QRect(250, 150, 50, 50),
		                                    button3, new QRect(250, 200, 50, 50),
		                                    button4, new QRect(250, 250, 50, 50),
		                                    group);

		QState state3 = createGeometryState(button1, new QRect(150, 250, 50, 50),
		                                    button2, new QRect(100, 250, 50, 50),
		                                    button3, new QRect(50, 250, 50, 50),
		                                    button4, new QRect(0, 250, 50, 50),
		                                    group);

		QState state4 = createGeometryState(button1, new QRect(0, 150, 50, 50),
		                                    button2, new QRect(0, 100, 50, 50),
		                                    button3, new QRect(0, 50, 50, 50),
		                                    button4, new QRect(0, 0, 50, 50),
		                                    group);

		QState state5 = createGeometryState(button1, new QRect(100, 100, 50, 50),
		                                    button2, new QRect(150, 100, 50, 50),
		                                    button3, new QRect(100, 150, 50, 50),
		                                    button4, new QRect(150, 150, 50, 50),
		                                    group);

		QState state6 = createGeometryState(button1, new QRect(50, 50, 50, 50),
		                                    button2, new QRect(200, 50, 50, 50),
		                                    button3, new QRect(50, 200, 50, 50),
		                                    button4, new QRect(200, 200, 50, 50),
		                                    group);

		QState state7 = createGeometryState(button1, new QRect(0, 0, 50, 50),
		                                    button2, new QRect(250, 0, 50, 50),
		                                    button3, new QRect(0, 250, 50, 50),
		                                    button4, new QRect(250, 250, 50, 50),
		                                    group);

		group.setInitialState(state1);

		QParallelAnimationGroup animationGroup = new QParallelAnimationGroup();

		QPropertyAnimation anim = new QPropertyAnimation(button4, "geometry");
		anim.setDuration(1000);
		anim.setEasingCurve(QEasingCurve.Type.OutElastic);
		animationGroup.addAnimation(anim);

		QSequentialAnimationGroup subGroup = new QSequentialAnimationGroup(animationGroup);
		subGroup.addPause(100);

		anim = new QPropertyAnimation(button3, "geometry");
		anim.setDuration(1000);
		anim.setEasingCurve(QEasingCurve.Type.OutElastic);
		subGroup.addAnimation(anim);

		subGroup = new QSequentialAnimationGroup(animationGroup);
		subGroup.addPause(150);

		anim = new QPropertyAnimation(button2, "geometry");
		anim.setDuration(1000);
		anim.setEasingCurve(QEasingCurve.Type.OutElastic);
		subGroup.addAnimation(anim);

		subGroup = new QSequentialAnimationGroup(animationGroup);
		subGroup.addPause(200);

		anim = new QPropertyAnimation(button1, "geometry");
		anim.setDuration(1000);
		anim.setEasingCurve(QEasingCurve.Type.OutElastic);
		subGroup.addAnimation(anim);

		StateSwitcher stateSwitcher = new StateSwitcher(machine);
		stateSwitcher.setObjectName("stateSwitcher");

		// Signal/Slot Ersatz
		group.addTransition(timer, "timeout()", stateSwitcher);

		stateSwitcher.addState(state1, animationGroup);
		stateSwitcher.addState(state2, animationGroup);

		stateSwitcher.addState(state3, animationGroup);
		stateSwitcher.addState(state4, animationGroup);
		stateSwitcher.addState(state5, animationGroup);
		stateSwitcher.addState(state6, animationGroup);

		stateSwitcher.addState(state7, animationGroup);

		machine.addState(group);
		machine.setInitialState(group);
		machine.start();

		window.resize(300, 300);
		window.show();
		QTimer.singleShot(5000, QCoreApplication.instance(), QCoreApplication::quit);

	    QApplication.exec();
	    machine.stop();
	}
}
