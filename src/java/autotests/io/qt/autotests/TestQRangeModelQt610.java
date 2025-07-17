package io.qt.autotests;

import static org.junit.Assert.*;

import java.util.*;
import java.util.function.*;

import org.junit.*;

import io.qt.*;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.widgets.*;

public class TestQRangeModelQt610 extends ApplicationInitializer {
	
    @BeforeClass
    public static void testInitialize() throws Exception {
    	QApplication.class.hashCode();
        ApplicationInitializer.testInitializeWithWidgets();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
	}
    
	public static class ColorEntryValue implements Cloneable{
		ColorEntryValue() {
			this("black");
		}
		ColorEntryValue(ColorEntryValue other) {
			this.colorName = other.colorName;
		}
		ColorEntryValue(String colorName) {
			this.colorName = colorName;
		}
		@QtPropertyReader
		public QColor decoration()
	    {
	        return QColor.fromString(colorName);
	    }
		@QtPropertyReader
		public String toolTip()
	    {
	        return QColor.fromString(colorName).name();
	    }
		@QtPropertyReader
		public String display() {
			return colorName;
		}
		@QtPropertyWriter
		public void setDisplay(String colorName) {
			this.colorName = colorName;
		}
		private String colorName;
		@Override
		public ColorEntryValue clone(){
			return new ColorEntryValue(this);
		}
		@Override
		public String toString() {
			return "ColorEntryValue(" + colorName + ")";
		}
		@Override
		public int hashCode() {
			final int prime = 31;
			int result = 1;
			result = prime * result + ((colorName == null) ? 0 : colorName.hashCode());
			return result;
		}
		@Override
		public boolean equals(Object obj) {
			if (this == obj)
				return true;
			if (obj == null)
				return false;
			if (getClass() != obj.getClass())
				return false;
			ColorEntryValue other = (ColorEntryValue) obj;
			if (colorName == null) {
				if (other.colorName != null)
					return false;
			} else if (!colorName.equals(other.colorName))
				return false;
			return true;
		}
	}
	
	public static class ColorEntryGadget{
		ColorEntryGadget() {
			this("black");
		}
		ColorEntryGadget(String colorName) {
			this.colorName = colorName;
		}
		@QtPropertyReader
		public QColor decoration()
	    {
	        return QColor.fromString(colorName);
	    }
		@QtPropertyReader
		public String toolTip()
	    {
	        return QColor.fromString(colorName).name();
	    }
		@QtPropertyReader
		public String display() {
			return colorName;
		}
		@QtPropertyWriter
		public void setDisplay(String colorName) {
			this.colorName = colorName;
		}
		private String colorName;
		@Override
		public String toString() {
			return "ColorEntryGadget(" + colorName + ")";
		}
	}
	
	public static class ColorEntryObject extends QObject{
		ColorEntryObject() {
			this("black");
		}
		ColorEntryObject(String colorName) {
			this.colorName = colorName;
		}
		@QtPropertyReader
		public QColor decoration()
	    {
	        return QColor.fromString(colorName);
	    }
		@QtPropertyReader
		public String toolTip()
	    {
	        return QColor.fromString(colorName).name();
	    }
		@QtPropertyReader
		public String display() {
			return colorName;
		}
		@QtPropertyWriter
		public void setDisplay(String colorName) {
			this.colorName = colorName;
		}
		private String colorName;
		@Override
		public String toString() {
			return "ColorEntryObject(" + colorName + ")";
		}
	}
	
	public static class ColorConstTreeEntryGadget extends QRangeModel.ConstTreeRow<ColorConstTreeEntryGadget>{
		ColorConstTreeEntryGadget(ColorConstTreeEntryGadget parent) {
			this("black", parent);
		}
		ColorConstTreeEntryGadget() {
			this("black");
		}
		ColorConstTreeEntryGadget(String colorName) {
			this(colorName, null);
		}
		ColorConstTreeEntryGadget(String colorName, ColorConstTreeEntryGadget parent) {
			super(parent);
			this.colorName = colorName;
		}
		@QtPropertyReader
		public QColor decoration()
	    {
	        return QColor.fromString(colorName);
	    }
		@QtPropertyReader
		public String toolTip()
	    {
	        return QColor.fromString(colorName).name();
	    }
		@QtPropertyReader
		public String display() {
			return colorName;
		}
		@QtPropertyWriter
		public void setDisplay(String colorName) {
			this.colorName = colorName;
		}
		private String colorName;
		@Override
		public String toString() {
			return "ColorConstTreeEntryGadget(" + colorName + ")";
		}
		public ColorConstTreeEntryGadget addChild(String colorName) {
			return new ColorConstTreeEntryGadget(colorName, this);
		}
	}
	
	public static class ColorTreeEntryGadget extends QRangeModel.TreeRow<ColorTreeEntryGadget>{
		ColorTreeEntryGadget(ColorTreeEntryGadget parent) {
			this("black", parent);
		}
		ColorTreeEntryGadget() {
			this("black");
		}
		ColorTreeEntryGadget(String colorName) {
			this(colorName, null);
		}
		ColorTreeEntryGadget(String colorName, ColorTreeEntryGadget parent) {
			super(parent);
			this.colorName = colorName;
		}
		@QtPropertyReader
		public QColor decoration()
	    {
	        return QColor.fromString(colorName);
	    }
		@QtPropertyReader
		public String toolTip()
	    {
	        return QColor.fromString(colorName).name();
	    }
		@QtPropertyReader
		public String display() {
			return colorName;
		}
		@QtPropertyWriter
		public void setDisplay(String colorName) {
			this.colorName = colorName;
		}
		private String colorName;
		@Override
		public String toString() {
			return "ColorTreeEntryGadget(" + colorName + ")";
		}
		public ColorTreeEntryGadget addChild(String colorName) {
			return new ColorTreeEntryGadget(colorName, this);
		}
	}
	
	public static class ColorConstTreeEntryObject extends QObject implements QRangeModel.ConstTreeRowInterface<ColorConstTreeEntryObject>{
		ColorConstTreeEntryObject(ColorConstTreeEntryObject parent) {
			this("black", parent);
		}
		ColorConstTreeEntryObject() {
			this("black");
		}
		ColorConstTreeEntryObject(String colorName) {
			this(colorName, null);
		}
		ColorConstTreeEntryObject(String colorName, ColorConstTreeEntryObject parent) {
			super(parent);
			this.colorName = colorName;
		}
		@QtPropertyReader
		public String display() {
			return colorName;
		}
		@QtPropertyReader
		public String toolTip()
	    {
	        return QColor.fromString(colorName).name();
	    }
		@QtPropertyReader
		public QColor decoration()
	    {
	        return QColor.fromString(colorName);
	    }
		@QtPropertyWriter
		public void setDisplay(String colorName) {
			this.colorName = colorName;
		}
		private String colorName;
		@Override
		public String toString() {
			return "ColorConstTreeEntryObject(" + colorName + ")";
		}
		public ColorConstTreeEntryObject addChild(String colorName) {
			return new ColorConstTreeEntryObject(colorName, this);
		}
		@Override
		public @Nullable ColorConstTreeEntryObject parentRow() {
			return (ColorConstTreeEntryObject)parent();
		}
		@Override
		public @StrictNonNull List<@NonNull ColorConstTreeEntryObject> childRows() {
			return findChildren(ColorConstTreeEntryObject.class, Qt.FindChildOption.FindDirectChildrenOnly);
		}
	}
	
	public static class ColorTreeEntryObject extends QObject implements QRangeModel.TreeRowInterface<ColorTreeEntryObject>{
		ColorTreeEntryObject(ColorTreeEntryObject parent) {
			this("black", parent);
		}
		ColorTreeEntryObject() {
			this("black");
		}
		ColorTreeEntryObject(String colorName) {
			this(colorName, null);
		}
		ColorTreeEntryObject(String colorName, ColorTreeEntryObject parent) {
			super(parent);
			this.colorName = colorName;
		}
		@QtPropertyReader
		public QColor decoration()
	    {
	        return QColor.fromString(colorName);
	    }
		@QtPropertyReader
		public String toolTip()
	    {
	        return QColor.fromString(colorName).name();
	    }
		@QtPropertyReader
		public String display() {
			return colorName;
		}
		@QtPropertyWriter
		public void setDisplay(String colorName) {
			this.colorName = colorName;
		}
		private String colorName;
		@Override
		public String toString() {
			return "ColorTreeEntryObject(" + colorName + ")";
		}
		public ColorTreeEntryObject addChild(String colorName) {
			return new ColorTreeEntryObject(colorName, this);
		}
		@Override
		public @Nullable ColorTreeEntryObject parentRow() {
			return (ColorTreeEntryObject)parent();
		}
		@Override
		public @StrictNonNull List<@NonNull ColorTreeEntryObject> childRows() {
			return findChildren(ColorTreeEntryObject.class, Qt.FindChildOption.FindDirectChildrenOnly);
		}
		@Override
		public void setParentRow(@Nullable ColorTreeEntryObject parentRow) {
			setParent(parentRow);
		}
	}
	
	public static class AbstractRolesTreeEntry<Row extends AbstractRolesTreeEntry<Row>> extends QList<QMap<Integer, QVariant>> implements QRangeModel.ConstTreeRowInterface<Row>{
		@SafeVarargs
		public AbstractRolesTreeEntry(java.util.NavigableMap<Integer,Object>...values) {
			this(null, values);
		}
		@SafeVarargs
		public AbstractRolesTreeEntry(Row parentRow, java.util.NavigableMap<Integer,Object>...values) {
			super(QMetaType.fromType(QMap.class, new QMetaType(QMetaType.Type.Int), new QMetaType(QMetaType.Type.QVariant)));
            setParentRow(parentRow);
            this.reserve(values.length);
            for (NavigableMap<Integer, Object> map : values) {
            	QMap<Integer, QVariant> _map = new QMap<>(int.class, QVariant.class);
            	for(java.util.Map.Entry<Integer,Object> entry : map.entrySet()) {
            		_map.put(entry.getKey(), QVariant.fromValue(entry.getValue()));
            	}
				this.append(_map);
			}
		}
		
		Row parentRow;
        final java.util.List<Row> childRows = new java.util.ArrayList<>();
        @Override
        public final Row parentRow() {
             return parentRow;
        }
        @Override
        public final java.util.List<Row> childRows() {
             return childRows;
        }
        @SuppressWarnings({ "rawtypes", "unchecked" })
        protected void setParentRow(Row parentRow) {
             if(this.parentRow!=null) {
                  if(this.parentRow.childRows!=null)
                       this.parentRow.childRows.remove(this);
             }
             this.parentRow = parentRow;
             if(this.parentRow!=null) {
                  ((java.util.List)this.parentRow.childRows).add(this);
             }
        }

		@Override
		public String toString() {
			return getClass().getSimpleName()+"["+super.stream().collect(StringBuilder::new, (sb, map)->{
					Object display = map.value(Qt.ItemDataRole.DisplayRole);
					if(!sb.isEmpty())
						sb.append(", ");
					sb.append(""+display);
				}, StringBuilder::append)+( childRows.isEmpty() ? "" : " >> "+childRows)+"]";
		}
	}
	
	public static class ConstRolesTreeEntry extends AbstractRolesTreeEntry<ConstRolesTreeEntry>{
		@SafeVarargs
		ConstRolesTreeEntry(ConstRolesTreeEntry parentRow, java.util.NavigableMap<Integer, Object>... values) {
			super(parentRow, values);
		}

		@SafeVarargs
		ConstRolesTreeEntry(java.util.NavigableMap<Integer, Object>... values) {
			super(values);
		}
		
		@SafeVarargs
		public final ConstRolesTreeEntry addChild(java.util.NavigableMap<Integer, Object>... values) {
			return new ConstRolesTreeEntry(this, values);
		}
	}
	
	public static class RolesTreeEntry extends AbstractRolesTreeEntry<RolesTreeEntry> implements QRangeModel.TreeRowInterface<RolesTreeEntry>{
		@SafeVarargs
		RolesTreeEntry(RolesTreeEntry parentRow, java.util.NavigableMap<Integer, Object>... values) {
			super(parentRow, values);
		}
		
		RolesTreeEntry(){
			super();
		}

		@SafeVarargs
		RolesTreeEntry(java.util.NavigableMap<Integer, Object>... values) {
			super(values);
		}

		@Override
		public void setParentRow(RolesTreeEntry parentRow) {
			super.setParentRow(parentRow);
		}
		
		@SafeVarargs
		public final RolesTreeEntry addChild(java.util.NavigableMap<Integer, Object>... values) {
			return new RolesTreeEntry(this, values);
		}
	}
    
    @Test
    public void test1DimensionString() {
    	Consumer<QRangeModel> test = model->{
        	assertEquals(1, model.columnCount());
        	assertEquals(0, model.rowCount(model.index(0,0)));
        	assertEquals(0, model.rowCount(model.index(1,0)));
        	assertEquals(0, model.rowCount(model.index(2,0)));
        	assertEquals("A", model.data(model.index(0,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("B", model.data(model.index(1,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("C", model.data(model.index(2,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals(null, model.data(model.index(0,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals(null, model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals(null, model.data(model.index(2,0), Qt.ItemDataRole.ToolTipRole));
    	};
    	QObject parent = new QObject();
    	QList<?> range = QList.of("A", "B", "C");
    	QRangeModel model = new QRangeModel(range, parent);
    	assertEquals(parent, model.parent());
    	assertEquals(3, model.rowCount());
    	test.accept(model);
    	assertTrue(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(3,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(3,0), "D", Qt.ItemDataRole.DisplayRole));
    	assertEquals("D", range.at(3));
    	model.dispose();
    	model = new QRangeModel(QSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(4, model.rowCount());
    	test.accept(model);
    	assertTrue(model.flags(model.index(3,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(3,0), "E", Qt.ItemDataRole.DisplayRole));
    	assertEquals("E", range.at(3));
    	model.dispose();
    	model = new QRangeModel(QConstSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(4, model.rowCount());
    	test.accept(model);
    	assertFalse(model.flags(model.index(3,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(model.index(3,0), "F", Qt.ItemDataRole.DisplayRole));
    	assertEquals("E", range.at(3));
    	model.dispose();
    	model = new QRangeModel(range);
    	assertTrue(model.moveRow(null, 0, null, 2));
    	assertEquals("B", range.at(0));
    	model.dispose();
    }
    
    @Test
    public void test1DimensionVariant() {
    	Consumer<QRangeModel> test = model->{
        	assertEquals(1, model.columnCount());
        	assertEquals(0, model.rowCount(model.index(0,0)));
        	assertEquals(0, model.rowCount(model.index(1,0)));
        	assertEquals(0, model.rowCount(model.index(2,0)));
        	assertEquals("A", model.data(model.index(0,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("B", model.data(model.index(1,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("C", model.data(model.index(2,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals(null, model.data(model.index(0,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals(null, model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals(null, model.data(model.index(2,0), Qt.ItemDataRole.ToolTipRole));
    	};
    	QObject parent = new QObject();
    	QList<?> range = QList.of(QVariant.fromValue("A"), QVariant.fromValue("B"), QVariant.fromValue("C"));
    	QRangeModel model = new QRangeModel(range, parent);
    	assertEquals(parent, model.parent());
    	assertEquals(3, model.rowCount());
    	test.accept(model);
    	assertTrue(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(3,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(3,0), "D", Qt.ItemDataRole.DisplayRole));
    	assertEquals("D", range.at(3));
    	model.dispose();
    	model = new QRangeModel(QSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(4, model.rowCount());
    	assertEquals(1, model.columnCount());
    	test.accept(model);
    	assertTrue(model.flags(model.index(3,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(3,0), "E", Qt.ItemDataRole.DisplayRole));
    	assertEquals("E", range.at(3));
    	model.dispose();
    	model = new QRangeModel(QConstSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(4, model.rowCount());
    	assertEquals(1, model.columnCount());
    	test.accept(model);
//    	assertFalse(model.flags(model.index(3,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(model.index(3,0), "F", Qt.ItemDataRole.DisplayRole));
    	assertEquals("E", range.at(3));
    	model.dispose();
    	model = new QRangeModel(range);
    	assertTrue(model.moveRow(null, 0, null, 2));
    	assertEquals("B", range.at(0));
    	model.dispose();
    }
    
    @Test
    public void test1DimensionInteger() {
    	Consumer<QRangeModel> test = model->{
        	assertEquals(0, model.rowCount(model.index(0,0)));
        	assertEquals(0, model.rowCount(model.index(1,0)));
        	assertEquals(0, model.rowCount(model.index(2,0)));
        	assertEquals(12, model.data(model.index(0,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals(23, model.data(model.index(1,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals(34, model.data(model.index(2,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals(null, model.data(model.index(0,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals(null, model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals(null, model.data(model.index(2,0), Qt.ItemDataRole.ToolTipRole));
    	};
    	QObject parent = new QObject();
    	QList<?> range = QList.of(12, 23, 34);
    	QRangeModel model = new QRangeModel(range, parent);
    	assertEquals(parent, model.parent());
    	assertEquals(3, model.rowCount());
    	test.accept(model);
    	assertEquals(1, model.columnCount());
    	assertTrue(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(3,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(3,0), 45, Qt.ItemDataRole.DisplayRole));
    	assertEquals(45, range.at(3));
    	model.dispose();
    	model = new QRangeModel(QSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(4, model.rowCount());
    	test.accept(model);
    	assertTrue(model.flags(model.index(3,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(3,0), 56, Qt.ItemDataRole.DisplayRole));
    	assertEquals(56, range.at(3));
    	model.dispose();
    	model = new QRangeModel(QConstSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(4, model.rowCount());
    	test.accept(model);
    	assertFalse(model.flags(model.index(3,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(model.index(3,0), 65, Qt.ItemDataRole.DisplayRole));
    	assertEquals(56, range.at(3));
    	model.dispose();
    	model = new QRangeModel(range);
    	assertTrue(model.moveRow(null, 0, null, 2));
    	assertEquals(23, range.at(0));
    	model.dispose();
    }
    
    @Test
    public void test1DimensionRoles() {
    	Consumer<QRangeModel> test = model->{
        	assertEquals(1, model.columnCount());
        	assertEquals(0, model.rowCount(model.index(0,0)));
        	assertEquals(0, model.rowCount(model.index(1,0)));
        	assertEquals(0, model.rowCount(model.index(2,0)));
        	assertEquals(0, model.rowCount(model.index(3,0)));
        	assertEquals("Q", model.data(model.index(0,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("F", model.data(model.index(1,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("P", model.data(model.index(2,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("M", model.data(model.index(3,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("q", model.data(model.index(0,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("f", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("p", model.data(model.index(2,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("m", model.data(model.index(3,0), Qt.ItemDataRole.ToolTipRole));
    	};
    	QObject parent = new QObject();
    	QList<?> range = QList.of(  QMap.of(Qt.ItemDataRole.DisplayRole, QVariant.fromValue("Q"), Qt.ItemDataRole.ToolTipRole, QVariant.fromValue("q")),
									QMap.of(Qt.ItemDataRole.DisplayRole, QVariant.fromValue("F"), Qt.ItemDataRole.ToolTipRole, QVariant.fromValue("f")),
									QMap.of(Qt.ItemDataRole.DisplayRole, QVariant.fromValue("P"), Qt.ItemDataRole.ToolTipRole, QVariant.fromValue("p")),
									QMap.of(Qt.ItemDataRole.DisplayRole, QVariant.fromValue("M"), Qt.ItemDataRole.ToolTipRole, QVariant.fromValue("m"))
								 );
    	QRangeModel model = new QRangeModel(range, parent);
    	assertEquals(parent, model.parent());
    	assertEquals(4, model.rowCount());
    	test.accept(model);
    	assertTrue(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(4,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(4,0), "Z", Qt.ItemDataRole.DisplayRole));
    	assertTrue(model.setData(model.index(4,0), "z", Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new TreeMap<>(QMap.of(Qt.ItemDataRole.DisplayRole, QVariant.fromValue("Z"), Qt.ItemDataRole.ToolTipRole, QVariant.fromValue("z"))), range.at(4));
    	model.dispose();
    	model = new QRangeModel(QSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(5, model.rowCount());
    	test.accept(model);
    	assertTrue(model.flags(model.index(4,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(4,0), "Y", Qt.ItemDataRole.DisplayRole));
    	assertTrue(model.setData(model.index(4,0), "y", Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new TreeMap<>(QMap.of(Qt.ItemDataRole.DisplayRole, QVariant.fromValue("Y"), Qt.ItemDataRole.ToolTipRole, QVariant.fromValue("y"))), range.at(4));
    	model.dispose();
    	model = new QRangeModel(QConstSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(5, model.rowCount());
    	test.accept(model);
    	assertFalse(model.flags(model.index(4,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(model.index(4,0), "X", Qt.ItemDataRole.DisplayRole));
    	assertFalse(model.setData(model.index(4,0), "x", Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new TreeMap<>(QMap.of(Qt.ItemDataRole.DisplayRole, QVariant.fromValue("Y"), Qt.ItemDataRole.ToolTipRole, QVariant.fromValue("y"))), range.at(4));
    	model.dispose();
    	model = new QRangeModel(range);
    	assertTrue(model.moveRow(null, 0, null, 2));
    	assertEquals(new TreeMap<>(QMap.of(Qt.ItemDataRole.DisplayRole, QVariant.fromValue("F"), Qt.ItemDataRole.ToolTipRole, QVariant.fromValue("f"))), range.at(0));
    	model.dispose();
    }
    
    @Test
    public void test1DimensionQObject() {
    	QMetaObject metaObject = QMetaObject.forType(ColorEntryObject.class);
		QMetaProperty displayProp = metaObject.property("display");
		QMetaProperty decorationProp = metaObject.property("decoration");
		QMetaProperty toolTipProp = metaObject.property("toolTip");
    	Consumer<QRangeModel> multiColumnTest = model->{
        	assertEquals(3, model.columnCount());
        	assertEquals(0, model.rowCount(model.index(0,0)));
        	assertEquals(0, model.rowCount(model.index(1,0)));
        	assertEquals(0, model.rowCount(model.index(2,0)));
        	assertEquals("red", model.data(model.index(0,displayProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals("green", model.data(model.index(1,displayProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals("blue", model.data(model.index(2,displayProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals("#ff0000", model.data(model.index(0,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals("#008000", model.data(model.index(1,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals("#0000ff", model.data(model.index(2,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals(new QColor("red"), model.data(model.index(0,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals(new QColor("green"), model.data(model.index(1,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals(new QColor("blue"), model.data(model.index(2,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	};
     	Consumer<QRangeModel> singleColumnTest = model->{
        	assertEquals(1, model.columnCount());
        	assertEquals(0, model.rowCount(model.index(0,0)));
        	assertEquals(0, model.rowCount(model.index(1,0)));
        	assertEquals(0, model.rowCount(model.index(2,0)));
        	assertEquals("red", model.data(model.index(0,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("green", model.data(model.index(1,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("blue", model.data(model.index(2,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("#ff0000", model.data(model.index(0,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("#008000", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("#0000ff", model.data(model.index(2,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals(new QColor("red"), model.data(model.index(0,0), Qt.ItemDataRole.DecorationRole));
        	assertEquals(new QColor("green"), model.data(model.index(1,0), Qt.ItemDataRole.DecorationRole));
        	assertEquals(new QColor("blue"), model.data(model.index(2,0), Qt.ItemDataRole.DecorationRole));
    	};
    	QObject parent = new QObject();
    	QList<ColorEntryObject> range = QList.of(
    								new ColorEntryObject("red"),
    								new ColorEntryObject("green"),
    								new ColorEntryObject("blue")
								 );
    	QRangeModel model = new QRangeModel(range, parent);
    	assertEquals(parent, model.parent());
    	assertEquals(3, model.rowCount());
    	multiColumnTest.accept(model);
    	assertTrue(model.insertRow(model.rowCount()));
    	assertEquals(null, range.at(3));
    	assertTrue(model.flags(model.index(2,displayProp.relativePropertyIndex())).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(2,displayProp.relativePropertyIndex()), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(2).display());
    	assertEquals("#ffff00", model.data(model.index(2,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(2,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	assertFalse(model.setData(model.index(3,displayProp.relativePropertyIndex()), "yellow", Qt.ItemDataRole.DisplayRole));
    	model.dispose();
    	range.at(2).setDisplay("blue");
    	range.remove(3);
    	model = new QRangeModel(QSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(3, model.rowCount());
    	multiColumnTest.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(2,displayProp.relativePropertyIndex())).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(2,displayProp.relativePropertyIndex()), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(2).display());
    	assertEquals("#ffff00", model.data(model.index(2,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(2,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	model.dispose();
    	range.at(2).setDisplay("blue");
    	model = new QRangeModel(QConstSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(3, model.rowCount());
    	multiColumnTest.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertFalse(model.flags(model.index(2,displayProp.relativePropertyIndex())).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(model.index(2,displayProp.relativePropertyIndex()), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("blue", range.at(2).display());
    	assertEquals("#0000ff", model.data(model.index(2,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	assertEquals(new QColor("blue"), model.data(model.index(2,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	model.dispose();
    	
    	model = new QRangeModel(range, true, parent);
    	assertEquals(parent, model.parent());
    	assertEquals(3, model.rowCount());
    	singleColumnTest.accept(model);
    	assertTrue(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(2,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(2,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(2).display());
    	assertEquals("#ffff00", model.data(model.index(2,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(2,0), Qt.ItemDataRole.DecorationRole));
    	assertFalse(model.setData(model.index(3,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals(null, range.at(3));
    	model.dispose();
    	range.at(2).setDisplay("blue");
    	range.remove(3);
    	model = new QRangeModel(QSpan.ofList(range), true);
    	assertEquals(null, model.parent());
    	assertEquals(3, model.rowCount());
    	singleColumnTest.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(2,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(2,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(2).display());
    	assertEquals("#ffff00", model.data(model.index(2,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(2,0), Qt.ItemDataRole.DecorationRole));
    	model.dispose();
    	range.at(2).setDisplay("blue");
    	model = new QRangeModel(QConstSpan.ofList(range), true);
    	assertEquals(null, model.parent());
    	assertEquals(3, model.rowCount());
    	singleColumnTest.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertFalse(model.flags(model.index(2,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(model.index(2,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("blue", range.at(2).display());
    	assertEquals("#0000ff", model.data(model.index(2,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("blue"), model.data(model.index(2,0), Qt.ItemDataRole.DecorationRole));
    	model.dispose();
    }
    
    @Test
    public void test1DimensionGadget() {
    	QMetaObject metaObject = QMetaObject.forType(ColorEntryGadget.class);
		QMetaProperty displayProp = metaObject.property("display");
		QMetaProperty decorationProp = metaObject.property("decoration");
		QMetaProperty toolTipProp = metaObject.property("toolTip");
    	Consumer<QRangeModel> multiColumnTest = model->{
        	assertEquals(3, model.columnCount());
        	assertEquals(0, model.rowCount(model.index(0,0)));
        	assertEquals(0, model.rowCount(model.index(1,0)));
        	assertEquals(0, model.rowCount(model.index(2,0)));
        	assertEquals("red", model.data(model.index(0,displayProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals("green", model.data(model.index(1,displayProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals("blue", model.data(model.index(2,displayProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals("#ff0000", model.data(model.index(0,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals("#008000", model.data(model.index(1,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals("#0000ff", model.data(model.index(2,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals(new QColor("red"), model.data(model.index(0,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals(new QColor("green"), model.data(model.index(1,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals(new QColor("blue"), model.data(model.index(2,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	};
     	Consumer<QRangeModel> singleColumnTest = model->{
        	assertEquals(1, model.columnCount());
        	assertEquals(0, model.rowCount(model.index(0,0)));
        	assertEquals(0, model.rowCount(model.index(1,0)));
        	assertEquals(0, model.rowCount(model.index(2,0)));
        	assertEquals("red", model.data(model.index(0,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("green", model.data(model.index(1,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("blue", model.data(model.index(2,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("#ff0000", model.data(model.index(0,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("#008000", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("#0000ff", model.data(model.index(2,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals(new QColor("red"), model.data(model.index(0,0), Qt.ItemDataRole.DecorationRole));
        	assertEquals(new QColor("green"), model.data(model.index(1,0), Qt.ItemDataRole.DecorationRole));
        	assertEquals(new QColor("blue"), model.data(model.index(2,0), Qt.ItemDataRole.DecorationRole));
    	};
    	QObject parent = new QObject();
    	QList<ColorEntryGadget> range = QList.of(
    								new ColorEntryGadget("red"),
    								new ColorEntryGadget("green"),
    								new ColorEntryGadget("blue")
								 );
    	QRangeModel model = new QRangeModel(range, parent);
    	assertEquals(parent, model.parent());
    	assertEquals(3, model.rowCount());
    	multiColumnTest.accept(model);
    	assertTrue(model.insertRow(model.rowCount()));
    	assertEquals(null, range.at(3));
    	assertTrue(model.flags(model.index(2,displayProp.relativePropertyIndex())).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(2,displayProp.relativePropertyIndex()), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(2).display());
    	assertEquals("#ffff00", model.data(model.index(2,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(2,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	assertFalse(model.setData(model.index(3,displayProp.relativePropertyIndex()), "magenta", Qt.ItemDataRole.DisplayRole));
    	model.dispose();
    	range.at(2).setDisplay("blue");
    	range.remove(3);
    	model = new QRangeModel(QSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(3, model.rowCount());
    	multiColumnTest.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(2,displayProp.relativePropertyIndex())).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(2,displayProp.relativePropertyIndex()), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(2).display());
    	assertEquals("#ffff00", model.data(model.index(2,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(2,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	model.dispose();
    	range.at(2).setDisplay("blue");
    	model = new QRangeModel(QConstSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(3, model.rowCount());
    	multiColumnTest.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertFalse(model.flags(model.index(2,displayProp.relativePropertyIndex())).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(model.index(2,displayProp.relativePropertyIndex()), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("blue", range.at(2).display());
    	assertEquals("#0000ff", model.data(model.index(2,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	assertEquals(new QColor("blue"), model.data(model.index(2,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	model.dispose();
    	
    	model = new QRangeModel(range, true, parent);
    	assertEquals(parent, model.parent());
    	assertEquals(3, model.rowCount());
    	singleColumnTest.accept(model);
    	assertTrue(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(2,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(2,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(2).display());
    	assertEquals("#ffff00", model.data(model.index(2,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(2,0), Qt.ItemDataRole.DecorationRole));
    	assertFalse(model.setData(model.index(3,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals(null, range.at(3));
    	model.dispose();
    	range.at(2).setDisplay("blue");
    	range.remove(3);
    	model = new QRangeModel(QSpan.ofList(range), true);
    	assertEquals(null, model.parent());
    	assertEquals(3, model.rowCount());
    	singleColumnTest.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(2,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(2,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(2).display());
    	assertEquals("#ffff00", model.data(model.index(2,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(2,0), Qt.ItemDataRole.DecorationRole));
    	model.dispose();
    	range.at(2).setDisplay("blue");
    	model = new QRangeModel(QConstSpan.ofList(range), true);
    	assertEquals(null, model.parent());
    	assertEquals(3, model.rowCount());
    	singleColumnTest.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertFalse(model.flags(model.index(2,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(model.index(2,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("blue", range.at(2).display());
    	assertEquals("#0000ff", model.data(model.index(2,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("blue"), model.data(model.index(2,0), Qt.ItemDataRole.DecorationRole));
    	assertFalse(model.moveRows(null, 0, 1, null, 1));
    	model.dispose();
    	model = new QRangeModel(range, true, parent);
//    	assertTrue(model.moveRows(null, 0, 1, null, 1));
    	model.dispose();
    }
    
    @Test
    public void test1DimensionValue() {
    	QMetaObject metaObject = QMetaObject.forType(ColorEntryValue.class);
		QMetaProperty displayProp = metaObject.property("display");
		QMetaProperty decorationProp = metaObject.property("decoration");
		QMetaProperty toolTipProp = metaObject.property("toolTip");
    	Consumer<QRangeModel> multiColumnTest = model->{
        	assertEquals(3, model.columnCount());
        	assertEquals(0, model.rowCount(model.index(0,0)));
        	assertEquals(0, model.rowCount(model.index(1,0)));
        	assertEquals(0, model.rowCount(model.index(2,0)));
        	assertEquals("red", model.data(model.index(0,displayProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals("green", model.data(model.index(1,displayProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals("blue", model.data(model.index(2,displayProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals("#ff0000", model.data(model.index(0,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals("#008000", model.data(model.index(1,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals("#0000ff", model.data(model.index(2,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals(new QColor("red"), model.data(model.index(0,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals(new QColor("green"), model.data(model.index(1,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
        	assertEquals(new QColor("blue"), model.data(model.index(2,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	};
     	Consumer<QRangeModel> singleColumnTest = model->{
        	assertEquals(1, model.columnCount());
        	assertEquals(0, model.rowCount(model.index(0,0)));
        	assertEquals(0, model.rowCount(model.index(1,0)));
        	assertEquals(0, model.rowCount(model.index(2,0)));
        	assertEquals("red", model.data(model.index(0,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("green", model.data(model.index(1,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("blue", model.data(model.index(2,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("#ff0000", model.data(model.index(0,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("#008000", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("#0000ff", model.data(model.index(2,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals(new QColor("red"), model.data(model.index(0,0), Qt.ItemDataRole.DecorationRole));
        	assertEquals(new QColor("green"), model.data(model.index(1,0), Qt.ItemDataRole.DecorationRole));
        	assertEquals(new QColor("blue"), model.data(model.index(2,0), Qt.ItemDataRole.DecorationRole));
    	};
    	QObject parent = new QObject();
    	QList<ColorEntryValue> range = QList.of(
    								new ColorEntryValue("red"),
    								new ColorEntryValue("green"),
    								new ColorEntryValue("blue")
								 );
    	QRangeModel model = new QRangeModel(range, parent);
    	assertEquals(parent, model.parent());
    	assertEquals(3, model.rowCount());
    	multiColumnTest.accept(model);
    	assertTrue(model.insertRow(model.rowCount()));
    	assertEquals(new ColorEntryValue(), range.at(3));
    	assertTrue(model.flags(model.index(2,displayProp.relativePropertyIndex())).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(2,displayProp.relativePropertyIndex()), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(2).display());
    	assertEquals("#ffff00", model.data(model.index(2,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(2,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	assertTrue(model.setData(model.index(3,displayProp.relativePropertyIndex()), "magenta", Qt.ItemDataRole.DisplayRole));
    	assertEquals("magenta", range.at(3).display());
    	assertEquals(new ColorEntryValue("magenta"), range.at(3));
    	assertTrue(model.setData(model.index(2,displayProp.relativePropertyIndex()), "blue", Qt.ItemDataRole.DisplayRole));
    	model.dispose();
    	range.remove(3);
    	model = new QRangeModel(QSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(3, model.rowCount());
    	multiColumnTest.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(2,displayProp.relativePropertyIndex())).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(2,displayProp.relativePropertyIndex()), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(2).display());
    	assertEquals("#ffff00", model.data(model.index(2,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(2,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	assertTrue(model.setData(model.index(2,displayProp.relativePropertyIndex()), "blue", Qt.ItemDataRole.DisplayRole));
    	model.dispose();
    	model = new QRangeModel(QConstSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(3, model.rowCount());
    	multiColumnTest.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertFalse(model.flags(model.index(2,displayProp.relativePropertyIndex())).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(model.index(2,displayProp.relativePropertyIndex()), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("blue", range.at(2).display());
    	assertEquals("#0000ff", model.data(model.index(2,toolTipProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	assertEquals(new QColor("blue"), model.data(model.index(2,decorationProp.relativePropertyIndex()), Qt.ItemDataRole.DisplayRole));
    	model.dispose();
    	
    	model = new QRangeModel(range, true, parent);
    	assertEquals(parent, model.parent());
    	assertEquals(3, model.rowCount());
    	singleColumnTest.accept(model);
    	assertTrue(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(2,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(2,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(2).display());
    	assertEquals("#ffff00", model.data(model.index(2,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(2,0), Qt.ItemDataRole.DecorationRole));
    	assertTrue(model.setData(model.index(3,0), "magenta", Qt.ItemDataRole.DisplayRole));
    	assertEquals(new ColorEntryValue("magenta"), range.at(3));
    	assertTrue(model.setData(model.index(2,0), "blue", Qt.ItemDataRole.DisplayRole));
    	model.dispose();
    	range.remove(3);
    	model = new QRangeModel(QSpan.ofList(range), true);
    	assertEquals(null, model.parent());
    	assertEquals(3, model.rowCount());
    	singleColumnTest.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(2,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(2,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(2).display());
    	assertEquals("#ffff00", model.data(model.index(2,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(2,0), Qt.ItemDataRole.DecorationRole));
    	assertTrue(model.setData(model.index(2,0), "blue", Qt.ItemDataRole.DisplayRole));
    	model.dispose();
    	model = new QRangeModel(QConstSpan.ofList(range), true);
    	assertEquals(null, model.parent());
    	assertEquals(3, model.rowCount());
    	singleColumnTest.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertFalse(model.flags(model.index(2,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(model.index(2,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("blue", range.at(2).display());
    	assertEquals("#0000ff", model.data(model.index(2,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("blue"), model.data(model.index(2,0), Qt.ItemDataRole.DecorationRole));
    	model.dispose();
    	model = new QRangeModel(range);
    	assertTrue(model.moveRow(null, 0, null, 2));
    	assertEquals(new ColorEntryValue("green"), range.at(0));
    	model.dispose();
    }
    
    @Test
    public void test2DimensionsString() {
    	QList<QStringList> range = new QList<>(QStringList.class,
				new QStringList("A1", "B1", "C1"), 
				new QStringList("A2", "B2", "C2"), 
				new QStringList("A3", "B3", "C3"), 
				new QStringList("A4", "B4", "C4")
			);
    	Consumer<QRangeModel> test = model->{
        	assertEquals(3, model.columnCount());
        	assertEquals(0, model.rowCount(model.index(0,0)));
        	assertEquals("A1", model.data(model.index(0,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("A2", model.data(model.index(1,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("A3", model.data(model.index(2,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("A4", model.data(model.index(3,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals(null, model.data(model.index(0,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("B1", model.data(model.index(0,1), Qt.ItemDataRole.DisplayRole));
        	assertEquals("B2", model.data(model.index(1,1), Qt.ItemDataRole.DisplayRole));
        	assertEquals("B3", model.data(model.index(2,1), Qt.ItemDataRole.DisplayRole));
        	assertEquals("B4", model.data(model.index(3,1), Qt.ItemDataRole.DisplayRole));
        	assertEquals(null, model.data(model.index(0,1), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("C1", model.data(model.index(0,2), Qt.ItemDataRole.DisplayRole));
        	assertEquals("C2", model.data(model.index(1,2), Qt.ItemDataRole.DisplayRole));
        	assertEquals("C3", model.data(model.index(2,2), Qt.ItemDataRole.DisplayRole));
        	assertEquals("C4", model.data(model.index(3,2), Qt.ItemDataRole.DisplayRole));
        	assertEquals(null, model.data(model.index(0,2), Qt.ItemDataRole.ToolTipRole));
    	};
    	QObject parent = new QObject();
    	QRangeModel model = new QRangeModel(range, parent);
    	assertEquals(parent, model.parent());
    	assertEquals(4, model.rowCount());
    	test.accept(model);
    	assertTrue(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(4,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(4,0), "A5", Qt.ItemDataRole.DisplayRole));
    	assertTrue(model.setData(model.index(4,1), "B5", Qt.ItemDataRole.DisplayRole));
    	assertTrue(model.setData(model.index(4,2), "C5", Qt.ItemDataRole.DisplayRole));
    	assertEquals(new QStringList("A5", "B5", "C5"), range.at(4));
    	model.dispose();
    	model = new QRangeModel(QSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(5, model.rowCount());
    	test.accept(model);
    	assertTrue(model.setData(model.index(4,0), "E", Qt.ItemDataRole.DisplayRole));
    	assertEquals("E", range.at(4).at(0));
    	model.dispose();
    	model = new QRangeModel(QConstSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(5, model.rowCount());
    	test.accept(model);
    	assertFalse(model.flags(model.index(4,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(model.index(4,0), "F", Qt.ItemDataRole.DisplayRole));
    	assertEquals("E", range.at(4).at(0));
    	model.dispose();
    	model = new QRangeModel(range);
    	assertTrue(model.moveRow(null, 0, null, 2));
    	assertEquals(new QStringList("A2", "B2", "C2"), range.at(0));
    	assertTrue(model.moveColumn(null, 0, null, 2));
    	assertEquals(new QStringList("B2", "A2", "C2"), range.at(0));
    	model.dispose();
    }
    
    @Test
    public void test2DimensionsRoles() {
    	Consumer<QRangeModel> test = model->{
        	assertEquals(2, model.columnCount());
        	assertEquals(0, model.rowCount(model.index(0,0)));
        	assertEquals("Q", model.data(model.index(0,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("F", model.data(model.index(0,1), Qt.ItemDataRole.DisplayRole));
        	assertEquals("P", model.data(model.index(1,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("M", model.data(model.index(1,1), Qt.ItemDataRole.DisplayRole));
        	assertEquals("q", model.data(model.index(0,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("f", model.data(model.index(0,1), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("p", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("m", model.data(model.index(1,1), Qt.ItemDataRole.ToolTipRole));
    	};
    	QObject parent = new QObject();
    	QList<QList<?>> range = QList.of(  
    							QList.of(
    								QMap.of(Qt.ItemDataRole.DisplayRole, QVariant.fromValue("Q"), Qt.ItemDataRole.ToolTipRole, QVariant.fromValue("q")),
									QMap.of(Qt.ItemDataRole.DisplayRole, QVariant.fromValue("F"), Qt.ItemDataRole.ToolTipRole, QVariant.fromValue("f"))),
    							QList.of(
									QMap.of(Qt.ItemDataRole.DisplayRole, QVariant.fromValue("P"), Qt.ItemDataRole.ToolTipRole, QVariant.fromValue("p")),
									QMap.of(Qt.ItemDataRole.DisplayRole, QVariant.fromValue("M"), Qt.ItemDataRole.ToolTipRole, QVariant.fromValue("m")))
								 );
    	QRangeModel model = new QRangeModel(range, parent);
    	assertEquals(parent, model.parent());
    	assertEquals(2, model.rowCount());
    	test.accept(model);
    	assertTrue(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(2,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(2,0), "Z1", Qt.ItemDataRole.DisplayRole));
    	assertTrue(model.setData(model.index(2,0), "z1", Qt.ItemDataRole.ToolTipRole));
    	assertTrue(model.setData(model.index(2,1), "Z2", Qt.ItemDataRole.DisplayRole));
    	assertTrue(model.setData(model.index(2,1), "z2", Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new TreeMap<>(QMap.of(Qt.ItemDataRole.DisplayRole, QVariant.fromValue("Z1"), Qt.ItemDataRole.ToolTipRole, QVariant.fromValue("z1"))), range.at(2).at(0));
    	assertTrue(model.insertColumn(model.columnCount()));
    	assertTrue(model.setData(model.index(2,2), "Z3", Qt.ItemDataRole.DisplayRole));
    	assertTrue(model.setData(model.index(2,2), "z3", Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new TreeMap<>(QMap.of(Qt.ItemDataRole.DisplayRole, QVariant.fromValue("Z3"), Qt.ItemDataRole.ToolTipRole, QVariant.fromValue("z3"))), range.at(2).at(2));
    	assertTrue(model.removeColumn(model.columnCount()-1));
    	assertEquals(2, model.columnCount());
    	model.dispose();
    	model = new QRangeModel(QSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(3, model.rowCount());
    	test.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertTrue(model.setData(model.index(2,0), "Y1", Qt.ItemDataRole.DisplayRole));
    	assertTrue(model.setData(model.index(2,0), "y1", Qt.ItemDataRole.ToolTipRole));
    	assertTrue(model.setData(model.index(2,1), "Y2", Qt.ItemDataRole.DisplayRole));
    	assertTrue(model.setData(model.index(2,1), "y2", Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new TreeMap<>(QMap.of(Qt.ItemDataRole.DisplayRole, QVariant.fromValue("Y1"), Qt.ItemDataRole.ToolTipRole, QVariant.fromValue("y1"))), range.at(2).at(0));
    	model.dispose();
    	model = new QRangeModel(QConstSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(3, model.rowCount());
    	test.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertFalse(model.flags(model.index(4,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(model.index(4,0), "X", Qt.ItemDataRole.DisplayRole));
    	assertFalse(model.setData(model.index(4,0), "x", Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new TreeMap<>(QMap.of(Qt.ItemDataRole.DisplayRole, QVariant.fromValue("Y1"), Qt.ItemDataRole.ToolTipRole, QVariant.fromValue("y1"))), range.at(2).at(0));
    	model.dispose();
    }
    
    @Test
    public void test2DimensionsQObject() {
		Consumer<QRangeModel> test = model->{
        	assertEquals(2, model.columnCount());
        	assertEquals(0, model.rowCount(model.index(0,0)));
        	assertEquals("red", model.data(model.index(0,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("green", model.data(model.index(0,1), Qt.ItemDataRole.DisplayRole));
        	assertEquals("blue", model.data(model.index(1,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("cyan", model.data(model.index(1,1), Qt.ItemDataRole.DisplayRole));
        	assertEquals("#ff0000", model.data(model.index(0,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("#008000", model.data(model.index(0,1), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("#0000ff", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("#00ffff", model.data(model.index(1,1), Qt.ItemDataRole.ToolTipRole));
        	assertEquals(new QColor("red"), model.data(model.index(0,0), Qt.ItemDataRole.DecorationRole));
        	assertEquals(new QColor("green"), model.data(model.index(0,1), Qt.ItemDataRole.DecorationRole));
        	assertEquals(new QColor("blue"), model.data(model.index(1,0), Qt.ItemDataRole.DecorationRole));
        	assertEquals(new QColor("cyan"), model.data(model.index(1,1), Qt.ItemDataRole.DecorationRole));
    	};
    	QObject parent = new QObject();
    	QList<QList<ColorEntryObject>> range = QList.of(
		    										QList.of(
					    								new ColorEntryObject("red"),
					    								new ColorEntryObject("green")
				    								),
		    										QList.of(
					    								new ColorEntryObject("blue"),
					    								new ColorEntryObject("cyan")
				    								)
												);
    	QRangeModel model = new QRangeModel(range, parent);
    	assertEquals(parent, model.parent());
    	assertEquals(2, model.rowCount());
    	test.accept(model);
    	assertTrue(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(1,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(1,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(1).at(0).display());
    	assertEquals("#ffff00", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(1,0), Qt.ItemDataRole.DecorationRole));
    	assertFalse(model.setData(model.index(2,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals(null, range.at(2).at(0));
    	assertTrue(model.insertColumn(model.columnCount()));
    	assertEquals(3, range.at(0).size());
    	assertTrue(model.removeColumn(model.columnCount()-1));
    	model.dispose();
    	range.at(1).at(0).setDisplay("blue");
    	range.remove(2);
    	model = new QRangeModel(QSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(2, model.rowCount());
    	test.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertTrue(model.setData(model.index(1,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(1).at(0).display());
    	assertEquals("#ffff00", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(1,0), Qt.ItemDataRole.DecorationRole));
    	model.dispose();
    	range.at(1).at(0).setDisplay("blue");
    	model = new QRangeModel(QConstSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(2, model.rowCount());
    	test.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertFalse(model.flags(model.index(1,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(model.index(1,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("blue", range.at(1).at(0).display());
    	assertEquals("#0000ff", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("blue"), model.data(model.index(1,0), Qt.ItemDataRole.DecorationRole));
    	model.dispose();
    }
    
    @Test
    public void test2DimensionsGadget() {
    	Consumer<QRangeModel> test = model->{
        	assertEquals(2, model.columnCount());
        	assertEquals(0, model.rowCount(model.index(0,0)));
        	assertEquals("red", model.data(model.index(0,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("green", model.data(model.index(0,1), Qt.ItemDataRole.DisplayRole));
        	assertEquals("blue", model.data(model.index(1,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("cyan", model.data(model.index(1,1), Qt.ItemDataRole.DisplayRole));
        	assertEquals("#ff0000", model.data(model.index(0,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("#008000", model.data(model.index(0,1), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("#0000ff", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("#00ffff", model.data(model.index(1,1), Qt.ItemDataRole.ToolTipRole));
        	assertEquals(new QColor("red"), model.data(model.index(0,0), Qt.ItemDataRole.DecorationRole));
        	assertEquals(new QColor("green"), model.data(model.index(0,1), Qt.ItemDataRole.DecorationRole));
        	assertEquals(new QColor("blue"), model.data(model.index(1,0), Qt.ItemDataRole.DecorationRole));
        	assertEquals(new QColor("cyan"), model.data(model.index(1,1), Qt.ItemDataRole.DecorationRole));
    	};
    	QObject parent = new QObject();
    	QList<QList<ColorEntryGadget>> range = QList.of(
		    										QList.of(
					    								new ColorEntryGadget("red"),
					    								new ColorEntryGadget("green")
				    								),
		    										QList.of(
					    								new ColorEntryGadget("blue"),
					    								new ColorEntryGadget("cyan")
				    								)
												);
    	QRangeModel model = new QRangeModel(range, parent);
    	assertEquals(parent, model.parent());
    	assertEquals(2, model.rowCount());
    	test.accept(model);
    	assertTrue(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(1,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(1,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(1).at(0).display());
    	assertEquals("#ffff00", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(1,0), Qt.ItemDataRole.DecorationRole));
    	assertFalse(model.setData(model.index(2,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals(null, range.at(2).at(0));
    	assertTrue(model.insertColumn(model.columnCount()));
    	assertEquals(3, range.at(0).size());
    	assertTrue(model.removeColumn(model.columnCount()-1));
    	model.dispose();
    	range.at(1).at(0).setDisplay("blue");
    	range.remove(2);
    	model = new QRangeModel(QSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(2, model.rowCount());
    	test.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertTrue(model.setData(model.index(1,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(1).at(0).display());
    	assertEquals("#ffff00", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(1,0), Qt.ItemDataRole.DecorationRole));
    	model.dispose();
    	range.at(1).at(0).setDisplay("blue");
    	model = new QRangeModel(QConstSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(2, model.rowCount());
    	test.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertFalse(model.flags(model.index(1,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(model.index(1,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("blue", range.at(1).at(0).display());
    	assertEquals("#0000ff", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("blue"), model.data(model.index(1,0), Qt.ItemDataRole.DecorationRole));
    	model.dispose();
    }
    
    @Test
    public void test2DimensionsValue() {
    	Consumer<QRangeModel> test = model->{
        	assertEquals(2, model.columnCount());
        	assertEquals(0, model.rowCount(model.index(0,0)));
        	assertEquals("red", model.data(model.index(0,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("green", model.data(model.index(0,1), Qt.ItemDataRole.DisplayRole));
        	assertEquals("blue", model.data(model.index(1,0), Qt.ItemDataRole.DisplayRole));
        	assertEquals("cyan", model.data(model.index(1,1), Qt.ItemDataRole.DisplayRole));
        	assertEquals("#ff0000", model.data(model.index(0,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("#008000", model.data(model.index(0,1), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("#0000ff", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
        	assertEquals("#00ffff", model.data(model.index(1,1), Qt.ItemDataRole.ToolTipRole));
        	assertEquals(new QColor("red"), model.data(model.index(0,0), Qt.ItemDataRole.DecorationRole));
        	assertEquals(new QColor("green"), model.data(model.index(0,1), Qt.ItemDataRole.DecorationRole));
        	assertEquals(new QColor("blue"), model.data(model.index(1,0), Qt.ItemDataRole.DecorationRole));
        	assertEquals(new QColor("cyan"), model.data(model.index(1,1), Qt.ItemDataRole.DecorationRole));
    	};
    	QObject parent = new QObject();
    	QList<QList<ColorEntryValue>> range = QList.of(
		    										QList.of(
					    								new ColorEntryValue("red"),
					    								new ColorEntryValue("green")
				    								),
		    										QList.of(
					    								new ColorEntryValue("blue"),
					    								new ColorEntryValue("cyan")
				    								)
												);
    	QRangeModel model = new QRangeModel(range, parent);
    	assertEquals(parent, model.parent());
    	assertEquals(2, model.rowCount());
    	test.accept(model);
    	assertTrue(model.insertRow(model.rowCount()));
    	assertTrue(model.flags(model.index(1,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(model.index(1,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(1).at(0).display());
    	assertEquals("#ffff00", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(1,0), Qt.ItemDataRole.DecorationRole));
    	assertTrue(model.setData(model.index(2,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals(new ColorEntryValue("yellow"), range.at(2).at(0));
    	assertTrue(model.setData(model.index(1,0), "blue", Qt.ItemDataRole.DisplayRole));
    	assertTrue(model.insertColumn(model.columnCount()));
    	assertEquals(3, range.at(0).size());
    	assertTrue(model.removeColumn(model.columnCount()-1));
    	model.dispose();
    	range.remove(2);
    	model = new QRangeModel(QSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(2, model.rowCount());
    	test.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertTrue(model.setData(model.index(1,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.at(1).at(0).display());
    	assertEquals("#ffff00", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("yellow"), model.data(model.index(1,0), Qt.ItemDataRole.DecorationRole));
    	assertTrue(model.setData(model.index(1,0), "blue", Qt.ItemDataRole.DisplayRole));
    	model.dispose();
    	model = new QRangeModel(QConstSpan.ofList(range));
    	assertEquals(null, model.parent());
    	assertEquals(2, model.rowCount());
    	test.accept(model);
    	assertFalse(model.insertRow(model.rowCount()));
    	assertFalse(model.flags(model.index(1,0)).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(model.index(1,0), "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("blue", range.at(1).at(0).display());
    	assertEquals("#0000ff", model.data(model.index(1,0), Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("blue"), model.data(model.index(1,0), Qt.ItemDataRole.DecorationRole));
    	model.dispose();
    }
    
    @Test
    public void testConstTreeEntryObject() {
    	QMetaObject metaObject = QMetaObject.forType(ColorConstTreeEntryObject.class);
		QMetaProperty displayProp = metaObject.property("display");
		QMetaProperty decorationProp = metaObject.property("decoration");
		QMetaProperty toolTipProp = metaObject.property("toolTip");
    	QList<ColorConstTreeEntryObject> range = QList.of(new ColorConstTreeEntryObject("red"), new ColorConstTreeEntryObject("green"));
    	range.at(1).addChild("yellow");
    	ColorConstTreeEntryObject child = range.at(1).addChild("lightgreen");
    	child.addChild("darkgreen");
    	child.addChild("blue");
    	QRangeModel model = new QRangeModel(range, true);
    	assertEquals(2, model.rowCount());
    	assertEquals(1, model.columnCount());
    	
    	QModelIndex redIndex = model.index(0,0);
    	assertEquals("red", model.data(redIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#ff0000", model.data(redIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("red"), model.data(redIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(0, model.rowCount(redIndex));
    	
    	QModelIndex greenIndex = model.index(1,0);
    	assertEquals("green", model.data(greenIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#008000", model.data(greenIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("green"), model.data(greenIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(2, model.rowCount(greenIndex));
    	
    	QModelIndex yellowIndex = model.index(0,0, greenIndex);
    	assertEquals("yellow", model.data(yellowIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#ffff00", model.data(yellowIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("yellow"), model.data(yellowIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(0, model.rowCount(yellowIndex));
    	
    	QModelIndex lightgreenIndex = model.index(1,0, greenIndex);
    	assertEquals("lightgreen", model.data(lightgreenIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#90ee90", model.data(lightgreenIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("lightgreen"), model.data(lightgreenIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(2, model.rowCount(lightgreenIndex));
    	
    	QModelIndex darkgreenIndex = model.index(0,0, lightgreenIndex);
    	assertEquals("darkgreen", model.data(darkgreenIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#006400", model.data(darkgreenIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("darkgreen"), model.data(darkgreenIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(0, model.rowCount(darkgreenIndex));

    	QModelIndex blueIndex = model.index(1,0, lightgreenIndex);
    	assertEquals("blue", model.data(blueIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#0000ff", model.data(blueIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("blue"), model.data(blueIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(0, model.rowCount(blueIndex));
    	
    	assertFalse(model.flags(redIndex).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(redIndex, "gray", Qt.ItemDataRole.DisplayRole));
    	assertFalse(model.insertRow(model.rowCount()));
    	model.dispose();
    	
    	model = new QRangeModel(range);
    	redIndex = model.index(0,0);
    	assertEquals("red", model.data(redIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#ff0000", model.data(redIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("red"), model.data(redIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(0, model.rowCount(redIndex));
    	
    	greenIndex = model.index(1,0);
    	assertEquals("green", model.data(greenIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#008000", model.data(greenIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("green"), model.data(greenIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(2, model.rowCount(greenIndex));
    	
    	yellowIndex = model.index(0,0, greenIndex);
    	assertTrue(yellowIndex.isValid());
    	assertEquals("yellow", model.data(yellowIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#ffff00", model.data(yellowIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("yellow"), model.data(yellowIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(0, model.rowCount(yellowIndex));
    	
    	lightgreenIndex = model.index(1,0, greenIndex);
    	assertEquals("lightgreen", model.data(lightgreenIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#90ee90", model.data(lightgreenIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("lightgreen"), model.data(lightgreenIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(2, model.rowCount(lightgreenIndex));
    	
    	darkgreenIndex = model.index(0,0, lightgreenIndex);
    	assertEquals("darkgreen", model.data(darkgreenIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#006400", model.data(darkgreenIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("darkgreen"), model.data(darkgreenIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(0, model.rowCount(darkgreenIndex));

    	blueIndex = model.index(1,0, lightgreenIndex);
    	assertEquals("blue", model.data(blueIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#0000ff", model.data(blueIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("blue"), model.data(blueIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(0, model.rowCount(blueIndex));
    	
    	assertTrue(redIndex.isValid());
    	assertFalse(model.flags(redIndex).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(redIndex.siblingAtColumn(displayProp.relativePropertyIndex()), "lavender", Qt.ItemDataRole.DisplayRole));
    	assertFalse(model.insertRow(model.rowCount()));
    	
//    	QTreeView view = new QTreeView();
//    	view.setModel(model);
//    	view.show();
//    	QApplication.exec();
    	model.dispose();
    }
    
    @Test
    public void testTreeEntryObject() {
    	QMetaObject metaObject = QMetaObject.forType(ColorTreeEntryObject.class);
		QMetaProperty displayProp = metaObject.property("display");
		QMetaProperty decorationProp = metaObject.property("decoration");
		QMetaProperty toolTipProp = metaObject.property("toolTip");
    	QList<ColorTreeEntryObject> range = QList.of(new ColorTreeEntryObject("red"), new ColorTreeEntryObject("green"));
    	range.at(1).addChild("yellow");
    	ColorTreeEntryObject child = range.at(1).addChild("lightgreen");
    	child.addChild("darkgreen");
    	child.addChild("blue");
    	QRangeModel model = new QRangeModel(range, true);
    	assertEquals(2, model.rowCount());
    	assertEquals(1, model.columnCount());
    	
    	QModelIndex redIndex = model.index(0,0);
    	assertEquals("red", model.data(redIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#ff0000", model.data(redIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("red"), model.data(redIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(0, model.rowCount(redIndex));
    	
    	QModelIndex greenIndex = model.index(1,0);
    	assertEquals("green", model.data(greenIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#008000", model.data(greenIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("green"), model.data(greenIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(2, model.rowCount(greenIndex));
    	
    	QModelIndex yellowIndex = model.index(0,0, greenIndex);
    	assertTrue(yellowIndex.isValid());
    	assertEquals("yellow", model.data(yellowIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#ffff00", model.data(yellowIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("yellow"), model.data(yellowIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(0, model.rowCount(yellowIndex));
    	
    	QModelIndex lightgreenIndex = model.index(1,0, greenIndex);
    	assertEquals("lightgreen", model.data(lightgreenIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#90ee90", model.data(lightgreenIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("lightgreen"), model.data(lightgreenIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(2, model.rowCount(lightgreenIndex));
    	
    	QModelIndex darkgreenIndex = model.index(0,0, lightgreenIndex);
    	assertEquals("darkgreen", model.data(darkgreenIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#006400", model.data(darkgreenIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("darkgreen"), model.data(darkgreenIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(0, model.rowCount(darkgreenIndex));

    	QModelIndex blueIndex = model.index(1,0, lightgreenIndex);
    	assertEquals("blue", model.data(blueIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#0000ff", model.data(blueIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("blue"), model.data(blueIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(0, model.rowCount(blueIndex));
    	
    	assertTrue(redIndex.isValid());
    	assertTrue(model.flags(redIndex).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(redIndex, "gray", Qt.ItemDataRole.DisplayRole));
    	assertEquals(2, range.size());
    	assertTrue(model.insertRow(model.rowCount()));
    	assertEquals(3, range.size());
    	QModelIndex newIndex = model.index(2,0);
    	assertTrue(model.setData(newIndex, "magenta", Qt.ItemDataRole.DisplayRole));
    	assertEquals("magenta", range.get(2).display());
    	assertEquals(0, range.get(2).childRows().size());
    	assertTrue(model.insertRow(model.rowCount(newIndex), newIndex));
    	assertEquals(1, range.get(2).childRows().size());
    	QModelIndex newSubIndex = model.index(0,0, newIndex);
    	assertTrue(newSubIndex.isValid());
    	assertTrue(model.setData(newSubIndex, "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.get(2).childRows().get(0).display());
    	model.dispose();
    	
    	model = new QRangeModel(range);
    	redIndex = model.index(0,0);
    	assertEquals("gray", model.data(redIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#808080", model.data(redIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("gray"), model.data(redIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(0, model.rowCount(redIndex));
    	
    	greenIndex = model.index(1,0);
    	assertEquals("green", model.data(greenIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#008000", model.data(greenIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("green"), model.data(greenIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(2, model.rowCount(greenIndex));
    	
    	yellowIndex = model.index(0,0, greenIndex);
    	assertTrue(yellowIndex.isValid());
    	assertEquals("yellow", model.data(yellowIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#ffff00", model.data(yellowIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("yellow"), model.data(yellowIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(0, model.rowCount(yellowIndex));
    	
    	lightgreenIndex = model.index(1,0, greenIndex);
    	assertEquals("lightgreen", model.data(lightgreenIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#90ee90", model.data(lightgreenIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("lightgreen"), model.data(lightgreenIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(2, model.rowCount(lightgreenIndex));
    	
    	darkgreenIndex = model.index(0,0, lightgreenIndex);
    	assertEquals("darkgreen", model.data(darkgreenIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#006400", model.data(darkgreenIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("darkgreen"), model.data(darkgreenIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(0, model.rowCount(darkgreenIndex));

    	blueIndex = model.index(1,0, lightgreenIndex);
    	assertEquals("blue", model.data(blueIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#0000ff", model.data(blueIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("blue"), model.data(blueIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(0, model.rowCount(blueIndex));
    	
    	assertTrue(redIndex.isValid());
    	assertTrue(model.flags(redIndex).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(redIndex.siblingAtColumn(displayProp.relativePropertyIndex()), "lavender", Qt.ItemDataRole.DisplayRole));
    	assertEquals(3, range.size());
    	assertTrue(model.insertRow(model.rowCount()));
    	assertEquals(4, range.size());
    	newIndex = model.index(3,0);
    	assertTrue(model.setData(newIndex.siblingAtColumn(displayProp.relativePropertyIndex()), "aqua", Qt.ItemDataRole.DisplayRole));
    	assertEquals("aqua", range.get(3).display());
    	assertEquals(0, range.get(3).childRows().size());
    	assertTrue(model.insertRow(model.rowCount(newIndex), newIndex));
    	assertEquals(1, range.get(3).childRows().size());
    	newSubIndex = model.index(0,0, newIndex);
    	assertTrue(model.setData(newSubIndex.siblingAtColumn(displayProp.relativePropertyIndex()), "gold", Qt.ItemDataRole.DisplayRole));
    	assertEquals("gold", range.get(3).childRows().get(0).display());
    	
    	model = new QRangeModel(QSpan.ofList(range), true);
    	assertEquals(4, model.rowCount());
    	assertEquals(1, model.columnCount());
    	assertFalse(model.insertRow(model.rowCount()));
    	redIndex = model.index(0,0);
    	assertTrue(redIndex.isValid());
    	assertTrue(model.flags(redIndex).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(redIndex, "purple", Qt.ItemDataRole.DisplayRole));
    	assertTrue(model.insertRow(model.rowCount(redIndex), redIndex));
    	model.dispose();
    	
    	model = new QRangeModel(QConstSpan.ofList(range), true);
    	assertEquals(4, model.rowCount());
    	assertEquals(1, model.columnCount());
    	assertFalse(model.insertRow(model.rowCount()));
    	redIndex = model.index(0,0);
    	assertTrue(redIndex.isValid());
    	assertFalse(model.insertRow(model.rowCount(redIndex), redIndex));
    	
    	QModelIndex subIndex = model.index(0,0, redIndex);
    	assertTrue(model.flags(subIndex).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(subIndex, "red", Qt.ItemDataRole.DisplayRole));
    	
//    	QTreeView view = new QTreeView();
//    	view.setModel(model);
//    	view.show();
//    	QApplication.exec();
    	model.dispose();
    }
    
    @Test
    public void testConstTreeEntryGadget() {
    	QMetaObject metaObject = QMetaObject.forType(ColorConstTreeEntryGadget.class);
		QMetaProperty displayProp = metaObject.property("display");
		QMetaProperty decorationProp = metaObject.property("decoration");
		QMetaProperty toolTipProp = metaObject.property("toolTip");
    	QList<ColorConstTreeEntryGadget> range = QList.of(new ColorConstTreeEntryGadget("red"), new ColorConstTreeEntryGadget("green"));
    	range.at(1).addChild("yellow");
    	ColorConstTreeEntryGadget child = range.at(1).addChild("lightgreen");
    	child.addChild("darkgreen");
    	child.addChild("blue");
    	QRangeModel model = new QRangeModel(range, true);
    	assertEquals(2, model.rowCount());
    	assertEquals(1, model.columnCount());
    	
    	QModelIndex redIndex = model.index(0,0);
    	assertEquals("red", model.data(redIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#ff0000", model.data(redIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("red"), model.data(redIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(0, model.rowCount(redIndex));
    	
    	QModelIndex greenIndex = model.index(1,0);
    	assertEquals("green", model.data(greenIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#008000", model.data(greenIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("green"), model.data(greenIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(2, model.rowCount(greenIndex));
    	
    	QModelIndex yellowIndex = model.index(0,0, greenIndex);
    	assertEquals("yellow", model.data(yellowIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#ffff00", model.data(yellowIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("yellow"), model.data(yellowIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(0, model.rowCount(yellowIndex));
    	
    	QModelIndex lightgreenIndex = model.index(1,0, greenIndex);
    	assertEquals("lightgreen", model.data(lightgreenIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#90ee90", model.data(lightgreenIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("lightgreen"), model.data(lightgreenIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(2, model.rowCount(lightgreenIndex));
    	
    	QModelIndex darkgreenIndex = model.index(0,0, lightgreenIndex);
    	assertEquals("darkgreen", model.data(darkgreenIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#006400", model.data(darkgreenIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("darkgreen"), model.data(darkgreenIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(0, model.rowCount(darkgreenIndex));

    	QModelIndex blueIndex = model.index(1,0, lightgreenIndex);
    	assertEquals("blue", model.data(blueIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#0000ff", model.data(blueIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("blue"), model.data(blueIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(0, model.rowCount(blueIndex));
    	
    	assertFalse(model.flags(redIndex).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(redIndex, "gray", Qt.ItemDataRole.DisplayRole));
    	assertFalse(model.insertRow(model.rowCount()));
    	model.dispose();
    	
    	model = new QRangeModel(range);
    	redIndex = model.index(0,0);
    	assertEquals("red", model.data(redIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#ff0000", model.data(redIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("red"), model.data(redIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(0, model.rowCount(redIndex));
    	
    	greenIndex = model.index(1,0);
    	assertEquals("green", model.data(greenIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#008000", model.data(greenIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("green"), model.data(greenIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(2, model.rowCount(greenIndex));
    	
    	yellowIndex = model.index(0,0, greenIndex);
    	assertTrue(yellowIndex.isValid());
    	assertEquals("yellow", model.data(yellowIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#ffff00", model.data(yellowIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("yellow"), model.data(yellowIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(0, model.rowCount(yellowIndex));
    	
    	lightgreenIndex = model.index(1,0, greenIndex);
    	assertEquals("lightgreen", model.data(lightgreenIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#90ee90", model.data(lightgreenIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("lightgreen"), model.data(lightgreenIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(2, model.rowCount(lightgreenIndex));
    	
    	darkgreenIndex = model.index(0,0, lightgreenIndex);
    	assertEquals("darkgreen", model.data(darkgreenIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#006400", model.data(darkgreenIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("darkgreen"), model.data(darkgreenIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(0, model.rowCount(darkgreenIndex));

    	blueIndex = model.index(1,0, lightgreenIndex);
    	assertEquals("blue", model.data(blueIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#0000ff", model.data(blueIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("blue"), model.data(blueIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(0, model.rowCount(blueIndex));
    	
    	assertTrue(redIndex.isValid());
    	assertFalse(model.flags(redIndex).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertFalse(model.setData(redIndex.siblingAtColumn(displayProp.relativePropertyIndex()), "lavender", Qt.ItemDataRole.DisplayRole));
    	assertFalse(model.insertRow(model.rowCount()));
    	
//    	QTreeView view = new QTreeView();
//    	view.setModel(model);
//    	view.show();
//    	QApplication.exec();
    	model.dispose();
    }
    
    @Test
    public void testTreeEntryGadget() {
    	QMetaObject metaObject = QMetaObject.forType(ColorTreeEntryGadget.class);
		QMetaProperty displayProp = metaObject.property("display");
		QMetaProperty decorationProp = metaObject.property("decoration");
		QMetaProperty toolTipProp = metaObject.property("toolTip");
    	QList<ColorTreeEntryGadget> range = QList.of(new ColorTreeEntryGadget("red"), new ColorTreeEntryGadget("green"));
    	range.at(1).addChild("yellow");
    	ColorTreeEntryGadget child = range.at(1).addChild("lightgreen");
    	child.addChild("darkgreen");
    	child.addChild("blue");
    	QRangeModel model = new QRangeModel(range, true);
    	assertEquals(2, model.rowCount());
    	assertEquals(1, model.columnCount());
    	
    	QModelIndex redIndex = model.index(0,0);
    	assertEquals("red", model.data(redIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#ff0000", model.data(redIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("red"), model.data(redIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(0, model.rowCount(redIndex));
    	
    	QModelIndex greenIndex = model.index(1,0);
    	assertEquals("green", model.data(greenIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#008000", model.data(greenIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("green"), model.data(greenIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(2, model.rowCount(greenIndex));
    	
    	QModelIndex yellowIndex = model.index(0,0, greenIndex);
    	assertTrue(yellowIndex.isValid());
    	assertEquals("yellow", model.data(yellowIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#ffff00", model.data(yellowIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("yellow"), model.data(yellowIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(0, model.rowCount(yellowIndex));
    	
    	QModelIndex lightgreenIndex = model.index(1,0, greenIndex);
    	assertEquals("lightgreen", model.data(lightgreenIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#90ee90", model.data(lightgreenIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("lightgreen"), model.data(lightgreenIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(2, model.rowCount(lightgreenIndex));
    	
    	QModelIndex darkgreenIndex = model.index(0,0, lightgreenIndex);
    	assertEquals("darkgreen", model.data(darkgreenIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#006400", model.data(darkgreenIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("darkgreen"), model.data(darkgreenIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(0, model.rowCount(darkgreenIndex));

    	QModelIndex blueIndex = model.index(1,0, lightgreenIndex);
    	assertEquals("blue", model.data(blueIndex, Qt.ItemDataRole.DisplayRole));
    	assertEquals("#0000ff", model.data(blueIndex, Qt.ItemDataRole.ToolTipRole));
    	assertEquals(new QColor("blue"), model.data(blueIndex, Qt.ItemDataRole.DecorationRole));
    	assertEquals(0, model.rowCount(blueIndex));
    	
    	assertTrue(redIndex.isValid());
    	assertTrue(model.flags(redIndex).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(redIndex, "gray", Qt.ItemDataRole.DisplayRole));
    	assertEquals(2, range.size());
    	assertTrue(model.insertRow(model.rowCount()));
    	assertEquals(3, range.size());
    	QModelIndex newIndex = model.index(2,0);
    	assertTrue(model.setData(newIndex, "magenta", Qt.ItemDataRole.DisplayRole));
    	assertEquals("magenta", range.get(2).display());
    	assertEquals(0, range.get(2).childRows().size());
    	assertTrue(model.insertRow(model.rowCount(newIndex), newIndex));
    	assertEquals(1, range.get(2).childRows().size());
    	QModelIndex newSubIndex = model.index(0,0, newIndex);
    	assertTrue(model.setData(newSubIndex, "yellow", Qt.ItemDataRole.DisplayRole));
    	assertEquals("yellow", range.get(2).childRows().get(0).display());
    	assertFalse(model.insertColumn(model.columnCount()));
    	model.dispose();
    	
    	model = new QRangeModel(range);
    	redIndex = model.index(0,0);
    	assertEquals("gray", model.data(redIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#808080", model.data(redIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("gray"), model.data(redIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(0, model.rowCount(redIndex));
    	
    	greenIndex = model.index(1,0);
    	assertEquals("green", model.data(greenIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#008000", model.data(greenIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("green"), model.data(greenIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(2, model.rowCount(greenIndex));
    	
    	yellowIndex = model.index(0,0, greenIndex);
    	assertTrue(yellowIndex.isValid());
    	assertEquals("yellow", model.data(yellowIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#ffff00", model.data(yellowIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("yellow"), model.data(yellowIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(0, model.rowCount(yellowIndex));
    	
    	lightgreenIndex = model.index(1,0, greenIndex);
    	assertEquals("lightgreen", model.data(lightgreenIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#90ee90", model.data(lightgreenIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("lightgreen"), model.data(lightgreenIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(2, model.rowCount(lightgreenIndex));
    	
    	darkgreenIndex = model.index(0,0, lightgreenIndex);
    	assertEquals("darkgreen", model.data(darkgreenIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#006400", model.data(darkgreenIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("darkgreen"), model.data(darkgreenIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(0, model.rowCount(darkgreenIndex));

    	blueIndex = model.index(1,0, lightgreenIndex);
    	assertEquals("blue", model.data(blueIndex.siblingAtColumn(displayProp.relativePropertyIndex())));
    	assertEquals("#0000ff", model.data(blueIndex.siblingAtColumn(toolTipProp.relativePropertyIndex())));
    	assertEquals(new QColor("blue"), model.data(blueIndex.siblingAtColumn(decorationProp.relativePropertyIndex())));
    	assertEquals(0, model.rowCount(blueIndex));
    	
    	assertTrue(redIndex.isValid());
    	assertTrue(model.flags(redIndex).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(redIndex.siblingAtColumn(displayProp.relativePropertyIndex()), "lavender", Qt.ItemDataRole.DisplayRole));
    	assertEquals(3, range.size());
    	assertTrue(model.insertRow(model.rowCount()));
    	assertEquals(4, range.size());
    	newIndex = model.index(3,0);
    	assertTrue(model.setData(newIndex.siblingAtColumn(displayProp.relativePropertyIndex()), "aqua", Qt.ItemDataRole.DisplayRole));
    	assertEquals("aqua", range.get(3).display());
    	assertEquals(0, range.get(3).childRows().size());
    	assertTrue(model.insertRow(model.rowCount(newIndex), newIndex));
    	assertEquals(1, range.get(3).childRows().size());
    	newSubIndex = model.index(0,0, newIndex);
    	assertTrue(model.setData(newSubIndex.siblingAtColumn(displayProp.relativePropertyIndex()), "gold", Qt.ItemDataRole.DisplayRole));
    	assertEquals("gold", range.get(3).childRows().get(0).display());
    	
    	model = new QRangeModel(QSpan.ofList(range), true);
    	assertEquals(4, model.rowCount());
    	assertEquals(1, model.columnCount());
    	assertFalse(model.insertRow(model.rowCount()));
    	redIndex = model.index(0,0);
    	assertTrue(redIndex.isValid());
    	assertTrue(model.flags(redIndex).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(redIndex, "purple", Qt.ItemDataRole.DisplayRole));
    	assertTrue(model.insertRow(model.rowCount(redIndex), redIndex));
    	model.dispose();
    	
    	model = new QRangeModel(QConstSpan.ofList(range), true);
    	assertEquals(4, model.rowCount());
    	assertEquals(1, model.columnCount());
    	assertFalse(model.insertRow(model.rowCount()));
    	redIndex = model.index(0,0);
    	assertTrue(redIndex.isValid());
    	assertFalse(model.insertRow(model.rowCount(redIndex), redIndex));
    	
    	QModelIndex subIndex = model.index(0,0, redIndex);
    	assertTrue(model.flags(subIndex).testFlag(Qt.ItemFlag.ItemIsEditable));
    	assertTrue(model.setData(subIndex, "red", Qt.ItemDataRole.DisplayRole));
    	
//    	QTreeView view = new QTreeView();
//    	view.setModel(model);
//    	view.show();
//    	QApplication.exec();
    	model.dispose();
    }
    
    @Test
    public void testConstTreeEntryRoles() {
    	QList<ConstRolesTreeEntry> range = QList.of(new ConstRolesTreeEntry(QMap.of(Qt.ItemDataRole.DisplayRole, "A1", Qt.ItemDataRole.ToolTipRole, "tooltip_A1"),
						QMap.of(Qt.ItemDataRole.DisplayRole, "A2", Qt.ItemDataRole.ToolTipRole, "tooltip_A2")),
    			new ConstRolesTreeEntry(QMap.of(Qt.ItemDataRole.DisplayRole, "B1", Qt.ItemDataRole.ToolTipRole, "tooltip_B1"),
						QMap.of(Qt.ItemDataRole.DisplayRole, "B2", Qt.ItemDataRole.ToolTipRole, "tooltip_B2")));
    	range.at(0).addChild(QMap.of(Qt.ItemDataRole.DisplayRole, "C1", Qt.ItemDataRole.ToolTipRole, "tooltip_C1"));
    	range.at(0).addChild(QMap.of(Qt.ItemDataRole.DisplayRole, "D1", Qt.ItemDataRole.ToolTipRole, "tooltip_D1"),
			    			 QMap.of(Qt.ItemDataRole.DisplayRole, "E1", Qt.ItemDataRole.ToolTipRole, "tooltip_E1"),
			    			 QMap.of(Qt.ItemDataRole.DisplayRole, "F1", Qt.ItemDataRole.ToolTipRole, "tooltip_F1"));
    	QRangeModel model = new QRangeModel(range);
    	try {
	    	assertEquals(2, model.rowCount());
	    	assertEquals(3, model.columnCount());
	    	QModelIndex indexA = model.index(0,0);
	    	assertEquals("A1", model.data(indexA, Qt.ItemDataRole.DisplayRole));
	    	assertEquals("tooltip_A1", model.data(indexA, Qt.ItemDataRole.ToolTipRole));
	    	assertEquals("A2", model.data(indexA.siblingAtColumn(1), Qt.ItemDataRole.DisplayRole));
	    	assertEquals("tooltip_A2", model.data(indexA.siblingAtColumn(1), Qt.ItemDataRole.ToolTipRole));
	    	QModelIndex indexB = model.index(1,0);
	    	assertEquals("B1", model.data(indexB, Qt.ItemDataRole.DisplayRole));
	    	assertEquals("tooltip_B1", model.data(indexB, Qt.ItemDataRole.ToolTipRole));
	    	assertEquals("B2", model.data(indexB.siblingAtColumn(1), Qt.ItemDataRole.DisplayRole));
	    	assertEquals("tooltip_B2", model.data(indexB.siblingAtColumn(1), Qt.ItemDataRole.ToolTipRole));
    	}finally {
    		model.dispose();
    	}
    }
    
    @Test
    public void testTreeEntryRoles() {
    	QList<RolesTreeEntry> range = QList.of(new RolesTreeEntry(QMap.of(Qt.ItemDataRole.DisplayRole, "A1", Qt.ItemDataRole.ToolTipRole, "tooltip_A1"),
						QMap.of(Qt.ItemDataRole.DisplayRole, "A2", Qt.ItemDataRole.ToolTipRole, "tooltip_A2")),
    			new RolesTreeEntry(QMap.of(Qt.ItemDataRole.DisplayRole, "B1", Qt.ItemDataRole.ToolTipRole, "tooltip_B1"),
						QMap.of(Qt.ItemDataRole.DisplayRole, "B2", Qt.ItemDataRole.ToolTipRole, "tooltip_B2")));
    	range.at(0).addChild(QMap.of(Qt.ItemDataRole.DisplayRole, "AA1", Qt.ItemDataRole.ToolTipRole, "tooltip_AA1"));
    	range.at(0).addChild(QMap.of(Qt.ItemDataRole.DisplayRole, "AB1", Qt.ItemDataRole.ToolTipRole, "tooltip_AB1"),
			    			 QMap.of(Qt.ItemDataRole.DisplayRole, "AB2", Qt.ItemDataRole.ToolTipRole, "tooltip_AB2"),
			    			 QMap.of(Qt.ItemDataRole.DisplayRole, "AB3", Qt.ItemDataRole.ToolTipRole, "tooltip_AB3"));
    	Consumer<QList<RolesTreeEntry>> printer = _range -> {
    												@SuppressWarnings("unused")
													StringBuilder s = _range.stream().collect(StringBuilder::new, (StringBuilder sb, RolesTreeEntry map)->{
														if(!sb.isEmpty())
															sb.append(", ");
														sb.append(""+map);
													}, StringBuilder::append);
											    	System.out.println(s);
												};
    	QRangeModel model = new QRangeModel(range);
    	try {
	    	printer.accept(range);
	    	assertEquals(2, model.rowCount());
	    	assertEquals(3, model.columnCount());
	    	QModelIndex indexA = model.index(0,0);
	    	QModelIndex indexB = model.index(1,0);
	    	assertEquals("A1", model.data(indexA, Qt.ItemDataRole.DisplayRole));
	    	assertEquals("tooltip_A1", model.data(indexA, Qt.ItemDataRole.ToolTipRole));
	    	assertEquals("A2", model.data(indexA.siblingAtColumn(1), Qt.ItemDataRole.DisplayRole));
	    	assertEquals("tooltip_A2", model.data(indexA.siblingAtColumn(1), Qt.ItemDataRole.ToolTipRole));
	    	assertEquals("B1", model.data(indexB, Qt.ItemDataRole.DisplayRole));
	    	assertEquals("tooltip_B1", model.data(indexB, Qt.ItemDataRole.ToolTipRole));
	    	assertEquals("B2", model.data(indexB.siblingAtColumn(1), Qt.ItemDataRole.DisplayRole));
	    	assertEquals("tooltip_B2", model.data(indexB.siblingAtColumn(1), Qt.ItemDataRole.ToolTipRole));
	    	assertTrue(model.setData(indexA.siblingAtColumn(2), "A3"));
	    	assertEquals("A1", model.data(indexA, Qt.ItemDataRole.DisplayRole));
	    	assertTrue(model.insertRow(model.rowCount()));
	    	printer.accept(range);
	    	QModelIndex indexC = model.index(2,0);
	    	assertTrue(indexC.isValid());
	    	assertTrue(model.setData(indexC, "C1"));
	    	assertTrue(model.setData(indexC.siblingAtColumn(1), "C2"));
	    	assertTrue(model.setData(indexC.siblingAtColumn(2), "C3"));
	    	assertEquals("A1", model.data(indexA, Qt.ItemDataRole.DisplayRole));
	    	assertEquals("tooltip_A1", model.data(indexA, Qt.ItemDataRole.ToolTipRole));
	    	assertEquals("A2", model.data(indexA.siblingAtColumn(1), Qt.ItemDataRole.DisplayRole));
	    	assertEquals("tooltip_A2", model.data(indexA.siblingAtColumn(1), Qt.ItemDataRole.ToolTipRole));
	    	assertEquals("B1", model.data(indexB, Qt.ItemDataRole.DisplayRole));
	    	assertEquals("tooltip_B1", model.data(indexB, Qt.ItemDataRole.ToolTipRole));
	    	assertEquals("B2", model.data(indexB.siblingAtColumn(1), Qt.ItemDataRole.DisplayRole));
	    	assertEquals("tooltip_B2", model.data(indexB.siblingAtColumn(1), Qt.ItemDataRole.ToolTipRole));
	    	assertEquals("A3", model.data(indexA.siblingAtColumn(2), Qt.ItemDataRole.DisplayRole));
	    	assertEquals(2, model.rowCount(indexA));
	    	assertTrue(model.removeRow(0, indexA));
	    	printer.accept(range);
	    	assertEquals(1, model.rowCount(indexA));
	    	assertTrue(model.removeRow(0));
	    	printer.accept(range);
	    	assertEquals(2, model.rowCount());
	    	assertEquals("B1", model.data(indexA, Qt.ItemDataRole.DisplayRole));
	    	assertEquals("tooltip_B1", model.data(indexA, Qt.ItemDataRole.ToolTipRole));
	    	assertEquals("B2", model.data(indexA.siblingAtColumn(1), Qt.ItemDataRole.DisplayRole));
	    	assertEquals("tooltip_B2", model.data(indexA.siblingAtColumn(1), Qt.ItemDataRole.ToolTipRole));
	    	assertEquals("C3", model.data(indexB.siblingAtColumn(2), Qt.ItemDataRole.DisplayRole));
	    	assertTrue(model.insertRow(model.rowCount(indexB), indexB));
	    	QModelIndex indexSubB = model.index(0,0, indexB);
	    	assertTrue(model.setItemData(indexSubB, QMap.of(Qt.ItemDataRole.DisplayRole, "CA1", Qt.ItemDataRole.ToolTipRole, "tooltip_CA1")));
	    	assertTrue(model.setItemData(indexSubB.siblingAtColumn(1), QMap.of(Qt.ItemDataRole.DisplayRole, "CA2", Qt.ItemDataRole.ToolTipRole, "tooltip_CA2")));
	    	assertTrue(model.setItemData(indexSubB.siblingAtColumn(2), QMap.of(Qt.ItemDataRole.DisplayRole, "CA3", Qt.ItemDataRole.ToolTipRole, "tooltip_CA3")));
	    	assertEquals(QMap.of(Qt.ItemDataRole.DisplayRole, "CA1", Qt.ItemDataRole.ToolTipRole, "tooltip_CA1"), model.itemData(indexSubB));
	    	assertTrue(model.clearItemData(indexSubB));
	    	assertEquals(new QMap<>(int.class, QVariant.class), model.itemData(indexSubB));
	    	printer.accept(range);
	    	assertTrue(model.setItemData(indexSubB, QMap.of(Qt.ItemDataRole.DisplayRole, "CA1", Qt.ItemDataRole.ToolTipRole, "tooltip_CA1")));
    	}finally {
    		model.dispose();
    	}
    	try {
	    	model = new QRangeModel(range);
	    	QModelIndex indexA = model.index(0,0);
	    	QModelIndex indexB = model.index(1,0);
	    	assertEquals("B1", model.data(indexA, Qt.ItemDataRole.DisplayRole));
	    	assertEquals("B1", range.at(0).value(0).get(Qt.ItemDataRole.DisplayRole));
	    	printer.accept(range);
	    	assertTrue(model.moveRow(null, 0, null, 2));
	    	printer.accept(range);
	    	assertEquals("C1", model.data(indexA, Qt.ItemDataRole.DisplayRole));
	    	assertEquals("C1", range.at(0).value(0).get(Qt.ItemDataRole.DisplayRole));
	    	printer.accept(range);
	    	assertTrue(model.insertRow(0));
	    	assertTrue(model.setItemData(indexA, QMap.of(Qt.ItemDataRole.DisplayRole, "A1", Qt.ItemDataRole.ToolTipRole, "tooltip_A1")));
	    	assertTrue(model.setItemData(indexA.siblingAtColumn(1), QMap.of(Qt.ItemDataRole.DisplayRole, "A2", Qt.ItemDataRole.ToolTipRole, "tooltip_A2")));
	    	assertTrue(model.setItemData(indexA.siblingAtColumn(2), QMap.of(Qt.ItemDataRole.DisplayRole, "A3", Qt.ItemDataRole.ToolTipRole, "tooltip_A3")));
	    	printer.accept(range);
	    	assertEquals("CA1", range.at(1).childRows().get(0).value(0).get(Qt.ItemDataRole.DisplayRole));
	    	assertTrue(model.moveRow(indexB, 0, indexA, 0));
	    	printer.accept(range);
	    	assertEquals("CA1", range.at(0).childRows().get(0).value(0).get(Qt.ItemDataRole.DisplayRole));
	    	assertTrue(model.insertRows(0, 2, indexA));
	    	QModelIndex indexSubA = model.index(0,0, indexA);
	    	assertTrue(model.setItemData(indexSubA, QMap.of(Qt.ItemDataRole.DisplayRole, "AA1", Qt.ItemDataRole.ToolTipRole, "tooltip_AA1")));
	    	assertTrue(model.setItemData(indexSubA.siblingAtColumn(1), QMap.of(Qt.ItemDataRole.DisplayRole, "AA2", Qt.ItemDataRole.ToolTipRole, "tooltip_AA2")));
	    	assertTrue(model.setItemData(indexSubA.siblingAtColumn(2), QMap.of(Qt.ItemDataRole.DisplayRole, "AA3", Qt.ItemDataRole.ToolTipRole, "tooltip_AA3")));
	    	indexSubA = model.index(1,0, indexA);
	    	assertTrue(model.setItemData(indexSubA, QMap.of(Qt.ItemDataRole.DisplayRole, "AB1", Qt.ItemDataRole.ToolTipRole, "tooltip_AB1")));
	    	assertTrue(model.setItemData(indexSubA.siblingAtColumn(1), QMap.of(Qt.ItemDataRole.DisplayRole, "AB2", Qt.ItemDataRole.ToolTipRole, "tooltip_AB2")));
	    	assertTrue(model.setItemData(indexSubA.siblingAtColumn(2), QMap.of(Qt.ItemDataRole.DisplayRole, "AB3", Qt.ItemDataRole.ToolTipRole, "tooltip_AB3")));
	    	indexSubA = model.index(2,0, indexA);
	    	assertTrue(model.setItemData(indexSubA, QMap.of(Qt.ItemDataRole.DisplayRole, "AC1", Qt.ItemDataRole.ToolTipRole, "tooltip_AC1")));
	    	assertTrue(model.setItemData(indexSubA.siblingAtColumn(1), QMap.of(Qt.ItemDataRole.DisplayRole, "AC2", Qt.ItemDataRole.ToolTipRole, "tooltip_AC2")));
	    	assertTrue(model.setItemData(indexSubA.siblingAtColumn(2), QMap.of(Qt.ItemDataRole.DisplayRole, "AC3", Qt.ItemDataRole.ToolTipRole, "tooltip_AC3")));
	    	printer.accept(range);
	    	assertEquals("AA1", range.at(0).childRows().get(0).value(0).get(Qt.ItemDataRole.DisplayRole));
	    	assertTrue(model.moveRow(indexA, 0, indexA, 2));
	    	assertEquals("AB1", range.at(0).childRows().get(0).value(0).get(Qt.ItemDataRole.DisplayRole));
	    	printer.accept(range);
	//    	QTreeView view = new QTreeView();
	//    	view.setModel(model);
	//    	view.show();
	//    	QApplication.exec();
    	}finally {
    		model.dispose();
    	}
    }
}
