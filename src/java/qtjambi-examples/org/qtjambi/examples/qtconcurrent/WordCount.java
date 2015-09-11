/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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

package org.qtjambi.examples.qtconcurrent;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Set;

import org.qtjambi.qt.concurrent.QtConcurrent;
import org.qtjambi.qt.core.QDir;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QFuture;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.core.QTextStream;
import org.qtjambi.qt.core.QTime;
import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;

public class WordCount implements QtConcurrent.MappedFunctor<HashMap<String, Integer>, String>,
                                  QtConcurrent.ReducedFunctor<HashMap<String, Integer>, HashMap<String, Integer>> {
    private static List<String >findFiles(String startDir, List<String> filters) {
        List<String> names = new ArrayList<String>();
        QDir dir = new QDir(startDir);

        for (String file : dir.entryList(filters, new QDir.Filters(QDir.Filter.Files)))
            names.add(startDir + "/" + file);

        for (String subdir : dir.entryList(new QDir.Filters(QDir.Filter.AllDirs, QDir.Filter.NoDotAndDotDot)))
            names.addAll(findFiles(startDir + "/" + subdir, filters));
        return names;
    }

    /*
        Single threaded word counter function.
    */
    static HashMap<String, Integer> singleThreadedWordCount(List<String> files) {
        HashMap<String, Integer> wordCount = new HashMap<String,Integer>();

        for (String file : files) {
            QFile f = new QFile(file);
            f.open(QIODevice.OpenModeFlag.ReadOnly);
            QTextStream textStream = new QTextStream(f);
            while (!textStream.atEnd()) {
                String[] words = RetroTranslatorHelper.split(textStream.readLine(), " ");
                for (String word : words) {
                    int i = wordCount.containsKey(word) ? wordCount.get(word) : 0;
                    wordCount.put(word, i + 1);
                }
            }

            f.close();
        }

        return wordCount;
    }


//     countWords counts the words in a single file. This function is
//     called in parallel by several threads and must be thread
//     safe.
    public HashMap<String, Integer> map(String file) {
        QFile f = new QFile(file);
        f.open(QIODevice.OpenModeFlag.ReadOnly);
        QTextStream textStream = new QTextStream(f);
        HashMap<String, Integer> wordCount = new HashMap<String, Integer>();

        while (!textStream.atEnd()) {
            String[] words = RetroTranslatorHelper.split(textStream.readLine(), " ");
            for (String word : words) {
                int i = wordCount.containsKey(word) ? wordCount.get(word) : 0;
                wordCount.put(word, i + 1);
            }
        }

        return wordCount;
    }

//     reduce adds the results from map to the final
//     result. This functor will only be called by one thread
//     at a time.
    public void reduce(HashMap<String, Integer> result, HashMap<String, Integer> w) {
        Set<String> keys = w.keySet();
        for (String key : keys) {
            int i = result.containsKey(key) ? result.get(key) : 0;
            result.put(key, i + w.get(key));
        }
    }

    public HashMap<String, Integer> defaultResult() {
        return new HashMap<String, Integer>();
    }

    public static void main(String args[])
    {
        System.out.println("finding files...");

        List<String> filters = new ArrayList<String>();
        filters.add("*.java");

        List<String> files = findFiles("classpath:org/qtjambi/examples", filters);
        System.out.println(files.size() + " files");

        System.out.println("warmup");
        {
            QTime time = new QTime();
            time.start();
            singleThreadedWordCount(files);
        }

        System.out.println("warmup done");

        int singleThreadTime = 0;
        {
            QTime time = new QTime();
            time.start();
            singleThreadedWordCount(files);
            singleThreadTime = time.elapsed();
            System.out.println("single thread: " + singleThreadTime);
        }

        int mapReduceTime = 0;
        {
            QTime time = new QTime();
            time.start();
            WordCount wc = new WordCount();
            QFuture<HashMap<String, Integer>> total = QtConcurrent.mappedReduced(files, wc, wc);
            total.waitForFinished();
            mapReduceTime = time.elapsed();
            System.out.println("MapReduce: " + mapReduceTime);
        }
        System.out.println("MapReduce speedup x " + ((double)singleThreadTime - (double)mapReduceTime) / (double)mapReduceTime + 1);
    }

}
