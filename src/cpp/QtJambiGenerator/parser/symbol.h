/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2002-2005 Roberto Raggi <roberto@kdevelop.org>
**
** This file is part of QtJambi.
**
** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#ifndef SYMBOL_H
#define SYMBOL_H

#include <QtCore/QString>
#include <cstring>

#include <QtCore/QHash>
#include <QtCore/QPair>

struct NameSymbol {
    const char *data;
    std::size_t count;

    inline QString as_string() const {
        return QString::fromUtf8(data, int(count));
    }

    inline bool operator == (const NameSymbol &other) const {
        return count == other.count
               && std::strncmp(data, other.data, count) == 0;
    }

protected:
    inline NameSymbol() {}
    inline NameSymbol(const char *d, std::size_t c)
            : data(d), count(c) {}

private:
    void operator = (const NameSymbol &);

    friend class NameTable;
};

inline uint qHash(const NameSymbol &r) {
    uint hash_value = 0;

    for (std::size_t i = 0; i < r.count; ++i)
        hash_value = (hash_value << 5) - hash_value + uint(r.data[i]);

    return hash_value;
}

inline uint qHash(const QPair<const char*, std::size_t> &r) {
    uint hash_value = 0;

    for (std::size_t i = 0; i < r.second; ++i)
        hash_value = (hash_value << 5) - hash_value + uint(r.first[i]);

    return hash_value;
}

class NameTable {
    public:
        typedef QPair<const char *, std::size_t> KeyType;
        typedef QHash<KeyType, NameSymbol*> ContainerType;

    public:
        NameTable() {}

        ~NameTable() {
            qDeleteAll(_M_storage);
        }

        inline const NameSymbol *findOrInsert(const char *str, std::size_t len) {
            KeyType key(str, len);

            NameSymbol *name = _M_storage.value(key);
            if (!name) {
                name = new NameSymbol(str, len);
                _M_storage.insert(key, name);
            }

            return name;
        }

        inline std::size_t count() const {
            return std::size_t(_M_storage.size());
        }

    private:
        ContainerType _M_storage;

    private:
        NameTable(const NameTable &other);
        void operator = (const NameTable &other);
};

#endif // SYMBOL_H

// kate: space-indent on; indent-width 2; replace-tabs on;
