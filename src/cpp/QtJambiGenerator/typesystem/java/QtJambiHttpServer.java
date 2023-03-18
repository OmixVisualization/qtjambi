/****************************************************************************
 **
 ** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

    @QtUninvokable
    private native void missingHandler(long __this__nativeId, long request, long argX);

    @QtUninvokable
    private native boolean handleRequest(long __this__nativeId, long request, long argX);

    @QtUninvokable
    private native void sendResponse(long __this__nativeId, long response, long request, long argX);

    public interface GenericViewHandler {
        void invoke(@Nullable Object @NonNull[] args, @NonNull QHttpServerResponder responder, @NonNull QHttpServerRequest request);
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
    private <R> QHttpServerRouterRule.@NonNull RouterHandler createRouterHandler(
            java.util.function.@Nullable BiFunction<@Nullable Object @NonNull[], @NonNull QHttpServerRequest, R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, responder) -> {
            Object[] values = values(rem, metaTypes);
            R result = viewHandler.apply(values, request);
            sendResponse(result, request, responder);
        };
    }

    @QtUninvokable
    private <R> QHttpServerRouterRule.@NonNull RouterHandler createRouterHandler(
            java.util.function.Function<@Nullable Object @NonNull[], R> viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, responder) -> {
            Object[] values = values(rem, metaTypes);
            R result = viewHandler.apply(values);
            sendResponse(result, request, responder);
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.@NonNull RouterHandler createRouterHandlerRequest(
            @NonNull ViewHandler viewHandler, @StrictNonNull SlotInvoker<ViewHandler> slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, responder) -> {
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
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.@NonNull RouterHandler createRouterHandler(
            @NonNull ViewHandler viewHandler, @StrictNonNull SlotInvoker<ViewHandler> slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, responder) -> {
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
        };
    }

    @QtUninvokable
    public boolean route(@NonNull String pathPattern, @NonNull GenericViewHandler viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(new QHttpServerRouterRule(pathPattern, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @QtUninvokable
    public boolean route(
            java.util.function.@StrictNonNull Function<io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull QHttpServerRouterRule> ruleFactory,
            @NonNull GenericViewHandler viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(ruleFactory.apply(createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @QtUninvokable
    public boolean route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods,
            @NonNull GenericViewHandler viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(
                new QHttpServerRouterRule(pathPattern, methods, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @QtUninvokable
    public <R> boolean route(
            java.util.function.@StrictNonNull Function<io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull QHttpServerRouterRule> ruleFactory,
            java.util.function.@Nullable BiFunction<@Nullable Object @NonNull[], @NonNull QHttpServerRequest, R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(ruleFactory.apply(createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @QtUninvokable
    public <R> boolean route(@NonNull String pathPattern,
            java.util.function.@Nullable BiFunction<@Nullable Object @NonNull[], @NonNull QHttpServerRequest, R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(new QHttpServerRouterRule(pathPattern, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @QtUninvokable
    public <R> boolean route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods,
            java.util.function.@Nullable BiFunction<@Nullable Object @NonNull[], @NonNull QHttpServerRequest, R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(
                new QHttpServerRouterRule(pathPattern, methods, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @QtUninvokable
    public boolean route(
            java.util.function.@StrictNonNull Function<io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull QHttpServerRouterRule> ruleFactory,
            java.util.function.@Nullable BiConsumer<@Nullable Object @NonNull[], @NonNull QHttpServerResponder> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(ruleFactory.apply(createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @QtUninvokable
    public boolean route(@NonNull String pathPattern, java.util.function.@Nullable BiConsumer<@Nullable Object @NonNull[], @NonNull QHttpServerResponder> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(new QHttpServerRouterRule(pathPattern, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @QtUninvokable
    public boolean route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.Methods methods,
            java.util.function.@Nullable BiConsumer<@Nullable Object @NonNull[], @NonNull QHttpServerResponder> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(
                new QHttpServerRouterRule(pathPattern, methods, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @QtUninvokable
    public <R> boolean route(
            java.util.function.@StrictNonNull Function<io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull QHttpServerRouterRule> ruleFactory,
            java.util.function.@Nullable Function<@Nullable Object @NonNull[], R> viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(ruleFactory.apply(createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @QtUninvokable
    public <R> boolean route(@NonNull String pathPattern, java.util.function.@Nullable Function<@Nullable Object @NonNull[], R> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(new QHttpServerRouterRule(pathPattern, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @QtUninvokable
    public <R> boolean route(@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods,
            java.util.function.@Nullable Function<@Nullable Object @NonNull[], R> viewHandler, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(
                new QHttpServerRouterRule(pathPattern, methods, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @FunctionalInterface
    private interface SlotInvoker<ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> {
        Object invoke(ViewHandler object, Object... args) throws Throwable;
    }

    private static class InvokableTypeInfo<ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> {
        SlotInvoker<ViewHandler> slotInvoker;
        java.lang.reflect.Parameter[] parameters;
        boolean hasReturnType;
    }

    @SuppressWarnings({ "unchecked", "rawtypes" })
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> InvokableTypeInfo<ViewHandler> type(
            ViewHandler slot) {
        InvokableTypeInfo<ViewHandler> info = new InvokableTypeInfo<>();
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
    public <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> boolean route(@NonNull String pathPattern,
            ViewHandler viewHandler) {
        InvokableTypeInfo<ViewHandler> info = type(viewHandler);
        if (info.slotInvoker == null)
            return false;
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
                return router().addRule(new QHttpServerRouterRule(pathPattern,
                        createRouterHandlerResponder(viewHandler, info.slotInvoker, metaTypes)), metaTypes);
            } else if (requestArg < responderArg) {
                return router().addRule(
                        new QHttpServerRouterRule(pathPattern,
                                createRouterHandlerRequestResponder(viewHandler, info.slotInvoker, metaTypes)),
                        metaTypes);
            } else {
                return router().addRule(
                        new QHttpServerRouterRule(pathPattern,
                                createRouterHandlerResponderRequest(viewHandler, info.slotInvoker, metaTypes)),
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
                return router().addRule(new QHttpServerRouterRule(pathPattern,
                        createRouterHandler(viewHandler, info.slotInvoker, metaTypes)), metaTypes);
            } else {
                return router().addRule(new QHttpServerRouterRule(pathPattern,
                        createRouterHandlerRequest(viewHandler, info.slotInvoker, metaTypes)), metaTypes);
            }
        }
    }

    @QtUninvokable
    public <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> boolean route(
            java.util.function.@StrictNonNull Function<io.qt.httpserver.QHttpServerRouterRule.@NonNull RouterHandler, @NonNull QHttpServerRouterRule> ruleFactory,
            @NonNull ViewHandler viewHandler) {
        InvokableTypeInfo<ViewHandler> info = type(viewHandler);
        if (info.slotInvoker == null)
            return false;
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
    public <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> boolean route(@NonNull String pathPattern,
            io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, @NonNull ViewHandler viewHandler) {
        InvokableTypeInfo<ViewHandler> info = type(viewHandler);
        if (info.slotInvoker == null)
            return false;
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
                return router().addRule(new QHttpServerRouterRule(pathPattern, methods,
                        createRouterHandlerResponder(viewHandler, info.slotInvoker, metaTypes)), metaTypes);
            } else if (requestArg < responderArg) {
                return router().addRule(
                        new QHttpServerRouterRule(pathPattern, methods,
                                createRouterHandlerRequestResponder(viewHandler, info.slotInvoker, metaTypes)),
                        metaTypes);
            } else {
                return router().addRule(
                        new QHttpServerRouterRule(pathPattern, methods,
                                createRouterHandlerResponderRequest(viewHandler, info.slotInvoker, metaTypes)),
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
                return router().addRule(new QHttpServerRouterRule(pathPattern, methods,
                        createRouterHandler(viewHandler, info.slotInvoker, metaTypes)), metaTypes);
            } else {
                return router().addRule(new QHttpServerRouterRule(pathPattern, methods,
                        createRouterHandlerRequest(viewHandler, info.slotInvoker, metaTypes)), metaTypes);
            }
        }
    }
}// class

class QHttpServer_64__ {
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
        return (rem, request, socket) -> {
            Object[] values = values(rem, metaTypes);
            viewHandler.invoke(values, makeResponder(request, socket), request);
        };
    }

    @QtUninvokable
    private QHttpServerRouterRule.RouterHandler createRouterHandler(
            java.util.function.@Nullable BiConsumer<@Nullable Object @NonNull[], @NonNull QHttpServerResponder> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, socket) -> {
            Object[] values = values(rem, metaTypes);
            viewHandler.accept(values, makeResponder(request, socket));
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerResponder(
            ViewHandler viewHandler, SlotInvoker<ViewHandler> slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, socket) -> {
            Object[] values = values(rem, metaTypes, makeResponder(request, socket));
            try {
                slotInvoker.invoke(viewHandler, values);
            } catch (RuntimeException | Error e) {
                throw e;
            } catch (Throwable e) {
                throw new RuntimeException(e);
            }
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerRequestResponder(
            ViewHandler viewHandler, SlotInvoker<ViewHandler> slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, socket) -> {
            Object[] values = values(rem, metaTypes, request, makeResponder(request, socket));
            try {
                slotInvoker.invoke(viewHandler, values);
            } catch (RuntimeException | Error e) {
                throw e;
            } catch (Throwable e) {
                throw new RuntimeException(e);
            }
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerResponderRequest(
            ViewHandler viewHandler, SlotInvoker<ViewHandler> slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, socket) -> {
            Object[] values = values(rem, metaTypes, makeResponder(request, socket), request);
            try {
                slotInvoker.invoke(viewHandler, values);
            } catch (RuntimeException | Error e) {
                throw e;
            } catch (Throwable e) {
                throw new RuntimeException(e);
            }
        };
    }
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
            java.util.function.@Nullable BiConsumer<@Nullable Object @NonNull[], @NonNull QHttpServerResponder> viewHandler,
            io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, responder) -> {
            Object[] values = values(rem, metaTypes);
            viewHandler.accept(values, responder);
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerResponder(
            ViewHandler viewHandler, SlotInvoker<ViewHandler> slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, responder) -> {
            Object[] values = values(rem, metaTypes, responder);
            try {
                slotInvoker.invoke(viewHandler, values);
            } catch (RuntimeException | Error e) {
                throw e;
            } catch (Throwable e) {
                throw new RuntimeException(e);
            }
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerRequestResponder(
            ViewHandler viewHandler, SlotInvoker<ViewHandler> slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, responder) -> {
            Object[] values = values(rem, metaTypes, request, responder);
            try {
                slotInvoker.invoke(viewHandler, values);
            } catch (RuntimeException | Error e) {
                throw e;
            } catch (Throwable e) {
                throw new RuntimeException(e);
            }
        };
    }

    @QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerResponderRequest(
            ViewHandler viewHandler, SlotInvoker<ViewHandler> slotInvoker, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        return (rem, request, responder) -> {
            Object[] values = values(rem, metaTypes, responder, request);
            try {
                slotInvoker.invoke(viewHandler, values);
            } catch (RuntimeException | Error e) {
                throw e;
            } catch (Throwable e) {
                throw new RuntimeException(e);
            }
        };
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
    boolean addRule(@NonNull QHttpServerRouterRule rule, io.qt.core.@NonNull QMetaType @NonNull... metaTypes) {
        io.qt.core.QList<io.qt.core.QMetaType> _metaTypes = new io.qt.core.QList<>(io.qt.core.QMetaType.class);
        _metaTypes.append(java.util.Arrays.asList(metaTypes));
        return addRule(rule, _metaTypes);
    }

    @QtUninvokable
    boolean addRule(@NonNull QHttpServerRouterRule rule, io.qt.core.QList<io.qt.core.QMetaType> metaTypes) {
        return addRuleImpl(QtJambi_LibraryUtilities.internal.nativeId(this),
                QtJambi_LibraryUtilities.internal.checkedNativeId(rule),
                QtJambi_LibraryUtilities.internal.checkedNativeId(metaTypes));
    }

    @QtUninvokable
    private native boolean addRuleImpl(long __this__nativeId, long rule, long metaTypes);
}// class

class QAbstractHttpServer___ {
    @SuppressWarnings({ "exports" })
    @QtUninvokable
    public final int listen(io.qt.network.QHostAddress.@NonNull SpecialAddress address, int port) {
        return listen(new io.qt.network.QHostAddress(address), port);
    }

    @QtUninvokable
    public final int listen(@NonNull String address, int port) {
        return listen(new io.qt.network.QHostAddress(address), port);
    }
}// class

class QHttpServerResponder___ {
    @QtUninvokable
    public final void write(@NonNull String data, @NonNull String mimeType) {
        write(io.qt.core.QString.toUtf8(data), io.qt.core.QString.toUtf8(mimeType));
    }

    @QtUninvokable
    public final void write(@NonNull String data, @NonNull String mimeType, io.qt.httpserver.QHttpServerResponder.@NonNull StatusCode status) {
        write(io.qt.core.QString.toUtf8(data), io.qt.core.QString.toUtf8(mimeType), status);
    }

    @QtUninvokable
    public final void write(io.qt.core.@NonNull QIODevice data, @NonNull String mimeType) {
        write(data, io.qt.core.QString.toUtf8(mimeType));
    }

    @QtUninvokable
    public final void write(io.qt.core.@NonNull QIODevice data, @NonNull String mimeType,
            io.qt.httpserver.QHttpServerResponder.StatusCode status) {
        write(data, io.qt.core.QString.toUtf8(mimeType), status);
    }

    @QtUninvokable
    public final void write(io.qt.core.@NonNull QByteArray data, @NonNull String mimeType) {
        write(data, io.qt.core.QString.toUtf8(mimeType));
    }

    @QtUninvokable
    public final void write(io.qt.core.@NonNull QByteArray data, @NonNull String mimeType,
            io.qt.httpserver.QHttpServerResponder.@NonNull StatusCode status) {
        write(data, io.qt.core.QString.toUtf8(mimeType), status);
    }

    @QtUninvokable
    public final void writeBody(byte @NonNull[] body) {
        writeBody(new io.qt.core.QByteArray(body));
    }

    @QtUninvokable
    public final void writeHeader(io.qt.core.@NonNull QByteArray key, @NonNull String value) {
        writeHeader(key, io.qt.core.QString.toUtf8(value));
    }

    @QtUninvokable
    public final void writeHeader(@NonNull String key, io.qt.core.@NonNull QByteArray value) {
        writeHeader(io.qt.core.QString.toUtf8(key), value);
    }

    @SafeVarargs
    @QtUninvokable
    public final io.qt.core.@NonNull QPair<io.qt.core.@NonNull QByteArray, io.qt.core.@NonNull QByteArray>[] headerList(
            io.qt.core.@NonNull QPair<io.qt.core.@NonNull QByteArray, io.qt.core.@NonNull QByteArray> @NonNull... pairs) {
        return pairs;
    }

    @SafeVarargs
    @QtUninvokable
    public final void write(io.qt.core.@NonNull QByteArray data,
            io.qt.core.@NonNull QPair<io.qt.core.@NonNull QByteArray, io.qt.core.@NonNull QByteArray> @NonNull... headers) {
        write(data, headers, io.qt.httpserver.QHttpServerResponder.StatusCode.Ok);
    }

    @SafeVarargs
    @QtUninvokable
    public final void write(io.qt.core.@NonNull QIODevice data,
            io.qt.core.@NonNull QPair<io.qt.core.@NonNull QByteArray, io.qt.core.@NonNull QByteArray> @NonNull... headers) {
        write(data, headers, io.qt.httpserver.QHttpServerResponder.StatusCode.Ok);
    }

    @SafeVarargs
    @QtUninvokable
    public final void write(io.qt.core.@NonNull QJsonDocument document,
            io.qt.core.@NonNull QPair<io.qt.core.@NonNull QByteArray, io.qt.core.@NonNull QByteArray> @NonNull... headers) {
        write(document, headers, io.qt.httpserver.QHttpServerResponder.StatusCode.Ok);
    }

    @SafeVarargs
    @QtUninvokable
    public final void write(io.qt.core.@NonNull QPair<io.qt.core.@NonNull QByteArray, io.qt.core.@NonNull QByteArray> @NonNull... headers) {
        write(headers, io.qt.httpserver.QHttpServerResponder.StatusCode.Ok);
    }
}// class

class QHttpServerResponse___ {
    @QtUninvokable
    public final void addHeader(io.qt.core.@NonNull QByteArray key, @NonNull String value) {
        addHeader(key, io.qt.core.QString.toUtf8(value));
    }

    @QtUninvokable
    public final void addHeader(@NonNull String key, io.qt.core.@NonNull QByteArray value) {
        addHeader(io.qt.core.QString.toUtf8(key), value);
    }

    @QtUninvokable
    public final void setHeader(io.qt.core.@NonNull QByteArray key, @NonNull String value) {
        setHeader(key, io.qt.core.QString.toUtf8(value));
    }

    @QtUninvokable
    public final void setHeader(@NonNull String key, io.qt.core.@NonNull QByteArray value) {
        setHeader(io.qt.core.QString.toUtf8(key), value);
    }

    @QtUninvokable
    public final void clearHeader(@NonNull String key) {
        clearHeader(io.qt.core.QString.toUtf8(key));
    }

    @QtUninvokable
    public final boolean hasHeader(@NonNull String name) {
        return hasHeader(io.qt.core.QString.toUtf8(name));
    }

    @QtUninvokable
    public final boolean hasHeader(@NonNull String name, io.qt.core.@NonNull QByteArray value) {
        return hasHeader(io.qt.core.QString.toUtf8(name), value);
    }

    @QtUninvokable
    public final boolean hasHeader(@NonNull String name, @NonNull String value) {
        return hasHeader(io.qt.core.QString.toUtf8(name), io.qt.core.QString.toUtf8(value));
    }

    @QtUninvokable
    public final io.qt.core.@NonNull QList<io.qt.core.@NonNull QByteArray> headers(String name) {
        return headers(io.qt.core.QString.toUtf8(name));
    }

    public QHttpServerResponse(@NonNull String mimeType, io.qt.core.@NonNull QByteArray data) {
        this(io.qt.core.QString.toUtf8(mimeType), data);
    }

    public QHttpServerResponse(@NonNull String mimeType, @NonNull String data) {
        this(io.qt.core.QString.toUtf8(mimeType), io.qt.core.QString.toUtf8(data));
    }

    public QHttpServerResponse(io.qt.core.@NonNull QByteArray mimeType, @NonNull String data) {
        this(mimeType, io.qt.core.QString.toUtf8(data));
    }

    public QHttpServerResponse(@NonNull String mimeType, io.qt.core.@NonNull QByteArray data, io.qt.httpserver.QHttpServerResponder.@NonNull StatusCode status) {
        this(io.qt.core.QString.toUtf8(mimeType), data, status);
    }

    public QHttpServerResponse(@NonNull String mimeType, @NonNull String data, io.qt.httpserver.QHttpServerResponder.@NonNull StatusCode status) {
        this(io.qt.core.QString.toUtf8(mimeType), io.qt.core.QString.toUtf8(data), status);
    }

    public QHttpServerResponse(io.qt.core.@NonNull QByteArray mimeType, @NonNull String data, io.qt.httpserver.QHttpServerResponder.@NonNull StatusCode status) {
        this(mimeType, io.qt.core.QString.toUtf8(data), status);
    }
}// class

class QHttpServerRequest___ {
    @QtUninvokable
    public final io.qt.core.@NonNull QByteArray value(@NonNull String key) {
        return value(io.qt.core.QString.toUtf8(key));
    }
}// class
