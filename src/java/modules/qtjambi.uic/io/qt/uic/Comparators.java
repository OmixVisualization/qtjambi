/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.uic;

import java.util.Comparator;

import io.qt.uic.ui4.DomFont;
import io.qt.uic.ui4.DomResourceIcon;
import io.qt.uic.ui4.DomSizePolicy;

public class Comparators {
	public static class DomFontComparator implements Comparator<DomFont>{
		public int compare(DomFont v1, DomFont v2) {
			String family    = v1.hasElementFamily()     ?     v1.elementFamily() : "";
		    String rhsFamily = v2.hasElementFamily() ? v2.elementFamily() : "";

		    int result;
		    if ((result = family.compareTo(rhsFamily))!=0)
		        return result;

		    int pointSize    = v1.hasElementPointSize()     ?     v1.elementPointSize() : -1;
		    int rhsPointSize = v2.hasElementPointSize() ? v2.elementPointSize() : -1;

		    if ((result = Integer.compare(pointSize, rhsPointSize))!=0)
		        return result;

		    int bold    = v1.hasElementBold()     ? (v1.elementBold()     ? 1 : 0) : -1;
		    int rhsBold = v2.hasElementBold() ? (v2.elementBold() ? 1 : 0) : -1;
		    if ((result = Integer.compare(bold, rhsBold))!=0)
		        return result;

		    int italic    = v1.hasElementItalic()     ? (v1.elementItalic()     ? 1 : 0) : -1;
		    int rhsItalic = v2.hasElementItalic() ? (v2.elementItalic() ? 1 : 0) : -1;
		    if ((result = Integer.compare(italic, rhsItalic))!=0)
		        return result;

		    int underline    = v1.hasElementUnderline()     ? (v1.elementUnderline()     ? 1 : 0) : -1;
		    int rhsUnderline = v2.hasElementUnderline() ? (v2.elementUnderline() ? 1 : 0) : -1;
		    if ((result = Integer.compare(underline, rhsUnderline))!=0)
		        return result;

		    int weight    = v1.hasElementWeight()     ?     v1.elementWeight() : -1;
		    int rhsWeight = v2.hasElementWeight() ? v2.elementWeight() : -1;
		    if ((result = Integer.compare(weight, rhsWeight))!=0)
		        return result;

		    int strikeOut    = v1.hasElementStrikeOut()     ? (v1.elementStrikeOut()     ? 1 : 0) : -1;
		    int rhsStrikeOut = v2.hasElementStrikeOut() ? (v2.elementStrikeOut() ? 1 : 0) : -1;
		    if ((result = Integer.compare(strikeOut, rhsStrikeOut))!=0)
		        return result;

		    int kerning    = v1.hasElementKerning()     ? (v1.elementKerning()     ? 1 : 0) : -1;
		    int rhsKerning = v2.hasElementKerning() ? (v2.elementKerning() ? 1 : 0) : -1;
		    if ((result = Integer.compare(kerning, rhsKerning))!=0)
		        return result;

		    int antialiasing    = v1.hasElementAntialiasing()     ? (v1.elementAntialiasing()     ? 1 : 0) : -1;
		    int rhsAntialiasing = v2.hasElementAntialiasing() ? (v2.elementAntialiasing() ? 1 : 0) : -1;
		    if ((result = Integer.compare(antialiasing, rhsAntialiasing))!=0)
		        return result;

		    String styleStrategy    = v1.hasElementStyleStrategy()     ?     v1.elementStyleStrategy() : "";
		    String rhsStyleStrategy = v2.hasElementStyleStrategy() ? v2.elementStyleStrategy() : "";

		    if ((result = styleStrategy.compareTo(rhsStyleStrategy))!=0)
		        return result;

		    return 0;
		}
	}
	
	public static class DomResourceIconComparator implements Comparator<DomResourceIcon>{
		public int compare(DomResourceIcon v1, DomResourceIcon v2) {
			int comp;
		    if (0!=(comp = v1.attributeTheme().compareTo(v2.attributeTheme())))
		        return comp;

		    String normalOff    =     v1.hasElementNormalOff() ?     v1.elementNormalOff().text() : "";
		    String rhsNormalOff = v2.hasElementNormalOff() ? v2.elementNormalOff().text() : "";
		    if (0!=(comp = normalOff.compareTo(rhsNormalOff)))
		        return comp;

		    String normalOn    =     v1.hasElementNormalOn() ?     v1.elementNormalOn().text() : "";
		    String rhsNormalOn = v2.hasElementNormalOn() ? v2.elementNormalOn().text() : "";
		    if (0!=(comp = normalOn.compareTo(rhsNormalOn)))
		        return comp;

		    String disabledOff    =     v1.hasElementDisabledOff() ?     v1.elementDisabledOff().text() : "";
		    String rhsDisabledOff = v2.hasElementDisabledOff() ? v2.elementDisabledOff().text() : "";
		    if (0!=(comp = disabledOff.compareTo(rhsDisabledOff)))
		        return comp;

		    String disabledOn    =     v1.hasElementDisabledOn() ?     v1.elementDisabledOn().text() : "";
		    String rhsDisabledOn = v2.hasElementDisabledOn() ? v2.elementDisabledOn().text() : "";
		    if (0!=(comp = disabledOn.compareTo(rhsDisabledOn)))
		        return comp;

		    String activeOff    =     v1.hasElementActiveOff() ?     v1.elementActiveOff().text() : "";
		    String rhsActiveOff = v2.hasElementActiveOff() ? v2.elementActiveOff().text() : "";
		    if (0!=(comp = activeOff.compareTo(rhsActiveOff)))
		        return comp;

		    String activeOn    =     v1.hasElementActiveOn() ?     v1.elementActiveOn().text() : "";
		    String rhsActiveOn = v2.hasElementActiveOn() ? v2.elementActiveOn().text() : "";
		    if (0!=(comp = activeOn.compareTo(rhsActiveOn)))
		        return comp;

		    String selectedOff    =     v1.hasElementSelectedOff() ?     v1.elementSelectedOff().text() : "";
		    String rhsSelectedOff = v2.hasElementSelectedOff() ? v2.elementSelectedOff().text() : "";
		    if (0!=(comp = selectedOff.compareTo(rhsSelectedOff)))
		        return comp;

		    String selectedOn    =     v1.hasElementSelectedOn() ?     v1.elementSelectedOn().text() : "";
		    String rhsSelectedOn = v2.hasElementSelectedOn() ? v2.elementSelectedOn().text() : "";
		    if (0!=(comp = selectedOn.compareTo(rhsSelectedOn)))
		        return comp;
		    // Pre 4.4 Legacy
		    if (0!=(comp = v1.text().compareTo(v2.text())))
		        return comp;

		    return 0;
		}
	}
	
	public static class DomSizePolicyComparator implements Comparator<DomSizePolicy>{
		public int compare(DomSizePolicy v1, DomSizePolicy v2) {
		    int hSizeType    = v1.hasElementHSizeType()     ? v1.elementHSizeType()     : -1;
		    int rhsHSizeType = v2.hasElementHSizeType() ? v2.elementHSizeType() : -1;
		    
		    if (hSizeType!=rhsHSizeType)
		        return Integer.compare(hSizeType, rhsHSizeType);

		    int vSizeType    = v1.hasElementVSizeType()     ? v1.elementVSizeType()     : -1;
		    int rhsVSizeType = v2.hasElementVSizeType() ? v2.elementVSizeType() : -1;
		    if (vSizeType!=rhsVSizeType)
		        return Integer.compare(vSizeType, rhsVSizeType);

		    int hStretch    =  v1.hasElementHorStretch()     ? v1.elementHorStretch()     : -1;
		    int rhsHStretch =  v2.hasElementHorStretch() ? v2.elementHorStretch() : -1;
		    if (hStretch!=rhsHStretch)
		        return Integer.compare(hStretch, rhsHStretch);

		    int vStretch    =  v1.hasElementVerStretch()     ? v1.elementVerStretch()     : -1;
		    int rhsVStretch =  v2.hasElementVerStretch() ? v2.elementVerStretch() : -1;
		    if (vStretch!=rhsVStretch)
		        return Integer.compare(vStretch, rhsVStretch);

		    String attributeHSizeType    = v1.hasAttributeHSizeType()     ? v1.attributeHSizeType()     : "";
		    String rhsAttributeHSizeType = v2.hasAttributeHSizeType() ? v2.attributeHSizeType() : "";

		    int result;
		    if ((result = attributeHSizeType.compareTo(rhsAttributeHSizeType))!=0)
		        return result;

		    String attributeVSizeType    = v1.hasAttributeVSizeType()     ? v1.attributeVSizeType()     : "";
		    String rhsAttributeVSizeType = v2.hasAttributeVSizeType() ? v2.attributeVSizeType() : "";

		    return attributeVSizeType.compareTo(rhsAttributeVSizeType);
		}
	}
}
