/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.uic.kotlin;

import static io.qt.core.QLogging.qWarning;
import static io.qt.uic.Utils.propertyMap;

import java.lang.reflect.Array;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

import io.qt.QtEnumerator;
import io.qt.core.QHash;
import io.qt.core.QIODevice;
import io.qt.core.QLogging;
import io.qt.core.QMultiMap;
import io.qt.core.QPair;
import io.qt.core.QSet;
import io.qt.core.QStack;
import io.qt.core.QStringList;
import io.qt.core.QTextStream;
import io.qt.core.QVersionNumber;
import io.qt.core.Qt;
import io.qt.gui.QPalette;
import io.qt.uic.ClassInfoEntries;
import io.qt.uic.Comparators;
import io.qt.uic.ConnectionSyntax;
import io.qt.uic.CustomWidgetsInfo;
import io.qt.uic.Driver;
import io.qt.uic.Option;
import io.qt.uic.SignalSlot;
import io.qt.uic.TreeWalker;
import io.qt.uic.Uic;
import io.qt.uic.Utils;
import io.qt.uic.ui4.DomAction;
import io.qt.uic.ui4.DomActionGroup;
import io.qt.uic.ui4.DomActionRef;
import io.qt.uic.ui4.DomBrush;
import io.qt.uic.ui4.DomButtonGroup;
import io.qt.uic.ui4.DomChar;
import io.qt.uic.ui4.DomColor;
import io.qt.uic.ui4.DomColorGroup;
import io.qt.uic.ui4.DomColorRole;
import io.qt.uic.ui4.DomColumn;
import io.qt.uic.ui4.DomConnection;
import io.qt.uic.ui4.DomCustomWidget;
import io.qt.uic.ui4.DomCustomWidgets;
import io.qt.uic.ui4.DomDate;
import io.qt.uic.ui4.DomDateTime;
import io.qt.uic.ui4.DomFont;
import io.qt.uic.ui4.DomGradient;
import io.qt.uic.ui4.DomGradientStop;
import io.qt.uic.ui4.DomItem;
import io.qt.uic.ui4.DomLayout;
import io.qt.uic.ui4.DomLayoutDefault;
import io.qt.uic.ui4.DomLayoutFunction;
import io.qt.uic.ui4.DomLayoutItem;
import io.qt.uic.ui4.DomLayoutItem.Kind;
import io.qt.uic.ui4.DomLocale;
import io.qt.uic.ui4.DomPalette;
import io.qt.uic.ui4.DomPoint;
import io.qt.uic.ui4.DomPointF;
import io.qt.uic.ui4.DomProperty;
import io.qt.uic.ui4.DomRect;
import io.qt.uic.ui4.DomRectF;
import io.qt.uic.ui4.DomResourceIcon;
import io.qt.uic.ui4.DomResourcePixmap;
import io.qt.uic.ui4.DomRow;
import io.qt.uic.ui4.DomSize;
import io.qt.uic.ui4.DomSizeF;
import io.qt.uic.ui4.DomSizePolicy;
import io.qt.uic.ui4.DomSlots;
import io.qt.uic.ui4.DomSpacer;
import io.qt.uic.ui4.DomString;
import io.qt.uic.ui4.DomStringList;
import io.qt.uic.ui4.DomTabStops;
import io.qt.uic.ui4.DomTime;
import io.qt.uic.ui4.DomUI;
import io.qt.uic.ui4.DomUrl;
import io.qt.uic.ui4.DomWidget;
import io.qt.widgets.QSizePolicy;

/**
 * @hidden
 */
public class WriteInitialization extends TreeWalker {

	public static void formatMemberFnPtrConnection(QTextStream str, SignalSlot signal, SignalSlot slot) {
		str.append(signal.name).append("?.");
		if(signal.metaMethod!=null) {
			str.append(signal.metaMethod.name());
		}else {
			int parenPos = signal.signature.indexOf('(');
			String functionName = signal.signature.substring(0, parenPos);
			str.append(functionName);			
		}
		str.append("?.connect(");
		if(slot.metaMethod!=null) {
			if(slot.isAmbiguous) {
				str.append("QMetaObject.Slot").append(slot.metaMethod.parameterCount());
				List<Class<?>> parameterClassTypes = slot.metaMethod.parameterClassTypes();
				if(!parameterClassTypes.isEmpty()) {
					str.append('<');
					for (int i = 0; i < parameterClassTypes.size(); i++) {
						if(i>0)
							str.append(", ");
						str.append(parameterClassTypes.get(i).getTypeName().replace('$', '.'));
					}
					str.append('>');
				}
				str.append('(');
			}
			str.append(slot.name).append("::").append(slot.metaMethod.name());
			if(slot.isAmbiguous)
				str.append(')');
		}else if(slot.method!=null) {
			if(slot.isAmbiguous) {
				str.append("QMetaObject.Slot").append(slot.method.getParameterCount());
				Class<?>[] parameterClassTypes = slot.method.getParameterTypes();
				if(parameterClassTypes.length>0) {
					str.append('<');
					for (int i = 0; i < parameterClassTypes.length; i++) {
						if(i>0)
							str.append(", ");
						str.append(parameterClassTypes[i].getTypeName().replace('$', '.'));
					}
					str.append('>');
				}
				str.append('(');
			}
			str.append(slot.name).append("::").append(slot.metaMethod.name());
			if(slot.isAmbiguous)
				str.append(')');
		}else {
			int parenPos = slot.signature.indexOf('(');
			String functionName = slot.signature.substring(0, parenPos);
			str.append(slot.name).append("::").append(functionName);
		}
		str.append(')');
	}

	public static void formatStringBasedConnection(QTextStream str, SignalSlot sender, SignalSlot receiver) {
		str.append("QObject.connect(").append(sender.name).append(", \"");
		if(sender.metaMethod!=null)
			str.append(sender.metaMethod.methodSignature());
		else
			str.append(sender.signature);
		str.append("\", ").append(receiver.name).append(", \"");
		if(receiver.metaMethod!=null)
			str.append(receiver.metaMethod.methodSignature());
		else if(receiver.method!=null) {
			str.append(receiver.method.getName()).append('(');
			Class<?>[] params = receiver.method.getParameterTypes();
			for (int i = 0; i < params.length; i++) {
				if(i > 0)
					str.append(", ");
				str.append(params[i].getTypeName().replace('$', '.'));
			}
			str.append(')');
		}else
			str.append(receiver.signature);
		str.append("\")");
	}

	public void formatConnection(QTextStream str, SignalSlot theSignal, SignalSlot theSlot) {
		ConnectionSyntax connectionSyntax = connectionSyntax(theSignal, theSlot);
		switch (connectionSyntax) {
		case MemberFunctionPtr:
			formatMemberFnPtrConnection(str, theSignal, theSlot);
			break;
		case StringBased:
			formatStringBasedConnection(str, theSignal, theSlot);
			break;
		default:
			break;
		}
	}

	public static String formatString(String text) {
		StringBuilder textBuilder = new StringBuilder();
		textBuilder.append('"');
		for (int i = 0, length = Character.codePointCount(text, 0, text.length()); i < length; i++) {
			int codePoint = Character.codePointAt(text, i);
			if(Character.UnicodeBlock.of(codePoint)==Character.UnicodeBlock.BASIC_LATIN) {
				switch(new String(Character.toChars(codePoint))) {
				case "\n": textBuilder.append("\\n"); break;
				case "\r": textBuilder.append("\\r"); break;
				case "\\": textBuilder.append("\\\\"); break;
				case "\b": textBuilder.append("\\b"); break;
				case "\t": textBuilder.append("\\t"); break;
				case "\f": textBuilder.append("\\f"); break;
				case "\"": textBuilder.append("\\\""); break;
				case "\'": textBuilder.append("\\\'"); break;
				default:
					textBuilder.appendCodePoint(codePoint); break;
				}
			}else {
				String hex = Integer.toHexString(codePoint).toUpperCase();
				textBuilder.append("\\u");
				for (int j = hex.length(); j < 4; j++) {
					textBuilder.append('0');
				}
				textBuilder.append(hex);
			}
		}
		textBuilder.append('"');
		return textBuilder.toString();
	}
	
	public WriteInitialization(Uic uic) {
	      m_uic = uic;
	      m_driver = uic.driver();
	      m_output = uic.output();
	      m_option = uic.option();
	      m_indent = m_option.indent + m_option.indent;
	}
	
	enum Enum implements QtEnumerator{
        Use43UiFile,
        TopLevelMargin,
        ChildMargin,
        SubLayoutMargin
    };
    
    enum Flag implements QtEnumerator { 
    	WritePropertyIgnoreMargin(1), 
    	WritePropertyIgnoreSpacing(2), 
    	WritePropertyIgnoreObjectName(4);
    	private final int value;

		private Flag(int value) {
			this.value = value;
		}

		@Override
		public int value() {
			return value;
		}
	}
    
    private static class Item {
    	public enum EmptyItemPolicy implements QtEnumerator{
            DontConstruct,
            ConstructItemOnly,
            ConstructItemAndVariable
        }
    	
    	private static class ItemData
        {
    		ItemData(){}
    		
			enum TemporaryVariableGeneratorPolicy implements QtEnumerator{
                DontGenerate(1),
                GenerateWithMultiDirective(2),
                Generate(3);
                private final int value;

				private TemporaryVariableGeneratorPolicy(int value) {
					this.value = value;
				}

				@Override
				public int value() {
					return value;
				}
            }
            final QMultiMap<String, String> setters = new QMultiMap<>(String.class, String.class);
            final QSet<String> directives = new QSet<>(String.class);
            TemporaryVariableGeneratorPolicy policy = TemporaryVariableGeneratorPolicy.DontGenerate;
        }
    	
		public Item(String itemClassName, String indent, QTextStream setupUiStream, QTextStream retranslateUiStream, Driver driver) {
		    m_itemClassName = itemClassName;
		    m_indent = indent;
		    m_setupUiStream = setupUiStream;
		    m_retranslateUiStream = retranslateUiStream;
		    m_driver = driver;
		}
		
		public String writeSetupUi(String parent) {
			return writeSetupUi(parent, EmptyItemPolicy.ConstructItemOnly);
		}
		
		public String writeSetupUi(String parent, EmptyItemPolicy emptyItemPolicy) {
		    if (emptyItemPolicy == Item.EmptyItemPolicy.DontConstruct && m_setupUiData.policy == ItemData.TemporaryVariableGeneratorPolicy.DontGenerate)
		        return "";

		    if (emptyItemPolicy == Item.EmptyItemPolicy.ConstructItemOnly && m_children.isEmpty()) {
		        if (m_setupUiData.policy == ItemData.TemporaryVariableGeneratorPolicy.DontGenerate) {
		            m_setupUiStream.append(m_indent);
		            m_setupUiStream.append(m_itemClassName);
		            m_setupUiStream.append("(");
		            m_setupUiStream.append(parent);
		            m_setupUiStream.append(")").endl();
		            return "";
		        }
		    }

		    String uniqueName = m_driver.unique("__" + m_itemClassName.toLowerCase());
		    m_setupUiStream.append(m_indent);
            m_setupUiStream.append("val ");
		    m_setupUiStream.append(uniqueName);
            m_setupUiStream.append(" = ");
            m_setupUiStream.append(m_itemClassName);
            m_setupUiStream.append("(");
            m_setupUiStream.append(parent);
            m_setupUiStream.append(")").endl();

		    for(QPair<String, String> pair : m_setupUiData.setters) {
		        m_setupUiStream.append(m_indent);
	            m_setupUiStream.append(uniqueName);
	            m_setupUiStream.append(pair.second);
	            m_setupUiStream.endl();
		    }
		    for(QPair<String, String> pair : m_setupUiData.setters) {
		        m_setupUiStream.append(m_indent).append(uniqueName).append(pair.second).endl();
		    }
		    for (Item child : m_children)
		        child.writeSetupUi(uniqueName);
		    return uniqueName;
		}
		
		public void writeRetranslateUi(String parentPath) {
		    if (m_retranslateUiData.policy == ItemData.TemporaryVariableGeneratorPolicy.DontGenerate)
		        return;

		    String uniqueName = m_driver.unique("___" + m_itemClassName.toLowerCase());
		    m_retranslateUiStream.append(m_indent);
	        m_retranslateUiStream.append("val ");
		    m_retranslateUiStream.append(uniqueName).append(" = ").append(parentPath).endl();

		    for(QPair<String, String> pair : m_retranslateUiData.setters) {
		        m_retranslateUiStream.append(m_indent).append(uniqueName).append("?").append(pair.second).endl();
		    }

		    for (int i = 0; i < m_children.size(); i++) {
		        StringBuilder method = new StringBuilder();
		        method.append(uniqueName).append("?").append(".child(").append(i).append(')');
		        m_children.get(i).writeRetranslateUi(method.toString());
		    }
		}
		
		public void addSetter(String setter, String directive, boolean translatable) {
		    ItemData.TemporaryVariableGeneratorPolicy newPolicy = directive==null || directive.isEmpty() ? ItemData.TemporaryVariableGeneratorPolicy.Generate : ItemData.TemporaryVariableGeneratorPolicy.GenerateWithMultiDirective;
		    if (translatable) {
		        m_retranslateUiData.setters.insert(directive, setter);
		        if (ItemData.TemporaryVariableGeneratorPolicy.GenerateWithMultiDirective == newPolicy)
		            m_retranslateUiData.directives.insert(directive);
		        if (m_retranslateUiData.policy.value() < newPolicy.value())
		            m_retranslateUiData.policy = newPolicy;
		    } else {
		        m_setupUiData.setters.insert(directive, setter);
		        if (ItemData.TemporaryVariableGeneratorPolicy.GenerateWithMultiDirective == newPolicy)
		            m_setupUiData.directives.insert(directive);
		        if (m_setupUiData.policy.value() < newPolicy.value())
		            m_setupUiData.policy = newPolicy;
		    }
		}
		
		public void addChild(Item child) {
		    m_children.add(child);
		    child.m_parent = this;

		    Item c = child;
		    Item p = this;
		    while (p!=null) {
		        p.m_setupUiData.directives.unite(c.m_setupUiData.directives);
		        p.m_retranslateUiData.directives.unite(c.m_retranslateUiData.directives);
		        if (p.m_setupUiData.policy.value() < c.m_setupUiData.policy.value())
		            p.m_setupUiData.policy = c.m_setupUiData.policy;
		        if (p.m_retranslateUiData.policy.value() < c.m_retranslateUiData.policy.value())
		            p.m_retranslateUiData.policy = c.m_retranslateUiData.policy;
		        c = p;
		        p = p.m_parent;
		    }
		}
		
		private ItemData m_setupUiData = new ItemData();
		private ItemData m_retranslateUiData = new ItemData();
		private final List<Item> m_children = new ArrayList<>();
		private Item m_parent = null;

		private String m_itemClassName = "";
		private String m_indent = "";
		private QTextStream m_setupUiStream;
		private QTextStream m_retranslateUiStream;
		private Driver m_driver;
	}
    
    private static class Declaration
    {
        Declaration(String name, String className) {
			this.name = name;
			this.className = className;
		}
		public final String name;
        public final String className;
    }
    
    private static class Buddy{
        Buddy(String labelVarName, String buddyAttributeName) {
			this.labelVarName = labelVarName;
			this.buddyAttributeName = buddyAttributeName;
		}
        
		public final String labelVarName;
        public final String buddyAttributeName;
    }
    
    private static class LayoutDefaultHandler {
    	LayoutDefaultHandler(){}
        void acceptLayoutDefault(DomLayoutDefault node) {
            if (node==null)
                return;
            if (node.hasAttributeMargin()) {
                m_state[Properties.Margin.value()] |= StateFlags.HasDefaultValue.value();
                m_defaultValues[Properties.Margin.value()] = node.attributeMargin();
            }
            if (node.hasAttributeSpacing()) {
                m_state[Properties.Spacing.value()] |= StateFlags.HasDefaultValue.value();
                m_defaultValues[Properties.Spacing.value()]  = node.attributeSpacing();
            }
        }
        void acceptLayoutFunction(DomLayoutFunction node) {
            if (node==null)
                return;
            if (node.hasAttributeMargin()) {
                m_state[Properties.Margin.value()]     |= StateFlags.HasDefaultFunction.value();
                m_functions[Properties.Margin.value()] =  node.attributeMargin();
                m_functions[Properties.Margin.value()] += "()";
            }
            if (node.hasAttributeSpacing()) {
                m_state[Properties.Spacing.value()]     |= StateFlags.HasDefaultFunction.value();
                m_functions[Properties.Spacing.value()] =  node.attributeSpacing();
                m_functions[Properties.Spacing.value()] += "()";
            }
        }
        
        private static void writeSetter(String indent, String varName, String setter, String v, QTextStream str) {
        	str.append(indent).append(varName);
        	if(varName.startsWith("this."))
        		str.append('?');
        	str.append('.').append(setter)
        		.append('(').append(v).append(")").endl();
        }
        
        private static void writeContentsMargins(String indent, String objectName, int value, QTextStream str)
        {
            writeSetter(indent, objectName, "setContentsMargins", value + ", " + value + ", " + value + ", " + value, str);
        }

        // Write out the layout margin and spacing properties applying the defaults.
        public void writeProperties(String indent, String varName,
        					Map<String, DomProperty> properties, int marginType,
                            boolean suppressMarginDefault, QTextStream str) {
            // Write out properties and ignore the ones found in
            // subsequent writing of the property list.
            int defaultSpacing = marginType == WriteInitialization.Enum.Use43UiFile.value() ? -1 : 6;
            writeProperty(Properties.Spacing.value(), indent, varName, properties, "spacing", "setSpacing",
                          defaultSpacing, false, str);
            // We use 9 as TopLevelMargin, since Designer seem to always use 9.
            final int[] layoutmargins = {-1, 9, 9, 0};
            writeProperty(Properties.Margin.value(),  indent, varName, properties, "margin",  "setMargin", layoutmargins[marginType], suppressMarginDefault, str);
        }
        
        private void writeProperty(int p, String indent, String objectName, Map<String, DomProperty> properties,
                String propertyName, String setter, int defaultStyleValue,
                boolean suppressDefault, QTextStream str) {
        	// User value
        	DomProperty prop = properties.get(propertyName);
            if (prop!=null) {
                int value = prop.elementNumber();
                // Emulate the pre 4.3 behaviour: The value form default value was only used to determine
                // the default value, layout properties were always written
                boolean useLayoutFunctionPre43 = !suppressDefault && (m_state[p] == (StateFlags.HasDefaultFunction.value()|StateFlags.HasDefaultValue.value())) && value == m_defaultValues[p];
                if (!useLayoutFunctionPre43) {
                    boolean ifndefMac = (0==(m_state[p] & (StateFlags.HasDefaultFunction.value()|StateFlags.HasDefaultValue.value()))
                                     && value == defaultStyleValue);
                    if (ifndefMac)
                        str.append("#ifndef Q_OS_MAC\n");
                    if (p == Properties.Margin.value()) { // Use setContentsMargins for numeric values
                        writeContentsMargins(indent, objectName, value, str);
                    } else {
                        writeSetter(indent, objectName, setter, ""+value, str);
                    }
                    if (ifndefMac)
                    	str.append("#endif\n");
                    return;
                }
            }
            if (suppressDefault)
                return;
            // get default.
            if ((m_state[p] & StateFlags.HasDefaultFunction.value())!=0) {
                // Do not use setContentsMargins to avoid repetitive evaluations.
                writeSetter(indent, objectName, setter, m_functions[p], str);
                return;
            }
            if ((m_state[p] & StateFlags.HasDefaultValue.value())!=0) {
                if (p == Properties.Margin.value()) { // Use setContentsMargins for numeric values
                    writeContentsMargins(indent, objectName, m_defaultValues[p], str);
                } else {
                    writeSetter(indent, objectName, setter, ""+m_defaultValues[p], str);
                }
            }
        }
        
        private enum Properties implements QtEnumerator { Margin, Spacing, NumProperties };
        private enum StateFlags implements QtEnumerator { 
        	HasDefaultValue(1), 
			HasDefaultFunction(2);
        	private final int value;

			private StateFlags(int value) {
				this.value = value;
			}

			@Override
			public int value() {
				return value;
			}
    	};
    	
    	private final int[] m_state = new int[Properties.NumProperties.value()];
    	private int[] m_defaultValues = new int[Properties.NumProperties.value()];
    	private String[] m_functions = new String[Properties.NumProperties.value()];
    	{
    		Arrays.fill(m_functions, "");
    	}
    }
	
	//
	// widgets
	//
	@Override
    public void acceptUI(DomUI node){
	    m_actionGroupChain.push(null);
	    m_widgetChain.push(null);
	    m_layoutChain.push(null);

	    if (node.hasAttributeConnectslotsbyname())
	        m_connectSlotsByName = node.attributeConnectslotsbyname();

	    DomSlots customSlots = node.elementSlots();
	    if (customSlots!=null) {
	        m_customSlots = customSlots.elementSlot();
	        m_customSignals = customSlots.elementSignal();
	    }

	    acceptLayoutDefault(node.elementLayoutDefault());
	    acceptLayoutFunction(node.elementLayoutFunction());

	    if (node.elementCustomWidgets()!=null)
	        super.acceptCustomWidgets(node.elementCustomWidgets());

	    String className = node.elementClass() + m_option.postfix;
	    m_generatedClass = className;

	    String varName = m_driver.findOrInsertWidget(node.elementWidget());
	    m_mainFormVarName = varName;

	    String widgetClassName = node.elementWidget().attributeClass();

	    m_output.append(m_option.indent)
	            .append("fun setupUi(").append(varName).append(": ").append(widgetClassName).append(")").append("{").endl()
	    		.append(m_option.indent).endl();

	    for (String connection : m_uic.databaseInfo().connections()) {
	        if ("(default)".equals(connection))
	            continue;
	        m_output.append(m_indent).append(Driver.normalizedName(connection)).append("Connection = io.qt.sql.QSqlDatabase.database(").append(formatString(connection)).append(")").endl();
	    }

	    acceptWidget(node.elementWidget());

	    for (Buddy b : m_buddies) {
	        String buddyVarName = m_driver.widgetVariableName(b.buddyAttributeName);
	        if (buddyVarName.isEmpty()) {
	        	System.err.println(String.format("%1$s: Warning: Buddy assignment: '%2$s' is not a valid widget.", m_option.messagePrefix(), b.buddyAttributeName));
	            continue;
	        }

	        m_output.append(m_indent).append(b.labelVarName);
        	if(b.labelVarName.startsWith("this."))
        		m_output.append('?');
        	m_output.append(".setBuddy(").append(buddyVarName).append(")").endl();
	    }

	    if (node.elementTabStops()!=null)
	        acceptTabStops(node.elementTabStops());

	    if (m_delayedActionInitialization.length()>0)
	        m_output.endl().append(m_delayedActionInitialization.toString());

	    m_output.endl().append(m_indent).append("this.retranslateUi(").append(varName).append(")").endl();

	    if (node.elementConnections()!=null)
	        acceptConnections(node.elementConnections());

	    m_delayedOut.flush();
	    if (m_delayedInitialization.length()>0)
	        m_output.endl().append(m_delayedInitialization.toString()).endl();

	    if (m_option.autoConnection && m_connectSlotsByName)
	        m_output.endl().append(m_indent).append("QMetaObject.connectSlotsByName(").append(varName).append(")").endl();

	    m_output.append(m_option.indent).append("}").endl().endl();

	    m_refreshOut.flush();

	    m_output.append(m_option.indent)
		        .append("fun retranslateUi(").append(varName).append(": ").append(widgetClassName).append(")").endl()
				.append(m_option.indent).append("{").endl()
				.append(m_refreshInitialization.toString())
				.append(m_option.indent).append("}").endl();
	    m_layoutChain.pop();
	    m_widgetChain.pop();
	    m_actionGroupChain.pop();
	}
	
	private void addWizardPage(String pageVarName, DomWidget page, String parentWidget){
	    /* If the node has a (free-format) string "pageId" attribute (which could
	     * an integer or an enumeration value), use setPage(), else addPage(). */
	    String id = "";
	    List<DomProperty> attributes = page.elementAttribute();
	    if (!attributes.isEmpty()) {
	        for (DomProperty p : attributes) {
	            if ("pageId".equals(p.attributeName())) {
	            	DomString ds = p.elementString();
	            	if (ds!=null)
	                    id = ds.text();
	                break;
	            }
	        }
	    }
	    m_output.append(m_indent).append(parentWidget);
    	if(parentWidget.startsWith("this."))
    		m_output.append('?');
	    if (id.isEmpty()) {
	    	m_output.append(".addPage(").append(pageVarName).append(")").endl();
	    } else {
	    	m_output.append(".setPage(").append(id).append(", ").append(pageVarName).append(")").endl();
	    }
	}
	
    //
    // Special handling for qtableview/qtreeview fake header attributes
    //
    private final static String realPropertyNames[] = {
        "visible",
        "cascadingSectionResizes",
        "minimumSectionSize",    // before defaultSectionSize
        "defaultSectionSize",
        "highlightSections",
        "showSortIndicator",
        "stretchLastSection"
    };

    private final static String headerPrefixes[] = {"horizontalHeader", "verticalHeader"};
	
	@Override
    public void acceptWidget(DomWidget node){
		m_layoutMarginType = m_widgetChain.count() == 1 ? Enum.TopLevelMargin : Enum.ChildMargin;
	    String className = node.attributeClass();
	    String varName = m_driver.findOrInsertWidget(node);
	    final String qm = varName!=null && varName.startsWith("this.") ? "?" : "";
	    String objectName = node.attributeName();

	    String parentWidget = "", parentClass = "";
	    if (m_widgetChain.top()!=null) {
	        parentWidget = m_driver.findOrInsertWidget(m_widgetChain.top());
	        parentClass = m_widgetChain.top().attributeClass();
	    }

	    String savedParentWidget = parentWidget;

	    if (m_uic.isContainer(parentClass))
	        parentWidget = "";

	    CustomWidgetsInfo cwi = m_uic.customWidgetsInfo();

	    if (m_widgetChain.size() != 1) {
    		m_output.append(m_indent).append(varName).append(" = ").append(cwi.realClassName(className)).append('(').append(parentWidget).append(")").endl();
	    }

	    parentWidget = savedParentWidget;
	    final String pqm = parentWidget!=null && parentWidget.startsWith("this.") ? "?" : "";


	    if (cwi._extends(className, "QComboBox")) {
	        initializeComboBox(node);
	    } else if (cwi._extends(className, "QListWidget")) {
	        initializeListWidget(node);
	    } else if (cwi._extends(className, "QTreeWidget")) {
	        initializeTreeWidget(node);
	    } else if (cwi._extends(className, "QTableWidget")) {
	        initializeTableWidget(node);
	    }

	    if (m_uic.isButton(className))
	        addButtonGroup(node, varName);

	    writeProperties(varName, objectName, className, node.elementProperty());

	    if (parentWidget!=null 
	    		&& !parentWidget.isEmpty()
	    		&& cwi._extends(className, "QMenu")) {
	        initializeMenu(node, parentWidget);
	    }

	    if (node.elementLayout().isEmpty())
	        m_layoutChain.push(null);

	    m_layoutWidget = false;
	    if ("QWidget".equals(className) && !node.hasAttributeNative()) {
	    	DomWidget _parentWidget = m_widgetChain.top();
	    	if (_parentWidget!=null) {
	            String _parentClass = _parentWidget.attributeClass();
	            if (!"QMainWindow".equals(_parentClass)
	                && !m_uic.customWidgetsInfo().isCustomWidgetContainer(_parentClass)
	                && !m_uic.isContainer(_parentClass))
	            m_layoutWidget = true;
	        }
	    }
	    m_widgetChain.push(node);
	    m_layoutChain.push(null);
	    super.acceptWidget(node);
	    m_layoutChain.pop();
	    m_widgetChain.pop();
	    m_layoutWidget = false;

	    Map<String, DomProperty> attributes = propertyMap(node.elementAttribute());

	    String pageDefaultString = "Page";

	    if (cwi._extends(parentClass, "QMainWindow")) {
	        if (cwi._extends(className, "QMenuBar")) {
	            m_output.append(m_indent).append(parentWidget).append(pqm).append(".setMenuBar(").append(varName).append(")").endl();
	        } else if (cwi._extends(className, "QToolBar")) {
	            m_output.append(m_indent).append(parentWidget).append(pqm).append(".addToolBar("
	               ).append(toolBarAreaStringFromDOMAttributes(attributes).replace("::", ".")).append(varName
	               ).append(")").endl();

	            DomProperty pbreak = attributes.get("toolBarBreak");
                if (pbreak!=null && Boolean.parseBoolean(pbreak.elementBool())) {
                    m_output.append(m_indent).append(parentWidget).append(pqm).append(".insertToolBarBreak(").append(varName).append(")").endl();
                }

	        } else if (cwi._extends(className, "QDockWidget")) {
	            m_output.append(m_indent).append(parentWidget).append(pqm).append(".addDockWidget(");
	            DomProperty pstyle = attributes.get("dockWidgetArea");
	            if (pstyle!=null) {
	                m_output.append("Qt.DockWidgetArea.").append(Qt.DockWidgetArea.resolve(pstyle.elementNumber()).name()).append(", ");
	            }
	            m_output.append(varName).append(")").endl();
	        } else if (m_uic.customWidgetsInfo()._extends(className, "QStatusBar")) {
	            m_output.append(m_indent).append(parentWidget).append(pqm).append(".setStatusBar(").append(varName).append(")").endl();
	        } else {
	                m_output.append(m_indent).append(parentWidget).append(pqm).append(".setCentralWidget(").append(varName).append(")").endl();
	        }
	    }

	    // Check for addPageMethod of a custom plugin first
	    String addPageMethod = cwi.customWidgetAddPageMethod(parentClass);
	    if (addPageMethod.isEmpty())
	        addPageMethod = cwi.simpleContainerAddPageMethod(parentClass);
	    if (!addPageMethod.isEmpty()) {
	        m_output.append(m_indent).append(parentWidget).append(pqm).append(".").append(addPageMethod).append('(').append(varName).append(")").endl();
	    } else if (m_uic.customWidgetsInfo()._extends(parentClass, "QWizard")) {
	        addWizardPage(varName, node, parentWidget);
	    } else if (m_uic.customWidgetsInfo()._extends(parentClass, "QToolBox")) {
	        DomProperty plabel = attributes.get("label");
	        DomString plabelString = plabel!=null ? plabel.elementString() : null;
	        String icon = "";
	        DomProperty picon = attributes.get("icon");
	        if (picon!=null)
	            icon = ", " + iconCall(picon); // Side effect: Writes icon definition
	        m_output.append(m_indent).append(parentWidget).append(pqm).append(".addItem("
	           ).append(varName).append(icon).append(", ").append(noTrCall(plabelString, pageDefaultString)
	           ).append(")").endl();

	        autoTrOutput(plabelString, pageDefaultString).append(m_indent).append(parentWidget).append(pqm)
	        	.append(".setItemText(").append(parentWidget).append(pqm).append(".indexOf(" ).append(varName).append(")").append(pqm.isEmpty() ? "" : " as Int")
	        	.append(", ").append(autoTrCall(plabelString, pageDefaultString)).append(")").endl().flush();

	        DomProperty ptoolTip = attributes.get("toolTip");
	        if (ptoolTip!=null) {
	            autoTrOutput(ptoolTip.elementString())
	               .append(m_indent).append(parentWidget).append(pqm).append(".setItemToolTip(").append(parentWidget).append(pqm).append(".indexOf(").append(varName)
	               .append(")").append(pqm.isEmpty() ? "" : " as Int").append(", ").append(autoTrCall(ptoolTip.elementString())).append(")").flush();
	        }
	    } else if (m_uic.customWidgetsInfo()._extends(parentClass, "QTabWidget")) {
	        DomProperty ptitle = attributes.get("title");
	        DomString ptitleString = ptitle!=null ? ptitle.elementString() : null;
	        String icon = "";
	        DomProperty picon = attributes.get("icon");
	        if (picon!=null)
	            icon = ", " + iconCall(picon); // Side effect: Writes icon definition
	        m_output.append(m_indent).append(parentWidget).append(pqm).append(".addTab(").append(varName).append(icon).append(", \"\")").endl();

	        autoTrOutput(ptitleString, pageDefaultString).append(m_indent)
	        .append(parentWidget).append(pqm).append(".setTabText(").append(parentWidget).append(pqm).append(".indexOf(").append(varName)
	        .append(")").append(pqm.isEmpty() ? "" : " as Int").append(", ").append(autoTrCall(ptitleString, pageDefaultString)).append(")").endl().flush();

	        DomProperty ptoolTip = attributes.get("toolTip");
	        if (ptoolTip!=null) {
	            autoTrOutput(ptoolTip.elementString())
	               .append(m_indent).append(parentWidget).append(pqm).append(".setTabToolTip(")
	               .append(parentWidget).append(pqm).append(".indexOf(").append(varName).append(")").append(pqm.isEmpty() ? "" : " as Int").append(", ").append(autoTrCall(ptoolTip.elementString())).append(")").endl().flush();
	        }
	        DomProperty pwhatsThis = attributes.get("whatsThis");
	        if (pwhatsThis!=null) {
	            autoTrOutput(pwhatsThis.elementString())
	               .append(m_indent).append(parentWidget).append(pqm)
	               .append(".setTabWhatsThis(").append(parentWidget).append(pqm).append(".indexOf(")
	               .append(varName).append(")").append(pqm.isEmpty() ? "" : " as Int").append(", ").append(autoTrCall(pwhatsThis.elementString())).append(")").endl().flush();
	        }
	    }

	    if (cwi.extendsOneOf(className, Arrays.asList("QTreeView", "QTreeWidget"))) {
	        List<DomProperty> headerProperties = new ArrayList<>();
	        for (String realPropertyName : realPropertyNames) {
	            String fakePropertyName = "header" + Character.toUpperCase(realPropertyName.charAt(0)) + realPropertyName.substring(1);
	            DomProperty fakeProperty = attributes.get(fakePropertyName);
	            if (fakeProperty!=null) {
	                fakeProperty.setAttributeName(realPropertyName);
	                headerProperties.add(fakeProperty);
	            }
	        }
	        writeProperties(varName + qm + ".header()", objectName + "_headerView",
	                        "QHeaderView", headerProperties,
	                        Flag.WritePropertyIgnoreObjectName.value());

	    } else if (cwi.extendsOneOf(className, Arrays.asList("QTableView", "QTableWidget"))) {
	        for (String headerPrefix : headerPrefixes) {
	        	List<DomProperty> headerProperties = new ArrayList<>();
	            for (String realPropertyName : realPropertyNames) {
	                String fakePropertyName = headerPrefix
	                        + Character.toLowerCase(realPropertyName.charAt(0)) + realPropertyName.substring(1);
	                DomProperty fakeProperty = attributes.get(fakePropertyName);
	                if (fakeProperty!=null) {
	                    fakeProperty.setAttributeName(realPropertyName);
	                    headerProperties.add(fakeProperty);
	                }
	            }
	            String headerVar = varName + qm + "." + headerPrefix + "()";
	            writeProperties(headerVar, objectName+"_"+headerPrefix, "QHeaderView",
	                            headerProperties, Flag.WritePropertyIgnoreObjectName.value());
	        }
	    }

	    if (node.elementLayout().isEmpty())
	        m_layoutChain.pop();

	    QStringList zOrder = node.elementZOrder();
	    for (String name : zOrder) {
	        String _varName = m_driver.widgetVariableName(name);
	        if (_varName.isEmpty()) {
	        	System.err.println(String.format("%1$s: Warning: Z-order assignment: '%2$s' is not a valid widget.",
	                    m_option.messagePrefix(),
	                    name));
	        } else {
	            m_output.append(m_indent).append(_varName).append(qm).append(".raise_()").endl();
	        }
	    }
	}

	@Override
    public void acceptLayout(DomLayout node){
	    String className = node.attributeClass();
	    String varName = m_driver.findOrInsertLayout(node);
	    String objectName = node.attributeName();

	    Map<String, DomProperty> properties = propertyMap(node.elementProperty());
	    boolean oldLayoutProperties = properties.get("margin") != null;

	    boolean isGroupBox = false;

	    m_output.append(m_indent).append(varName).append(" = ").append(className).append('(');

	    if (m_layoutChain.top()==null && !isGroupBox)
	        m_output.append(m_driver.findOrInsertWidget(m_widgetChain.top()));

	    m_output.append(")").endl();

	    // Suppress margin on a read child layout
	    boolean suppressMarginDefault = m_layoutChain.top()!=null;
	    int marginType = Enum.Use43UiFile.value();
	    if (oldLayoutProperties)
	        marginType = m_layoutMarginType.value();
	    m_LayoutDefaultHandler.writeProperties(m_indent, varName, properties, marginType, suppressMarginDefault, m_output);

	    m_layoutMarginType = Enum.SubLayoutMargin;

	    List<DomProperty> propList = node.elementProperty();
	    List<DomProperty> newPropList = new ArrayList<>();
	    if (m_layoutWidget) {
	        boolean left, top, right, bottom;
	        left = top = right = bottom = false;
	        for (DomProperty p : propList) {
	            String propertyName = p.attributeName();
	            if ("leftMargin".equals(propertyName)  && p.kind() == DomProperty.Kind.Number)
	                left = true;
	            else if ("topMargin".equals(propertyName) && p.kind() == DomProperty.Kind.Number)
	                top = true;
	            else if ("rightMargin".equals(propertyName) && p.kind() == DomProperty.Kind.Number)
	                right = true;
	            else if ("bottomMargin".equals(propertyName) && p.kind() == DomProperty.Kind.Number)
	                bottom = true;
	        }
	        if (!left) {
	            DomProperty p = new DomProperty();
	            p.setAttributeName("leftMargin");
	            p.setElementNumber(0);
	            newPropList.add(p);
	        }
	        if (!top) {
	            DomProperty p = new DomProperty();
	            p.setAttributeName("topMargin");
	            p.setElementNumber(0);
	            newPropList.add(p);
	        }
	        if (!right) {
	            DomProperty p = new DomProperty();
	            p.setAttributeName("rightMargin");
	            p.setElementNumber(0);
	            newPropList.add(p);
	        }
	        if (!bottom) {
	            DomProperty p = new DomProperty();
	            p.setAttributeName("bottomMargin");
	            p.setElementNumber(0);
	            newPropList.add(p);
	        }
	        m_layoutWidget = false;
	    }

	    propList.addAll(newPropList);

	    writeProperties(varName, objectName, className, propList, Flag.WritePropertyIgnoreMargin.value()|Flag.WritePropertyIgnoreSpacing.value());

	    // Clean up again:
	    propList.clear();
	    newPropList.clear();

	    m_layoutChain.push(node);
	    super.acceptLayout(node);
	    m_layoutChain.pop();

	    // Stretch? (Unless we are compiling for UIC3)
	    String numberNull = "0";
	    writePropertyList(varName, "setStretch", node.attributeStretch(), numberNull);
	    writePropertyList(varName, "setRowStretch", node.attributeRowStretch(), numberNull);
	    writePropertyList(varName, "setColumnStretch", node.attributeColumnStretch(), numberNull);
	    writePropertyList(varName, "setColumnMinimumWidth", node.attributeColumnMinimumWidth(), numberNull);
	    writePropertyList(varName, "setRowMinimumHeight", node.attributeRowMinimumHeight(), numberNull);
	}
	
	private static String formLayoutRole(int column, int colspan)
	{
	    if (colspan > 1)
	        return "QFormLayout.ItemRole.SpanningRole";
	    return column == 0 ? "QFormLayout.ItemRole.LabelRole" : "QFormLayout.ItemRole.FieldRole";
	}

	private static String layoutAddMethod(DomLayoutItem.Kind kind, String layoutClass)
	{
	    String methodPrefix = "QFormLayout".equals(layoutClass)  ? "set" : "add";
	    switch (kind) {
	    case Widget:
	        return methodPrefix + "Widget";
	    case Layout:
	        return methodPrefix + "Layout";
	    case Spacer:
	        return methodPrefix + "Item";
	    case Unknown:
	        break;
	    }
	    return "";
	}
	
	static String toolbarArea(int v){
		switch(v) {
        case 0:   return "NoToolBarArea";
        case 0x1: return "LeftToolBarArea";
        case 0x2: return "RightToolBarArea";
        case 0x4: return "TopToolBarArea";
        case 0x8: return "BottomToolBarArea";
        case 0xf: return "AllToolBarAreas";
		}
		return "NoToolBarArea";
	}
	
	String toolBarAreaStringFromDOMAttributes(Map<String,DomProperty> attributes) {
        DomProperty pstyle = attributes.get("toolBarArea");
        String result = "";
        if (pstyle==null)
            return "";
        switch (pstyle.kind()) {
        case Number:
            result = toolbarArea(pstyle.elementNumber());
            break;
        case Enum:
            result = pstyle.elementEnum();
            break;
        default:
            break;
        }
        if (!result.startsWith("Qt::")) 
        	result = "Qt.ToolBarArea." + result;
        else
        	result = "Qt.ToolBarArea." + result.substring(4);
        return result + ", ";
    }

    // Write a statement to create a spacer item.
    void writeSpacerItem(DomSpacer node, QTextStream output) {
        Map<String, DomProperty> properties = propertyMap(node.elementProperty());
        output.append("QSpacerItem(");

        int w = 0;
        int h = 0;
        DomProperty sh = properties.get("sizeHint");
        if (sh!=null) {
        	DomSize sizeHint = sh.elementSize();
        	if (sizeHint!=null) {
                w = sizeHint.elementWidth();
                h = sizeHint.elementHeight();
            }
        }
        output.append(w).append(", ").append(h).append(", ");

        // size type
        String sizeType;
        DomProperty st = properties.get("sizeType");
        if (st!=null) {
            String value = st.elementEnum();
            if (value.startsWith("QSizePolicy::"))
                sizeType = value.replace("QSizePolicy::", "QSizePolicy.Policy.");
            else
                sizeType = "QSizePolicy.Policy." + value;
        } else {
            sizeType = "QSizePolicy.Policy.Expanding";
        }

        // orientation
        boolean isVspacer = false;
        DomProperty o = properties.get("orientation");
        if (o!=null) {
            String orientation = o.elementEnum();
            if ("Qt::Vertical".equals(orientation)  || "Vertical".equals(orientation))
                isVspacer = true;
        }
        String horizType = isVspacer ? "QSizePolicy.Policy.Minimum" : sizeType;
        String vertType = isVspacer ? sizeType : "QSizePolicy.Policy.Minimum";
        output.append(horizType).append(", ").append(vertType).append(')');
    }
	
	@Override
    public void acceptSpacer(DomSpacer node){
	    m_output.append(m_indent).append(m_driver.findOrInsertSpacer(node)).append(" = ");
	    writeSpacerItem(node, m_output);
	    m_output.endl();
	}
	
	@Override
    public void acceptLayoutItem(DomLayoutItem node){
	    super.acceptLayoutItem(node);

	    DomLayout layout = m_layoutChain.top();

	    if (layout==null)
	        return;

	    String layoutName = m_driver.findOrInsertLayout(layout);
	    String itemName = m_driver.findOrInsertLayoutItem(node);

	    m_output.endl().append(m_indent).append(layoutName);
	    if(layoutName.startsWith("this."))
	    	m_output.append("?");
	    m_output.append(".").append(layoutAddMethod(node.kind(), layout.attributeClass())).append('(');
	    if ("QGridLayout".equals(layout.attributeClass())) {
	        int row = node.attributeRow();
	        int col = node.attributeColumn();

	        int rowSpan = node.hasAttributeRowSpan() ? node.attributeRowSpan() : 1;
	        int colSpan = node.hasAttributeColSpan() ? node.attributeColSpan() : 1;
	        m_output.append(itemName).append(", ").append(row).append(", ").append(col).append(", ").append(rowSpan).append(", ").append(colSpan);
	        if (!node.attributeAlignment().isEmpty())
	            m_output.append(", ").append(node.attributeAlignment().replace("::", "."));
	    } else if ("QFormLayout".equals(layout.attributeClass())) {
	        int row = node.attributeRow();
	        int colSpan = node.hasAttributeColSpan() ? node.attributeColSpan() : 1;
	        String role = formLayoutRole(node.attributeColumn(), colSpan);
	        m_output.append(row).append(", ").append(role.replace("::", ".")).append(", ").append(itemName);
	    } else {
	        m_output.append(itemName);
	        if (layout.attributeClass().contains("Box") && !node.attributeAlignment().isEmpty())
	            m_output.append(", 0, ").append(node.attributeAlignment().replace("::", "."));
	    }
	    if (node.kind()==Kind.Spacer) {
	    	m_output.append(" as QSpacerItem");
	    }
	    m_output.append(")").endl();
	}

	//
	// actions
	//
	@Override
    public void acceptActionGroup(DomActionGroup node){
	    String varName = m_driver.findOrInsertActionGroup(node);
	    String objectName = node.attributeName();
	    String parentName = m_driver.findOrInsertWidget(m_widgetChain.top());

	    if (m_actionGroupChain.top()!=null)
	        parentName = m_driver.findOrInsertActionGroup(m_actionGroupChain.top());

	    m_output.append(m_indent).append(varName).append(" = QActionGroup(").append(parentName).append(")").endl();
	    writeProperties(varName, objectName, "QActionGroup", node.elementProperty());

	    m_actionGroupChain.push(node);
	    super.acceptActionGroup(node);
	    m_actionGroupChain.pop();
	}
	
	@Override
    public void acceptAction(DomAction node){
	    if (node.hasAttributeMenu())
	        return;

	    String varName = m_driver.findOrInsertAction(node);
	    String objectName = node.attributeName();
	    String parentName = m_driver.findOrInsertWidget(m_widgetChain.top());

	    if (m_actionGroupChain.top()!=null)
	        parentName = m_driver.findOrInsertActionGroup(m_actionGroupChain.top());

	    m_output.append(m_indent).append(varName).append(" = QAction(").append(parentName).append(")").endl();
	    writeProperties(varName, objectName, "QAction", node.elementProperty());
	}
	
	@Override
    public void acceptActionRef(DomActionRef node){
	    String actionName = node.attributeName();
	    if (actionName.isEmpty() || m_widgetChain.top()==null
	        || m_driver.actionGroupByName(actionName)!=null) {
	        return;
	    }

	    final String varName = m_driver.findOrInsertWidget(m_widgetChain.top());
	    final String qm = varName.startsWith("this.") ? "?" : "";

	    if (m_widgetChain.top()!=null && "separator".equals(actionName)) {
	        // separator is always reserved!
	        m_actionOut.append(m_indent).append(varName).append(qm).append(".addSeparator()").endl();
	        return;
	    }

	    DomWidget domWidget = m_driver.widgetByName(actionName);
	    if (domWidget!=null && m_uic.isMenu(domWidget.attributeClass())) {
	        m_actionOut.append(m_indent).append(varName).append(qm).append(".addAction(").append(m_driver.findOrInsertWidget(domWidget)).append("?.menuAction())").endl();
	        return;
	    }

	    DomAction domAction = m_driver.actionByName(actionName);
	    if (domAction==null) {
	    	System.err.println(m_option.messagePrefix() + ": Warning: action `" + actionName + "' not declared");
	        return;
	    }

	    m_actionOut.append(m_indent).append(varName).append(qm).append(".addAction(").append(m_driver.findOrInsertAction(domAction)).append(")").endl();
	}

	//
	// tab stops
	//
	@Override
    public void acceptTabStops(DomTabStops tabStops){
	    String lastName = "";

	    QStringList l = tabStops.elementTabStop();
	    for (int i=0; i<l.size(); ++i) {
	        String name = m_driver.widgetVariableName(l.at(i));

	        if (name.isEmpty()) {
		    	System.err.println(m_option.messagePrefix() + ": Warning: Tab-stop assignment: `" + l.at(i) + "' is not a valid widget.");
	            continue;
	        }

	        if (i == 0) {
	            lastName = name;
	            continue;
	        }
	        if (name.isEmpty() || lastName.isEmpty())
	            continue;

	        m_output.append(m_indent).append("QWidget.setTabOrder("
	           ).append(lastName).append(", ").append(name).append(")").endl();

	        lastName = name;
	    }
	}

	//
	// custom widgets
	//
	@Override
    public void acceptCustomWidgets(DomCustomWidgets node){}
	@Override
    public void acceptCustomWidget(DomCustomWidget node){}

	//
	// layout defaults/functions
	//
	@Override
    public void acceptLayoutDefault(DomLayoutDefault node) { m_LayoutDefaultHandler.acceptLayoutDefault(node); }
	@Override
    public void acceptLayoutFunction(DomLayoutFunction node) { m_LayoutDefaultHandler.acceptLayoutFunction(node); }

	//
	// signal/slot connections
	//
	@Override
    public void acceptConnection(DomConnection connection){
	    String senderName = connection.elementSender();
	    String receiverName = connection.elementReceiver();

	    Declaration senderDecl = findDeclaration(senderName);
	    Declaration receiverDecl = findDeclaration(receiverName);

	    if (senderDecl.name.isEmpty() || receiverDecl.name.isEmpty()) {
	        StringBuilder message = new StringBuilder();
	        message.append(m_option.messagePrefix()
	           ).append(": Warning: Invalid signal/slot connection: \""
	           ).append(senderName).append("\" . \"").append(receiverName).append("\".");
	        System.err.println(message);
	        return;
	    }
	    String senderSignature = connection.elementSignal();
	    CustomWidgetsInfo.MethodInfo methodInfo = m_uic.customWidgetsInfo().methodInfo(senderDecl.className, senderSignature, true);
	    SignalSlot theSignal = new SignalSlot(
	    		senderDecl.name, 
	    		senderSignature,
                senderDecl.className, 
                methodInfo.metaMethod,
                methodInfo.method,
                methodInfo.isAmbiguous);
	    methodInfo = m_uic.customWidgetsInfo().methodInfo(receiverDecl.className, connection.elementSlot(), false);

	    SignalSlot theSlot = new SignalSlot(
	    		receiverDecl.name, 
	    		connection.elementSlot(),
	    		receiverDecl.className, 
	    		methodInfo.metaMethod,
                methodInfo.method,
                methodInfo.isAmbiguous);

	    m_output.append(m_indent);
	    formatConnection(m_output, theSignal, theSlot);
	    m_output.append(';').endl();
	}
	
	private static String domColor2QString(DomColor c) {
	    if (c.hasAttributeAlpha())
	        return String.format("QColor(%1$s, %2$s, %3$s, %4$s)", c.elementRed(), c.elementGreen(), c.elementBlue(), c.attributeAlpha());
	    return String.format("QColor(%1$s, %2$s, %3$s)", c.elementRed(), c.elementGreen(), c.elementBlue());
	}
	
	public static QVersionNumber colorRoleVersionAdded(String roleName)
	{
	    if ("PlaceholderText".equals(roleName))
	        return new QVersionNumber(5, 12, 0);
	    return new QVersionNumber();
	}
	
	private String iconCall(DomProperty icon) {
	    if (icon.kind() == DomProperty.Kind.IconSet)
	        return writeIconProperties(icon.elementIconSet());
	    return pixCall(icon);
	}
	
	private String pixCall(DomProperty p){
	    String type, s;
	    switch (p.kind()) {
	    case IconSet:
	        type = "QIcon";
	        s = p.elementIconSet().text();
	        break;
	    case Pixmap:
	        type = "QPixmap";
	        s = p.elementPixmap().text();
	        break;
	    default:
	        QLogging.qWarning("%s: Warning: Unknown icon format encountered. The ui-file was generated with a too-recent version of Designer.", m_option.messagePrefix());
	        return "QIcon()";
	    }
	    return pixCall(type, s);
	}
	
	private String pixCall(String t, String text){
	    if (text.isEmpty()) {
	        return "" + t + "()";
	    }

	    StringBuilder str = new StringBuilder();
	    str.append("");
	    str.append(t);
	    str.append("(");
	    String pixFunc = m_uic.pixmapFunction();
	    if (pixFunc.isEmpty())
	        str.append(formatString(text));
	    else
	        str.append(pixFunc).append('(').append(formatString(text)).append(')');
	    str.append(')');
	    return str.toString();
	}
	
	private String trCall(String str, String comment){
		return trCall(str, comment, "");
	}
	
	private String trCall(String str, String commentHint, String id){
	    if (str.isEmpty())
	        return "\"\"";

	    StringBuilder result = new StringBuilder();

	    boolean idBasedTranslations = m_driver.useIdBasedTranslations();
	    if (m_option.translateFunction.isEmpty()) {
        	result.append("QCoreApplication.translate("
               ).append('"').append(m_generatedClass.replace("::", ".")).append("\", ");
	    } else {
	    	result.append(m_option.translateFunction).append('(');
	    }

	    result.append(formatString(idBasedTranslations ? id : str));

	    if (!idBasedTranslations && !m_option.idBased) {
	    	result.append(", ");
	        if (commentHint.isEmpty())
	        	result.append("null");
	        else
	        	result.append(formatString(commentHint));
	    }

	    result.append(')');
	    return result.toString();
	}
	
	private String trCall(DomString str){
		return trCall(str, "");
	}
	
	private String trCall(DomString str, String defaultString){
	    String value = defaultString;
	    String comment = "";
	    String id = "";
	    if (str!=null) {
	        value = Utils.toString(str);
	        comment = str.attributeComment();
	        id = str.attributeId();
	    }
	    return trCall(value, comment, id);
	}
	
	private String noTrCall(DomString str){
		return noTrCall(str, "");
	}
	
	private String noTrCall(DomString str, String defaultString){
	    String value = defaultString;
	    if (str==null && defaultString.isEmpty())
	        return "";
	    if (str!=null)
	        value = str.text();
	    StringBuilder ret = new StringBuilder();
	    ret.append(formatString(value));
	    return ret.toString();
	}
	
	private String autoTrCall(DomString str){
		return autoTrCall(str, "");
	}
	
	private String autoTrCall(DomString str, String defaultString){
	    if ((str!=null && !defaultString.isEmpty()) || needsTranslation(str))
	        return trCall(str, defaultString);
	    return noTrCall(str, defaultString);
	}
	
	private static boolean needsTranslation(DomStringList element)
	{
	    if (element==null)
	        return false;
	    return !element.hasAttributeNotr() || !Boolean.parseBoolean(element.attributeNotr());
	}
	
	private static boolean needsTranslation(DomString element)
	{
	    if (element==null)
	        return false;
	    return !element.hasAttributeNotr() || !Boolean.parseBoolean(element.attributeNotr());
	}
	
	private QTextStream autoTrOutput(DomProperty property) {
		DomString str = property.elementString();
	    if (str!=null)
	        return autoTrOutput(str);
	    DomStringList list = property.elementStringList();
	    if (list!=null)
	        if (needsTranslation(list))
	            return m_refreshOut;
	    return m_output;
	}
	
	private QTextStream autoTrOutput(DomString str) {
		return autoTrOutput(str, "");
	}
	
	private QTextStream autoTrOutput(DomString str, String defaultString) {
	    if ((str!=null && !defaultString.isEmpty()) || needsTranslation(str))
	        return m_refreshOut;
	    return m_output;
	}
    
	private void writePropertyList(String varName, String setFunction, String value, String defaultValue) {
	    if (value.isEmpty())
	        return;
	    String[] list = value.split(",");
	    int count =  list.length;
	    for (int i = 0; i < count; i++) {
	        if (!list[i].equals(defaultValue)) {
	            m_output.append(m_indent).append(varName).append('.').append(setFunction
	               ).append('(').append(i).append(", ").append(list[i]).append(")").endl();
	        }
	    }
	}
	
	private String writeStringListProperty(DomStringList list){
	    StringBuilder propertyValue = new StringBuilder();
	    propertyValue.append("QStringList(");
	    QStringList values = list.elementString();
	    if (values.isEmpty())
	        return propertyValue.toString();
	    if (needsTranslation(list)) {
	        String comment = list.attributeComment();
	        for (int i = 0; i < values.size(); ++i) {
	        	if(i!=0)
	        		propertyValue.append(", ");
	            propertyValue.append('\n').append(m_indent).append("   ").append(trCall(values.at(i), comment));
	        }
	    } else {
	        for (int i = 0; i < values.size(); ++i) {
	        	if(i!=0)
	        		propertyValue.append(", ");
	        	propertyValue.append(formatString(values.at(i)));
	        }
	    }
	    return propertyValue.toString();
	}
	
	private void writeProperties(String varName, String objectName, String className, List<DomProperty> lst) {
		writeProperties(varName, objectName, className, lst, 0);
	}
	
	private static boolean checkProperty(CustomWidgetsInfo customWidgetsInfo,
        String fileName, String className, DomProperty p) {
		switch (p.kind()) {
		// ### fixme Qt 7 remove this: Exclude deprecated properties of Qt 5.
		case Set:
			if ("features".equals(p.attributeName()) 
					&& customWidgetsInfo._extends(className, "QDockWidget")
					&& "QDockWidget::AllDockWidgetFeatures".equals(p.elementSet())) {
				String msg = fileName + ": Warning: Deprecated enum value QDockWidget::AllDockWidgetFeatures was encountered.";
				qWarning("%s", msg);
				return false;
			}
			break;
		case Enum:
			if ("sizeAdjustPolicy".equals(p.attributeName())
					&& customWidgetsInfo._extends(className, "QComboBox")
					&& "QComboBox::AdjustToMinimumContentsLength".equals(p.elementEnum())) {
				String msg = fileName + ": Warning: Deprecated enum value QComboBox::AdjustToMinimumContentsLength was encountered.";
				qWarning("%s", msg);
				return false;
			}
		break;
		case IconSet:
			DomResourceIcon dri = p.elementIconSet();
			if (dri!=null) {
				if (!isIconFormat44(dri)) {
				  if (dri.text().isEmpty())  {
				      String msg = String.format("%1$s: Warning: An invalid icon property '%2$s' was encountered.", fileName, p.attributeName());
				      qWarning("%s", msg);
				      return false;
				  }
				}
			}
		break;
		case Pixmap:
			DomResourcePixmap drp = p.elementPixmap();
			if (drp!=null)
				if (drp.text().isEmpty()) {
					String msg = String.format("%1: Warning: An invalid pixmap property '%2' was encountered.", fileName, p.attributeName());
				  qWarning("%s", msg);
				  return false;
				}
			break;
		default:
		break;
		}
		return  true;
}
	
    private final static QStringList currentIndexWidgets = new QStringList(
            "QComboBox", "QStackedWidget",
            "QTabWidget", "QToolBox"
        );
	
	@SuppressWarnings({ "rawtypes", "unchecked" })
	private void writeProperties(String varName, String objectName, String className, List<DomProperty> lst, int flags) {
		final String qm = varName.startsWith("this.") ? "?" : "";
	    boolean isTopLevel = m_widgetChain.count() == 1;

	    if (m_uic.customWidgetsInfo()._extends(className, "QAxWidget")) {
	        Map<String, DomProperty> properties = propertyMap(lst);
	        DomProperty p = properties.get("control");
	        if (p!=null) {
	            m_output.append(m_indent).append(varName).append(qm).append(".setControl("
	               ).append(formatString(Utils.toString(p.elementString()))
	               ).append(")").endl();
	        }
	    }

	    String indent = "";
	    if (m_widgetChain.top()==null) {
	        indent = m_option.indent;
            m_output.append(m_indent).append("if (").append(varName).append(qm).append(".objectName.isEmpty())\n");
	    }
	    if (0==(flags & Flag.WritePropertyIgnoreObjectName.value())) {
	        if (objectName.startsWith("this."))
	        	objectName = objectName.substring(5);
	        m_output.append(m_indent).append(indent
	           ).append(varName).append(qm).append(".objectName = "
	           ).append(formatString(objectName)).endl();
	    }

	    int leftMargin, topMargin, rightMargin, bottomMargin;
	    leftMargin = topMargin = rightMargin = bottomMargin = -1;
	    boolean frameShadowEncountered = false;

	    for (DomProperty p : lst) {
			if (!checkProperty(m_uic.customWidgetsInfo(), m_option.inputFile, className, p))
	            continue;
	        String propertyName = p.attributeName();
	        String propertyValue = "";
	        boolean delayProperty = false;

	        // special case for the property `geometry': Do not use position
	        if (isTopLevel && "geometry".equals(propertyName)  && p.elementRect()!=null) {
	            DomRect r = p.elementRect();
	            m_output.append(m_indent).append(varName).append(qm).append(".resize("
	               ).append(r.elementWidth()).append(", ").append(r.elementHeight()).append(")").endl();
	            continue;
	        }
	        if (propertyName.equals("currentRow") // QListWidget::currentRow
	                && m_uic.customWidgetsInfo()._extends(className, "QListWidget")) {
	            m_delayedOut.append(m_indent).append(varName).append(qm).append(".setCurrentRow(").append(p.elementNumber()).append(")").endl();
	            continue;
	        }
	        if (propertyName.equals("currentIndex") // set currentIndex later
	            && (m_uic.customWidgetsInfo().extendsOneOf(className, currentIndexWidgets))) {
	            m_delayedOut.append(m_indent).append(varName).append(qm).append(".setCurrentIndex(").append(p.elementNumber()).append(")").endl();
	            continue;
	        }
	        if (propertyName.equals("tabSpacing")
	            && m_uic.customWidgetsInfo()._extends(className, "QToolBox")) {
	            m_delayedOut.append(m_indent).append(varName).append(qm).append(".layout().setSpacing(").append(p.elementNumber()).append(")").endl();
	            continue;
	        }
	        if (propertyName.equals("control") // ActiveQt support
	            && m_uic.customWidgetsInfo()._extends(className, "QAxWidget")) {
	            // already done ;)
	            continue;
	        }
	        if (propertyName.equals("default")
	            && m_uic.customWidgetsInfo()._extends(className, "QPushButton")) {
	            // QTBUG-44406: Setting of QPushButton::default needs to be delayed until the parent is set
	            delayProperty = true;
	        } else if (propertyName.equals("database")
	                    && p.elementStringList()!=null) {
	            // Sql support
	            continue;
	        } else if (propertyName.equals("frameworkCode")
	                    && p.kind() == DomProperty.Kind.Bool) {
	            // Sql support
	            continue;
	        } else if (propertyName.equals("orientation")
	                    && m_uic.customWidgetsInfo()._extends(className, "Line")) {
	            // Line support
	            String shape = "QFrame.Shape.HLine";
	            if (p.elementEnum().equals("Qt.Orientation.Vertical"))
	                shape = "QFrame.Shape.VLine";

	            m_output.append(m_indent).append(varName).append(qm).append(".frameShape = ").append(shape).endl();
	            // QFrame Default is 'Plain'. Make the line 'Sunken' unless otherwise specified
	            if (!frameShadowEncountered) {
	                m_output.append(m_indent).append(varName).append(qm).append(".frameShadow = QFrame.Shadow.Sunken").endl();
	            }
	            continue;
	        } else if ((flags & Flag.WritePropertyIgnoreMargin.value())!=0  && propertyName.equals("margin")) {
	            continue;
	        } else if ((flags & Flag.WritePropertyIgnoreSpacing.value())!=0 && propertyName.equals("spacing")) {
	            continue;
	        } else if (propertyName.equals("leftMargin") && p.kind() == DomProperty.Kind.Number) {
	            leftMargin = p.elementNumber();
	            continue;
	        } else if (propertyName.equals("topMargin") && p.kind() == DomProperty.Kind.Number) {
	            topMargin = p.elementNumber();
	            continue;
	        } else if (propertyName.equals("rightMargin") && p.kind() == DomProperty.Kind.Number) {
	            rightMargin = p.elementNumber();
	            continue;
	        } else if (propertyName.equals("bottomMargin") && p.kind() == DomProperty.Kind.Number) {
	            bottomMargin = p.elementNumber();
	            continue;
	        } else if (propertyName.equals("numDigits") // Deprecated in Qt 4, removed in Qt 5.
	                   && m_uic.customWidgetsInfo()._extends(className, "QLCDNumber")) {
	            QLogging.qWarning("Widget '%s': Deprecated property QLCDNumber::numDigits encountered. It has been replaced by QLCDNumber::digitCount.", varName);
	            propertyName = "digitCount";
	        } else if (propertyName.equals("frameShadow")) {
	            frameShadowEncountered = true;
	        }

	        boolean stdset = m_stdsetdef;
	        if (p.hasAttributeStdset())
	            stdset = p.attributeStdset()!=0;

        	StringBuilder setFunction = new StringBuilder();
        	boolean needPar = false;
	        {
	            if (stdset) {
	            	setFunction.append(".").append(propertyName).append(" = ");
	            } else {
	            	needPar = true;
	            	setFunction.append(".setProperty(\"").append(propertyName).append("\", ");
	            }
	        } // QTextStream

	        String varNewName = varName;
	        
	        boolean needBracket = false;
	        QTextStream o = delayProperty ? m_delayedOut : autoTrOutput(p);

	        switch (p.kind()) {
	        case Bool: {
	            propertyValue = ""+Boolean.parseBoolean(p.elementBool());
	            break;
	        }
	        case Color:
	            propertyValue = domColor2QString(p.elementColor());
	            break;
	        case Cstring:
	            if (propertyName.equals("buddy") && m_uic.customWidgetsInfo()._extends(className, "QLabel")) {
	                Buddy buddy = new Buddy(varName, p.elementCstring());
	                m_buddies.add(buddy);
	            } else {
	                StringBuilder str = new StringBuilder();
	                if (!stdset)
	                    str.append("QByteArray(");
	                str.append(formatString(p.elementCstring()));
	                if (!stdset)
	                    str.append(')');
	                propertyName = str.toString();
	            }
	            break;
	        case Cursor:
	            propertyValue = String.format("QCursor(Qt.CursorShape.%1$s)", Qt.CursorShape.resolve(p.elementCursor()).name());
	            break;
	        case CursorShape:
	            if (p.hasAttributeStdset() && p.attributeStdset()==0)
	                varNewName += ".viewport()";
	            propertyValue = String.format("QCursor(Qt.CursorShape.%1$s)", p.elementCursorShape());
	            break;
	        case Enum:
	            propertyValue = p.elementEnum();
	            {
	            	int idx = propertyValue.lastIndexOf("::");
	            	if(idx>0) {
	            		String ns = propertyValue.substring(0, idx);
	            		String enm = propertyValue.substring(idx+2);
	            		String qual = ClassInfoEntries.classToJavaQualifier(ns);
	            		propertyValue = null;
	            		if(qual!=null) {
	            			try {
								Class<?> cls = Class.forName(qual);
								for(Class ecl : cls.getDeclaredClasses()) {
									if(ecl.isEnum()) {
										java.lang.Enum e = null;
										try {
											e = java.lang.Enum.valueOf(ecl, enm);
										} catch (IllegalArgumentException e1) {}
										if(e!=null) {
											propertyValue = ecl.getName().replace('$', '.') + '.' + e.name();
											if(cls.getPackage()!=null && !cls.getPackage().getName().isEmpty())
												propertyValue = propertyValue.substring(cls.getPackage().getName().length()+1);
											break;
										}
									}
								}
							} catch (ClassNotFoundException e) {
								e.printStackTrace();
							}
	            		}
	            		if(propertyValue==null) {
	            			propertyValue = ns.replace("::", ".") + "." + enm;
	            		}
	            	}else {
	            		propertyValue = propertyValue.replace("::", ".");
	            	}
	            }
	            break;
	        case Set:
		        {
		        	List<String> flagList = new ArrayList<>();
		        	Class enumClass = null;
		        	for(String _flg : p.elementSet().split("\\|")) {
		        		int idx = _flg.lastIndexOf("::");
		            	if(idx>0) {
			        		String ns = _flg.substring(0, idx);
		            		String enm = _flg.substring(idx+2);
		            		String qual = ClassInfoEntries.classToJavaQualifier(ns);
		            		_flg = null;
		            		if(qual!=null) {
		            			try {
		            				if(enumClass==null) {
										Class<?> cls = Class.forName(qual);
										for(Class ecl : cls.getDeclaredClasses()) {
											if(ecl.isEnum()) {
												java.lang.Enum e = null;
												try {
													e = java.lang.Enum.valueOf(ecl, enm);
												} catch (IllegalArgumentException e1) {}
												if(e!=null) {
													_flg = ecl.getName().replace('$', '.') + '.' + e.name();
													enumClass = ecl;
													if(cls.getPackage()!=null && !cls.getPackage().getName().isEmpty())
														_flg = _flg.substring(cls.getPackage().getName().length()+1);
													break;
												}
											}
										}
		            				}else {
		            					java.lang.Enum e = null;
										try {
											e = java.lang.Enum.valueOf(enumClass, enm);
										} catch (IllegalArgumentException e1) {}
										if(e!=null) {
											_flg = enumClass.getName().replace('$', '.') + '.' + e.name();
											if(enumClass.getDeclaringClass().getPackage()!=null && !enumClass.getDeclaringClass().getPackage().getName().isEmpty())
												_flg = _flg.substring(enumClass.getDeclaringClass().getPackage().getName().length()+1);
										}
		            				}
								} catch (ClassNotFoundException e) {
									e.printStackTrace();
								}
		            			if(_flg==null) {
		            				_flg = ns.replace("::", ".") + "." + enm;
			            		}
		            			flagList.add(_flg);
		            		}
		            	}else {
		            		flagList.add(_flg.replace("::", "."));
		            	}
		        	}
		        	propertyValue = String.join(", ", flagList);
		        	if(enumClass!=null) {
		        		try {
							if(Modifier.isStatic(enumClass.getMethod("flags", Array.newInstance(enumClass, 0).getClass()).getModifiers())) {
								propertyValue = enumClass.getName().replace('$', '.') + ".flags(" + propertyValue + ")";
								if(enumClass.getDeclaringClass().getPackage()!=null && !enumClass.getDeclaringClass().getPackage().getName().isEmpty())
									propertyValue = propertyValue.substring(enumClass.getDeclaringClass().getPackage().getName().length()+1);
							}
						} catch (Exception e) {}
		        	}
		        }
	            break;
	        case Font:
//	        	o.append(m_indent).append('{').endl().flush();
//	        	m_indent = m_indent+"    ";
//	        	needBracket = true;
	            propertyValue = writeFontProperties(p.elementFont());
	            break;
	        case IconSet:
//	        	o.append(m_indent).append('{').endl().flush();
//	        	m_indent = m_indent+"    ";
//	        	needBracket = true;
	            propertyValue = writeIconProperties(p.elementIconSet());
	            break;
	        case Pixmap:
//	        	o.append(m_indent).append('{').endl().flush();
//	        	m_indent = m_indent+"    ";
//	        	needBracket = true;
	            propertyValue = pixCall(p);
	            break;
	        case Palette: {
//	        	o.append(m_indent).append('{').endl().flush();
//	        	m_indent = m_indent+"    ";
//	        	needBracket = true;
	            DomPalette pal = p.elementPalette();
	            String paletteName = m_driver.unique("palette");
	            m_output.append(m_indent).append("val ").append(paletteName).append(" = QPalette()").endl();
	            writeColorGroup(pal.elementActive(), "QPalette.ColorGroup.Active", paletteName);
	            writeColorGroup(pal.elementInactive(), "QPalette.ColorGroup.Inactive", paletteName);
	            writeColorGroup(pal.elementDisabled(), "QPalette.ColorGroup.Disabled", paletteName);

	            propertyValue = paletteName;
	            break;
	        }
	        case Point: {
	            DomPoint po = p.elementPoint();
	            propertyValue = String.format("QPoint(%1$s, %2$s)",po.elementX(),po.elementY());
	            break;
	        }
	        case PointF: {
	            DomPointF pof = p.elementPointF();
	            propertyValue = String.format("QPointF(%1$s, %2$s)",pof.elementX(),pof.elementY());
	            break;
	        }
	        case Rect: {
	            DomRect r = p.elementRect();
	            propertyValue = String.format("QRect(%1$s, %2$s, %3$s, %4$s)",r.elementX(),r.elementY(),r.elementWidth(),r.elementHeight());
	            break;
	        }
	        case RectF: {
	            DomRectF rf = p.elementRectF();
	            propertyValue = String.format("QRectF(%1$s, %2$s, %3$s, %4$s)",rf.elementX(),rf.elementY(),rf.elementWidth(),rf.elementHeight());
	            break;
	        }
	        case Locale: {
	             DomLocale locale = p.elementLocale();
	             StringBuilder str = new StringBuilder();
	             str.append("QLocale(QLocale.Language.")
	             	.append(locale.attributeLanguage())
	             	.append(", QLocale.Country.").append(locale.attributeCountry()).append(')');
	             propertyValue = str.toString();
	            break;
	        }
	        case SizePolicy: {
	            String spName = writeSizePolicy( p.elementSizePolicy());
	            m_output.append(m_indent).append(spName).append(".setHeightForWidth("
	               ).append(varName).append(qm).append(".sizePolicy").append(qm).append(".hasHeightForWidth()").append(qm.isEmpty() ? "" : "==true").append(")").endl();

	            propertyValue = spName;
	            break;
	        }
	        case Size: {
	             DomSize s = p.elementSize();
	              propertyValue = String.format("QSize(%1$s, %2$s)", s.elementWidth(), s.elementHeight());
	            break;
	        }
	        case SizeF: {
	            DomSizeF sf = p.elementSizeF();
	             propertyValue = String.format("QSizeF(%1$s, %2$s)", sf.elementWidth(), sf.elementHeight());
	            break;
	        }
	        case String: {
	            if (propertyName.equals("objectName")) {
	                String v = p.elementString().text();
	                if (v.equals(varName))
	                    break;
	                // ### qWarning("Deprecated: the property `objectName' is different from the variable name");
	            }

	            propertyValue = autoTrCall(p.elementString());
	            break;
	        }
	        case Number:
	            propertyValue = ""+p.elementNumber();
	            break;
	        case UInt:
	            propertyValue = ""+p.elementUInt();
	            break;
	        case LongLong:
	            propertyValue = p.elementLongLong()+"L";
	            break;
	        case ULongLong:
	            propertyValue = p.elementULongLong()+"L";
	            break;
	        case Float:
	            propertyValue = Float.toString(p.elementFloat());
	            break;
	        case Double:
	            propertyValue = Double.toString(p.elementDouble());
	            break;
	        case Char: {
	            DomChar c = p.elementChar();
	            StringBuilder textBuilder = new StringBuilder();
	            textBuilder.append('\'');
	            if(Character.UnicodeBlock.of(c.elementUnicode())==Character.UnicodeBlock.BASIC_LATIN) {
					textBuilder.appendCodePoint(c.elementUnicode());
				}else {
					String hex = Integer.toHexString(c.elementUnicode()).toUpperCase();
					textBuilder.append("\\u");
					for (int j = hex.length(); j < 4; j++) {
						textBuilder.append('0');
					}
					textBuilder.append(hex);
				}
	            textBuilder.append('\'');
	            propertyValue = textBuilder.toString();
	            break;
	        }
	        case Date: {
	            DomDate d = p.elementDate();
	            propertyValue = String.format("QDate(%1$s, %2$s, %3$s)",
	            						d.elementYear(),
	            						d.elementMonth(),
	            						d.elementDay());
	            break;
	        }
	        case Time: {
	            DomTime t = p.elementTime();
	            propertyValue = String.format("QTime(%1$s, %2$s, %3$s)", 
					            		t.elementHour(),
					            		t.elementMinute(),
					            		t.elementSecond());
	            break;
	        }
	        case DateTime: {
	            DomDateTime dt = p.elementDateTime();
	            propertyValue = String.format("QDateTime(new QDate(%1$s, %2$s, %3$s), new QTime(%4$s, %5$s, %6$s))", 
	            					dt.elementYear(),
	                            	dt.elementMonth(),
	                            	dt.elementDay(),
	                            	dt.elementHour(),
	                            	dt.elementMinute(),
	                            	dt.elementSecond());
	            break;
	        }
	        case StringList:
	            propertyValue = writeStringListProperty(p.elementStringList());
	            break;

	        case Url: {
	            DomUrl u = p.elementUrl();
	            StringBuilder _propertyValue = new StringBuilder();
	            _propertyValue.append("QUrl("
	               ).append(formatString(u.elementString().text())).append(")");
                propertyValue = ""+propertyValue;
	            break;
	        }
	        case Brush:
//	        	o.append(m_indent).append('{').endl().flush();
//	        	m_indent = m_indent+"    ";
//	        	needBracket = true;
	            propertyValue = writeBrushInitialization(p.elementBrush());
	            break;
	        case Unknown:
	            break;
	        }

	        if (!propertyValue.isEmpty()) {
	            o.append(m_indent).append(varNewName).append(qm).append(setFunction.toString()).append(propertyValue);
	            if(needPar)
	            	o.append(")");
	            o.endl().flush();
	            if(needBracket) {
	            	m_indent = m_indent.substring(4);
	            	o.append(m_indent).append('}').endl().flush();
	            }
	        }
	    }
	    if (leftMargin != -1 || topMargin != -1 || rightMargin != -1 || bottomMargin != -1) {
	        m_output.append(m_indent).append(varName).append(qm).append(".setContentsMargins("
	           ).append(leftMargin).append(", ").append(topMargin).append(", "
	           ).append(rightMargin).append(", ").append(bottomMargin).append(")").endl();
	    }
	}
	
	private void writeColorGroup(DomColorGroup colorGroup, String group, String paletteName) {
	    if (colorGroup==null)
	        return;

	    // old format
	    List<DomColor> colors = colorGroup.elementColor();
	    for (int i=0; i<colors.size(); ++i) {
	        DomColor color = colors.get(i);

	        m_output.append(m_indent).append(paletteName).append(".setColor(").append(group
	           ).append(", ").append("QPalette.ColorRole.").append(QPalette.ColorRole.resolve(i).name()
	           ).append(", ").append(domColor2QString(color)
	           ).append(")").endl();
	    }

	    // new format
	    List<DomColorRole> colorRoles = colorGroup.elementColorRole();
	    for (DomColorRole colorRole : colorRoles) {
	        if (colorRole.hasAttributeRole()) {
	            String roleName = colorRole.attributeRole();
//	            QVersionNumber versionAdded = colorRoleVersionAdded(roleName);
	            String brushName = writeBrushInitialization(colorRole.elementBrush());
	            m_output.append(m_indent).append(paletteName).append(".setBrush("
	               ).append(group).append(", QPalette.ColorRole.").append(roleName
	               ).append(", ").append(brushName).append(")").endl();
	        }
	    }
	}
	
	private void writeBrush(DomBrush brush, String brushName) {
	    String style = "SolidPattern";
	    if (brush.hasAttributeBrushStyle())
	        style = brush.attributeBrushStyle();

	    if (style.equals("LinearGradientPattern") ||
	            style.equals("RadialGradientPattern") ||
	            style.equals("ConicalGradientPattern")) {
	        DomGradient gradient = brush.elementGradient();
	        String gradientType = gradient.attributeType();
	        String gradientName = m_driver.unique("gradient");
	        if (gradientType.equals("LinearGradient")) {
	            m_output.append(m_indent
        		   ).append("val ").append(gradientName).append(" = QLinearGradient("
	               ).append(gradient.attributeStartX()
	               ).append(", ").append(gradient.attributeStartY()
	               ).append(", ").append(gradient.attributeEndX()
	               ).append(", ").append(gradient.attributeEndY()).append(")").endl();
	        } else if (gradientType.equals("RadialGradient")) {
	            m_output.append(m_indent
	            	   ).append("val ").append(gradientName).append(" = QRadialGradient("
		               ).append(gradient.attributeCentralX()
		               ).append(", ").append(gradient.attributeCentralY()
		               ).append(", ").append(gradient.attributeRadius()
		               ).append(", ").append(gradient.attributeFocalX()
		               ).append(", ").append(gradient.attributeFocalY()).append(")").endl();
	        } else if (gradientType.equals("ConicalGradient")) {
	            m_output.append(m_indent
	            		).append("val ").append(gradientName).append(" = QConicalGradient("
	               ).append(gradient.attributeCentralX()
	               ).append(", ").append(gradient.attributeCentralY()
	               ).append(", ").append(gradient.attributeAngle()).append(")").endl();
	        }

	        m_output.append(m_indent).append(gradientName).append(".setSpread(QGradient.").append(gradient.attributeSpread()
	           ).append(")").endl();

	        if (gradient.hasAttributeCoordinateMode()) {
	            m_output.append(m_indent).append(gradientName).append(".setCoordinateMode(QGradient.").append(gradient.attributeCoordinateMode()
	               ).append(")").endl();
	        }

	       List<DomGradientStop> stops = gradient.elementGradientStop();
	        for (DomGradientStop stop : stops) {
	            DomColor color = stop.elementColor();
	            m_output.append(m_indent).append(gradientName).append(".setColorAt("
	               ).append(stop.attributePosition()).append(", "
	               ).append(domColor2QString(color)).append(")").endl();
	        }
	        m_output.append(m_indent).append("val ").append(brushName)
	        		.append(" = QBrush(").append(gradientName).append(")").endl();
	    } else if (style.equals("TexturePattern")) {
	        DomProperty property = brush.elementTexture();
	        String iconValue = iconCall(property);

	        m_output.append(m_indent).append("val ").append(brushName)
	        		.append(" = QBrush(").append(iconValue).append(")").endl();
	    } else {
	        DomColor color = brush.elementColor();
	        m_output.append(m_indent).append("val ").append(brushName)
	        		.append(" = QBrush(").append(domColor2QString(color)).append(")").endl();

	        m_output.append(m_indent).append(brushName)
	        		.append(".setStyle(Qt.BrushStyle.SolidPattern").append(")").endl();
	    }
	}
	
	private void addInitializer(Item item, String name, int column, String value) {
		addInitializer(item, name, column, value, "", false);
	}
	
	private void addInitializer(Item item, String name, int column, String value, String directive, boolean translatable) {
	    if (!value.isEmpty()) {
	        StringBuilder setter = new StringBuilder();
	        setter.append(".set").append(Character.toUpperCase(name.charAt(0))).append(name.substring(1)).append('(');
	        if (column >= 0)
	            setter.append(column).append(", ");
	        setter.append(value).append(")");
	        item.addSetter(setter.toString(), directive, translatable);
	    }
	}
	
	private void addQtFlagsInitializer(Item item, Map<String, DomProperty> properties, String name) {
		addQtFlagsInitializer(item, properties, name, -1);
	}
	
	private void addQtFlagsInitializer(Item item, Map<String, DomProperty> properties, String name, int column) {
		DomProperty p = properties.get(name);
		if (p!=null) {
	        String orOperator = "|Qt.";
	        String v = p.elementSet();
	        if (!v.isEmpty()) {
	            v = v.replace("|", orOperator);
	            addInitializer(item, name, column, "Qt." + v);
	        }
	    }
	}
	
	private void addQtEnumInitializer(Item item, Map<String, DomProperty> properties, String name, int column) {
		DomProperty p = properties.get(name);
		if (p!=null) {
	        String v = p.elementEnum();
	        if (!v.isEmpty())
	            addInitializer(item, name, column, "Qt." + v);
	    }
	}
	
	private void addBrushInitializer(Item item, Map<String, DomProperty> properties, String name, int column) {
		DomProperty p = properties.get(name);
		if (p!=null) {
	        if (p.elementBrush()!=null)
	            addInitializer(item, name, column, writeBrushInitialization(p.elementBrush()));
	        else if (p.elementColor()!=null)
	            addInitializer(item, name, column, domColor2QString(p.elementColor()));
	    }
	}
	
	private void addStringInitializer(Item item, Map<String, DomProperty> properties, String name, int column) {
		addStringInitializer(item, properties, name, column, "");
	}
	
	private void addStringInitializer(Item item, Map<String, DomProperty> properties, String name, int column, String directive) {
		DomProperty p = properties.get(name);
		if (p!=null) {
	        DomString str = p.elementString();
	        String text = Utils.toString(str);
	        if (!text.isEmpty()) {
	            boolean translatable = needsTranslation(str);
	            String value = autoTrCall(str);
	            addInitializer(item, name, column, value, directive, translatable);
	        }
	    }
	}
	
	private void addCommonInitializers(Item item, Map<String, DomProperty> properties) {
		addCommonInitializers(item, properties, -1);
	}
	
	private void addCommonInitializers(Item item, Map<String, DomProperty> properties, int column) {
		DomProperty icon = properties.get("icon");
		if (icon!=null)
	        addInitializer(item, "icon", column, iconCall(icon));
	    addBrushInitializer(item, properties, "foreground", column);
	    addBrushInitializer(item, properties, "background", column);
	    DomProperty font = properties.get("font");
	    if (font!=null)
	        addInitializer(item, "font", column, writeFontProperties(font.elementFont()));
	    addQtFlagsInitializer(item, properties, "textAlignment", column);
	    addQtEnumInitializer(item, properties, "checkState", column);
	    addStringInitializer(item, properties, "text", column);
	    addStringInitializer(item, properties, "toolTip", column,
	                         "tooltip");
	    addStringInitializer(item, properties, "whatsThis", column,
	                         "whatsthis");
	    addStringInitializer(item, properties, "statusTip", column,
	                         "statustip");
	}

	private void initializeMenu(DomWidget w, String parentWidget) {
	    String menuName = m_driver.findOrInsertWidget(w);
	    String menuAction = menuName + "Action";

	    DomAction action = m_driver.actionByName(menuAction);
	    if (action!=null && action.hasAttributeMenu()) {
	        m_output.append(m_indent).append(menuAction).append(" = ").append(menuName
	           ).append("?.menuAction()").endl();
	    }
	}
	
	private void initializeComboBox(DomWidget w) {
	    final String varName = m_driver.findOrInsertWidget(w);
	    final String qm = varName.startsWith("this.") ? "?" : "";

	    List<DomItem> items = w.elementItem();

	    if (items.isEmpty())
	        return;

	    for (int i = 0; i < items.size(); ++i) {
	        DomItem item = items.get(i);
	        Map<String, DomProperty> properties = propertyMap(item.elementProperty());
	        DomProperty text = properties.get("text");
	        DomProperty icon = properties.get("icon");

	        String iconValue = "";
	        if (icon!=null)
	            iconValue = iconCall(icon);

	        m_output.append(m_indent).append(varName).append(qm).append(".addItem(");
	        if (icon!=null)
	            m_output.append(iconValue).append(", ");

	        if (needsTranslation(text.elementString())) {
	            m_output.append("\"\")").endl();
	            m_refreshOut.append(m_indent).append(varName).append(qm).append(".setItemText(").append(i).append(", ").append(trCall(text.elementString())
	               ).append(")").endl();
	        } else {
	            m_output.append(noTrCall(text.elementString())).append(")").endl();
	        }
	    }
	    m_refreshOut.endl();
	}
	
	private void initializeListWidget(DomWidget w) {
	    final String varName = m_driver.findOrInsertWidget(w);
	    final String qm = varName.startsWith("this.") ? "?" : "";

	    List<DomItem> items = w.elementItem();

	    if (items.isEmpty())
	        return;

	    String tempName = disableSorting(w, varName);
	    
	    for (int i = 0; i < items.size(); ++i) {
	        DomItem domItem = items.get(i);

	        Map<String, DomProperty> properties = propertyMap(domItem.elementProperty());

	        Item item = new Item("QListWidgetItem", m_indent, m_output, m_refreshOut, m_driver);
	        addQtFlagsInitializer(item, properties, "flags");
	        addCommonInitializers(item, properties);

	        item.writeSetupUi(varName);
	        StringBuilder parentPath = new StringBuilder();
	        parentPath.append(varName).append(qm).append(".item(").append(i).append(')');
	        item.writeRetranslateUi(parentPath.toString());
	    }
	    enableSorting(w, varName, tempName);
	}
	
	private void initializeTreeWidget(DomWidget w) {
	    final String varName = m_driver.findOrInsertWidget(w);
	    final String qm = varName.startsWith("this.") ? "?" : "";

	    // columns
	    Item item = new Item("QTreeWidgetItem", m_indent, m_output, m_refreshOut, m_driver);

	    List<DomColumn> columns = w.elementColumn();
	    for (int i = 0; i < columns.size(); ++i) {
	        DomColumn column = columns.get(i);

	        Map<String, DomProperty> properties = propertyMap(column.elementProperty());
	        addCommonInitializers(item, properties, i);

	        DomProperty p = properties.get("text");
	        if (p!=null) {
	            DomString str = p.elementString();
	            if (str!=null && str.text().isEmpty()) {
	                m_output.append(m_indent).append(varName).append(qm).append(".headerItem()").append(qm).append(".setText(").append(i).append(", \"\")").endl();
	            }
	        }
	    }
	    String itemName = item.writeSetupUi("", Item.EmptyItemPolicy.DontConstruct);
	    item.writeRetranslateUi(varName + qm + ".headerItem()");
	    if (itemName!=null && !itemName.isEmpty()) {
	        m_output.append(m_indent).append(varName).append(qm).append(".setHeaderItem(").append(itemName).append(")").endl();
	    }

	    if (w.elementItem().isEmpty())
	        return;

	    String tempName = disableSorting(w, varName);

	    List<Item> items = initializeTreeWidgetItems(w.elementItem());
	    for (int i = 0; i < items.size(); i++) {
	        Item itm = items.get(i);
	        itm.writeSetupUi(varName);
	        StringBuilder parentPath = new StringBuilder();
	        parentPath.append(varName).append(qm).append(".topLevelItem(").append(i).append(')');
	        itm.writeRetranslateUi(parentPath.toString());
	    }

	    enableSorting(w, varName, tempName);
	}
	
	private List<Item> initializeTreeWidgetItems(List<DomItem > domItems) {
	    // items
	    List<Item> items = new ArrayList<>();
	    int numDomItems = domItems.size();

	    for (int i = 0; i < numDomItems; ++i) {
	        DomItem domItem = domItems.get(i);

	        Item item = new Item("QTreeWidgetItem", m_indent, m_output, m_refreshOut, m_driver);
	        items.add(item);

	        QHash<String, DomProperty> map = new QHash<>(String.class, DomProperty.class);

	        int col = -1;
	        List<DomProperty> properties = domItem.elementProperty();
	        for (DomProperty p : properties) {
	             if (p.attributeName().equals("text")) {
	                if (!map.isEmpty()) {
	                    addCommonInitializers(item, map, col);
	                    map.clear();
	                }
	                col++;
	            }
	            map.insert(p.attributeName(), p);
	        }
	        addCommonInitializers(item, map, col);
	        // AbstractFromBuilder saves flags last, so they always end up in the last column's map.
	        addQtFlagsInitializer(item, map, "flags");

	        List<Item> subItems = initializeTreeWidgetItems(domItem.elementItem());
	        for (Item subItem : subItems)
	            item.addChild(subItem);
	    }
	    return items;
	}
	
	private void initializeTableWidget(DomWidget w) {
	    final String varName = m_driver.findOrInsertWidget(w);
    	final String qm = varName.startsWith("this.") ? "?" : "";

	    // columns
	    List<DomColumn> columns = w.elementColumn();

	    if (!columns.isEmpty()) {
	        m_output.append(m_indent).append("if (").append(qm.isEmpty() ? "" : "(").append(varName).append(qm).append(".columnCount").append(qm.isEmpty() ? "" : " as Int)").append(" < ").append(columns.size()).append(')');
	        m_output.endl().append(m_indent).append(m_option.indent).append(varName).append(qm).append(".columnCount = ").append(columns.size()).endl();
	    }

	    for (int i = 0; i < columns.size(); ++i) {
	        DomColumn column = columns.get(i);
	        if (!column.elementProperty().isEmpty()) {
	            Map<String, DomProperty> properties = propertyMap(column.elementProperty());

	            Item item = new Item("QTableWidgetItem", m_indent, m_output, m_refreshOut, m_driver);
	            addCommonInitializers(item, properties);

	            String itemName = item.writeSetupUi("", Item.EmptyItemPolicy.ConstructItemAndVariable);
	            StringBuilder parentPath = new StringBuilder();
	            parentPath.append(varName).append(qm).append(".horizontalHeaderItem(").append(i).append(')');
	            item.writeRetranslateUi(parentPath.toString());
	            m_output.append(m_indent).append(varName).append(qm).append(".setHorizontalHeaderItem("
	               ).append(i).append(", ").append(itemName).append(")").endl();
	        }
	    }

	    // rows
	    List<DomRow> rows = w.elementRow();

	    if (!rows.isEmpty()) {
	        m_output.append(m_indent).append("if (").append(qm.isEmpty() ? "" : "(").append(varName).append(qm).append(".rowCount").append(qm.isEmpty() ? "" : " as Int)").append(" < ").append(rows.size()).append(')').endl()
	        		.append(m_indent).append(m_option.indent).append(varName).append(qm).append(".rowCount = ").append(rows.size()).endl();
	    }

	    for (int i = 0; i < rows.size(); ++i) {
	        DomRow row = rows.get(i);
	        if (!row.elementProperty().isEmpty()) {
	            Map<String, DomProperty> properties = propertyMap(row.elementProperty());

	            Item item = new Item("QTableWidgetItem", m_indent, m_output, m_refreshOut, m_driver);
	            addCommonInitializers(item, properties);

	            String itemName = item.writeSetupUi("", Item.EmptyItemPolicy.ConstructItemAndVariable);
	            StringBuilder parentPath = new StringBuilder();
	            parentPath.append(varName).append(qm).append(".verticalHeaderItem(").append(i).append(')');
	            item.writeRetranslateUi(parentPath.toString());
	            m_output.append(m_indent).append(varName).append(qm).append(".setVerticalHeaderItem("
	               ).append(i).append(", ").append(itemName).append(")").endl();
	        }
	    }

	    // items
	    String tempName = disableSorting(w, varName);

	    List<DomItem> items = w.elementItem();

	    for (DomItem cell : items) {
	        if (cell.hasAttributeRow() && cell.hasAttributeColumn() && !cell.elementProperty().isEmpty()) {
	            int r = cell.attributeRow();
	            int c = cell.attributeColumn();
	            Map<String, DomProperty> properties = propertyMap(cell.elementProperty());

	            Item item = new Item("QTableWidgetItem", m_indent, m_output, m_refreshOut, m_driver);
	            addQtFlagsInitializer(item, properties, "flags");
	            addCommonInitializers(item, properties);

	            String itemName = item.writeSetupUi("", Item.EmptyItemPolicy.ConstructItemAndVariable);
	            StringBuilder parentPath = new StringBuilder();
	            parentPath.append(varName).append(qm).append(".item(").append(r
	               ).append(", ").append(c).append(')');
	            item.writeRetranslateUi(parentPath.toString());
	            m_output.append(m_indent).append(varName).append(qm).append(".setItem(").append(r).append(", ").append(c).append(", ").append(itemName).append(")").endl();
	        }
	    }
	    enableSorting(w, varName, tempName);
	}

	private String disableSorting(DomWidget w, String varName) {
	    // turn off sortingEnabled to force programmatic item order (setItem())
	    String tempName = "";
	    if (!w.elementItem().isEmpty()) {
	    	final String qm = varName.startsWith("this.") ? "?" : "";
	        tempName = m_driver.unique("__sortingEnabled");
	        m_refreshOut.endl();
	        m_refreshOut.append(m_indent);
            m_refreshOut.append("val ");
	        m_refreshOut.append(tempName).append(" = ").append(varName).append(qm).append(".isSortingEnabled()==true").endl()
	        			.append(m_indent).append(varName).append(qm).append(".setSortingEnabled(false)").endl();
	    }
	    return tempName;
	}
	
	private void enableSorting(DomWidget w, String varName, String tempName) {
	    if (!w.elementItem().isEmpty()) {
	    	final String qm = varName.startsWith("this.") ? "?" : "";
	        m_refreshOut.append(m_indent).append(varName).append(qm).append(".setSortingEnabled(").append(tempName).append(")").append('\n');
	    }
	}
	
	private Declaration findDeclaration(String name) {
		DomWidget widget = m_driver.widgetByName(name);
		if (widget!=null)
	        return new Declaration(m_driver.findOrInsertWidget(widget), widget.attributeClass());
		DomAction action = m_driver.actionByName(name);
		if (action!=null)
	        return new Declaration(m_driver.findOrInsertAction(action), "QAction");
		DomButtonGroup group = m_driver.findButtonGroup(name);
	    if (group!=null)
	         return new Declaration(m_driver.findOrInsertButtonGroup(group), "QButtonGroup");
	    return new Declaration("", "");
	}
	
	private String writeFontProperties(DomFont f) {
	    // check cache
	    if (m_fontPropertiesNameMap.containsKey(f)) {
	        return m_fontPropertiesNameMap.get(f);
	    }

	    // insert with new name
	    String fontName = m_driver.unique("font");
	    m_fontPropertiesNameMap.put(f, fontName);

	    m_output.append(m_indent).append("val ").append(fontName).append(" = QFont()").endl();
	    if (f.hasElementFamily() && !f.elementFamily().isEmpty()) {
	        m_output.append(m_indent).append(fontName).append(".setFamilies("
	           ).append("QList.of("
	           ).append(formatString(f.elementFamily())
	           ).append("))").endl();
	    }
	    if (f.hasElementPointSize() && f.elementPointSize() > 0) {
	         m_output.append(m_indent).append(fontName).append(".setPointSize(").append(f.elementPointSize()
	            ).append(")").endl();
	    }

	    if (f.hasElementBold()) {
	        m_output.append(m_indent).append(fontName).append(".setBold("
	           ).append(Boolean.toString(f.elementBold())).append(")").endl();
	    }
	    if (f.hasElementItalic()) {
	        m_output.append(m_indent).append(fontName).append(".setItalic("
	           ).append(Boolean.toString(f.elementItalic())).append(")").endl();
	    }
	    if (f.hasElementUnderline()) {
	        m_output.append(m_indent).append(fontName).append(".setUnderline("
	           ).append(Boolean.toString(f.elementUnderline())).append(")").endl();
	    }
	    if (f.hasElementStrikeOut()) {
	         m_output.append(m_indent).append(fontName).append(".setStrikeOut("
	           ).append(Boolean.toString(f.elementStrikeOut())).append(")").endl();
	    }
	    if (f.hasElementKerning()) {
	        m_output.append(m_indent).append(fontName).append(".setKerning("
	           ).append(Boolean.toString(f.elementKerning())).append(")").endl();
	    }
	    if (f.hasElementAntialiasing()) {
	        m_output.append(m_indent).append(fontName).append(".setStyleStrategy(QFont.StyleStrategy."
	           ).append((f.elementAntialiasing() ? "PreferDefault" : "NoAntialias")
	           ).append(")").endl();
	    }
	    if (f.hasElementStyleStrategy()) {
	         m_output.append(m_indent).append(fontName).append(".setStyleStrategy(QFont.StyleStrategy.")
	         		 .append(f.elementStyleStrategy()).append(")").endl();
	    }
	    return  fontName;
	}
	
	private static void writeIconAddFile(QTextStream output, String indent,
            String iconName, String fileName,
            String mode, String state){
		output.append(indent).append(iconName).append(".addFile(").append(formatString(fileName)).append(", QSize(), QIcon.Mode.")
			  .append(mode).append(", QIcon.State.").append(state).append(")").endl();
	}
	
	private static void writeResourceIcon(QTextStream output,
            String iconName,
            String indent,
            DomResourceIcon i){
		if (i.hasElementNormalOff()) {
			writeIconAddFile(output, indent, iconName, i.elementNormalOff().text(),
		       "Normal", "Off");
		}
		if (i.hasElementNormalOn()) {
			writeIconAddFile(output, indent, iconName, i.elementNormalOn().text(),
		       "Normal", "On");
		}
		if (i.hasElementDisabledOff()) {
			writeIconAddFile(output, indent, iconName, i.elementDisabledOff().text(),
		       "Disabled", "Off");
		}
		if (i.hasElementDisabledOn()) {
			writeIconAddFile(output, indent, iconName, i.elementDisabledOn().text(),
		       "Disabled", "On");
		}
		if (i.hasElementActiveOff()) {
			writeIconAddFile(output, indent, iconName, i.elementActiveOff().text(),
		       "Active", "Off");
		}
		if (i.hasElementActiveOn()) {
			writeIconAddFile(output, indent, iconName, i.elementActiveOn().text(),
		       "Active", "On");
		}
		if (i.hasElementSelectedOff()) {
			writeIconAddFile(output, indent, iconName, i.elementSelectedOff().text(),
		       "Selected", "Off");
		}
		if (i.hasElementSelectedOn()) {
			writeIconAddFile(output, indent, iconName, i.elementSelectedOn().text(),
		       "Selected", "On");
		}
	}
	
	private static void writeIconAddPixmap(QTextStream output, String indent,
	             String iconName, String call,
	             String mode, String state){
		output.append(indent).append(iconName).append(".addPixmap(").append(call)
		      .append(", QIcon.Mode.").append(mode).append(", QIcon.State.")
		      .append(state).append(")").endl();
	}
	
	private static boolean iconHasStatePixmaps(DomResourceIcon i) {
        return i.hasElementNormalOff()   || i.hasElementNormalOn() ||
               i.hasElementDisabledOff() || i.hasElementDisabledOn() ||
               i.hasElementActiveOff()   || i.hasElementActiveOn() ||
               i.hasElementSelectedOff() || i.hasElementSelectedOn();
    }

	private static boolean isIconFormat44(DomResourceIcon i) {
        return iconHasStatePixmaps(i) || !i.attributeTheme().isEmpty();
    }
	
	private String writeIconProperties(DomResourceIcon i) {
	    // check cache
	    if (m_iconPropertiesNameMap.containsKey(i))
	        return m_iconPropertiesNameMap.get(i);

	    // insert with new name
	    String iconName = m_driver.unique("icon");
	    m_iconPropertiesNameMap.put(i, iconName);

	    if (!isIconFormat44(i)) { // pre-4.4 legacy
	        m_output.append(m_indent);
            m_output.append("val ");
	        m_output.append(iconName).append(" = ").append(pixCall("QIcon", i.text())).append(';').endl();
	        return iconName;
	    }

	    // 4.4 onwards
	    if (i.attributeTheme().isEmpty()) {
	        // No theme: Write resource icon as is
	    	m_output.append(m_indent).append("val ").append(iconName).append(" = QIcon()").endl();
	        if (m_uic.pixmapFunction().isEmpty())
	            writeResourceIcon(m_output, iconName, m_indent, i);
	        else
	            writePixmapFunctionIcon(m_output, iconName, m_indent, i);
	        return iconName;
	    }else if(i.attributeTheme().startsWith(":") || i.attributeTheme().startsWith("classpath:") || i.attributeTheme().startsWith("/:classpath:")) {
	    	m_output.append(m_indent).append("val ").append(iconName).append(" = QIcon(").append(formatString(i.attributeTheme())).append(")").endl();
	    	return iconName;
	    }else if(i.attributeResource().startsWith(":") || i.attributeResource().startsWith("classpath:") || i.attributeResource().startsWith("/:classpath:")) {
	    	m_output.append(m_indent).append("val ").append(iconName).append(" = QIcon(").append(formatString(i.attributeResource())).append(")").endl();
	    	return iconName;
	    }

	    // Theme: Generate code to check the theme and default to resource
	    if (iconHasStatePixmaps(i)) {
	        // Theme + default state pixmaps:
	        // Generate code to check the theme and default to state pixmaps
	    	m_output.append(m_indent).append("val ").append(iconName).append(" = QIcon()").endl();
	        String themeNameStringVariableC = "iconThemeName";
	        // Store theme name in a variable
	        m_output.append(m_indent);
	        if (m_firstThemeIcon) { // Declare variable string
                m_output.append("String ");
	            m_firstThemeIcon = false;
	        }
	        m_output.append(themeNameStringVariableC).append(" = "
	           ).append(formatString(i.attributeTheme())).append(';').endl();
	        m_output.append(m_indent).append("if(");
	        m_output.append("QIcon.hasThemeIcon("
	           ).append(themeNameStringVariableC).append(')').append(") {").endl()
	        	.append(m_indent).append(m_option.indent).append(iconName).append(" = QIcon.fromTheme("
	           ).append(themeNameStringVariableC).append(")").append(m_indent).append("} else {").endl();
	        if (m_uic.pixmapFunction().isEmpty())
	            writeResourceIcon(m_output, iconName, m_indent+m_option.indent, i);
	        else
	            writePixmapFunctionIcon(m_output, iconName, m_indent+m_option.indent, i);
	        m_output.append(m_indent);
            m_output.append('}').endl();
	        return iconName;
	    }

	    // Theme, but no state pixmaps: Construct from theme directly.
	    m_output.append(m_indent).append("val ").append(iconName).append(" = QIcon(QIcon.fromTheme(")
	    					 					  .append(formatString(i.attributeTheme())).append("))").endl();
	    return iconName;
	}
	
	private void writePixmapFunctionIcon(QTextStream output, String iconName,
                                 String indent, DomResourceIcon i) {
	    if (i.hasElementNormalOff()) {
	        writeIconAddPixmap(output, indent,  iconName,
	                           pixCall("QPixmap", i.elementNormalOff().text()),
	                           "Normal", "Off");
	    }
	    if (i.hasElementNormalOn()) {
	        writeIconAddPixmap(output, indent,  iconName,
	                           pixCall("QPixmap", i.elementNormalOn().text()),
	                           "Normal", "On");
	    }
	    if (i.hasElementDisabledOff()) {
	        writeIconAddPixmap(output, indent,  iconName,
	                           pixCall("QPixmap", i.elementDisabledOff().text()),
	                           "Disabled", "Off");
	    }
	    if (i.hasElementDisabledOn()) {
	        writeIconAddPixmap(output, indent,  iconName,
	                           pixCall("QPixmap", i.elementDisabledOn().text()),
	                           "Disabled", "On");
	    }
	    if (i.hasElementActiveOff()) {
	        writeIconAddPixmap(output, indent,  iconName,
	                           pixCall("QPixmap", i.elementActiveOff().text()),
	                           "Active", "Off");
	    }
	    if (i.hasElementActiveOn()) {
	        writeIconAddPixmap(output, indent,  iconName,
	                           pixCall("QPixmap", i.elementActiveOn().text()),
	                           "Active", "On");
	    }
	    if (i.hasElementSelectedOff()) {
	        writeIconAddPixmap(output, indent,  iconName,
	                           pixCall("QPixmap", i.elementSelectedOff().text()),
	                           "Selected", "Off");
	    }
	    if (i.hasElementSelectedOn()) {
	        writeIconAddPixmap(output, indent,  iconName,
	                           pixCall("QPixmap", i.elementSelectedOn().text()),
	                           "Selected", "On");
	    }
	}
	
	private String writeSizePolicy(DomSizePolicy sp) {
	    // check cache
	    if (m_sizePolicyNameMap.containsKey(sp)) {
	        return m_sizePolicyNameMap.get(sp);
	    }


	    // insert with new name
	    String spName = m_driver.unique("sizePolicy");
	    m_sizePolicyNameMap.put(sp, spName);

	    m_output.append(m_indent).append("val ").append(spName).append(" = QSizePolicy(");
	    if (sp.hasElementHSizeType() && sp.hasElementVSizeType()) {
	        m_output.append("QSizePolicy.Policy.")
	        		.append(QSizePolicy.Policy.resolve(sp.elementHSizeType()).name())
	        		.append(", QSizePolicy.Policy.")
	        		.append(QSizePolicy.Policy.resolve(sp.elementVSizeType()).name());
	    } else if (sp.hasAttributeHSizeType() && sp.hasAttributeVSizeType()) {
	        m_output.append("QSizePolicy.Policy.").append(sp.attributeHSizeType()
	           ).append(", QSizePolicy.Policy.").append(sp.attributeVSizeType());
	    }
	    m_output.append(")").endl();

	    m_output.append(m_indent).append(spName).append(".setHorizontalStretch("
	       ).append(sp.elementHorStretch()).append(")").endl();
	    m_output.append(m_indent).append(spName).append(".setVerticalStretch("
	       ).append(sp.elementVerStretch()).append(")").endl();
	    return spName;
	}
	
	private String writeBrushInitialization(DomBrush brush) {
	    // Simple solid, colored  brushes are cached
	    boolean solidColoredBrush = !brush.hasAttributeBrushStyle() || brush.attributeBrushStyle().equals("SolidPattern");
	    Integer rgb = 0;
	    if (solidColoredBrush) {
	    	DomColor color = brush.elementColor();
	    	if (color != null) {
	            rgb = ((color.elementRed() & 0xFF)) << 24 |
	                  ((color.elementGreen() & 0xFF)) << 16 |
	                  ((color.elementBlue() & 0xFF)) << 8 |
	                  ((color.attributeAlpha() & 0xFF));
	            if (m_colorBrushHash.containsKey(rgb))
	                return m_colorBrushHash.get(rgb);
	        }
	    }
	    // Create and enter into cache if simple
	    String brushName = m_driver.unique("brush");
	    writeBrush(brush, brushName);
	    if (solidColoredBrush)
	        m_colorBrushHash.put(rgb, brushName);
	    return brushName;
	}
	
	private void addButtonGroup(DomWidget buttonNode, String varName) {
	    Map<String, DomProperty> attributes = propertyMap(buttonNode.elementAttribute());
	    // Look up the button group name as specified in the attribute and find the uniquified name
	    DomProperty prop = attributes.get("buttonGroup");
	    if (prop==null)
	        return;
	    String attributeName = Utils.toString(prop.elementString());
	    DomButtonGroup group = m_driver.findButtonGroup(attributeName);
	    // Legacy feature: Create missing groups on the fly as the UIC button group feature
	    // was present before the actual Designer support (4.5)
	    boolean createGroupOnTheFly = group == null;
	    if (createGroupOnTheFly) {
	        DomButtonGroup newGroup = new DomButtonGroup();
	        newGroup.setAttributeName(attributeName);
	        group = newGroup;
	        System.err.println(m_option.messagePrefix()+": Warning: Creating button group `" + attributeName + "'");
	    }
	    String groupName = m_driver.findOrInsertButtonGroup(group);
	    String objectName = group.attributeName();
	    // Create on demand
	    if (!m_buttonGroups.contains(groupName)) {
	        String className = "QButtonGroup";
	        m_output.append(m_indent);
	        if (createGroupOnTheFly)
	            m_output.append("val ");
	        m_output.append(groupName).append(" = ").append(className).append('(').append(m_mainFormVarName).append(")").endl();
	        m_buttonGroups.add(groupName);
	        writeProperties(groupName, objectName, className, group.elementProperty());
	    }
	    m_output.append(m_indent).append(groupName).append(".addButton(").append(varName).append(")").endl();
	}
	
	@SuppressWarnings("unused")
	private boolean isCustomWidget(String className) {
	    return m_uic.customWidgetsInfo().customWidget(className) != null;
	}
	
	private ConnectionSyntax connectionSyntax(SignalSlot sender, SignalSlot receiver) {
	    if (m_option.forceMemberFnPtrConnectionSyntax)
	        return ConnectionSyntax.MemberFunctionPtr;
	    if (m_option.forceStringConnectionSyntax)
	        return ConnectionSyntax.StringBased;

	    if ((m_mainFormVarName.equals(sender.name)  && m_customSignals.contains(sender.signature))
	         || (m_mainFormVarName.equals(receiver.name) && m_customSlots.contains(receiver.signature))) {
	        return ConnectionSyntax.StringBased;
	    }
//
//	    return sender.signature.endsWith("()")
//	        || (!isCustomWidget(sender.className) && !isCustomWidget(receiver.className))
//	        ? ConnectionSyntax.MemberFunctionPtr : ConnectionSyntax.StringBased;
	    return ConnectionSyntax.MemberFunctionPtr;
	}	
	
	private final Uic m_uic;
	private final Driver m_driver;
	private final QTextStream m_output;
	private final Option m_option;
	private String m_indent;
	private boolean m_stdsetdef = true;
	private final QStack<DomWidget> m_widgetChain = new QStack<>(DomWidget.class);
	private final QStack<DomLayout> m_layoutChain = new QStack<>(DomLayout.class);
	private final QStack<DomActionGroup> m_actionGroupChain = new QStack<>(DomActionGroup.class);
	private final List<Buddy> m_buddies = new ArrayList<>();
	private final Set<String> m_buttonGroups = new TreeSet<>();
	private final Map<Integer, String> m_colorBrushHash = new TreeMap<>();
	private final Map<DomFont, String> m_fontPropertiesNameMap = new TreeMap<>(new Comparators.DomFontComparator());
	
	private final Map<DomResourceIcon, String> m_iconPropertiesNameMap = new TreeMap<>(new Comparators.DomResourceIconComparator());
    private final Map<DomSizePolicy, String> m_sizePolicyNameMap = new TreeMap<>(new Comparators.DomSizePolicyComparator());
    
    private final LayoutDefaultHandler m_LayoutDefaultHandler = new LayoutDefaultHandler();
    private Enum m_layoutMarginType = Enum.TopLevelMargin;

    private String m_generatedClass;
    private String m_mainFormVarName;
    private List<String> m_customSlots = Collections.emptyList();
    private List<String> m_customSignals = Collections.emptyList();

    private final StringBuilder m_delayedInitialization = new StringBuilder();
    private final QTextStream m_delayedOut = new QTextStream(m_delayedInitialization, QIODevice.OpenModeFlag.WriteOnly);

    private final StringBuilder m_refreshInitialization = new StringBuilder();
    private final QTextStream m_refreshOut = new QTextStream(m_refreshInitialization, QIODevice.OpenModeFlag.WriteOnly);

    private final StringBuilder m_delayedActionInitialization = new StringBuilder();
    private final QTextStream m_actionOut = new QTextStream(m_delayedActionInitialization, QIODevice.OpenModeFlag.WriteOnly);

    private boolean m_layoutWidget = false;
    private boolean m_firstThemeIcon = true;
    private boolean m_connectSlotsByName = true;
}

