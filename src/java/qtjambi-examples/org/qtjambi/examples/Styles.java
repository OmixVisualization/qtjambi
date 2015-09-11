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

package org.qtjambi.examples;

import org.qtjambi.qt.core.QDateTime;
import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QTimer;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPainterPath;
import org.qtjambi.qt.gui.QPalette;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.gui.QPolygon;
import org.qtjambi.qt.gui.QRegion;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QCheckBox;
import org.qtjambi.qt.widgets.QComboBox;
import org.qtjambi.qt.widgets.QCommonStyle;
import org.qtjambi.qt.widgets.QDateTimeEdit;
import org.qtjambi.qt.widgets.QDial;
import org.qtjambi.qt.widgets.QDialog;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QGroupBox;
import org.qtjambi.qt.widgets.QHBoxLayout;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QLineEdit;
import org.qtjambi.qt.widgets.QProgressBar;
import org.qtjambi.qt.widgets.QPushButton;
import org.qtjambi.qt.widgets.QRadioButton;
import org.qtjambi.qt.widgets.QScrollBar;
import org.qtjambi.qt.widgets.QSizePolicy;
import org.qtjambi.qt.widgets.QSlider;
import org.qtjambi.qt.widgets.QSpinBox;
import org.qtjambi.qt.widgets.QStyle;
import org.qtjambi.qt.widgets.QStyle.ControlElement;
import org.qtjambi.qt.widgets.QStyle.PixelMetric;
import org.qtjambi.qt.widgets.QStyleFactory;
import org.qtjambi.qt.widgets.QStyleHintReturn;
import org.qtjambi.qt.widgets.QStyleOption;
import org.qtjambi.qt.widgets.QStyleOptionButton;
import org.qtjambi.qt.widgets.QTabWidget;
import org.qtjambi.qt.widgets.QTableWidget;
import org.qtjambi.qt.widgets.QTextEdit;
import org.qtjambi.qt.widgets.QVBoxLayout;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Styles")
public class Styles extends QDialog
{
    private QPalette originalPalette;

    private QLabel styleLabel;
    private QComboBox styleComboBox;
    private QCheckBox useStylePaletteCheckBox;
    private QCheckBox disableWidgetsCheckBox;

    private QGroupBox topLeftGroupBox;
    private QRadioButton radioButton1;
    private QRadioButton radioButton2;
    private QRadioButton radioButton3;
    private QCheckBox checkBox;

    private QGroupBox topRightGroupBox;
    private QPushButton defaultPushButton;
    private QPushButton togglePushButton;
    private QPushButton flatPushButton;

    private QTabWidget bottomLeftTabWidget;
    private QTableWidget tableWidget;
    private QTextEdit textEdit;

    private QGroupBox bottomRightGroupBox;
    private QLineEdit lineEdit;
    private QSpinBox spinBox;
    private QDateTimeEdit dateTimeEdit;
    private QSlider slider;
    private QScrollBar scrollBar;
    private QDial dial;

    private QProgressBar progressBar;

    public Styles()
    {
        originalPalette = QApplication.palette();

        styleComboBox = new QComboBox();
        styleComboBox.addItem("NorwegianWood");
        styleComboBox.addItems(QStyleFactory.keys());

        styleLabel = new QLabel(tr("&Style:"));
        styleLabel.setBuddy(styleComboBox);

        useStylePaletteCheckBox = new QCheckBox(tr("&Use style's standard palette"));
        useStylePaletteCheckBox.setChecked(true);

        disableWidgetsCheckBox = new QCheckBox(tr("&Disable widgets"));

        createTopLeftGroupBox();
        createTopRightGroupBox();
        createBottomLeftTabWidget();
        createBottomRightGroupBox();
        createProgressBar();

        styleComboBox.activated.connect(this, "changeStyle(String)");
        useStylePaletteCheckBox.toggled.connect(this, "changePalette()");
        disableWidgetsCheckBox.toggled.connect(topLeftGroupBox, "setDisabled(boolean)");
        disableWidgetsCheckBox.toggled.connect(topRightGroupBox, "setDisabled(boolean)");
        disableWidgetsCheckBox.toggled.connect(bottomLeftTabWidget, "setDisabled(boolean)");
        disableWidgetsCheckBox.toggled.connect(bottomRightGroupBox, "setDisabled(boolean)");

        QHBoxLayout topLayout = new QHBoxLayout();
        topLayout.addWidget(styleLabel);
        topLayout.addWidget(styleComboBox);
        topLayout.addStretch(1);
        topLayout.addWidget(useStylePaletteCheckBox);
        topLayout.addWidget(disableWidgetsCheckBox);

        QGridLayout mainLayout = new QGridLayout();
        mainLayout.addLayout(topLayout, 0, 0, 1, 2);
        mainLayout.addWidget(topLeftGroupBox, 1, 0);
        mainLayout.addWidget(topRightGroupBox, 1, 1);
        mainLayout.addWidget(bottomLeftTabWidget, 2, 0);
        mainLayout.addWidget(bottomRightGroupBox, 2, 1);
        mainLayout.addWidget(progressBar, 3, 0, 1, 2);
        mainLayout.setRowStretch(1, 1);
        mainLayout.setRowStretch(2, 1);
        mainLayout.setColumnStretch(0, 1);
        mainLayout.setColumnStretch(1, 1);
        setLayout(mainLayout);

        setWindowTitle(tr("Styles"));
        changeStyle("NorwegianWood");
    }

    public void changeStyle(String styleName)
    {
        if (styleName.equals("NorwegianWood")) {
            QApplication.setStyle(new NorwegianWoodStyle());
        } else {
            QApplication.setStyle(QStyleFactory.create(styleName));
        }
        changePalette();
    }

    public void changePalette()
    {
        if (useStylePaletteCheckBox.isChecked())
            QApplication.setPalette(QApplication.style().standardPalette());
        else
            QApplication.setPalette(originalPalette);
    }

    public void advanceProgressBar()
    {
        int curVal = progressBar.value();
        int maxVal = progressBar.maximum();
        progressBar.setValue(curVal + (maxVal - curVal) / 100);
    }

    public void createTopLeftGroupBox()
    {
        topLeftGroupBox = new QGroupBox(tr("Group 1"));

        radioButton1 = new QRadioButton(tr("Radio button 1"));
        radioButton2 = new QRadioButton(tr("Radio button 2"));
        radioButton3 = new QRadioButton(tr("Radio button 3"));
        radioButton1.setChecked(true);

        checkBox = new QCheckBox(tr("Tri-state check box"));
        checkBox.setTristate(true);
        checkBox.setCheckState(Qt.CheckState.PartiallyChecked);

        QVBoxLayout layout = new QVBoxLayout();
        layout.addWidget(radioButton1);
        layout.addWidget(radioButton2);
        layout.addWidget(radioButton3);
        layout.addWidget(checkBox);
        layout.addStretch(1);
        topLeftGroupBox.setLayout(layout);
    }

    private void createTopRightGroupBox()
    {
        topRightGroupBox = new QGroupBox(tr("Group 2"));

        defaultPushButton = new QPushButton(tr("Default Push Button"));
        defaultPushButton.setDefault(true);

        togglePushButton = new QPushButton(tr("Toggle Push Button"));
        togglePushButton.setCheckable(true);
        togglePushButton.setChecked(true);

        flatPushButton = new QPushButton(tr("Flat Push Button"));
        flatPushButton.setFlat(true);

        QVBoxLayout layout = new QVBoxLayout();
        layout.addWidget(defaultPushButton);
        layout.addWidget(togglePushButton);
        layout.addWidget(flatPushButton);
        layout.addStretch(1);
        topRightGroupBox.setLayout(layout);
    }

    private void createBottomLeftTabWidget()
    {
        bottomLeftTabWidget = new QTabWidget();
        bottomLeftTabWidget.setSizePolicy(QSizePolicy.Policy.Preferred,
                                           QSizePolicy.Policy.Ignored);

        QWidget tab1 = new QWidget();
        tableWidget = new QTableWidget(10, 10);

        QHBoxLayout tab1hbox = new QHBoxLayout();
        tab1hbox.setMargin(5);
        tab1hbox.addWidget(tableWidget);
        tab1.setLayout(tab1hbox);

        QWidget tab2 = new QWidget();
        textEdit = new QTextEdit();

        textEdit.setPlainText(tr("Twinkle, twinkle, little star,\n" +
                                  "How I wonder what you are.\n" +
                                  "Up above the world so high,\n" +
                                  "Like a diamond in the sky.\n" +
                                  "Twinkle, twinkle, little star,\n" +
                                  "How I wonder what you are!\n"));

        QHBoxLayout tab2hbox = new QHBoxLayout();
        tab2hbox.setMargin(5);
        tab2hbox.addWidget(textEdit);
        tab2.setLayout(tab2hbox);

        bottomLeftTabWidget.addTab(tab1, tr("&Table"));
        bottomLeftTabWidget.addTab(tab2, tr("Text &Edit"));
    }

    private void createBottomRightGroupBox()
    {
        bottomRightGroupBox = new QGroupBox(tr("Group 3"));
        bottomRightGroupBox.setCheckable(true);
        bottomRightGroupBox.setChecked(true);

        lineEdit = new QLineEdit("s3cRe7");
        lineEdit.setEchoMode(QLineEdit.EchoMode.Password);

        spinBox = new QSpinBox(bottomRightGroupBox);
        spinBox.setValue(50);

        dateTimeEdit = new QDateTimeEdit(bottomRightGroupBox);
        dateTimeEdit.setDateTime(QDateTime.currentDateTime());

        slider = new QSlider(Qt.Orientation.Horizontal, bottomRightGroupBox);
        slider.setValue(40);

        scrollBar = new QScrollBar(Qt.Orientation.Horizontal, bottomRightGroupBox);
        scrollBar.setValue(60);

        dial = new QDial(bottomRightGroupBox);
        dial.setValue(30);
        dial.setNotchesVisible(true);

        QGridLayout layout = new QGridLayout();
        layout.addWidget(lineEdit, 0, 0, 1, 2);
        layout.addWidget(spinBox, 1, 0, 1, 2);
        layout.addWidget(dateTimeEdit, 2, 0, 1, 2);
        layout.addWidget(slider, 3, 0);
        layout.addWidget(scrollBar, 4, 0);
        layout.addWidget(dial, 3, 1, 2, 1);
        layout.setRowStretch(5, 1);
        bottomRightGroupBox.setLayout(layout);
    }

    private void createProgressBar()
    {
        progressBar = new QProgressBar();
        progressBar.setRange(0, 10000);
        progressBar.setValue(0);

        QTimer timer = new QTimer(this);
        timer.timeout.connect(this, "advanceProgressBar()");
        timer.start(1000);
    }

    class NorwegianWoodStyle extends QCommonStyle
    {
        @Override
        public void polish(QPalette palette)
        {
            QColor beige = new QColor(236, 182, 120);
            QColor slightlyOpaqueBlack = new QColor(0, 0, 0, 63);

            QPixmap backgroundImage = new QPixmap("classpath:org/qtjambi/examples/images/woodbackground.png");
            QPixmap buttonImage = new QPixmap("classpath:org/qtjambi/examples/images/woodbutton.png");

            QPixmap midImage = new QPixmap(buttonImage);

            QPainter painter = new QPainter();
            painter.begin(midImage);
            painter.setPen(Qt.PenStyle.NoPen);
            painter.fillRect(midImage.rect(), new QBrush(slightlyOpaqueBlack));
            painter.end();

            palette.setBrush(QPalette.ColorRole.BrightText, new QBrush(new QColor(Qt.GlobalColor.white)));
            palette.setBrush(QPalette.ColorRole.Base, new QBrush(beige));
            palette.setBrush(QPalette.ColorRole.Highlight, new QBrush(new QColor(Qt.GlobalColor.darkGreen)));
            setTexture(palette, QPalette.ColorRole.Button, buttonImage);
            setTexture(palette, QPalette.ColorRole.Mid, midImage);
            setTexture(palette, QPalette.ColorRole.Window, backgroundImage);

            QBrush brush = palette.window();
            brush.setColor(brush.color().darker());

            palette.setBrush(QPalette.ColorGroup.Disabled, QPalette.ColorRole.Base, brush);
            palette.setBrush(QPalette.ColorGroup.Disabled, QPalette.ColorRole.Button, brush);
            palette.setBrush(QPalette.ColorGroup.Disabled, QPalette.ColorRole.Mid, brush);
        }

        @Override
        public void polish(QWidget widget)
        {
            if (widget instanceof QPushButton
                || widget instanceof QComboBox)
                widget.setAttribute(Qt.WidgetAttribute.WA_Hover, true);
        }

        @Override
        public void unpolish(QWidget widget)
        {
            if (widget instanceof QPushButton
                || widget instanceof QComboBox)
                widget.setAttribute(Qt.WidgetAttribute.WA_Hover, false);
        }

        @Override
        public int pixelMetric(PixelMetric metric, QStyleOption option,
                               QWidget widget)
        {
            switch (metric) {
                case PM_ComboBoxFrameWidth:
                    return 8;
                case PM_ScrollBarExtent:
                    return super.pixelMetric(metric, option, widget) + 4;
                default:
                    return super.pixelMetric(metric, option, widget);
            }
        }

        @Override
        public int styleHint(StyleHint hint, QStyleOption option, QWidget widget,
                             QStyleHintReturn returnData)
        {
            switch (hint) {
                case SH_DitherDisabledText:
                    return 0;
                case SH_EtchDisabledText:
                    return 0;
                default:
                    return super.styleHint(hint, option, widget, returnData);
            }
        }

        @Override
        public void drawPrimitive(QStyle.PrimitiveElement element, QStyleOption option,
                                  QPainter painter, QWidget widget)
        {
            switch (element) {
                case PE_PanelButtonCommand:
                    int delta = option.state().isSet(StateFlag.State_MouseOver) ? 64 : 0;
                    QColor slightlyOpaqueBlack = new QColor(0, 0, 0, 63);
                    QColor semiTransparentWhite = new QColor(255, 255, 255, 127 + delta);
                    QColor semiTransparentBlack = new QColor(0, 0, 0, 127 - delta);

                    int x, y, width, height;
                    x = option.rect().x();
                    y = option.rect().y();
                    width = option.rect().width();
                    height = option.rect().height();

                    QPainterPath roundRect = roundRectPath(option.rect());
                    int radius = Math.min(width, height) / 2;

                    QBrush brush;
                    boolean darker;

                    if (option instanceof QStyleOptionButton &&
                        ((QStyleOptionButton) option).features().isSet(QStyleOptionButton.ButtonFeature.Flat)) {
                        brush = option.palette().brush(QPalette.ColorRole.Window);
                        darker = option.state().isSet(StateFlag.State_Sunken) || option.state().isSet(StateFlag.State_On);
                    } else {
                        if (option.state().isSet(StateFlag.State_Sunken) || option.state().isSet(StateFlag.State_On)) {
                            brush = option.palette().mid();
                            darker = !option.state().isSet(StateFlag.State_Sunken);
                        } else {
                            brush = option.palette().button();
                            darker = false;
                        }
                    }
                    painter.save();
                    painter.setRenderHint(QPainter.RenderHint.Antialiasing, true);
                    painter.fillPath(roundRect, brush);
                    if (darker)
                        painter.fillPath(roundRect, new QBrush(slightlyOpaqueBlack));

                    int penWidth;
                    if (radius < 10)
                        penWidth = 3;
                    else if (radius < 20)
                        penWidth = 5;
                    else
                        penWidth = 7;

                    QPen topPen = new QPen(semiTransparentWhite, penWidth);
                    QPen bottomPen = new QPen(semiTransparentBlack, penWidth);

                    if (option.state().isSet(StateFlag.State_Sunken) ||
                        option.state().isSet(StateFlag.State_On)) {
                        QPen tmpPen = topPen;
                        topPen = bottomPen;
                        bottomPen = tmpPen;
                    }

                    int x1 = x;
                    int x2 = x + radius;
                    int x3 = x + width - radius;
                    int x4 = x + width;

                    if (option.direction().equals(Qt.LayoutDirection.RightToLeft)) {
                        int tmp;

                        tmp = x1;
                        x1 = x4;
                        x4 = tmp;
                        tmp = x2;
                        x3 = x2;
                        x2 = tmp;
                    }

                    QPolygon topHalf = new QPolygon();
                    topHalf.add(new QPoint(x1, y));
                    topHalf.add(new QPoint(x4, y));
                    topHalf.add(new QPoint(x3, y + radius));
                    topHalf.add(new QPoint(x2, y + height - radius));
                    topHalf.add(new QPoint(x1, y + height));

                    painter.setClipPath(roundRect);
                    painter.setClipRegion(new QRegion(topHalf), Qt.ClipOperation.IntersectClip);
                    painter.setPen(topPen);
                    painter.drawPath(roundRect);

                    QPolygon bottomHalf = new QPolygon(topHalf);
                    bottomHalf.replace(0, new QPoint(x4, y + height));
                    painter.setClipPath(roundRect);
                    painter.setClipRegion(new QRegion(bottomHalf), Qt.ClipOperation.IntersectClip);
                    painter.setPen(bottomPen);
                    painter.drawPath(roundRect);

                    painter.setPen(option.palette().windowText().color());
                    painter.setClipping(false);
                    painter.drawPath(roundRect);

                    painter.restore();
                break;
                default:
                    super.drawPrimitive(element, option, painter, widget);
            }
        }

        @Override
        public void drawControl(ControlElement element, QStyleOption option,
                                QPainter painter, QWidget widget)
        {
            switch (element) {
            case CE_PushButtonLabel:
                    QStyleOptionButton myButtonOption = (QStyleOptionButton) option;

                    if (option instanceof QStyleOptionButton) {
                        if (!myButtonOption.palette().currentColorGroup().equals(QPalette.ColorGroup.Disabled)) {
                            if (myButtonOption.state().isSet(StateFlag.State_Sunken) ||
                                myButtonOption.state().isSet(StateFlag.State_On)) {
                                QPalette pal = myButtonOption.palette();
                                pal.setBrush(QPalette.ColorRole.ButtonText,
                                             myButtonOption.palette().brightText());
                                myButtonOption.setPalette(pal);
                            }
                        }
                    }
                    super.drawControl(element, myButtonOption, painter, widget);
                break;
            default:
                super.drawControl(element, option, painter, widget);
            }
        }

        private void setTexture(QPalette palette, QPalette.ColorRole role, QPixmap pixmap)
        {
            for (QPalette.ColorGroup cg : QPalette.ColorGroup.values()) {
                if (cg.value() > 2)
                    continue;

                QColor color = palette.brush(cg, role).color();
                palette.setBrush(cg, role, new QBrush(color, pixmap));
            }
        }

        private QPainterPath roundRectPath(QRect rect)
        {
            int radius = Math.min(rect.width(), rect.height()) / 2;
            int diam = 2 * radius;

            int x1, y1, x2, y2;
            x1 = rect.left();
            y1 = rect.top();
            x2 = rect.right();
            y2 = rect.bottom();

            QPainterPath path = new QPainterPath();
            path.moveTo(x2, y1 + radius);
            path.arcTo(new QRectF(x2 - diam, y1, diam, diam), 0.0, +90.0);
            path.lineTo(x1 + radius, y1);
            path.arcTo(new QRectF(x1, y1, diam, diam), 90.0, +90.0);
            path.lineTo(x1, y2 - radius);
            path.arcTo(new QRectF(x1, y2 - diam, diam, diam), 180.0, +90.0);
            path.lineTo(x1 + radius, y2);
            path.arcTo(new QRectF(x2 - diam, y2 - diam, diam, diam), 270.0, +90.0);
            path.closeSubpath();

            return path;
        }

		@Override
		public QPixmap standardPixmap(StandardPixmap standardPixmap, QStyleOption opt, QWidget widget) {
			return null;
		}
    } // class NorwegianWoodStyle

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new Styles().setVisible(true);

        QApplication.execStatic();
        QApplication.shutdown();
    }

}
