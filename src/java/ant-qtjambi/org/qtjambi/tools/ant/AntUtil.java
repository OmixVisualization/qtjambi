package org.qtjambi.tools.ant;

import org.apache.tools.ant.PropertyHelper;

/**
 * This class exists to help ANT 1.7.x to 1.8.x migration.
 * We know we maybe using deprecated API as we support both.
 * At this time the QtJambi project doesn't need or make use of the new features in
 *  1.8.x at all but 1.7.x is much less likely to see updates and 1.8.x is being
 *  provided as standard.
 *
 */
abstract class AntUtil {
    //@SuppressWarnings("deprecation")
    public static Object getProperty(PropertyHelper props, String attr) {
        //return props.getProperty((String) null, attr);   // ANT 1.7.x
        return props.getProperty(attr);   // ANT 1.8.x
    }

    public static String getPropertyAsString(PropertyHelper props, String attr) {
        Object objectValue = getProperty(props, attr);
        if(objectValue instanceof String)
            return (String) objectValue;
        return null;
    }

    //@SuppressWarnings("deprecation")
    public static void setNewProperty(PropertyHelper props, String name, String value) {
        //props.setNewProperty((String) null, name, value);   // ANT 1.7.x
        props.setNewProperty(name, value);   // ANT 1.8.x
    }

    //@SuppressWarnings("deprecation")
    public static void setProperty(PropertyHelper props, String name, String value) {
        //props.setProperty((String) null, name, value, false);   // ANT 1.7.x
        props.setProperty(name, value, false);   // ANT 1.8.x
    }

    //@SuppressWarnings("deprecation")
    public static void setProperty(PropertyHelper props, String name, String value, boolean verbose) {
        //props.setProperty((String) null, name, value, verbose);   // ANT 1.7.x
        props.setProperty(name, value, verbose);   // ANT 1.8.x
    }
}
