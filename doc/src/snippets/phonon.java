/****************************************************************************
**
** Copyright (C) 2007-2009 Nokia. All rights reserved.
**
** This file is part of the $MODULE$ of the Qt Toolkit.
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

import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.phonon.*;

import java.util.*;

public class phonon extends QWidget
{

public phonon()
{
    {
//![0]
    MediaObject music =
        Phonon.createPlayer(Phonon.Category.MusicCategory,
                            new MediaSource("/path/mysong.wav"));
    music.play();
//![0]
    }

    {
    QWidget parentWidget = new QWidget();
    QUrl url = new QUrl("Myfancymusic");
//![1]
    VideoPlayer player =
        new VideoPlayer(Phonon.Category.VideoCategory, parentWidget);
    player.play(new MediaSource(url));
//![1]
    }

    {
//![2]
    MediaObject mediaObject = new MediaObject(this);
    mediaObject.setCurrentSource(new MediaSource("/mymusic/barbiegirl.wav"));
    AudioOutput audioOutput =
        new AudioOutput(Phonon.Category.MusicCategory, this);
    Path path = Phonon.createPath(mediaObject, audioOutput);
//![2]
    
//![3]
    Effect effect =
        new Effect(
            BackendCapabilities.availableAudioEffects().get(0), this);
    path.insertEffect(effect);
//![3]    
    }

    {
//![4]
    MediaObject mediaObject = new MediaObject(this);

    VideoWidget videoWidget = new VideoWidget(this);
    Phonon.createPath(mediaObject, videoWidget);

    AudioOutput audioOutput =
        new AudioOutput(Phonon.Category.VideoCategory, this);
    Phonon.createPath(mediaObject, audioOutput);
//![4]
//![5]
    mediaObject.play();
//![5]
    }
}

}
