/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.function.*;

import org.junit.*;

import io.qt.*;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.tasktree.*;
import io.qt.tasktree.QtTaskTree.*;
import io.qt.widgets.*;

public class TestTaskTree extends ApplicationInitializer{
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@org.junit.Before
	public void setUp() throws Exception {
		
	}
	
	@org.junit.After
	public void tearDown() throws Exception {
		
	}
	
	enum State implements QtEnumerator{
	    Initial,
	    Running,
	    Success,
	    Error,
	    Canceled,
	};

	enum ExecuteMode implements QtEnumerator{
	    Sequential, // default
	    Parallel,
	};
	
	static String colorButtonStyleSheet(QColor bgColor)
	{
	    String rc = "border-width: 2px; border-radius: 2px; border-color: black; ";
	    rc += bgColor.isValid() ? "border-style: solid; background: " + bgColor.name() + ";"
	                            : "border-style: dotted;";
	    return rc;
	}

	static QColor stateToColor(State state) {
	    switch (state) {
	    case Initial: return QColorConstants.Gray();
	    case Running: return QColorConstants.Yellow();
	    case Success: return QColorConstants.Green();
	    case Error: return QColorConstants.Red();
	    case Canceled: return QColorConstants.Cyan();
	    }
	    return new QColor();
	}
	
	static class OverlayWidget extends QWidget
	{
		public static interface PaintFunction{
			void invoke(QWidget w, QPainter p, QPaintEvent e);
		}
		public OverlayWidget() {
			this(null);
		}
	    public OverlayWidget(QWidget parent)
	    {
	        setAttribute(Qt.WidgetAttribute.WA_TransparentForMouseEvents);
	        if(parent!=null)
	            attachToWidget(parent);
	    }

	    public void attachToWidget(QWidget parent)
	    {
	        if (parentWidget()!=null)
	            parentWidget().removeEventFilter(this);
	        setParent(parent);
	        if (parent!=null) {
	            parent.installEventFilter(this);
	            resizeToParent();
	            raise();
	        }
	    }
	    public void setPaintFunction(PaintFunction paint) { m_paint = paint; }

	    @Override
		public boolean eventFilter(QObject obj, QEvent ev) {
	        if (obj == parent() && ev.type() == QEvent.Type.Resize)
	            resizeToParent();
	        return super.eventFilter(obj, ev);
	    }
	    
	    @Override
	    protected void paintEvent(QPaintEvent ev) {
	        if (m_paint!=null) {
	            QPainter p = new QPainter(this);
	            m_paint.invoke(this, p, ev);
	        }
	    }

	    private void resizeToParent() { setGeometry(new QRect(new QPoint(0, 0), parentWidget().size())); }

	    private PaintFunction m_paint;
	};

	static class ProgressIndicatorPainter{
		public ProgressIndicatorPainter() {
			m_timer.setSingleShot(false);
			m_timer.timeout.connect(m_timer, timer->{
		        nextAnimationStep();
		        if (m_callback!=null)
		            m_callback.run();
		    });

		    m_timer.setInterval(100);
		    m_pixmap = new QPixmap(":/io/qt/autotests/progressindicator.png");
		}

		public void setUpdateCallback(Runnable cb) { m_callback = cb; }

		public QSize size() { 
			var devicePixelRatio = m_pixmap.devicePixelRatio();
			if(devicePixelRatio!=0.0)
				return m_pixmap.size().div(devicePixelRatio);
			else 
				return m_pixmap.size();
		}

		public void paint(QPainter painter, QRect rect) {
			painter.save();
		    painter.setRenderHint(QPainter.RenderHint.SmoothPixmapTransform);
		    final QPoint translate = new QPoint(rect.x() + rect.width() / 2, rect.y() + rect.height() / 2);
		    QTransform t = new QTransform();
		    t.translate(translate.x(), translate.y());
		    t.rotate(m_rotation);
		    t.translate(-translate.x(), -translate.y());
		    painter.setTransform(t);
		    final QSize pixmapUserSize = size();
		    painter.drawPixmap(new QPoint(rect.x() + ((rect.width() - pixmapUserSize.width()) / 2),
		                              rect.y() + ((rect.height() - pixmapUserSize.height()) / 2)), m_pixmap);
		    painter.restore();
		}
		
		public void startAnimation() { m_timer.start(); }
		public void stopAnimation() { m_timer.stop(); }

		protected void nextAnimationStep() { m_rotation = (m_rotation + m_rotationStep + 360) % 360; }

		private final int m_rotationStep = 45;
		private int m_rotation = 0;
		private final QTimer m_timer = new QTimer();
		private QPixmap m_pixmap = new QPixmap();
		private Runnable m_callback;
	};

	static class ProgressIndicatorWidget extends OverlayWidget{
		public ProgressIndicatorWidget() {
			this(null);
		}
		public ProgressIndicatorWidget(QWidget parent)
	    {
			super(parent);
	        setPaintFunction((w, p, e) -> { m_paint.paint(p, w.rect()); });
	        m_paint.setUpdateCallback(()->{ update(); });
	        updateGeometry();
	    }

		public final QSize sizeHint() { return m_paint.size(); }

		protected final void showEvent(QShowEvent e) { m_paint.startAnimation(); }
		protected final void hideEvent(QHideEvent e) { m_paint.stopAnimation(); }

		private final ProgressIndicatorPainter m_paint = new ProgressIndicatorPainter();
	};
	
	static class ProgressIndicator extends QObject{
		public ProgressIndicator() {
			this(null);
		}
		public ProgressIndicator(QWidget parent) {
			super(parent);
			m_widget = new ProgressIndicatorWidget(parent);
		}

		public void show() {
			m_widget.show();
		}
		public void hide() {
			m_widget.hide();
		}
		private final ProgressIndicatorWidget m_widget;
	};
	
	static class StateIndicator extends QLabel {
		public StateIndicator() {
			this(State.Initial, null);
		}
		public StateIndicator(State initialState) {
			this(initialState, null);
		}
		public StateIndicator(QWidget parent) {
			this(State.Initial, parent);
		}
		public StateIndicator(State initialState, QWidget parent)
	    {
			super(parent);
			this.m_state = initialState;
			this.m_progressIndicator = new ProgressIndicator(this);
	        setAlignment(Qt.AlignmentFlag.AlignCenter);
	        QFont f = font();
	        f.setBold(true);
	        setFont(f);
	        updateState();
	    }

		public void setState(State state)
	    {
	        if (m_state == state)
	            return;
	        m_state = state;
	        updateState();
	    }

		private void updateState()
	    {
	        setStyleSheet(colorButtonStyleSheet(stateToColor(m_state)));
	        if (m_state == State.Running)
	            m_progressIndicator.show();
	        else
	            m_progressIndicator.hide();
	        setText(m_state == State.Canceled ? "X" : "");
	    }
		private State m_state = State.Initial;
		private final ProgressIndicator m_progressIndicator;
	};

	static class StateWidget extends QWidget {
		public StateWidget() {
			this(State.Initial);
		}
		public StateWidget(State initialState) {
			super();
			m_stateIndicator = new StateIndicator(initialState, this);
			QBoxLayout layout = new QHBoxLayout(this);
		    layout.setContentsMargins(0, 0, 0, 0);
		    layout.addWidget(m_stateIndicator);
		    setFixedSize(30, 30);
		}
		public void setState(State state) {
			m_stateIndicator.setState(state);
		}

		protected final StateIndicator m_stateIndicator;
	};

	static class TaskWidget extends QWidget
	{
		public TaskWidget(int busyTime, QtTaskTree.DoneResult result) {
			m_stateWidget = new StateWidget();
		    m_infoLabel = new QLabel(tr("Sleep:"));
		    m_spinBox = new QSpinBox();
		    m_checkBox = new QCheckBox(tr("Report success"));
		    m_spinBox.setSuffix(" sec");
		    m_spinBox.setValue(busyTime);
		    m_checkBox.setChecked(result == DoneResult.Success);

		    QBoxLayout layout = new QHBoxLayout(this);
		    layout.addWidget(m_stateWidget);
		    layout.addWidget(m_infoLabel);
		    layout.addWidget(m_spinBox);
		    layout.addWidget(m_checkBox);
		    layout.addStretch();
		    layout.setContentsMargins(0, 0, 0, 0);
		}

		public void setState(State state) { m_stateWidget.setState(state); }

		public int busyTime(){
		    return m_spinBox.value();
		}
		public QtTaskTree.DoneResult desiredResult() {
			return m_checkBox.isChecked() ? DoneResult.Success : DoneResult.Error;
		}

		private final StateWidget m_stateWidget;
		private final QLabel m_infoLabel;
		private final QSpinBox m_spinBox;
		private final QCheckBox m_checkBox;
	};

	static class GroupWidget extends QWidget
	{
		private final int QWIDGETSIZE_MAX = ((1<<24)-1);
		public GroupWidget() {
			m_stateWidget = new StateWidget();
		    m_executeCombo = new QComboBox();
		    m_workflowCombo = new QComboBox();
		    
		    m_stateWidget.setFixedSize(30, QWIDGETSIZE_MAX);

		    m_executeCombo.addItem(tr("Sequential"), ExecuteMode.Sequential.value());
		    m_executeCombo.addItem(tr("Parallel"), ExecuteMode.Parallel.value());
		    updateExecuteMode();
		    m_executeCombo.currentIndexChanged.connect(index -> {
		        m_executeMode = QVariant.convert(m_executeCombo.itemData(index), ExecuteMode.class);
		    });

		    WorkflowPolicy[] constants = WorkflowPolicy.class.getEnumConstants();
		    for (int i = 0; i < constants.length; ++i)
		        m_workflowCombo.addItem(constants[i].name(), constants[i]);

		    updateWorkflowPolicy();
		    m_workflowCombo.currentIndexChanged.connect(index -> {
		        m_workflowPolicy = QVariant.convert(m_workflowCombo.itemData(index), WorkflowPolicy.class);
		    });

		    QBoxLayout layout = new QHBoxLayout(this);
		    layout.addWidget(m_stateWidget);
		    QBoxLayout subLayout = new QVBoxLayout();
		    subLayout.addWidget(new QLabel(tr("Execute Mode:")));
		    subLayout.addWidget(m_executeCombo);
		    subLayout.addWidget(new QLabel(tr("Workflow Policy:")));
		    subLayout.addWidget(m_workflowCombo);
		    subLayout.addStretch();
		    layout.addLayout(subLayout);
		    layout.setContentsMargins(0, 0, 0, 0);

		    setSizePolicy(QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Preferred);
		}

		public void setState(State state) { m_stateWidget.setState(state); }

		public void setExecuteMode(ExecuteMode mode){
		    m_executeMode = mode;
		    updateExecuteMode();
		}
		public GroupItem executeModeItem(){
		    return m_executeMode == ExecuteMode.Sequential ? QtTaskTree.sequential() : QtTaskTree.parallel();
		}

		public void setWorkflowPolicy(QtTaskTree.WorkflowPolicy policy){
		    m_workflowPolicy = policy;
		    updateWorkflowPolicy();
		}
		
		public GroupItem workflowPolicyItem() {
		    return QtTaskTree.workflowPolicy(m_workflowPolicy);
		}

		private void updateExecuteMode(){
		    m_executeCombo.setCurrentIndex(m_executeCombo.findData(m_executeMode.value()));
		}
		private void updateWorkflowPolicy(){
		    m_workflowCombo.setCurrentIndex(m_workflowCombo.findData(m_workflowPolicy.value()));
		}

		private final StateWidget m_stateWidget;
		private final QComboBox m_executeCombo;
		private final QComboBox m_workflowCombo;

		private ExecuteMode m_executeMode = ExecuteMode.Sequential;
		private QtTaskTree.WorkflowPolicy m_workflowPolicy = QtTaskTree.WorkflowPolicy.StopOnError;
	};
	
	static String stateToString(State state)
	{
	    switch (state) {
	    case Initial: return QApplication.tr("Initial");
	    case Running: return QApplication.tr("Running");
	    case Success: return QApplication.tr("Success");
	    case Error: return QApplication.tr("Error");
	    case Canceled: return QApplication.tr("Canceled");
	    }
	    return "";
	}

	static class StateLabel extends QWidget
	{
		public StateLabel(State state){
		    QBoxLayout layout = new QHBoxLayout(this);
		    layout.addWidget(new StateWidget(state));
		    layout.addWidget(new QLabel(stateToString(state)));
		}
	};
	
	static QWidget hr()
	{
	    var frame = new QFrame();
	    frame.setFrameShape(QFrame.Shape.HLine);
	    frame.setFrameShadow(QFrame.Shadow.Sunken);
	    return frame;
	}

	static State resultToState(DoneWith result)
	{
	    switch (result) {
	    case Success: return State.Success;
	    case Error: return State.Error;
	    case Cancel: return State.Canceled;
	    }
	    return State.Initial;
	}

	static class GroupSetup
	{
	    public GroupSetup() {
			super();
		}
		public GroupSetup(ExecuteMode mode) {
			super();
			this.mode = mode;
		}
		public GroupSetup(WorkflowPolicy policy) {
			super();
			this.policy = policy;
		}
		public GroupSetup(WorkflowPolicy policy, ExecuteMode mode) {
			super();
			this.policy = policy;
			this.mode = mode;
		}
		public WorkflowPolicy policy = WorkflowPolicy.StopOnError;
	    public ExecuteMode mode = ExecuteMode.Sequential;
	};

	static interface GlueItem
	{
		public abstract ExecutableItem recipe();
		public abstract QWidget widget();
		public abstract void reset();
	};

	static class GroupGlueItem implements GlueItem
	{
		public GroupGlueItem(GroupSetup setup, GlueItem... children)
	    {
			m_children = QList.ofTyped(GlueItem.class, children);
			m_groupWidget = new GroupWidget();
	        m_widget = new QWidget();
	        QBoxLayout layout = new QHBoxLayout(m_widget);
	        layout.setContentsMargins(0, 0, 0, 0);
	        layout.addWidget(m_groupWidget);
	        QGroupBox groupBox = new QGroupBox();
	        QBoxLayout subLayout = new QVBoxLayout(groupBox);
	        for (int i = 0; i < children.length; ++i) {
	            if (i > 0)
	                subLayout.addWidget(hr());
	            subLayout.addWidget(children[i].widget());
	        }
	        layout.addWidget(groupBox);
	        m_groupWidget.setWorkflowPolicy(setup.policy);
	        m_groupWidget.setExecuteMode(setup.mode);
	    }

	    public ExecutableItem recipe() {
		    QList<GroupItem> childRecipes = new QList<>(GroupItem.class);
		    childRecipes.append(m_groupWidget.executeModeItem());
		    childRecipes.append(m_groupWidget.workflowPolicyItem());
		    childRecipes.append(Group.onGroupSetup(()->{ m_groupWidget.setState(State.Running); }));
		    for (GlueItem child : m_children)
		        childRecipes.append(child.recipe());
		    childRecipes.append(Group.onGroupDone((QtTaskTree.DoneWith result) -> { m_groupWidget.setState(resultToState(result)); }));

		    return new Group(childRecipes);
		}
	    public QWidget widget() { return m_widget; }
	    public void reset() {
	        m_groupWidget.setState(State.Initial);
	        for (GlueItem child : m_children)
	            child.reset();
	    }

	    private final QList<GlueItem> m_children;
	    private final GroupWidget m_groupWidget;
	    private final QWidget m_widget;
	};

	static class TaskGlueItem implements GlueItem
	{
		public TaskGlueItem(int busyTime, DoneResult result)
	    { 
			m_taskWidget = new TaskWidget(busyTime, result);
	    }

		public ExecutableItem recipe() {
		    return new Group(
		    	Group.onGroupSetup(()->{ m_taskWidget.setState(State.Running); }),
		    	QtTaskTree.timeoutTask(java.time.Duration.ofSeconds(m_taskWidget.busyTime()), m_taskWidget.desiredResult()),
		        Group.onGroupDone((DoneWith result) -> { m_taskWidget.setState(resultToState(result)); })
		    );
		}
		public QWidget widget() { return m_taskWidget; }
		public void reset() { m_taskWidget.setState(State.Initial); }

		private final TaskWidget m_taskWidget;
	};

	static GlueItem group(GroupSetup groupSetup, GlueItem... children)
	{
	    return new GroupGlueItem(groupSetup, children);
	}
	
	static GlueItem task() {
		return task(1, DoneResult.Success);
	}
	
	static GlueItem task(int busyTime) {
		return task(busyTime, DoneResult.Success);
	}
	
	static GlueItem task(DoneResult result) {
		return task(1, result);
	}

	static GlueItem task(int busyTime, DoneResult result)
	{
	    return new TaskGlueItem(busyTime, result);
	}
	
	@org.junit.Test
	public void test() {
		QWidget mainWidget = new QWidget();
	    mainWidget.setWindowTitle(QApplication.tr("Task Tree Demo"));

	    QToolButton startButton = new QToolButton();
	    startButton.setText(QApplication.tr("Start"));
	    QToolButton cancelButton = new QToolButton();
	    cancelButton.setText(QApplication.tr("Cancel"));
	    cancelButton.setEnabled(false);
	    QToolButton resetButton = new QToolButton();
	    resetButton.setText(QApplication.tr("Reset"));
	    QProgressBar progressBar = new QProgressBar();

	    GlueItem tree = group(new GroupSetup(WorkflowPolicy.ContinueOnSuccess),
	            group(new GroupSetup(),
	                task(),
	                task(2, DoneResult.Error),
	                task(3)
	            ),
	            task(),
	            task(),
	            group(new GroupSetup(WorkflowPolicy.FinishAllAndSuccess),
	                task(),
	                task(),
	                group(new GroupSetup(WorkflowPolicy.StopOnError, ExecuteMode.Parallel),
	                    task(4),
	                    task(2),
	                    task(1),
	                    task(3, DoneResult.Error)
	                ),
	                task(2),
	                task(3)
	            ),
	            task()
	        );
	    
	    {
	        QScrollArea scrollArea = new QScrollArea();
	        scrollArea.setWidgetResizable(true);
	        QWidget scrollAreaWidget = new QWidget();
	        QBoxLayout scrollLayout = new QVBoxLayout(scrollAreaWidget);
	        scrollLayout.addWidget(tree.widget());
	        scrollLayout.addStretch();
	        scrollArea.setWidget(scrollAreaWidget);

	        QBoxLayout mainLayout = new QVBoxLayout(mainWidget);
	        QBoxLayout subLayout = new QHBoxLayout();
	        subLayout.addWidget(startButton);
	        subLayout.addWidget(cancelButton);
	        subLayout.addWidget(resetButton);
	        subLayout.addWidget(progressBar);
	        mainLayout.addLayout(subLayout);
	        mainLayout.addWidget(hr());
	        mainLayout.addWidget(scrollArea);
	        mainLayout.addWidget(hr());
	        QBoxLayout footerLayout = new QHBoxLayout();
	        footerLayout.addWidget(new StateLabel(State.Initial));
	        footerLayout.addWidget(new StateLabel(State.Running));
	        footerLayout.addWidget(new StateLabel(State.Success));
	        footerLayout.addWidget(new StateLabel(State.Error));
	        footerLayout.addWidget(new StateLabel(State.Canceled));
	        mainLayout.addLayout(footerLayout);

	        final int margin = 4;
	        scrollArea.setMinimumSize(scrollAreaWidget.minimumSizeHint().grownBy(new QMargins(0, 0, margin, margin)));
	        QTimer.singleShot(0, scrollArea, ()->scrollArea.setMinimumSize(0, 0));
	    }

	    QSingleTaskTreeRunner taskTreeRunner = new QSingleTaskTreeRunner();

		QMetaObject.Slot0 resetTaskTree = ()-> {
	        taskTreeRunner.reset();
	        tree.reset();
	        progressBar.setValue(0);
	        cancelButton.setEnabled(false);
	    };

	    QMetaObject.Slot0 cancelTaskTree = ()-> { taskTreeRunner.cancel(); };

	    QMetaObject.Slot0 startTaskTree = ()-> {
	        resetTaskTree.invoke();
	        cancelButton.setEnabled(true);

	        Consumer<QTaskTree> onTaskTreeSetup = (QTaskTree taskTree) -> {
	            progressBar.setMaximum((int)taskTree.progressMaximum());
	            taskTree.progressValueChanged.connect(v->progressBar.setValue(v.intValue()));
	        };

	        Runnable onTaskTreeDone = ()->{ cancelButton.setEnabled(false); };

	        taskTreeRunner.start(new Group(tree.recipe()), onTaskTreeSetup, onTaskTreeDone);
	    };

	    startButton.clicked.connect(startTaskTree);
	    cancelButton.clicked.connect(cancelTaskTree);
	    resetButton.clicked.connect(resetTaskTree);

	    mainWidget.show();
	    QTimer.singleShot(50, startButton, QAbstractButton::click);
	    QTimer.singleShot(12000, QCoreApplication.instance(), QCoreApplication::quit);
	    QCoreApplication.exec();
	}
	
	@org.junit.Test
	public void testFor() {
		List<String> list = new ArrayList<>();
	    Group group = For.each( QList.of("C", "D", "E") )
	    		         .apply( t->{list.add(0, t);} );
	    QSingleTaskTreeRunner taskTreeRunner = new QSingleTaskTreeRunner();
	    taskTreeRunner.start(group);
	    Assert.assertEquals(Arrays.asList("E", "D", "C"), list);
	}
	
	@org.junit.Test
	public void testIfFor() {
		List<String> list1 = new ArrayList<>();
		List<String> list2 = new ArrayList<>();
		ListIterator<String> iterator = new ListIterator<>(QList.of("A", "B", "AA", "BB"));
	    Group group = For.iter(iterator)
	    		         .apply(
		    				If.test( ()->iterator.value().length()==1 )
		    				  .then( ()->list1.add(iterator.value()) )
		    				  .otherwise( ()->list2.add(iterator.value()) )
		    				  .endif()
		    			 );
	    QSingleTaskTreeRunner taskTreeRunner = new QSingleTaskTreeRunner();
	    taskTreeRunner.start(group);
	    Assert.assertEquals(Arrays.asList("A", "B"), list1);
	    Assert.assertEquals(Arrays.asList("AA", "BB"), list2);
	}
}
