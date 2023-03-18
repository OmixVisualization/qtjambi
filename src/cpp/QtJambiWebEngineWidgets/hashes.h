/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBIWEBENGINEWIDGETS_HASHES_H
#define QTJAMBIWEBENGINEWIDGETS_HASHES_H

#include <QtJambiCore/hashes.h>
#include <QtCore/QtCore>
#include <QtWebEngineWidgets/QtWebEngineWidgets>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QWebEngineScript &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.name());
    seed = hash(seed, value.isNull());
    seed = hash(seed, value.worldId());
    seed = hash(seed, value.sourceCode());
    seed = hash(seed, value.injectionPoint());
    seed = hash(seed, value.runsOnSubFrames());
    return seed;
}

inline bool operator==(const QWebEngineContextMenuData& arg1, const QWebEngineContextMenuData& arg2){
    return arg1.isValid()==arg2.isValid()
            && arg1.position()==arg2.position()
            && arg1.selectedText()==arg2.selectedText()
            && arg1.linkText()==arg2.linkText()
            && arg1.linkUrl()==arg2.linkUrl()
            && arg1.mediaUrl()==arg2.mediaUrl()
            && arg1.mediaType()==arg2.mediaType()
            && arg1.isContentEditable()==arg2.isContentEditable()
            && arg1.misspelledWord()==arg2.misspelledWord()
            && arg1.spellCheckerSuggestions()==arg2.spellCheckerSuggestions()
            && arg1.mediaFlags()==arg2.mediaFlags()
            && arg1.editFlags()==arg2.editFlags();
}

inline hash_type qHash(const QWebEngineContextMenuData &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.isValid());
    seed = hash(seed, value.position());
    seed = hash(seed, value.selectedText());
    seed = hash(seed, value.linkText());
    seed = hash(seed, value.linkUrl());
    seed = hash(seed, value.mediaUrl());
    seed = hash(seed, value.mediaType());
    seed = hash(seed, value.isContentEditable());
    seed = hash(seed, value.misspelledWord());
    seed = hash(seed, value.spellCheckerSuggestions());
    seed = hash(seed, value.mediaFlags());
    seed = hash(seed, value.editFlags());
    return seed;
}

inline bool operator==(const QWebEngineCertificateError& arg1, const QWebEngineCertificateError& arg2){
    return arg1.certificateChain()==arg2.certificateChain()
            && arg1.errorDescription()==arg2.errorDescription()
            && arg1.isOverridable()==arg2.isOverridable()
            && arg1.error()==arg2.error()
            && arg1.url()==arg2.url();
}

inline hash_type qHash(const QWebEngineCertificateError &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.certificateChain());
    seed = hash(seed, value.errorDescription());
    seed = hash(seed, value.isOverridable());
    seed = hash(seed, value.error());
    seed = hash(seed, value.url());
    return seed;
}

inline bool operator==(const QWebEngineFindTextResult& arg1, const QWebEngineFindTextResult& arg2){
    return arg1.numberOfMatches()==arg2.numberOfMatches()
            && arg1.activeMatch()==arg2.activeMatch();
}

inline hash_type qHash(const QWebEngineFindTextResult &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.numberOfMatches());
    seed = hash(seed, value.activeMatch());
    return seed;
}

inline bool operator==(const QWebEngineHistoryItem& arg1, const QWebEngineHistoryItem& arg2){
    return arg1.originalUrl()==arg2.originalUrl()
            && arg1.url()==arg2.url()
            && arg1.title()==arg2.title()
            && arg1.lastVisited()==arg2.lastVisited()
            && arg1.iconUrl()==arg2.iconUrl()
            && arg1.isValid()==arg2.isValid();
}

inline hash_type qHash(const QWebEngineHistoryItem &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.originalUrl());
    seed = hash(seed, value.url());
    seed = hash(seed, value.title());
    seed = hash(seed, value.lastVisited());
    seed = hash(seed, value.iconUrl());
    seed = hash(seed, value.isValid());
    return seed;
}

inline bool operator==(const QWebEngineCookieStore::FilterRequest& arg1, const QWebEngineCookieStore::FilterRequest& arg2){
    return arg1.firstPartyUrl==arg2.firstPartyUrl
            && arg1.origin==arg2.origin
            && arg1.thirdParty==arg2.thirdParty;
}

inline hash_type qHash(const QWebEngineCookieStore::FilterRequest &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.thirdParty);
    seed = hash(seed, value.origin);
    seed = hash(seed, value.firstPartyUrl);
    return seed;
}

#endif

#endif // QTJAMBIWEBENGINEWIDGETS_HASHES_H
