package io.qt.tasktree;

import java.util.function.BiConsumer;
import java.util.function.BiFunction;
import java.util.function.BiPredicate;
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.Predicate;
import java.util.function.Supplier;

import io.qt.NonNull;
import io.qt.Nullable;
import io.qt.StrictNonNull;
import io.qt.core.QObject;
import io.qt.core.QProcess;


/**
 * <p>A class template used for declaring custom task items and defining their setup and done handlers</p>
 * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qttasktree-qcustomtask.html">QtTaskTree::QCustomTask&lt;QTaskTree,QTaskTreeTaskAdapter&gt;</a></code></p>
 * @since This class was introduced in Qt 6.11.
 */
public final class QCustomTask<Task, Adapter extends java.util.function.BiConsumer<Task, io.qt.tasktree.@Nullable QTaskInterface>> extends io.qt.tasktree.ExecutableItem
{
	final Class<Task> taskType;
	final Class<Adapter> adapterType;
	
    public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
        super((QPrivateConstructor)null);
        this.taskType = taskType;
        this.adapterType = adapterType;
        java.util.Objects.requireNonNull(taskType, "Argument 'taskType': null not expected.");
        java.util.Objects.requireNonNull(adapterType, "Argument 'adapterType': null not expected.");
        if(taskType==QTaskTree.class && adapterType==QTaskTreeTaskAdapter.class) {
        	initialize_native_QTaskTreeTask(this, setup, done, callDone);
        }else if(taskType==Timeout.class && adapterType==QTimeoutTaskAdapter.class) {
        	initialize_native_QTimeoutTask(this, setup, done, callDone);
        }else if(QBarrier.class==taskType && adapterType==QDefaultTaskAdapter.<QBarrier>typedClass()) {
        	initialize_native_QBarrierTask(this, setup, done, callDone);
        }else if(QThreadFunction.typedClass()==taskType && adapterType==QThreadFunctionTaskAdapter.typedClass()) {
        	initialize_native_QThreadFunctionTask(this, setup, done, callDone);
        }else if(QThreadFunctionVoid.class==taskType && adapterType==QThreadFunctionVoidTaskAdapter.class) {
        	initialize_native_QThreadFunctionVoidTask(this, setup, done, callDone);
        }else if(QProcess.class==taskType && adapterType==QProcessTaskAdapter.class) {
        	initialize_native_QProcessTask(this, setup, done, callDone);
        }else {
	        Supplier<Task> taskFactory;
			try {
				taskFactory = QtJambi_LibraryUtilities.internal.getFactory0(taskType.getConstructor());
			} catch (NoSuchMethodException e) {
				throw new IllegalArgumentException("Argument 'taskType': requires default-constructible class.", e);
			}
		    Supplier<Adapter> adapterFactory;
			try {
				adapterFactory = QtJambi_LibraryUtilities.internal.getFactory0(adapterType.getConstructor());
			} catch (NoSuchMethodException e) {
				throw new IllegalArgumentException("Argument 'adapterType': requires default-constructible class.", e);
			}
			initialize_native_by_factories(this, taskFactory, adapterFactory, setup, done, callDone);
        }
    }
    public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
    	this(taskType, adapterType, setup, done, new QtTaskTree.CallDone(callDone));
    }
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		this(taskType, adapterType, setup, done, QtTaskTree.CallDoneFlag.Always.asFlags());
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup){
		this(taskType, adapterType, setup, (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)null, QtTaskTree.CallDoneFlag.Always.asFlags());
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType){
		this(taskType, adapterType, (Function<Task, QtTaskTree.SetupResult>)null, (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)null, QtTaskTree.CallDoneFlag.Always.asFlags());
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    	    QtTaskTree.@NonNull CallDone callDone){
		this(taskType, adapterType, setup, (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)null, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		this(taskType, adapterType, setup, (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)null, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    	    QtTaskTree.@NonNull CallDone callDone){
		this(taskType, adapterType, (Function<Task, QtTaskTree.SetupResult>)null, (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)null, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		this(taskType, adapterType, (Function<Task, QtTaskTree.SetupResult>)null, (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)null, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		this(taskType, adapterType, (Function<Task, QtTaskTree.SetupResult>)null, done, QtTaskTree.CallDoneFlag.Always.asFlags());
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    		QtTaskTree.@NonNull CallDone callDone){
		this(taskType, adapterType, (Function<Task, QtTaskTree.SetupResult>)null, done, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    		QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		this(taskType, adapterType, (Function<Task, QtTaskTree.SetupResult>)null, done, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		this(taskType, adapterType, setup==null ? null : task->{setup.accept(task); return QtTaskTree.SetupResult.Continue;}, done, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		this(taskType, adapterType, setup==null ? null : task->{setup.accept(task); return QtTaskTree.SetupResult.Continue;}, done, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		this(taskType, adapterType, setup, done, QtTaskTree.CallDoneFlag.Always.asFlags());
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup){
		this(taskType, adapterType, setup, (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)null, QtTaskTree.CallDoneFlag.Always.asFlags());
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    	    QtTaskTree.@NonNull CallDone callDone){
		this(taskType, adapterType, setup, (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)null, callDone);
	}

	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		this(taskType, adapterType, setup, (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)null, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<Task, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		this(taskType, adapterType, setup, done==null ? null : (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)(task,w)->done.apply(task), callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<Task, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		this(taskType, adapterType, setup, done==null ? null : (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)(task,w)->done.apply(task), callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<Task, QtTaskTree.@NonNull DoneResult> done){
		this(taskType, adapterType, setup, done, QtTaskTree.CallDoneFlag.Always.asFlags());
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Function<Task, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		this(taskType, adapterType, setup==null ? null : task->{setup.accept(task); return QtTaskTree.SetupResult.Continue;}, done, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Function<Task, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		this(taskType, adapterType, setup==null ? null : task->{setup.accept(task); return QtTaskTree.SetupResult.Continue;}, done, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Function<Task, QtTaskTree.@NonNull DoneResult> done){
		this(taskType, adapterType, setup, done, QtTaskTree.CallDoneFlag.Always.asFlags());
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<Task, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		this(taskType, adapterType, setup, done==null ? null : (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)(task,w)->QtTaskTree.toDoneResult(done.test(task,w)), callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<Task, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		this(taskType, adapterType, setup, done==null ? null : (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)(task,w)->QtTaskTree.toDoneResult(done.test(task,w)), callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<Task, QtTaskTree.@NonNull DoneWith> done){
		this(taskType, adapterType, setup, done, QtTaskTree.CallDoneFlag.Always.asFlags());
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiPredicate<Task, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		this(taskType, adapterType, setup==null ? null : task->{setup.accept(task); return QtTaskTree.SetupResult.Continue;}, done, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiPredicate<Task, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		this(taskType, adapterType, setup==null ? null : task->{setup.accept(task); return QtTaskTree.SetupResult.Continue;}, done, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiPredicate<Task, QtTaskTree.@NonNull DoneWith> done){
		this(taskType, adapterType, setup, done, QtTaskTree.CallDoneFlag.Always.asFlags());
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<Task> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		this(taskType, adapterType, setup, done==null ? null : (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)(task,w)->QtTaskTree.toDoneResult(done.test(task)), callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<Task> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		this(taskType, adapterType, setup, done==null ? null : (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)(task,w)->QtTaskTree.toDoneResult(done.test(task)), callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<Task> done){
		this(taskType, adapterType, setup, done, QtTaskTree.CallDoneFlag.Always.asFlags());
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Predicate<Task> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		this(taskType, adapterType, setup==null ? null : task->{setup.accept(task); return QtTaskTree.SetupResult.Continue;}, done, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Predicate<Task> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		this(taskType, adapterType, setup==null ? null : task->{setup.accept(task); return QtTaskTree.SetupResult.Continue;}, done, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Predicate<Task> done){
		this(taskType, adapterType, setup, done, QtTaskTree.CallDoneFlag.Always.asFlags());
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<Task, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		this(taskType, adapterType, setup, done==null ? null : (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)(task,w)->{done.accept(task,w); return QtTaskTree.toDoneResult(w==QtTaskTree.DoneWith.Success);}, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<Task, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		this(taskType, adapterType, setup, done==null ? null : (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)(task,w)->{done.accept(task,w); return QtTaskTree.toDoneResult(w==QtTaskTree.DoneWith.Success);}, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<Task, QtTaskTree.@NonNull DoneWith> done){
		this(taskType, adapterType, setup, done, QtTaskTree.CallDoneFlag.Always.asFlags());
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiConsumer<Task, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		this(taskType, adapterType, setup==null ? null : task->{setup.accept(task); return QtTaskTree.SetupResult.Continue;}, done, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiConsumer<Task, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		this(taskType, adapterType, setup==null ? null : task->{setup.accept(task); return QtTaskTree.SetupResult.Continue;}, done, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiConsumer<Task, QtTaskTree.@NonNull DoneWith> done){
		this(taskType, adapterType, setup, done, QtTaskTree.CallDoneFlag.Always.asFlags());
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<Task> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		this(taskType, adapterType, setup, done==null ? null : (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)(task,w)->{done.accept(task); return QtTaskTree.toDoneResult(w==QtTaskTree.DoneWith.Success);}, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<Task> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		this(taskType, adapterType, setup, done==null ? null : (BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult>)(task,w)->{done.accept(task); return QtTaskTree.toDoneResult(w==QtTaskTree.DoneWith.Success);}, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<Task> done){
		this(taskType, adapterType, setup, done, QtTaskTree.CallDoneFlag.Always.asFlags());
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Consumer<Task> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		this(taskType, adapterType, setup==null ? null : task->{setup.accept(task); return QtTaskTree.SetupResult.Continue;}, done, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
    		@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Consumer<Task> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		this(taskType, adapterType, setup==null ? null : task->{setup.accept(task); return QtTaskTree.SetupResult.Continue;}, done, callDone);
	}
	public QCustomTask(@StrictNonNull Class<Task> taskType, 
			@StrictNonNull Class<Adapter> adapterType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Consumer<Task> done){
		this(taskType, adapterType, setup, done, QtTaskTree.CallDoneFlag.Always.asFlags());
	}
    
    private native static <Task> void initialize_native_QProcessTask(QCustomTask<?,?> instance, 
    		Function<Task, QtTaskTree.SetupResult> setup,
    	    BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult> done,
    	    QtTaskTree.CallDone callDone);
    
    private native static <Task> void initialize_native_QTaskTreeTask(QCustomTask<?,?> instance, 
    		Function<Task, QtTaskTree.SetupResult> setup,
    	    BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult> done,
    	    QtTaskTree.CallDone callDone);
    
    private native static <Task> void initialize_native_QTimeoutTask(QCustomTask<?,?> instance, 
    		Function<Task, QtTaskTree.SetupResult> setup,
    	    BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult> done,
    	    QtTaskTree.CallDone callDone);
    
    private native static <Task> void initialize_native_QBarrierTask(QCustomTask<?,?> instance, 
    		Function<Task, QtTaskTree.SetupResult> setup,
    	    BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult> done,
    	    QtTaskTree.CallDone callDone);
    
    private native static <Task> void initialize_native_QThreadFunctionTask(QCustomTask<?,?> instance, 
    		Function<Task, QtTaskTree.SetupResult> setup,
    	    BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult> done,
    	    QtTaskTree.CallDone callDone);
    
    private native static <Task> void initialize_native_QThreadFunctionVoidTask(QCustomTask<?,?> instance, 
    		Function<Task, QtTaskTree.SetupResult> setup,
    	    BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult> done,
    	    QtTaskTree.CallDone callDone);
    
    private native static <Task, Adapter extends java.util.function.BiConsumer<Task, io.qt.tasktree.@Nullable QTaskInterface>> void initialize_native_by_factories(QCustomTask<?,?> instance,
    	    Supplier<Task> taskFactory,
    	    Supplier<Adapter> adapterFactory, 
    		Function<Task, QtTaskTree.SetupResult> setup,
    	    BiFunction<Task, QtTaskTree.DoneWith, QtTaskTree.DoneResult> done,
    	    QtTaskTree.CallDone callDone);
    
    // QTimeoutTask
    
    public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<Timeout, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone) {
    	return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
    }
    public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<Timeout, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
    	return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable BiFunction<Timeout, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, done);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable BiFunction<Timeout, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    		QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable BiFunction<Timeout, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable BiFunction<Timeout, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<Timeout, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<Timeout, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable Function<Timeout, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable Function<Timeout, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<Timeout, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<Timeout, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable BiPredicate<Timeout, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable BiPredicate<Timeout, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<Timeout> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<Timeout> done){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable Predicate<Timeout> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable Predicate<Timeout> done){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<Timeout, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<Timeout, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable BiConsumer<Timeout, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable BiConsumer<Timeout, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<Timeout> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<Timeout> done){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable Consumer<Timeout> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable Consumer<Timeout> done){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done);
	}
	

    public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<Timeout, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    	return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
    }
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable BiFunction<Timeout, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    		QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable BiFunction<Timeout, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<Timeout, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable Function<Timeout, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable BiPredicate<Timeout, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<Timeout> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable Predicate<Timeout> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<Timeout, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable BiConsumer<Timeout, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Function<Timeout, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<Timeout> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<Timeout,QTimeoutTaskAdapter> createQTimeoutTask(
    		@Nullable Consumer<Timeout> setup,
    		@Nullable Consumer<Timeout> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(Timeout.class, QTimeoutTaskAdapter.class, setup, done, callDone);
	}
	
	// QBarrierTask
    
    public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<QBarrier, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone) {
    	return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
    }
    public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<QBarrier, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
    	return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass());
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable BiFunction<QBarrier, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), done);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable BiFunction<QBarrier, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    		QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable BiFunction<QBarrier, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable BiFunction<QBarrier, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<QBarrier, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<QBarrier, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable BiPredicate<QBarrier, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable BiPredicate<QBarrier, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<QBarrier> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<QBarrier> done){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable Predicate<QBarrier> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable Predicate<QBarrier> done){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<QBarrier, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<QBarrier, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable BiConsumer<QBarrier, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable BiConsumer<QBarrier, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<QBarrier> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<QBarrier> done){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable Consumer<QBarrier> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable Consumer<QBarrier> done){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done);
	}
    public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<QBarrier, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    	return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
    }
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable BiFunction<QBarrier, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    		QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable BiFunction<QBarrier, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<QBarrier, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable BiPredicate<QBarrier, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<QBarrier> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable Predicate<QBarrier> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<QBarrier, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable BiConsumer<QBarrier, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Function<QBarrier, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<QBarrier> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static QCustomTask<QBarrier,QDefaultTaskAdapter<QBarrier>> createQBarrierTask(
    		@Nullable Consumer<QBarrier> setup,
    		@Nullable Consumer<QBarrier> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QBarrier.class, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	
	// QTaskTreeTask
    
    public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<QTaskTree, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone) {
    	return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
    }
    public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<QTaskTree, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
    	return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable BiFunction<QTaskTree, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, done);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable BiFunction<QTaskTree, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    		QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable BiFunction<QTaskTree, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable BiFunction<QTaskTree, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<QTaskTree, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<QTaskTree, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable BiPredicate<QTaskTree, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable BiPredicate<QTaskTree, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<QTaskTree> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<QTaskTree> done){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable Predicate<QTaskTree> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable Predicate<QTaskTree> done){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<QTaskTree, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<QTaskTree, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable BiConsumer<QTaskTree, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable BiConsumer<QTaskTree, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<QTaskTree> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<QTaskTree> done){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable Consumer<QTaskTree> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable Consumer<QTaskTree> done){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done);
	}
    public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<QTaskTree, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    	return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
    }
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable BiFunction<QTaskTree, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    		QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable BiFunction<QTaskTree, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<QTaskTree, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable BiPredicate<QTaskTree, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<QTaskTree> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable Predicate<QTaskTree> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<QTaskTree, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable BiConsumer<QTaskTree, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Function<QTaskTree, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<QTaskTree> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QTaskTree,QTaskTreeTaskAdapter> createQTaskTreeTask(
    		@Nullable Consumer<QTaskTree> setup,
    		@Nullable Consumer<QTaskTree> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QTaskTree.class, QTaskTreeTaskAdapter.class, setup, done, callDone);
	}
	
	// QProcessTask
    
    public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<QProcess, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone) {
    	return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
    }
    public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<QProcess, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
    	return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable BiFunction<QProcess, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, done);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable BiFunction<QProcess, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    		QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable BiFunction<QProcess, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable BiFunction<QProcess, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<QProcess, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<QProcess, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable Function<QProcess, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable Function<QProcess, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<QProcess, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<QProcess, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable BiPredicate<QProcess, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable BiPredicate<QProcess, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<QProcess> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<QProcess> done){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable Predicate<QProcess> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable Predicate<QProcess> done){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<QProcess, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<QProcess, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable BiConsumer<QProcess, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable BiConsumer<QProcess, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<QProcess> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<QProcess> done){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable Consumer<QProcess> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable Consumer<QProcess> done){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<QProcess, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    	return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
    }
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable BiFunction<QProcess, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    		QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable BiFunction<QProcess, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<QProcess, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable Function<QProcess, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<QProcess, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable BiPredicate<QProcess, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<QProcess> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable Predicate<QProcess> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<QProcess, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable BiConsumer<QProcess, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Function<QProcess, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<QProcess> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	public static QCustomTask<QProcess,QProcessTaskAdapter> createQProcessTask(
    		@Nullable Consumer<QProcess> setup,
    		@Nullable Consumer<QProcess> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QProcess.class, QProcessTaskAdapter.class, setup, done, callDone);
	}
	
	// QThreadFunctionTask
    
    public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone) {
    	return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
    }
    public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
    	return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass());
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable BiFunction<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), done);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable BiFunction<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    		QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable BiFunction<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable BiFunction<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable BiPredicate<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable BiPredicate<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<QThreadFunction<ResultType>> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<QThreadFunction<ResultType>> done){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable Predicate<QThreadFunction<ResultType>> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable Predicate<QThreadFunction<ResultType>> done){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable BiConsumer<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable BiConsumer<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<QThreadFunction<ResultType>> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<QThreadFunction<ResultType>> done){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable Consumer<QThreadFunction<ResultType>> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable Consumer<QThreadFunction<ResultType>> done){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    	return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
    }
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable BiFunction<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    		QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable BiFunction<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable BiPredicate<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<QThreadFunction<ResultType>> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable Predicate<QThreadFunction<ResultType>> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable BiConsumer<QThreadFunction<ResultType>, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Function<QThreadFunction<ResultType>, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<QThreadFunction<ResultType>> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunction<ResultType>,QThreadFunctionTaskAdapter<ResultType>> createQThreadFunctionTask(
    		@Nullable Consumer<QThreadFunction<ResultType>> setup,
    		@Nullable Consumer<QThreadFunction<ResultType>> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunction.typedClass(), QThreadFunctionTaskAdapter.typedClass(), setup, done, callDone);
	}
	
	// QThreadFunctionVoidTask
    
    public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone) {
    	return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
    }
    public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
    	return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable BiFunction<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, done);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable BiFunction<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    		QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable BiFunction<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable BiFunction<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable BiPredicate<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable BiPredicate<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<QThreadFunctionVoid> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<QThreadFunctionVoid> done){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable Predicate<QThreadFunctionVoid> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable Predicate<QThreadFunctionVoid> done){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable BiConsumer<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable BiConsumer<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<QThreadFunctionVoid> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<QThreadFunctionVoid> done){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable Consumer<QThreadFunctionVoid> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable Consumer<QThreadFunctionVoid> done){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    	return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
    }
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable BiFunction<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    		QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable BiFunction<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable BiPredicate<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<QThreadFunctionVoid> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable Predicate<QThreadFunctionVoid> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable BiConsumer<QThreadFunctionVoid, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Function<QThreadFunctionVoid, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<QThreadFunctionVoid> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	public static <ResultType> QCustomTask<QThreadFunctionVoid,QThreadFunctionVoidTaskAdapter> createQThreadFunctionVoidTask(
    		@Nullable Consumer<QThreadFunctionVoid> setup,
    		@Nullable Consumer<QThreadFunctionVoid> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(QThreadFunctionVoid.class, QThreadFunctionVoidTaskAdapter.class, setup, done, callDone);
	}
	
	// QDefaultTask
    
    public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone) {
    	return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
    }
    public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
    	return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass());
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), done);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    		QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<Task, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<Task, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Function<Task, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Function<Task, QtTaskTree.@NonNull DoneResult> done){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<Task, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<Task, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiPredicate<Task, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiPredicate<Task, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<Task> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<Task> done){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Predicate<Task> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Predicate<Task> done){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<Task, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<Task, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiConsumer<Task, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiConsumer<Task, QtTaskTree.@NonNull DoneWith> done){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<Task> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<Task> done){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Consumer<Task> done,
    	    QtTaskTree.@NonNull CallDone callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Consumer<Task> done){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
    	return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
    }
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    		QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiFunction<Task, QtTaskTree.@NonNull DoneWith, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Function<Task, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Function<Task, QtTaskTree.@NonNull DoneResult> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiPredicate<Task, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiPredicate<Task, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Predicate<Task> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Predicate<Task> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable BiConsumer<Task, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable BiConsumer<Task, QtTaskTree.@NonNull DoneWith> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Function<Task, QtTaskTree.@NonNull SetupResult> setup,
    		@Nullable Consumer<Task> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
	public static <Task extends QObject> QCustomTask<Task,QDefaultTaskAdapter<Task>> createQDefaultTask(@StrictNonNull Class<Task> taskType,
    		@Nullable Consumer<Task> setup,
    		@Nullable Consumer<Task> done,
    	    QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone){
		return new QCustomTask<>(taskType, QDefaultTaskAdapter.typedClass(), setup, done, callDone);
	}
}
