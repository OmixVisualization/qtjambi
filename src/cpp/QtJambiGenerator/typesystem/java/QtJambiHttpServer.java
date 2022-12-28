/****************************************************************************
 **
 ** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG.
 ** All rights reserved.
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

    @io.qt.QtUninvokable
    public void afterRequest(java.util.function.Function<QHttpServerResponse, QHttpServerResponse> action) {
        afterRequest(QtJambi_LibraryUtilities.internal.nativeId(this), false, action);
    }

    @io.qt.QtUninvokable
    public void afterRequest(
            java.util.function.BiFunction<QHttpServerResponse, QHttpServerRequest, QHttpServerResponse> action) {
        afterRequest(QtJambi_LibraryUtilities.internal.nativeId(this), true, action);
    }

    @io.qt.QtUninvokable
    private native void afterRequest(long __this__nativeId, boolean isBi, Object action);

    @Override
    protected void missingHandler(QHttpServerRequest request, io.qt.network.QTcpSocket socket) {
        missingHandler(QtJambi_LibraryUtilities.internal.nativeId(this),
                QtJambi_LibraryUtilities.internal.checkedNativeId(request),
                QtJambi_LibraryUtilities.internal.nativeId(socket));
    }

    @io.qt.QtUninvokable
    private native void missingHandler(long __this__nativeId, long request, long socket);

    @Override
    protected boolean handleRequest(QHttpServerRequest request, io.qt.network.QTcpSocket socket) {
        return handleRequest(QtJambi_LibraryUtilities.internal.nativeId(this),
                QtJambi_LibraryUtilities.internal.checkedNativeId(request),
                QtJambi_LibraryUtilities.internal.nativeId(socket));
    }

    @io.qt.QtUninvokable
    private native boolean handleRequest(long __this__nativeId, long request, long socket);

    @io.qt.QtUninvokable
    private void sendResponse(QHttpServerResponse response, QHttpServerRequest request,
            io.qt.network.QTcpSocket socket) {
        sendResponse(QtJambi_LibraryUtilities.internal.nativeId(this),
                QtJambi_LibraryUtilities.internal.checkedNativeId(response),
                QtJambi_LibraryUtilities.internal.checkedNativeId(request),
                QtJambi_LibraryUtilities.internal.nativeId(socket));
    }

    @io.qt.QtUninvokable
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

    @io.qt.QtUninvokable
    private native void sendResponse(long __this__nativeId, long response, long request, long socket);

    public interface GenericViewHandler {
        void invoke(Object[] args, QHttpServerResponder responder, QHttpServerRequest request);
    }

    private Object[] values(io.qt.core.QRegularExpressionMatch rem, io.qt.core.QMetaType[] metaTypes,
            Object... requestResponder) {
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

    @io.qt.QtUninvokable
    private QHttpServerRouterRule.RouterHandler createRouterHandler(GenericViewHandler viewHandler,
            io.qt.core.QMetaType... metaTypes) {
        return (rem, request, socket) -> {
            Object[] values = values(rem, metaTypes);
            viewHandler.invoke(values, makeResponder(request, socket), request);
        };
    }

    @io.qt.QtUninvokable
    private <R> QHttpServerRouterRule.RouterHandler createRouterHandler(
            java.util.function.BiFunction<Object[], QHttpServerRequest, R> viewHandler,
            io.qt.core.QMetaType... metaTypes) {
        return (rem, request, socket) -> {
            Object[] values = values(rem, metaTypes);
            R result = viewHandler.apply(values, request);
            sendResponse(result, request, socket);
        };
    }

    @io.qt.QtUninvokable
    private <R> QHttpServerRouterRule.RouterHandler createRouterHandler(
            java.util.function.Function<Object[], R> viewHandler, io.qt.core.QMetaType... metaTypes) {
        return (rem, request, socket) -> {
            Object[] values = values(rem, metaTypes);
            R result = viewHandler.apply(values);
            sendResponse(result, request, socket);
        };
    }

    @io.qt.QtUninvokable
    private QHttpServerRouterRule.RouterHandler createRouterHandler(
            java.util.function.BiConsumer<Object[], QHttpServerResponder> viewHandler,
            io.qt.core.QMetaType... metaTypes) {
        return (rem, request, socket) -> {
            Object[] values = values(rem, metaTypes);
            viewHandler.accept(values, makeResponder(request, socket));
        };
    }

    @io.qt.QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerResponder(
            ViewHandler viewHandler, SlotInvoker<ViewHandler> slotInvoker, io.qt.core.QMetaType... metaTypes) {
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

    @io.qt.QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerRequestResponder(
            ViewHandler viewHandler, SlotInvoker<ViewHandler> slotInvoker, io.qt.core.QMetaType... metaTypes) {
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

    @io.qt.QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerResponderRequest(
            ViewHandler viewHandler, SlotInvoker<ViewHandler> slotInvoker, io.qt.core.QMetaType... metaTypes) {
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

    @io.qt.QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandlerRequest(
            ViewHandler viewHandler, SlotInvoker<ViewHandler> slotInvoker, io.qt.core.QMetaType... metaTypes) {
        return (rem, request, socket) -> {
            Object[] values = values(rem, metaTypes, request);
            Object result;
            try {
                result = slotInvoker.invoke(viewHandler, values);
            } catch (RuntimeException | Error e) {
                throw e;
            } catch (Throwable e) {
                throw new RuntimeException(e);
            }
            sendResponse(result, request, socket);
        };
    }

    @io.qt.QtUninvokable
    private <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> QHttpServerRouterRule.RouterHandler createRouterHandler(
            ViewHandler viewHandler, SlotInvoker<ViewHandler> slotInvoker, io.qt.core.QMetaType... metaTypes) {
        return (rem, request, socket) -> {
            Object[] values = values(rem, metaTypes);
            Object result;
            try {
                result = slotInvoker.invoke(viewHandler, values);
            } catch (RuntimeException | Error e) {
                throw e;
            } catch (Throwable e) {
                throw new RuntimeException(e);
            }
            sendResponse(result, request, socket);
        };
    }

    @io.qt.QtUninvokable
    public boolean route(String pathPattern, GenericViewHandler viewHandler, io.qt.core.QMetaType... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(new QHttpServerRouterRule(pathPattern, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @io.qt.QtUninvokable
    public boolean route(
            java.util.function.Function<io.qt.httpserver.QHttpServerRouterRule.RouterHandler, QHttpServerRouterRule> ruleFactory,
            GenericViewHandler viewHandler, io.qt.core.QMetaType... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(ruleFactory.apply(createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @io.qt.QtUninvokable
    public boolean route(String pathPattern, io.qt.httpserver.QHttpServerRequest.Methods methods,
            GenericViewHandler viewHandler, io.qt.core.QMetaType... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(
                new QHttpServerRouterRule(pathPattern, methods, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @io.qt.QtUninvokable
    public <R> boolean route(
            java.util.function.Function<io.qt.httpserver.QHttpServerRouterRule.RouterHandler, QHttpServerRouterRule> ruleFactory,
            java.util.function.BiFunction<Object[], QHttpServerRequest, R> viewHandler,
            io.qt.core.QMetaType... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(ruleFactory.apply(createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @io.qt.QtUninvokable
    public <R> boolean route(String pathPattern,
            java.util.function.BiFunction<Object[], QHttpServerRequest, R> viewHandler,
            io.qt.core.QMetaType... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(new QHttpServerRouterRule(pathPattern, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @io.qt.QtUninvokable
    public <R> boolean route(String pathPattern, io.qt.httpserver.QHttpServerRequest.Methods methods,
            java.util.function.BiFunction<Object[], QHttpServerRequest, R> viewHandler,
            io.qt.core.QMetaType... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(
                new QHttpServerRouterRule(pathPattern, methods, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @io.qt.QtUninvokable
    public boolean route(
            java.util.function.Function<io.qt.httpserver.QHttpServerRouterRule.RouterHandler, QHttpServerRouterRule> ruleFactory,
            java.util.function.BiConsumer<Object[], QHttpServerResponder> viewHandler,
            io.qt.core.QMetaType... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(ruleFactory.apply(createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @io.qt.QtUninvokable
    public boolean route(String pathPattern, java.util.function.BiConsumer<Object[], QHttpServerResponder> viewHandler,
            io.qt.core.QMetaType... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(new QHttpServerRouterRule(pathPattern, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @io.qt.QtUninvokable
    public boolean route(String pathPattern, io.qt.httpserver.QHttpServerRequest.Methods methods,
            java.util.function.BiConsumer<Object[], QHttpServerResponder> viewHandler,
            io.qt.core.QMetaType... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(
                new QHttpServerRouterRule(pathPattern, methods, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @io.qt.QtUninvokable
    public <R> boolean route(
            java.util.function.Function<io.qt.httpserver.QHttpServerRouterRule.RouterHandler, QHttpServerRouterRule> ruleFactory,
            java.util.function.Function<Object[], R> viewHandler, io.qt.core.QMetaType... metaTypes) {
        if (viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(ruleFactory.apply(createRouterHandler(viewHandler, metaTypes)), metaTypes);
    }

    @io.qt.QtUninvokable
    public <R> boolean route(String pathPattern, java.util.function.Function<Object[], R> viewHandler,
            io.qt.core.QMetaType... metaTypes) {
        if (pathPattern == null || viewHandler == null || metaTypes == null)
            return false;
        return router().addRule(new QHttpServerRouterRule(pathPattern, createRouterHandler(viewHandler, metaTypes)),
                metaTypes);
    }

    @io.qt.QtUninvokable
    public <R> boolean route(String pathPattern, io.qt.httpserver.QHttpServerRequest.Methods methods,
            java.util.function.Function<Object[], R> viewHandler, io.qt.core.QMetaType... metaTypes) {
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

    @io.qt.QtUninvokable
    public <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> boolean route(String pathPattern,
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

    @io.qt.QtUninvokable
    public <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> boolean route(
            java.util.function.Function<io.qt.httpserver.QHttpServerRouterRule.RouterHandler, QHttpServerRouterRule> ruleFactory,
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

    @io.qt.QtUninvokable
    public <ViewHandler extends io.qt.core.QMetaObject.AbstractSlot> boolean route(String pathPattern,
            io.qt.httpserver.QHttpServerRequest.Methods methods, ViewHandler viewHandler) {
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

class QHttpServerRouter___ {

    @io.qt.QtUninvokable
    public final void addConverter(Class<?> type, java.lang.String regexp) {
        io.qt.core.QMetaType metaType = io.qt.core.QMetaType.fromType(type);
        addConverter(metaType, regexp);
    }

    @io.qt.QtUninvokable
    public final void removeConverter(Class<?> type) {
        io.qt.core.QMetaType metaType = io.qt.core.QMetaType.fromType(type);
        removeConverter(metaType);
    }

    @io.qt.QtUninvokable
    boolean addRule(QHttpServerRouterRule rule, io.qt.core.QMetaType... metaTypes) {
        io.qt.core.QList<io.qt.core.QMetaType> _metaTypes = new io.qt.core.QList<>(io.qt.core.QMetaType.class);
        _metaTypes.append(java.util.Arrays.asList(metaTypes));
        return addRule(rule, _metaTypes);
    }

    @io.qt.QtUninvokable
    boolean addRule(QHttpServerRouterRule rule, io.qt.core.QList<io.qt.core.QMetaType> metaTypes) {
        return addRuleImpl(QtJambi_LibraryUtilities.internal.nativeId(this),
                QtJambi_LibraryUtilities.internal.checkedNativeId(rule),
                QtJambi_LibraryUtilities.internal.checkedNativeId(metaTypes));
    }

    @io.qt.QtUninvokable
    private native boolean addRuleImpl(long __this__nativeId, long rule, long metaTypes);
}// class

class QAbstractHttpServer___ {
    @SuppressWarnings({ "exports" })
    @io.qt.QtUninvokable
    public final int listen(io.qt.network.QHostAddress.SpecialAddress address, int port) {
        return listen(new io.qt.network.QHostAddress(address), port);
    }

    @io.qt.QtUninvokable
    public final int listen(String address, int port) {
        return listen(new io.qt.network.QHostAddress(address), port);
    }
}// class

class QHttpServerResponder___ {
    @io.qt.QtUninvokable
    public final void write(String data, String mimeType) {
        write(io.qt.core.QString.toUtf8(data), io.qt.core.QString.toUtf8(mimeType));
    }

    @io.qt.QtUninvokable
    public final void write(String data, String mimeType, io.qt.httpserver.QHttpServerResponder.StatusCode status) {
        write(io.qt.core.QString.toUtf8(data), io.qt.core.QString.toUtf8(mimeType), status);
    }

    @io.qt.QtUninvokable
    public final void write(io.qt.core.QIODevice data, String mimeType) {
        write(data, io.qt.core.QString.toUtf8(mimeType));
    }

    @io.qt.QtUninvokable
    public final void write(io.qt.core.QIODevice data, String mimeType,
            io.qt.httpserver.QHttpServerResponder.StatusCode status) {
        write(data, io.qt.core.QString.toUtf8(mimeType), status);
    }

    @io.qt.QtUninvokable
    public final void write(io.qt.core.QByteArray data, String mimeType) {
        write(data, io.qt.core.QString.toUtf8(mimeType));
    }

    @io.qt.QtUninvokable
    public final void write(io.qt.core.QByteArray data, String mimeType,
            io.qt.httpserver.QHttpServerResponder.StatusCode status) {
        write(data, io.qt.core.QString.toUtf8(mimeType), status);
    }

    @io.qt.QtUninvokable
    public final void writeBody(byte[] body) {
        writeBody(new io.qt.core.QByteArray(body));
    }

    @io.qt.QtUninvokable
    public final void writeHeader(io.qt.core.QByteArray key, String value) {
        writeHeader(key, io.qt.core.QString.toUtf8(value));
    }

    @io.qt.QtUninvokable
    public final void writeHeader(String key, io.qt.core.QByteArray value) {
        writeHeader(io.qt.core.QString.toUtf8(key), value);
    }

    @SafeVarargs
    @io.qt.QtUninvokable
    public final io.qt.core.QPair<io.qt.core.QByteArray, io.qt.core.QByteArray>[] headerList(
            io.qt.core.QPair<io.qt.core.QByteArray, io.qt.core.QByteArray>... pairs) {
        return pairs;
    }

    @SafeVarargs
    @io.qt.QtUninvokable
    public final void write(io.qt.core.QByteArray data,
            io.qt.core.QPair<io.qt.core.QByteArray, io.qt.core.QByteArray>... headers) {
        write(data, headers, io.qt.httpserver.QHttpServerResponder.StatusCode.Ok);
    }

    @SafeVarargs
    @io.qt.QtUninvokable
    public final void write(io.qt.core.QIODevice data,
            io.qt.core.QPair<io.qt.core.QByteArray, io.qt.core.QByteArray>... headers) {
        write(data, headers, io.qt.httpserver.QHttpServerResponder.StatusCode.Ok);
    }

    @SafeVarargs
    @io.qt.QtUninvokable
    public final void write(io.qt.core.QJsonDocument document,
            io.qt.core.QPair<io.qt.core.QByteArray, io.qt.core.QByteArray>... headers) {
        write(document, headers, io.qt.httpserver.QHttpServerResponder.StatusCode.Ok);
    }

    @SafeVarargs
    @io.qt.QtUninvokable
    public final void write(io.qt.core.QPair<io.qt.core.QByteArray, io.qt.core.QByteArray>... headers) {
        write(headers, io.qt.httpserver.QHttpServerResponder.StatusCode.Ok);
    }
}// class

class QHttpServerResponse___ {
    @io.qt.QtUninvokable
    public final void addHeader(io.qt.core.QByteArray key, String value) {
        addHeader(key, io.qt.core.QString.toUtf8(value));
    }

    @io.qt.QtUninvokable
    public final void addHeader(String key, io.qt.core.QByteArray value) {
        addHeader(io.qt.core.QString.toUtf8(key), value);
    }

    @io.qt.QtUninvokable
    public final void setHeader(io.qt.core.QByteArray key, String value) {
        setHeader(key, io.qt.core.QString.toUtf8(value));
    }

    @io.qt.QtUninvokable
    public final void setHeader(String key, io.qt.core.QByteArray value) {
        setHeader(io.qt.core.QString.toUtf8(key), value);
    }

    @io.qt.QtUninvokable
    public final void clearHeader(String key) {
        clearHeader(io.qt.core.QString.toUtf8(key));
    }

    @io.qt.QtUninvokable
    public final boolean hasHeader(String name) {
        return hasHeader(io.qt.core.QString.toUtf8(name));
    }

    @io.qt.QtUninvokable
    public final boolean hasHeader(String name, io.qt.core.QByteArray value) {
        return hasHeader(io.qt.core.QString.toUtf8(name), value);
    }

    @io.qt.QtUninvokable
    public final boolean hasHeader(String name, String value) {
        return hasHeader(io.qt.core.QString.toUtf8(name), io.qt.core.QString.toUtf8(value));
    }

    @io.qt.QtUninvokable
    public final io.qt.core.QList<io.qt.core.QByteArray> headers(String name) {
        return headers(io.qt.core.QString.toUtf8(name));
    }

public QHttpServerResponse(String mimeType, io.qt.core.QByteArray data) {
        this(io.qt.core.QString.toUtf8(mimeType), data);
    }

public QHttpServerResponse(String mimeType, String data) {
        this(io.qt.core.QString.toUtf8(mimeType), io.qt.core.QString.toUtf8(data));
    }

public QHttpServerResponse(io.qt.core.QByteArray mimeType, String data) {
        this(mimeType, io.qt.core.QString.toUtf8(data));
    }

public QHttpServerResponse(String mimeType, io.qt.core.QByteArray data, io.qt.httpserver.QHttpServerResponder.StatusCode status) {
        this(io.qt.core.QString.toUtf8(mimeType), data, status);
    }

public QHttpServerResponse(String mimeType, String data, io.qt.httpserver.QHttpServerResponder.StatusCode status) {
        this(io.qt.core.QString.toUtf8(mimeType), io.qt.core.QString.toUtf8(data), status);
    }

public QHttpServerResponse(io.qt.core.QByteArray mimeType, String data, io.qt.httpserver.QHttpServerResponder.StatusCode status) {
        this(mimeType, io.qt.core.QString.toUtf8(data), status);
    }
}// class

class QHttpServerRequest___ {
    @io.qt.QtUninvokable
    public final io.qt.core.QByteArray value(String key) {
        return value(io.qt.core.QString.toUtf8(key));
    }
}// class
