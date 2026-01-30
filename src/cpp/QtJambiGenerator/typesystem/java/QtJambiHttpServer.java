/****************************************************************************
 **
 ** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
 **
 ** This file is part of Qt Jambi.
 **
 ** $BEGIN_LICENSE$
 **
 ** GNU Lesser General Public License Usage This file may be used under the terms
 ** of the GNU Lesser General Public License version 2.1 as published by the Free
 ** Software Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file. Please review the following information to ensure the
 ** GNU Lesser General Public License version 2.1 requirements will be met:
 ** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** GNU General Public License Usage Alternatively, this file may be used under
 ** the terms of the GNU General Public License version 3.0 as published by the
 ** Free Software Foundation and appearing in the file LICENSE.GPL included in
 ** the packaging of this file. Please review the following information to ensure
 ** the GNU General Public License version 3.0 requirements will be met:
 ** http://www.gnu.org/copyleft/gpl.html.
 **
 ** $END_LICENSE$
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

class QHttpServer___ {
    private QHttpServerRouter __rcRouter;

    @QtUninvokable
    private native void missingHandler(long __this__nativeId, long request, long argX);

    @QtUninvokable
    private native boolean handleRequest(long __this__nativeId, long request, long argX);

    @QtUninvokable
    private native void sendResponse(long __this__nativeId, long response, long request, long argX);

    public interface GenericViewHandler extends io.qt.core.QMetaObject.Slot3<@Nullable Object @NonNull[], @NonNull QHttpServerResponder, @NonNull QHttpServerRequest>{
    }

    public interface RequestViewHandler<R> extends io.qt.core.QMetaObject.Method2<@Nullable Object @NonNull[], @NonNull QHttpServerRequest, R>{
    }

    public interface SimpleViewHandler<R> extends io.qt.core.QMetaObject.Method1<@Nullable Object @NonNull[], R>{
    }

    public interface ResponderViewHandler extends io.qt.core.QMetaObject.Slot2<@Nullable Object @NonNull[], @NonNull QHttpServerResponder>{
    }

    private @Nullable Object @NonNull[] values(io.qt.core.@NonNull QRegularExpressionMatch rem, io.qt.core.@NonNull QMetaType @StrictNonNull[] metaTypes,
            @Nullable Object @StrictNonNull... requestResponder) {
        Object[] values = new Object[metaTypes.length + requestResponder.length];
        for (int i = 0; i < metaTypes.length; i++) {
            String cpt = rem.captured(i + 1);
            values[i] = io.qt.core.QVariant.convert(cpt, metaTypes[i]);
        }
        for (int i = 0; i < requestResponder.length; i++) {
            values[metaTypes.length + i] = requestResponder[i];
        }
        return values;
    }

    @QtUninvokable
    private QHttpServerRouterRule.RouterHandler createRouterHandlerRequest(
            io.qt.core.QMetaObject.AbstractSlot viewHandler, SlotInvoker slotInvoker, io.qt.core.QMetaType... metaTypes) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(viewHandler);
        return (rem, request, responder) -> {
            if(context==null || !context.isDisposed()) {
                Object[] values = values(rem, metaTypes, request);
                Object result;
                try {
                    result = slotInvoker.invoke(viewHandler, values);
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Throwable e) {
                    throw new RuntimeException(e);
                }
                sendResponse(result, request, responder);
            }
        };
    }

    @QtUninvokable
    private QHttpServerRouterRule.RouterHandler createRouterHandler(
            io.qt.core.QMetaObject.AbstractSlot viewHandler, SlotInvoker slotInvoker, io.qt.core.QMetaType... metaTypes) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(viewHandler);
        return (rem, request, responder) -> {
            if(context==null || !context.isDisposed()) {
                Object[] values = values(rem, metaTypes);
                Object result;
                try {
                    result = slotInvoker.invoke(viewHandler, values);
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Throwable e) {
                    throw new RuntimeException(e);
                }
                sendResponse(result, request, responder);
            }
        };
    }

    @QtUninvokable
    public QHttpServerRouterRule route(@NonNull String pathPattern, @NonNull GenericViewHandler viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(createHttpServerRouterRule(pathPattern, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @QtUninvokable
    public QHttpServerRouterRule route(
            java.util.function.@StrictNonNull Function<io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull QHttpServerRouterRule> ruleFactory,
            @NonNull GenericViewHandler viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(ruleFactory.apply(createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @QtUninvokable
    public QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods,
            @NonNull GenericViewHandler viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(
                createHttpServerRouterRule(pathPattern, methods, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @QtUninvokable
    public <R, Rule extends QHttpServerRouterRule> Rule route(
            java.util.function.@StrictNonNull Function<io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull Rule> ruleFactory,
            @Nullable RequestViewHandler<R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(ruleFactory.apply(createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @QtUninvokable
    public <R> QHttpServerRouterRule route(@NonNull String pathPattern,
            @Nullable RequestViewHandler<R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(createHttpServerRouterRule(pathPattern, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @QtUninvokable
    public <R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods,
            @Nullable RequestViewHandler<R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(
                createHttpServerRouterRule(pathPattern, methods, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @QtUninvokable
    public <Rule extends QHttpServerRouterRule> Rule route(
            java.util.function.@StrictNonNull Function<io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull Rule> ruleFactory,
            @Nullable ResponderViewHandler viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(ruleFactory.apply(createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @QtUninvokable
    public QHttpServerRouterRule route(@NonNull String pathPattern, @Nullable ResponderViewHandler viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(createHttpServerRouterRule(pathPattern, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @QtUninvokable
    public QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.Methods methods,
            @Nullable ResponderViewHandler viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(
                createHttpServerRouterRule(pathPattern, methods, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @QtUninvokable
    public <R, Rule extends QHttpServerRouterRule> Rule route(
            java.util.function.@StrictNonNull Function<io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull Rule> ruleFactory,
            @Nullable SimpleViewHandler<R> viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(ruleFactory.apply(createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @QtUninvokable
    public <R> QHttpServerRouterRule route(@NonNull String pathPattern, @Nullable SimpleViewHandler<R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(createHttpServerRouterRule(pathPattern, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @QtUninvokable
    public <R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods,
            @Nullable SimpleViewHandler<R> viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(
                createHttpServerRouterRule(pathPattern, methods, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @FunctionalInterface
    private interface SlotInvoker {
        Object invoke(io.qt.core.QMetaObject.AbstractSlot object, Object... args) throws Throwable;
    }

    private static class InvokableTypeInfo {
        SlotInvoker slotInvoker;
        java.lang.reflect.Parameter[] parameters;
        boolean hasReturnType;
        io.qt.core.QObject context;
    }

    @SuppressWarnings({ "unchecked", "rawtypes" })
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> InvokableTypeInfo type(
            ViewHandler slot, io.qt.core.QObject context) {
        if(context==null)
            context = findContext(slot);
        if(context==null)
            context = this;
        InvokableTypeInfo info = new InvokableTypeInfo();
        info.context = context;
        java.lang.reflect.Executable exec = QtJambi_LibraryUtilities.internal.lambdaExecutable(slot);
        java.lang.reflect.Method method = null;
        java.lang.reflect.Constructor constructor = null;
        if (exec instanceof java.lang.reflect.Method) {
            method = (java.lang.reflect.Method) exec;
            info.parameters = method.getParameters();
            info.hasReturnType = method.getReturnType() != void.class;
        } else if (exec instanceof java.lang.reflect.Constructor) {
            constructor = (java.lang.reflect.Constructor<?>) exec;
            info.parameters = constructor.getParameters();
            info.hasReturnType = true;
        } else {
            info.parameters = new java.lang.reflect.Parameter[0];
        }
        if (slot instanceof io.qt.core.QMetaObject.Slot0) {
            info.slotInvoker = (object, args) -> {
                ((io.qt.core.QMetaObject.Slot0) object).invoke();
                return null;
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Slot1) {
            info.slotInvoker = (object, args) -> {
                ((io.qt.core.QMetaObject.Slot1) object).invoke(args[0]);
                return null;
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Slot2) {
            info.slotInvoker = (object, args) -> {
                ((io.qt.core.QMetaObject.Slot2) object).invoke(args[0], args[1]);
                return null;
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Slot3) {
            info.slotInvoker = (object, args) -> {
                ((io.qt.core.QMetaObject.Slot3) object).invoke(args[0], args[1], args[2]);
                return null;
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Slot4) {
            info.slotInvoker = (object, args) -> {
                ((io.qt.core.QMetaObject.Slot4) object).invoke(args[0], args[1], args[2], args[3]);
                return null;
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Slot5) {
            info.slotInvoker = (object, args) -> {
                ((io.qt.core.QMetaObject.Slot5) object).invoke(args[0], args[1], args[2], args[3], args[4]);
                return null;
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Slot6) {
            info.slotInvoker = (object, args) -> {
                ((io.qt.core.QMetaObject.Slot6) object).invoke(args[0], args[1], args[2], args[3], args[4], args[5]);
                return null;
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Slot7) {
            info.slotInvoker = (object, args) -> {
                ((io.qt.core.QMetaObject.Slot7) object).invoke(args[0], args[1], args[2], args[3], args[4], args[5],
                        args[6]);
                return null;
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Slot8) {
            info.slotInvoker = (object, args) -> {
                ((io.qt.core.QMetaObject.Slot8) object).invoke(args[0], args[1], args[2], args[3], args[4], args[5],
                        args[6], args[7]);
                return null;
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Slot9) {
            info.slotInvoker = (object, args) -> {
                ((io.qt.core.QMetaObject.Slot9) object).invoke(args[0], args[1], args[2], args[3], args[4], args[5],
                        args[6], args[7], args[8]);
                return null;
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Method0) {
            info.slotInvoker = (object, args) -> {
                return ((io.qt.core.QMetaObject.Method0) object).invoke();
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Method1) {
            info.slotInvoker = (object, args) -> {
                return ((io.qt.core.QMetaObject.Method1) object).invoke(args[0]);
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Method2) {
            info.slotInvoker = (object, args) -> {
                return ((io.qt.core.QMetaObject.Method2) object).invoke(args[0], args[1]);
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Method3) {
            info.slotInvoker = (object, args) -> {
                return ((io.qt.core.QMetaObject.Method3) object).invoke(args[0], args[1], args[2]);
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Method4) {
            info.slotInvoker = (object, args) -> {
                return ((io.qt.core.QMetaObject.Method4) object).invoke(args[0], args[1], args[2], args[3]);
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Method5) {
            info.slotInvoker = (object, args) -> {
                return ((io.qt.core.QMetaObject.Method5) object).invoke(args[0], args[1], args[2], args[3], args[4]);
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Method6) {
            info.slotInvoker = (object, args) -> {
                return ((io.qt.core.QMetaObject.Method6) object).invoke(args[0], args[1], args[2], args[3], args[4],
                        args[5]);
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Method7) {
            info.slotInvoker = (object, args) -> {
                return ((io.qt.core.QMetaObject.Method7) object).invoke(args[0], args[1], args[2], args[3], args[4],
                        args[5], args[6]);
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Method8) {
            info.slotInvoker = (object, args) -> {
                return ((io.qt.core.QMetaObject.Method8) object).invoke(args[0], args[1], args[2], args[3], args[4],
                        args[5], args[6], args[7]);
            };
        } else if (slot instanceof io.qt.core.QMetaObject.Method9) {
            info.slotInvoker = (object, args) -> {
                return ((io.qt.core.QMetaObject.Method9) object).invoke(args[0], args[1], args[2], args[3], args[4],
                        args[5], args[6], args[7], args[8]);
            };
        }
        return info;
    }

    @QtUninvokable
    public <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule route(@NonNull String pathPattern, ViewHandler viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Slot0 viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Slot1<A> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Slot2<A,B> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Slot3<A,B,C> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Slot4<A,B,C,D> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Slot5<A,B,C,D,E> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Slot6<A,B,C,D,E,F> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Slot7<A,B,C,D,E,F,G> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,H> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Slot8<A,B,C,D,E,F,G,H> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,H,I> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Slot9<A,B,C,D,E,F,G,H,I> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Method0<R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Method1<A,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Method2<A,B,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Method3<A,B,C,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Method4<A,B,C,D,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Method5<A,B,C,D,E,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Method6<A,B,C,D,E,F,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Method7<A,B,C,D,E,F,G,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,H,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Method8<A,B,C,D,E,F,G,H,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,H,I,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.QMetaObject.@NonNull Method9<A,B,C,D,E,F,G,H,I,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot0 viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot1<A> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot2<A,B> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot3<A,B,C> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot4<A,B,C,D> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot5<A,B,C,D,E> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot6<A,B,C,D,E,F> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot7<A,B,C,D,E,F,G> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,H> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot8<A,B,C,D,E,F,G,H> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,H,I> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot9<A,B,C,D,E,F,G,H,I> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method0<R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method1<A,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method2<A,B,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method3<A,B,C,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method4<A,B,C,D,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method5<A,B,C,D,E,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method6<A,B,C,D,E,F,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method7<A,B,C,D,E,F,G,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,H,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method8<A,B,C,D,E,F,G,H,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,H,I,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method9<A,B,C,D,E,F,G,H,I,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot0 viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot1<A> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot2<A,B> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot3<A,B,C> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot4<A,B,C,D> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot5<A,B,C,D,E> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot6<A,B,C,D,E,F> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot7<A,B,C,D,E,F,G> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,H> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot8<A,B,C,D,E,F,G,H> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,H,I> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Slot9<A,B,C,D,E,F,G,H,I> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method0<R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method1<A,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method2<A,B,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method3<A,B,C,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method4<A,B,C,D,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method5<A,B,C,D,E,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method6<A,B,C,D,E,F,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method7<A,B,C,D,E,F,G,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,H,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method8<A,B,C,D,E,F,G,H,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,H,I,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@NonNull Method9<A,B,C,D,E,F,G,H,I,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Slot0 viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Slot1<A> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Slot2<A,B> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Slot3<A,B,C> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Slot4<A,B,C,D> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Slot5<A,B,C,D,E> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Slot6<A,B,C,D,E,F> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Slot7<A,B,C,D,E,F,G> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,H> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Slot8<A,B,C,D,E,F,G,H> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,H,I> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Slot9<A,B,C,D,E,F,G,H,I> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Method0<R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Method1<A,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Method2<A,B,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Method3<A,B,C,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Method4<A,B,C,D,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Method5<A,B,C,D,E,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Method6<A,B,C,D,E,F,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Method7<A,B,C,D,E,F,G,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,H,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Method8<A,B,C,D,E,F,G,H,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    public <A,B,C,D,E,F,G,H,I,R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@NonNull Method9<A,B,C,D,E,F,G,H,I,R> viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule route(String pathPattern, ViewHandler viewHandler, InvokableTypeInfo info) {
        if (info.slotInvoker == null)
            return null;
        java.lang.reflect.Parameter[] parameters = info.parameters;
        int requestArg = -1;
        int responderArg = -1;
        for (int i = 0; i < parameters.length; i++) {
            if (parameters[i].getType() == QHttpServerRequest.class) {
                requestArg = i;
            } else if (parameters[i].getType() == QHttpServerResponder.class) {
                responderArg = i;
            }
        }
        if (!info.hasReturnType) { // no responder!
            if (responderArg == -1) {
                throw new RuntimeException("QHttpServerResponder expected as argument.");
            } else {
                if (requestArg == -1) {
                    if (responderArg != parameters.length - 1) {
                        throw new RuntimeException("QHttpServerResponder expected to be last argument.");
                    }
                } else {
                    if (responderArg < parameters.length - 2) {
                        throw new RuntimeException(
                                "QHttpServerResponder and QHttpServerRequest expected to be last arguments.");
                    }
                    if (requestArg < parameters.length - 2) {
                        throw new RuntimeException(
                                "QHttpServerResponder and QHttpServerRequest expected to be last arguments.");
                    }
                }
            }
            io.qt.core.QMetaType[] metaTypes = new io.qt.core.QMetaType[parameters.length - (requestArg == -1 ? 1 : 2)];
            for (int i = 0; i < metaTypes.length; ++i) {
                metaTypes[i] = new io.qt.core.QMetaType(
                        QtJambi_LibraryUtilities.internal.registerMetaType(parameters[i]));
            }
            if (requestArg == -1) {
                return router().addRule(createHttpServerRouterRule(pathPattern,
                        createRouterHandlerResponder(viewHandler, info.slotInvoker, metaTypes), info.context), metaTypes);
            } else if (requestArg < responderArg) {
                return router().addRule(
                        createHttpServerRouterRule(pathPattern,
                                createRouterHandlerRequestResponder(viewHandler, info.slotInvoker, metaTypes), info.context),
                        metaTypes);
            } else {
                return router().addRule(
                        createHttpServerRouterRule(pathPattern,
                                createRouterHandlerResponderRequest(viewHandler, info.slotInvoker, metaTypes), info.context),
                        metaTypes);
            }
        } else {
            if (responderArg > 0) {
                throw new RuntimeException("QHttpServerResponder unexpected for suppliers.");
            }
            if (requestArg > 0 && requestArg != parameters.length - 1) {
                throw new RuntimeException("QHttpServerRequest expected to be last argument.");
            }
            io.qt.core.QMetaType[] metaTypes = new io.qt.core.QMetaType[parameters.length - (requestArg == -1 ? 0 : 1)];
            for (int i = 0; i < metaTypes.length; ++i) {
                metaTypes[i] = new io.qt.core.QMetaType(
                        QtJambi_LibraryUtilities.internal.registerMetaType(parameters[i]));
            }
            if (requestArg == -1) {
                return router().addRule(createHttpServerRouterRule(pathPattern,
                        createRouterHandler(viewHandler, info.slotInvoker, metaTypes), info.context), metaTypes);
            } else {
                return router().addRule(createHttpServerRouterRule(pathPattern,
                        createRouterHandlerRequest(viewHandler, info.slotInvoker, metaTypes), info.context), metaTypes);
            }
        }
    }

    @QtUninvokable
    public <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot, Rule extends QHttpServerRouterRule> Rule route(
            java.util.function.@StrictNonNull Function<io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull Rule> ruleFactory,
            @NonNull ViewHandler viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        if (info.slotInvoker == null)
            return null;
        java.lang.reflect.Parameter[] parameters = info.parameters;
        int requestArg = -1;
        int responderArg = -1;
        for (int i = 0; i < parameters.length; i++) {
            if (parameters[i].getType() == QHttpServerRequest.class) {
                requestArg = i;
            } else if (parameters[i].getType() == QHttpServerResponder.class) {
                responderArg = i;
            }
        }
        if (!info.hasReturnType) { // no responder!
            if (responderArg == -1) {
                throw new RuntimeException("QHttpServerResponder expected as argument.");
            } else {
                if (requestArg == -1) {
                    if (responderArg != parameters.length - 1) {
                        throw new RuntimeException("QHttpServerResponder expected to be last argument.");
                    }
                } else {
                    if (responderArg < parameters.length - 2) {
                        throw new RuntimeException(
                                "QHttpServerResponder and QHttpServerRequest expected to be last arguments.");
                    }
                    if (requestArg < parameters.length - 2) {
                        throw new RuntimeException(
                                "QHttpServerResponder and QHttpServerRequest expected to be last arguments.");
                    }
                }
            }
            io.qt.core.QMetaType[] metaTypes = new io.qt.core.QMetaType[parameters.length - (requestArg == -1 ? 1 : 2)];
            for (int i = 0; i < metaTypes.length; ++i) {
                metaTypes[i] = new io.qt.core.QMetaType(
                        QtJambi_LibraryUtilities.internal.registerMetaType(parameters[i]));
            }
            if (requestArg == -1) {
                return router().addRule(
                        ruleFactory.apply(createRouterHandlerResponder(viewHandler, info.slotInvoker, metaTypes)),
                        metaTypes);
            } else if (requestArg < responderArg) {
                return router().addRule(
                        ruleFactory
                                .apply(createRouterHandlerRequestResponder(viewHandler, info.slotInvoker, metaTypes)),
                        metaTypes);
            } else {
                return router().addRule(
                        ruleFactory
                                .apply(createRouterHandlerResponderRequest(viewHandler, info.slotInvoker, metaTypes)),
                        metaTypes);
            }
        } else {
            if (responderArg > 0) {
                throw new RuntimeException("QHttpServerResponder unexpected for suppliers.");
            }
            if (requestArg > 0 && requestArg != parameters.length - 1) {
                throw new RuntimeException("QHttpServerRequest expected to be last argument.");
            }
            io.qt.core.QMetaType[] metaTypes = new io.qt.core.QMetaType[parameters.length - (requestArg == -1 ? 0 : 1)];
            for (int i = 0; i < metaTypes.length; ++i) {
                metaTypes[i] = new io.qt.core.QMetaType(
                        QtJambi_LibraryUtilities.internal.registerMetaType(parameters[i]));
            }
            if (requestArg == -1) {
                return router().addRule(
                        ruleFactory.apply(createRouterHandler(viewHandler, info.slotInvoker, metaTypes)), metaTypes);
            } else {
                return router().addRule(
                        ruleFactory.apply(createRouterHandlerRequest(viewHandler, info.slotInvoker, metaTypes)),
                        metaTypes);
            }
        }
    }

    @QtUninvokable
    public <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule route(@NonNull String pathPattern,
            io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, @NonNull ViewHandler viewHandler) {
        InvokableTypeInfo info = type(viewHandler, null);
        return route(pathPattern, methods, viewHandler, info);
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule route(String pathPattern,
            io.qt.httpserver.QHttpServerRequest.Methods methods,
            ViewHandler viewHandler, InvokableTypeInfo info) {
        if (info.slotInvoker == null)
            return null;
        java.lang.reflect.Parameter[] parameters = info.parameters;
        int requestArg = -1;
        int responderArg = -1;
        for (int i = 0; i < parameters.length; i++) {
            if (parameters[i].getType() == QHttpServerRequest.class) {
                requestArg = i;
            } else if (parameters[i].getType() == QHttpServerResponder.class) {
                responderArg = i;
            }
        }
        if (!info.hasReturnType) { // no responder!
            if (responderArg == -1) {
                throw new RuntimeException("QHttpServerResponder expected as argument.");
            } else {
                if (requestArg == -1) {
                    if (responderArg != parameters.length - 1) {
                        throw new RuntimeException("QHttpServerResponder expected to be last argument.");
                    }
                } else {
                    if (responderArg < parameters.length - 2) {
                        throw new RuntimeException(
                                "QHttpServerResponder and QHttpServerRequest expected to be last arguments.");
                    }
                    if (requestArg < parameters.length - 2) {
                        throw new RuntimeException(
                                "QHttpServerResponder and QHttpServerRequest expected to be last arguments.");
                    }
                }
            }
            io.qt.core.QMetaType[] metaTypes = new io.qt.core.QMetaType[parameters.length - (requestArg == -1 ? 1 : 2)];
            for (int i = 0; i < metaTypes.length; ++i) {
                metaTypes[i] = new io.qt.core.QMetaType(
                        QtJambi_LibraryUtilities.internal.registerMetaType(parameters[i]));
            }
            if (requestArg == -1) {
                return router().addRule(createHttpServerRouterRule(pathPattern, methods,
                        createRouterHandlerResponder(viewHandler, info.slotInvoker, metaTypes), info.context), metaTypes);
            } else if (requestArg < responderArg) {
                return router().addRule(
                        createHttpServerRouterRule(pathPattern, methods,
                                createRouterHandlerRequestResponder(viewHandler, info.slotInvoker, metaTypes), info.context),
                        metaTypes);
            } else {
                return router().addRule(
                        createHttpServerRouterRule(pathPattern, methods,
                                createRouterHandlerResponderRequest(viewHandler, info.slotInvoker, metaTypes), info.context),
                        metaTypes);
            }
        } else {
            if (responderArg > 0) {
                throw new RuntimeException("QHttpServerResponder unexpected for suppliers.");
            }
            if (requestArg > 0 && requestArg != parameters.length - 1) {
                throw new RuntimeException("QHttpServerRequest expected to be last argument.");
            }
            io.qt.core.QMetaType[] metaTypes = new io.qt.core.QMetaType[parameters.length - (requestArg == -1 ? 0 : 1)];
            for (int i = 0; i < metaTypes.length; ++i) {
                metaTypes[i] = new io.qt.core.QMetaType(
                        QtJambi_LibraryUtilities.internal.registerMetaType(parameters[i]));
            }
            if (requestArg == -1) {
                return router().addRule(createHttpServerRouterRule(pathPattern, methods,
                        createRouterHandler(viewHandler, info.slotInvoker, metaTypes), info.context), metaTypes);
            } else {
                return router().addRule(createHttpServerRouterRule(pathPattern, methods,
                        createRouterHandlerRequest(viewHandler, info.slotInvoker, metaTypes), info.context), metaTypes);
            }
        }
    }
}// class

class QHttpServer_64__ {

    @QtUninvokable
    public void afterRequest(java.util.function.@Nullable Function<@NonNull QHttpServerResponse, @NonNull QHttpServerResponse> action) {
        afterRequest(QtJambi_LibraryUtilities.internal.nativeId(this), false, action);
    }

    @QtUninvokable
    public void afterRequest(
            java.util.function.@Nullable BiFunction<@NonNull QHttpServerResponse, @NonNull QHttpServerRequest, @NonNull QHttpServerResponse> action) {
        afterRequest(QtJambi_LibraryUtilities.internal.nativeId(this), true, action);
    }

    @QtUninvokable
    private native void afterRequest(long __this__nativeId, boolean isBi, Object action);

    @Override
    protected void missingHandler(@NonNull QHttpServerRequest request, io.qt.network.@Nullable QTcpSocket socket) {
        missingHandler(QtJambi_LibraryUtilities.internal.nativeId(this),
                QtJambi_LibraryUtilities.internal.checkedNativeId(request),
                QtJambi_LibraryUtilities.internal.checkedNativeId(socket));
    }

    @Override
    protected boolean handleRequest(@NonNull QHttpServerRequest request, io.qt.network.@Nullable QTcpSocket socket) {
        return handleRequest(QtJambi_LibraryUtilities.internal.nativeId(this),
                QtJambi_LibraryUtilities.internal.checkedNativeId(request),
                QtJambi_LibraryUtilities.internal.checkedNativeId(socket));
    }

    @QtUninvokable
    private void sendResponse(QHttpServerResponse response, QHttpServerRequest request, io.qt.network.QTcpSocket socket) {
        sendResponse(QtJambi_LibraryUtilities.internal.nativeId(this),
                QtJambi_LibraryUtilities.internal.checkedNativeId(response),
                QtJambi_LibraryUtilities.internal.checkedNativeId(request),
                QtJambi_LibraryUtilities.internal.checkedNativeId(socket));
    }

    @QtUninvokable
    private <R> QHttpServerRouterRule.@NonNull RouterHandler createRouterHandler(
            RequestViewHandler<R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(viewHandler);
        return (rem, request, responder) -> {
            if(context==null || !context.isDisposed()) {
                Object[] values = values(rem, metaTypes);
                R result;
                try {
                    result = viewHandler.invoke(values, request);
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Throwable e) {
                    throw new RuntimeException(e);
                }
                sendResponse(result, request, responder);
            }
        };
    }

    @QtUninvokable
    private <R> QHttpServerRouterRule.@NonNull RouterHandler createRouterHandler(
            SimpleViewHandler<R> viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(viewHandler);
        return (rem, request, responder) -> {
            if(context==null || !context.isDisposed()) {
                Object[] values = values(rem, metaTypes);
                R result;
                try {
                    result = viewHandler.invoke(values);
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Throwable e) {
                    throw new RuntimeException(e);
                }
                sendResponse(result, request, responder);
            }
        };
    }

    @QtUninvokable
    private void sendResponse(Object result, QHttpServerRequest request, io.qt.network.QTcpSocket socket) {
        if (result instanceof QHttpServerResponse)
            sendResponse((QHttpServerResponse) result, request, socket);
        else if (result instanceof String)
            sendResponse(new QHttpServerResponse((String) result), request, socket);
        else if (result instanceof io.qt.core.QByteArray)
            sendResponse(new QHttpServerResponse((io.qt.core.QByteArray) result), request, socket);
        else if (result instanceof io.qt.core.QJsonArray)
            sendResponse(new QHttpServerResponse((io.qt.core.QJsonArray) result), request, socket);
        else if (result instanceof io.qt.core.QJsonObject)
            sendResponse(new QHttpServerResponse((io.qt.core.QJsonObject) result), request, socket);
        else if (result == null)
            sendResponse(new QHttpServerResponse(QHttpServerResponder.StatusCode.NoContent), request, socket);
        else
            sendResponse(new QHttpServerResponse(result.toString()), request, socket);
    }

    @QtUninvokable
    private QHttpServerRouterRule.RouterHandler createRouterHandler(GenericViewHandler viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(viewHandler);
        return (rem, request, socket) -> {
            if(context==null || !context.isDisposed()) {
                Object[] values = values(rem, metaTypes);
                try{
                    viewHandler.invoke(values, makeResponder(request, socket), request);
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Throwable e) {
                    throw new RuntimeException(e);
                }
            }
        };
    }

    @QtUninvokable
    private QHttpServerRouterRule.RouterHandler createRouterHandler(
            ResponderViewHandler viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(viewHandler);
        return (rem, request, socket) -> {
            if(context==null || !context.isDisposed()) {
                Object[] values = values(rem, metaTypes);
                try{
                    viewHandler.invoke(values, makeResponder(request, socket));
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Throwable e) {
                    throw new RuntimeException(e);
                }
            }
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerResponder(
            ViewHandler viewHandler, SlotInvoker slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(viewHandler);
        return (rem, request, socket) -> {
            if(context==null || !context.isDisposed()) {
                Object[] values = values(rem, metaTypes, makeResponder(request, socket));
                try {
                    slotInvoker.invoke(viewHandler, values);
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Throwable e) {
                    throw new RuntimeException(e);
                }
            }
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerRequestResponder(
            ViewHandler viewHandler, SlotInvoker slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(viewHandler);
        return (rem, request, socket) -> {
            if(context==null || !context.isDisposed()) {
                Object[] values = values(rem, metaTypes, request, makeResponder(request, socket));
                try {
                    slotInvoker.invoke(viewHandler, values);
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Throwable e) {
                    throw new RuntimeException(e);
                }
            }
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerResponderRequest(
            ViewHandler viewHandler, SlotInvoker slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(viewHandler);
        return (rem, request, socket) -> {
            if(context==null || !context.isDisposed()) {
                Object[] values = values(rem, metaTypes, makeResponder(request, socket), request);
                try {
                    slotInvoker.invoke(viewHandler, values);
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Throwable e) {
                    throw new RuntimeException(e);
                }
            }
        };
    }

    private QHttpServerRouterRule createHttpServerRouterRule(String pathPattern,
                                                            io.qt.httpserver.QHttpServerRouterRule.RouterHandler handler){
        return new QHttpServerRouterRule(pathPattern, handler);
    }

    private QHttpServerRouterRule createHttpServerRouterRule(String pathPattern,
                                                            io.qt.httpserver.QHttpServerRequest.Methods methods,
                                                            io.qt.httpserver.QHttpServerRouterRule.RouterHandler handler){
        return new QHttpServerRouterRule(pathPattern, methods, handler);
    }

    private QHttpServerRouterRule createHttpServerRouterRule(String pathPattern,
                                                            io.qt.httpserver.QHttpServerRouterRule.RouterHandler handler,
                                                            io.qt.core.QObject context){
        return new QHttpServerRouterRule(pathPattern, handler);
    }

    private QHttpServerRouterRule createHttpServerRouterRule(String pathPattern,
                                                            io.qt.httpserver.QHttpServerRequest.Methods methods,
                                                            io.qt.httpserver.QHttpServerRouterRule.RouterHandler handler,
                                                            io.qt.core.QObject context){
        return new QHttpServerRouterRule(pathPattern, methods, handler);
    }

    private io.qt.core.QObject findContext(io.qt.core.QMetaObject.AbstractSlot slot){ return null; }
}// class

class QHttpServer_65__ {

    @QtUninvokable
    private native void sendResponseFuture(long __this__nativeId, io.qt.core.QFuture<QHttpServerResponse> response, long request, long responder);

    @Override
    protected void missingHandler(@NonNull QHttpServerRequest request, @NonNull QHttpServerResponder responder) {
        missingHandler(QtJambi_LibraryUtilities.internal.nativeId(this),
                QtJambi_LibraryUtilities.internal.checkedNativeId(request),
                QtJambi_LibraryUtilities.internal.checkedNativeId(responder));
    }

    @Override
    protected boolean handleRequest(@NonNull QHttpServerRequest request, @NonNull QHttpServerResponder responder) {
        return handleRequest(QtJambi_LibraryUtilities.internal.nativeId(this),
                QtJambi_LibraryUtilities.internal.checkedNativeId(request),
                QtJambi_LibraryUtilities.internal.checkedNativeId(responder));
    }

    @QtUninvokable
    private void sendResponse(QHttpServerResponse response, QHttpServerRequest request, QHttpServerResponder responder) {
        sendResponse(QtJambi_LibraryUtilities.internal.nativeId(this),
                QtJambi_LibraryUtilities.internal.checkedNativeId(response),
                QtJambi_LibraryUtilities.internal.checkedNativeId(request),
                QtJambi_LibraryUtilities.internal.checkedNativeId(responder));
    }

    @QtUninvokable
    private void sendResponse(io.qt.core.QFuture<QHttpServerResponse> response, QHttpServerRequest request, QHttpServerResponder responder) {
        sendResponseFuture(QtJambi_LibraryUtilities.internal.nativeId(this),
                response,
                QtJambi_LibraryUtilities.internal.checkedNativeId(request),
                QtJambi_LibraryUtilities.internal.checkedNativeId(responder));
    }

    @SuppressWarnings("unchecked")
    @QtUninvokable
    private void sendResponse(Object result, QHttpServerRequest request, QHttpServerResponder responder) {
        if (result instanceof QHttpServerResponse)
            sendResponse((QHttpServerResponse) result, request, responder);
        else if (result instanceof String)
            sendResponse(new QHttpServerResponse((String) result), request, responder);
        else if (result instanceof io.qt.core.QByteArray)
            sendResponse(new QHttpServerResponse((io.qt.core.QByteArray) result), request, responder);
        else if (result instanceof io.qt.core.QJsonArray)
            sendResponse(new QHttpServerResponse((io.qt.core.QJsonArray) result), request, responder);
        else if (result instanceof io.qt.core.QJsonObject)
            sendResponse(new QHttpServerResponse((io.qt.core.QJsonObject) result), request, responder);
        else if (result instanceof io.qt.core.QFuture)
            sendResponse((io.qt.core.QFuture<QHttpServerResponse>) result, request, responder);
        else if (result == null)
            sendResponse(new QHttpServerResponse(QHttpServerResponder.StatusCode.NoContent), request, responder);
        else
            sendResponse(new QHttpServerResponse(result.toString()), request, responder);
    }
}// class

class QHttpServer_65_7__ {

    @QtUninvokable
    public void afterRequest(java.util.function.@Nullable Function<@NonNull QHttpServerResponse, @NonNull QHttpServerResponse> action) {
        afterRequest(QtJambi_LibraryUtilities.internal.nativeId(this), false, action);
    }

    @QtUninvokable
    public void afterRequest(
            java.util.function.@Nullable BiFunction<@NonNull QHttpServerResponse, @NonNull QHttpServerRequest, @NonNull QHttpServerResponse> action) {
        afterRequest(QtJambi_LibraryUtilities.internal.nativeId(this), true, action);
    }

    @QtUninvokable
    private native void afterRequest(long __this__nativeId, boolean isBi, Object action);

    private QHttpServerRouterRule createHttpServerRouterRule(String pathPattern,
                                                            io.qt.httpserver.QHttpServerRouterRule.RouterHandler handler,
                                                            io.qt.core.QObject context){
        return new QHttpServerRouterRule(pathPattern, handler);
    }

    private QHttpServerRouterRule createHttpServerRouterRule(String pathPattern,
                                                            io.qt.httpserver.QHttpServerRequest.Methods methods,
                                                            io.qt.httpserver.QHttpServerRouterRule.RouterHandler handler,
                                                            io.qt.core.QObject context){
        return new QHttpServerRouterRule(pathPattern, methods, handler);
    }

    private QHttpServerRouterRule createHttpServerRouterRule(String pathPattern,
                                                            io.qt.httpserver.QHttpServerRouterRule.RouterHandler handler){
        return new QHttpServerRouterRule(pathPattern, handler);
    }

    private QHttpServerRouterRule createHttpServerRouterRule(String pathPattern,
                                                            io.qt.httpserver.QHttpServerRequest.Methods methods,
                                                            io.qt.httpserver.QHttpServerRouterRule.RouterHandler handler){
        return new QHttpServerRouterRule(pathPattern, methods, handler);
    }

    @QtUninvokable
    private <R> QHttpServerRouterRule.@NonNull RouterHandler createRouterHandler(
            RequestViewHandler<R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(viewHandler);
        return (rem, request, responder) -> {
            if(context==null || !context.isDisposed()) {
                Object[] values = values(rem, metaTypes);
                R result;
                try {
                    result = viewHandler.invoke(values, request);
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Throwable e) {
                    throw new RuntimeException(e);
                }
                sendResponse(result, request, responder);
            }
        };
    }

    @QtUninvokable
    private <R> QHttpServerRouterRule.@NonNull RouterHandler createRouterHandler(
            SimpleViewHandler<R> viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(viewHandler);
        return (rem, request, responder) -> {
            if(context==null || !context.isDisposed()) {
                Object[] values = values(rem, metaTypes);
                R result;
                try {
                    result = viewHandler.invoke(values);
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Throwable e) {
                    throw new RuntimeException(e);
                }
                sendResponse(result, request, responder);
            }
        };
    }

    @QtUninvokable
    private QHttpServerRouterRule.RouterHandler createRouterHandler(GenericViewHandler viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(viewHandler);
        return (rem, request, responder) -> {
            if(context==null || !context.isDisposed()) {
                Object[] values = values(rem, metaTypes);
                try {
                    viewHandler.invoke(values, responder, request);
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Throwable e) {
                    throw new RuntimeException(e);
                }
            }
        };
    }

    @QtUninvokable
    private QHttpServerRouterRule.RouterHandler createRouterHandler(
            ResponderViewHandler viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(viewHandler);
        return (rem, request, responder) -> {
            if(context==null || !context.isDisposed()) {
                Object[] values = values(rem, metaTypes);
                try {
                    viewHandler.invoke(values, responder);
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Throwable e) {
                    throw new RuntimeException(e);
                }
            }
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerResponder(
            ViewHandler viewHandler, SlotInvoker slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(viewHandler);
        return (rem, request, responder) -> {
            if(context==null || !context.isDisposed()) {
                Object[] values = values(rem, metaTypes, responder);
                try {
                    slotInvoker.invoke(viewHandler, values);
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Throwable e) {
                    throw new RuntimeException(e);
                }
            }
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerRequestResponder(
            ViewHandler viewHandler, SlotInvoker slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(viewHandler);
        return (rem, request, responder) -> {
            if(context==null || !context.isDisposed()) {
                Object[] values = values(rem, metaTypes, request, responder);
                try {
                    slotInvoker.invoke(viewHandler, values);
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Throwable e) {
                    throw new RuntimeException(e);
                }
            }
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerResponderRequest(
            ViewHandler viewHandler, SlotInvoker slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(viewHandler);
        return (rem, request, responder) -> {
            if(context==null || !context.isDisposed()) {
                Object[] values = values(rem, metaTypes, responder, request);
                try {
                    slotInvoker.invoke(viewHandler, values);
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Throwable e) {
                    throw new RuntimeException(e);
                }
            }
        };
    }

    private io.qt.core.QObject findContext(io.qt.core.QMetaObject.AbstractSlot slot){ return null; }
}// class

class QHttpServer_68__ {
    public interface MissingHandlerHandler extends io.qt.core.QMetaObject.Slot2<@NonNull QHttpServerRequest, @StrictNonNull QHttpServerResponder>{
    }

    public interface AfterRequestHandler extends io.qt.core.QMetaObject.Slot2<@NonNull QHttpServerRequest, @StrictNonNull QHttpServerResponse>{
    }

    private QHttpServerRouterRule createHttpServerRouterRule(String pathPattern,
                                                            io.qt.httpserver.QHttpServerRouterRule.RouterHandler handler,
                                                            io.qt.core.QObject context){
        return new QHttpServerRouterRule(pathPattern, context, handler);
    }

    private QHttpServerRouterRule createHttpServerRouterRule(String pathPattern,
                                                            io.qt.httpserver.QHttpServerRequest.Methods methods,
                                                            io.qt.httpserver.QHttpServerRouterRule.RouterHandler handler,
                                                            io.qt.core.QObject context){
        return new QHttpServerRouterRule(pathPattern, methods, context, handler);
    }

    private QHttpServerRouterRule createHttpServerRouterRule(String pathPattern,
                                                            io.qt.httpserver.QHttpServerRouterRule.RouterHandler handler){
        return new QHttpServerRouterRule(pathPattern, handler);
    }

    private QHttpServerRouterRule createHttpServerRouterRule(String pathPattern,
                                                            io.qt.httpserver.QHttpServerRequest.Methods methods,
                                                            io.qt.httpserver.QHttpServerRouterRule.RouterHandler handler){
        return new QHttpServerRouterRule(pathPattern, methods, handler);
    }

    private io.qt.core.QObject findContext(io.qt.core.QMetaObject.AbstractSlot slot){ return QtJambi_LibraryUtilities.internal.lambdaContext(slot); }

    @QtUninvokable
    private <R> QHttpServerRouterRule.@NonNull RouterHandler createRouterHandler(
            RequestViewHandler<R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, responder) -> {
            Object[] values = values(rem, metaTypes);
            R result = viewHandler.invoke(values, request);
            sendResponse(result, request, responder);
        };
    }

    @QtUninvokable
    private <R> QHttpServerRouterRule.@NonNull RouterHandler createRouterHandler(
            SimpleViewHandler<R> viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, responder) -> {
            Object[] values = values(rem, metaTypes);
            R result = viewHandler.invoke(values);
            sendResponse(result, request, responder);
        };
    }

    @QtUninvokable
    private QHttpServerRouterRule.RouterHandler createRouterHandler(GenericViewHandler viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, responder) -> {
            Object[] values = values(rem, metaTypes);
            viewHandler.invoke(values, responder, request);
        };
    }

    @QtUninvokable
    private QHttpServerRouterRule.RouterHandler createRouterHandler(
            ResponderViewHandler viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, responder) -> {
            Object[] values = values(rem, metaTypes);
            viewHandler.invoke(values, responder);
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerResponder(
            ViewHandler viewHandler, SlotInvoker slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, responder) -> {
            Object[] values = values(rem, metaTypes, responder);
            slotInvoker.invoke(viewHandler, values);
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerRequestResponder(
            ViewHandler viewHandler, SlotInvoker slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, responder) -> {
            Object[] values = values(rem, metaTypes, request, responder);
            slotInvoker.invoke(viewHandler, values);
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerResponderRequest(
            ViewHandler viewHandler, SlotInvoker slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, responder) -> {
            Object[] values = values(rem, metaTypes, responder, request);
            slotInvoker.invoke(viewHandler, values);
        };
    }

    @QtUninvokable
    public <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot, Rule extends QHttpServerRouterRule> Rule route(
            java.util.function.@StrictNonNull BiFunction<io.qt.core.QObject, io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull Rule> ruleFactory,
            @NonNull ViewHandler viewHandler) {
        return route(ruleFactory, null, viewHandler);
    }

    @QtUninvokable
    public <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot, Rule extends QHttpServerRouterRule> Rule route(
            java.util.function.@StrictNonNull BiFunction<io.qt.core.QObject, io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull Rule> ruleFactory,
            io.qt.core.@Nullable QObject context, @NonNull ViewHandler viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        if (info.slotInvoker == null)
            return null;
        java.lang.reflect.Parameter[] parameters = info.parameters;
        int requestArg = -1;
        int responderArg = -1;
        for (int i = 0; i < parameters.length; i++) {
            if (parameters[i].getType() == QHttpServerRequest.class) {
                requestArg = i;
            } else if (parameters[i].getType() == QHttpServerResponder.class) {
                responderArg = i;
            }
        }
        if (!info.hasReturnType) { // no responder!
            if (responderArg == -1) {
                throw new RuntimeException("QHttpServerResponder expected as argument.");
            } else {
                if (requestArg == -1) {
                    if (responderArg != parameters.length - 1) {
                        throw new RuntimeException("QHttpServerResponder expected to be last argument.");
                    }
                } else {
                    if (responderArg < parameters.length - 2) {
                        throw new RuntimeException(
                                "QHttpServerResponder and QHttpServerRequest expected to be last arguments.");
                    }
                    if (requestArg < parameters.length - 2) {
                        throw new RuntimeException(
                                "QHttpServerResponder and QHttpServerRequest expected to be last arguments.");
                    }
                }
            }
            io.qt.core.QMetaType[] metaTypes = new io.qt.core.QMetaType[parameters.length - (requestArg == -1 ? 1 : 2)];
            for (int i = 0; i < metaTypes.length; ++i) {
                metaTypes[i] = new io.qt.core.QMetaType(
                        QtJambi_LibraryUtilities.internal.registerMetaType(parameters[i]));
            }
            if (requestArg == -1) {
                return router().addRule(
                        ruleFactory.apply(info.context, createRouterHandlerResponder(viewHandler, info.slotInvoker, metaTypes)),
                        metaTypes);
            } else if (requestArg < responderArg) {
                return router().addRule(
                        ruleFactory
                                .apply(info.context, createRouterHandlerRequestResponder(viewHandler, info.slotInvoker, metaTypes)),
                        metaTypes);
            } else {
                return router().addRule(
                        ruleFactory
                                .apply(info.context, createRouterHandlerResponderRequest(viewHandler, info.slotInvoker, metaTypes)),
                        metaTypes);
            }
        } else {
            if (responderArg > 0) {
                throw new RuntimeException("QHttpServerResponder unexpected for suppliers.");
            }
            if (requestArg > 0 && requestArg != parameters.length - 1) {
                throw new RuntimeException("QHttpServerRequest expected to be last argument.");
            }
            io.qt.core.QMetaType[] metaTypes = new io.qt.core.QMetaType[parameters.length - (requestArg == -1 ? 0 : 1)];
            for (int i = 0; i < metaTypes.length; ++i) {
                metaTypes[i] = new io.qt.core.QMetaType(
                        QtJambi_LibraryUtilities.internal.registerMetaType(parameters[i]));
            }
            if (requestArg == -1) {
                return router().addRule(
                        ruleFactory.apply(info.context, createRouterHandler(viewHandler, info.slotInvoker, metaTypes)), metaTypes);
            } else {
                return router().addRule(
                        ruleFactory.apply(info.context, createRouterHandlerRequest(viewHandler, info.slotInvoker, metaTypes)),
                        metaTypes);
            }
        }
    }

    @QtUninvokable
    public QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, @NonNull GenericViewHandler viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(createHttpServerRouterRule(pathPattern, createRouterHandler(viewHandler, metaTypes), context),
                metaTypes);
    }

    @QtUninvokable
    public <Rule extends QHttpServerRouterRule> Rule route(
            java.util.function.@StrictNonNull BiFunction<io.qt.core.@Nullable QObject,io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull Rule> ruleFactory,
            io.qt.core.@Nullable QObject context, @NonNull GenericViewHandler viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(ruleFactory.apply(context, createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @QtUninvokable
    public <Rule extends QHttpServerRouterRule> Rule route(
            java.util.function.@StrictNonNull BiFunction<io.qt.core.@Nullable QObject,io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull Rule> ruleFactory,
            @NonNull GenericViewHandler viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(ruleFactory.apply(findContext(viewHandler), createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @QtUninvokable
    public QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods,
            io.qt.core.@Nullable QObject context, @NonNull GenericViewHandler viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(
                createHttpServerRouterRule(pathPattern, methods, createRouterHandler(viewHandler, metaTypes), context),
                metaTypes);
    }

    @QtUninvokable
    public <R, Rule extends QHttpServerRouterRule> Rule route(
            java.util.function.@StrictNonNull BiFunction<io.qt.core.@Nullable QObject, io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull Rule> ruleFactory,
            io.qt.core.@Nullable QObject context, @Nullable RequestViewHandler<R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(ruleFactory.apply(context, createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @QtUninvokable
    public <R, Rule extends QHttpServerRouterRule> Rule route(
            java.util.function.@StrictNonNull BiFunction<io.qt.core.@Nullable QObject, io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull Rule> ruleFactory,
            @Nullable RequestViewHandler<R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(ruleFactory.apply(findContext(viewHandler), createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @QtUninvokable
    public <R> QHttpServerRouterRule route(@NonNull String pathPattern,
            io.qt.core.@Nullable QObject context,
            @Nullable RequestViewHandler<R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(createHttpServerRouterRule(pathPattern, createRouterHandler(viewHandler, metaTypes), context),
                metaTypes);
    }

    @QtUninvokable
    public <R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods,
            io.qt.core.@Nullable QObject context,
            @Nullable RequestViewHandler<R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(
                createHttpServerRouterRule(pathPattern, methods, createRouterHandler(viewHandler, metaTypes), context),
                metaTypes);
    }

    @QtUninvokable
    public <Rule extends QHttpServerRouterRule> Rule route(
            java.util.function.@StrictNonNull BiFunction<io.qt.core.@Nullable QObject, io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull Rule> ruleFactory,
            @Nullable ResponderViewHandler viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(ruleFactory.apply(findContext(viewHandler), createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @QtUninvokable
    public <Rule extends QHttpServerRouterRule> Rule route(
            java.util.function.@StrictNonNull BiFunction<io.qt.core.@Nullable QObject, io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull Rule> ruleFactory,
            io.qt.core.@Nullable QObject context,
            @Nullable ResponderViewHandler viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(ruleFactory.apply(context, createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @QtUninvokable
    public QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, @Nullable ResponderViewHandler viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(createHttpServerRouterRule(pathPattern, createRouterHandler(viewHandler, metaTypes), context),
                metaTypes);
    }

    @QtUninvokable
    public QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.Methods methods,
            io.qt.core.@Nullable QObject context,
            @Nullable ResponderViewHandler viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(
                createHttpServerRouterRule(pathPattern, methods, createRouterHandler(viewHandler, metaTypes), context),
                metaTypes);
    }

    @QtUninvokable
    public <R, Rule extends QHttpServerRouterRule> Rule route(
            java.util.function.@StrictNonNull BiFunction<io.qt.core.@Nullable QObject, io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull Rule> ruleFactory,
            io.qt.core.@Nullable QObject context, @Nullable SimpleViewHandler<R> viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(ruleFactory.apply(context, createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @QtUninvokable
    public <R, Rule extends QHttpServerRouterRule> Rule route(
            java.util.function.@StrictNonNull BiFunction<io.qt.core.@Nullable QObject, io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull Rule> ruleFactory,
            @Nullable SimpleViewHandler<R> viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(ruleFactory.apply(findContext(viewHandler), createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @QtUninvokable
    public <R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, @Nullable SimpleViewHandler<R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(createHttpServerRouterRule(pathPattern, createRouterHandler(viewHandler, metaTypes), context),
                metaTypes);
    }

    @QtUninvokable
    public <R> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods,
            io.qt.core.@Nullable QObject context, @Nullable SimpleViewHandler<R> viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return null;
        return router().addRule(
                createHttpServerRouterRule(pathPattern, methods, createRouterHandler(viewHandler, metaTypes), context),
                metaTypes);
    }

    @QtUninvokable
    public <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule route(@NonNull String pathPattern, io.qt.core.@Nullable QObject context, ViewHandler viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, viewHandler, info);
    }

    @QtUninvokable
    public <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule route(@NonNull String pathPattern,
            io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, @NonNull ViewHandler viewHandler) {
        InvokableTypeInfo info = type(viewHandler, context);
        return route(pathPattern, methods, viewHandler, info);
    }
}// class

class QHttpServerRouter___ {

    @QtUninvokable
    public final void addConverter(@NonNull Class<?> type, java.lang.@NonNull String regexp) {
        io.qt.core.QMetaType metaType = io.qt.core.QMetaType.fromType(type);
        addConverter(metaType, regexp);
    }

    @QtUninvokable
    public final void removeConverter(@NonNull Class<?> type) {
        io.qt.core.QMetaType metaType = io.qt.core.QMetaType.fromType(type);
        removeConverter(metaType);
    }

    @QtUninvokable
    <Rule extends QHttpServerRouterRule> Rule addRule(@NonNull Rule rule, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        io.qt.core.QList<io.qt.core.QMetaType> _metaTypes = new io.qt.core.QList<>(io.qt.core.QMetaType.class);
        _metaTypes.append(java.util.Arrays.asList(metaTypes));
        return addRule(rule, _metaTypes);
    }

    @QtUninvokable
    <Rule extends QHttpServerRouterRule> Rule addRule(@NonNull Rule rule, io.qt.core.QList<io.qt.core.QMetaType> metaTypes) {
        if(addRuleImpl(QtJambi_LibraryUtilities.internal.nativeId(this),
                QtJambi_LibraryUtilities.internal.checkedNativeId(rule),
                QtJambi_LibraryUtilities.internal.checkedNativeId(metaTypes))){
            return rule;
        }else{
            return null;
        }
    }

    @QtUninvokable
    private native boolean addRuleImpl(long __this__nativeId, long rule, long metaTypes);
}// class

class QHttpServerResponder___ {
    /**
     * <p>Convenient method to create an array of pairs as <code><a href="@docRoot/qhttpserverresponder.html#HeaderList-typedef">QHttpServerResponder::HeaderList</a></code></p>
     */
    @SafeVarargs
    @QtUninvokable
    public static io.qt.core.@NonNull QPair<io.qt.core.@NonNull QByteArray, io.qt.core.@NonNull QByteArray>[] headerList(
            io.qt.core.@NonNull QPair<io.qt.core.@NonNull QByteArray, io.qt.core.@NonNull QByteArray> @NonNull... pairs) {
        return pairs;
    }
}// class

class QHttpServerRouterRule_cpp__ {
}// class
