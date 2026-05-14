package io.qt.tasktree;

import java.util.Objects;
import java.util.function.*;

import io.qt.*;
import io.qt.core.*;

public final class ForEachItem<T> {
	ForEachItem(QList<T> list) {
		super();
		this.iterator = new ListIterator<>(list);
		this.forItem = new For(this.iterator);
	}
	private final ListIterator<T> iterator;
	private final For forItem;
	public @NonNull Group apply(@StrictNonNull Runnable item) {
		Objects.requireNonNull(item);
		return forItem.apply(new QSyncTask(item));
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
	public Group apply(@StrictNonNull Consumer<T> item) {
		Objects.requireNonNull(item);
		return forItem.apply(new QSyncTask(()->{item.accept(iterator.value());}));
	}
	public Group apply(@StrictNonNull ObjLongConsumer<T> item) {
		Objects.requireNonNull(item);
		return forItem.apply(new QSyncTask(()->{item.accept(iterator.value(), iterator.iteration());}));
	}
	public Group apply(@StrictNonNull Function<T,QtTaskTree.@NonNull DoneResult> item) {
		Objects.requireNonNull(item);
		return forItem.apply(new QSyncTask(()->{return item.apply(iterator.value());}));
	}
	public Group apply(BiFunction<T,@NonNull Long,QtTaskTree.@NonNull DoneResult> item) {
		Objects.requireNonNull(item);
		return forItem.apply(new QSyncTask(()->{return item.apply(iterator.value(), iterator.iteration());}));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull Consumer<T> item) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{item.accept(iterator.value());}));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull ObjLongConsumer<T> item) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{item.accept(iterator.value(), iterator.iteration());}));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull Function<T,QtTaskTree.@NonNull DoneResult> item) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{return item.apply(iterator.value());}));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull BiFunction<T,@NonNull Long,QtTaskTree.@NonNull DoneResult> item) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{return item.apply(iterator.value(), iterator.iteration());}));
	}
	public Group apply(@StrictNonNull Consumer<T> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{item.accept(iterator.value());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull ObjLongConsumer<T> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{item.accept(iterator.value(), iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull Function<T,QtTaskTree.DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{return item.apply(iterator.value());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull BiFunction<T,Long,QtTaskTree.DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{return item.apply(iterator.value(), iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull Consumer<T> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull...  callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{item.accept(iterator.value());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull ObjLongConsumer<T> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull...  callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{item.accept(iterator.value(), iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull Function<T,QtTaskTree.DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull...  callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{return item.apply(iterator.value());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull BiFunction<T,Long,QtTaskTree.DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull...  callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{return item.apply(iterator.value(), iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull Consumer<T> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{item.accept(iterator.value());}), QtTaskTree.onGroupDone(doneHandler, io.qt.tasktree.QtTaskTree.CallDoneFlag.Always.asFlags()));
	}
	public Group apply(@StrictNonNull ObjLongConsumer<T> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{item.accept(iterator.value(), iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, io.qt.tasktree.QtTaskTree.CallDoneFlag.Always.asFlags()));
	}
	public Group apply(@StrictNonNull Function<T,QtTaskTree.DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{return item.apply(iterator.value());}), QtTaskTree.onGroupDone(doneHandler, io.qt.tasktree.QtTaskTree.CallDoneFlag.Always.asFlags()));
	}
	public Group apply(@StrictNonNull BiFunction<T,Long,QtTaskTree.DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(new QSyncTask(()->{return item.apply(iterator.value(), iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, io.qt.tasktree.QtTaskTree.CallDoneFlag.Always.asFlags()));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull Consumer<T> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{item.accept(iterator.value());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull ObjLongConsumer<T> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{item.accept(iterator.value(), iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull Function<T,QtTaskTree.DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{return item.apply(iterator.value());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull BiFunction<T,Long,QtTaskTree.DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDone callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{return item.apply(iterator.value(), iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull Consumer<T> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull...  callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{item.accept(iterator.value());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull ObjLongConsumer<T> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull...  callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{item.accept(iterator.value(), iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull Function<T,QtTaskTree.DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull...  callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{return item.apply(iterator.value());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull BiFunction<T,Long,QtTaskTree.DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler, QtTaskTree.@NonNull CallDoneFlag @NonNull...  callDone) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{return item.apply(iterator.value(), iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, callDone));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull Consumer<T> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{item.accept(iterator.value());}), QtTaskTree.onGroupDone(doneHandler, io.qt.tasktree.QtTaskTree.CallDoneFlag.Always.asFlags()));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull ObjLongConsumer<T> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{item.accept(iterator.value(), iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, io.qt.tasktree.QtTaskTree.CallDoneFlag.Always.asFlags()));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull Function<T,QtTaskTree.DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{return item.apply(iterator.value());}), QtTaskTree.onGroupDone(doneHandler, io.qt.tasktree.QtTaskTree.CallDoneFlag.Always.asFlags()));
	}
	public Group apply(@StrictNonNull Supplier<QtTaskTree.@NonNull SetupResult> setupHandler, @StrictNonNull BiFunction<T,Long,QtTaskTree.DoneResult> item, @StrictNonNull Consumer<QtTaskTree.@NonNull DoneWith> doneHandler) {
		Objects.requireNonNull(item);
		Objects.requireNonNull(doneHandler);
		Objects.requireNonNull(setupHandler);
		return forItem.apply(QtTaskTree.onGroupSetup(setupHandler), new QSyncTask(()->{return item.apply(iterator.value(), iterator.iteration());}), QtTaskTree.onGroupDone(doneHandler, io.qt.tasktree.QtTaskTree.CallDoneFlag.Always.asFlags()));
	}
}
