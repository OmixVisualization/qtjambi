/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

class QGeoServiceProviderFactory___ {
    public static final class Result<T extends io.qt.core.QObject>{
        private final T engine;
        private final QGeoServiceProvider.Error error;
        private final String errorString;
        
        public Result(@StrictNonNull T engine) {
            super();
            this.engine = java.util.Objects.requireNonNull(engine);
            this.error = QGeoServiceProvider.Error.NoError;
            this.errorString = null;
        }
        
        public Result(QGeoServiceProvider.@StrictNonNull Error error, @NonNull String errorString) {
            super();
            this.engine = null;
            this.error = java.util.Objects.requireNonNull(error);
            this.errorString = java.util.Objects.requireNonNull(errorString);
        }

        @NativeAccess
        private Result(T engine, QGeoServiceProvider.Error error, String errorString) {
            super();
            this.engine = engine;
            this.error = error;
            if(errorString!=null && errorString.isEmpty())
                errorString = null;
            this.errorString = errorString;
        }
        
        public @Nullable T engine() {
            return engine;
        }
        public QGeoServiceProvider.@Nullable Error error() {
            return error;
        }
        public @Nullable String errorString() {
            return errorString;
        }
    }
}// class
