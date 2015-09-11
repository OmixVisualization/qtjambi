
/*
 * This file serves as meta <phonon/phonon> include which is missing
 * in default KDE phonon installation.
 *
 * Used when flag --kde-phonon is passed to generator.
 */

#include <phonon/abstractaudiooutput.h>
#include <phonon/abstractmediastream.h>
#include <phonon/abstractvideooutput.h>
#include <phonon/addoninterface.h>
#include <phonon/audiodataoutput.h>
#include <phonon/audiodataoutputinterface.h>
#include <phonon/audiooutput.h>
#include <phonon/audiooutputinterface.h>
#include <phonon/backendcapabilities.h>
#include <phonon/backendinterface.h>
#include <phonon/effect.h>
#include <phonon/effectinterface.h>
#include <phonon/effectparameter.h>
#include <phonon/effectwidget.h>
#include <phonon/globalconfig.h>
#include <phonon/mediacontroller.h>
#include <phonon/medianode.h>
#include <phonon/mediaobject.h>
#include <phonon/mediaobjectinterface.h>
#include <phonon/mediasource.h>
#include <phonon/mrl.h>  /* since 4.8 ~F16 */
#include <phonon/objectdescription.h>
#include <phonon/objectdescriptionmodel.h>
#include <phonon/path.h>
#include <phonon/phonondefs.h>
#include <phonon/phononnamespace.h>
#include <phonon/platformplugin.h>
#include <phonon/pulsesupport.h>
#include <phonon/seekslider.h>
#include <phonon/streaminterface.h>
#include <phonon/videoplayer.h>
#include <phonon/videowidget.h>
#include <phonon/videowidgetinterface.h>
#include <phonon/volumefadereffect.h>
#include <phonon/volumefaderinterface.h>
#include <phonon/volumeslider.h>
