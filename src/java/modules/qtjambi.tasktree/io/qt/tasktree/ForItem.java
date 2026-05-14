package io.qt.tasktree;

import java.util.Objects;
import java.util.function.*;

import io.qt.*;

public final class ForItem {
	ForItem(long count) {
		super();
		this.iterator = new RepeatIterator(count);
		this.forItem = new For(this.iterator);
	}
	ForItem(java.util.function.@NonNull LongPredicate condition) {
		super();
		this.iterator = new UntilIterator(condition);
		this.forItem = new For(this.iterator);
	}
	private final Iterator iterator;
	private final For forItem;
	public Group apply(Runnable item) {
		return forItem.apply(Group.onGroupDone(item));
	}
	public final @NonNull Group apply(@StrictNonNull Do doItem){
        return forItem.apply(doItem);
    }
    public final @NonNull Group apply(@NonNull GroupItem@NonNull... children){
        return forItem.apply(children);
    }
    public final @NonNull Group apply(java.util.@NonNull Collection<? extends @NonNull GroupItem> children){
        return forItem.apply(children);
    }
	public Group apply(@StrictNonNull LongConsumer item) {
		Objects.requireNonNull(item);
		return forItem.apply(new QSyncTask(()->{item.accept(iterator.iteration());}));
	}
	public Group apply(@StrictNonNull LongFunction<QtTaskTree.@NonNull DoneResult> item) {
		Objects.requireNonNull(item);
		return forItem.apply(new QSyncTask(()->{return item.apply(iterator.iteration());}));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull LongConsumer item) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{item.accept(iterator.iteration());}));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull LongFunction<QtTaskTree.@NonNull DoneResult> item) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{return item.apply(iterator.iteration());}));
	}
	public Group apply(@StrictNonNull LongConsumer item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{item.accept(iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull LongFunction<QtTaskTree.@NonNull DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{return item.apply(iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull LongConsumer item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{item.accept(iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull LongFunction<QtTaskTree.@NonNull DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{return item.apply(iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull LongConsumer item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{item.accept(iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, io.qt.tasktree.QtTaskTree.CallDoneFlag.Always.asFlags()));
	}
	public Group apply(@StrictNonNull LongFunction<QtTaskTree.@NonNull DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{return item.apply(iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, io.qt.tasktree.QtTaskTree.CallDoneFlag.Always.asFlags()));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull LongConsumer item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{item.accept(iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull LongFunction<QtTaskTree.@NonNull DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{return item.apply(iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull LongConsumer item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{item.accept(iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull LongFunction<QtTaskTree.@NonNull DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull... callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{return item.apply(iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull LongConsumer item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{item.accept(iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, io.qt.tasktree.QtTaskTree.CallDoneFlag.Always.asFlags()));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull LongFunction<QtTaskTree.@NonNull DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{return item.apply(iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, io.qt.tasktree.QtTaskTree.CallDoneFlag.Always.asFlags()));
	}
}
