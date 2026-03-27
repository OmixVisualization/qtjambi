package io.qt.autotests;

import static org.junit.Assert.*;

import java.util.*;
import java.util.function.*;

import org.junit.*;

import io.qt.*;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.widgets.*;

public class TestQRangeModelQt611 extends ApplicationInitializer {
	
    @BeforeClass
    public static void testInitialize() throws Exception {
    	QApplication.class.hashCode();
        ApplicationInitializer.testInitializeWithWidgets();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
		defaultRoleNames = (QHash<Integer,QByteArray>)new QStandardItemModel().roleNames();
	}
    
    @AfterClass
    public static void testDispose() throws Exception {
    	ApplicationInitializer.testDispose();
    	defaultRoleNames = null;
    }
    
    private static QHash<Integer,QByteArray> defaultRoleNames;
    
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
		@QtPropertyReader(enabled = false)
		public QColor decoration()
	    {
	        return QColor.fromString(colorName);
	    }
		@QtPropertyReader(enabled = false)
		public String toolTip()
	    {
	        return QColor.fromString(colorName).name();
	    }
		@QtPropertyReader(enabled = false)
		public String display() {
			return colorName;
		}
		@QtPropertyWriter(enabled = false)
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
		@QtPropertyReader(enabled = false)
		public QColor decoration()
	    {
	        return QColor.fromString(colorName);
	    }
		@QtPropertyReader(enabled = false)
		public String toolTip()
	    {
	        return QColor.fromString(colorName).name();
	    }
		@QtPropertyReader(enabled = false)
		public String display() {
			return colorName;
		}
		@QtPropertyWriter(enabled = false)
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
		@QtPropertyReader(enabled = false)
		public QColor decoration()
	    {
	        return QColor.fromString(colorName);
	    }
		@QtPropertyReader(enabled = false)
		public String toolTip()
	    {
	        return QColor.fromString(colorName).name();
	    }
		@QtPropertyReader(enabled = false)
		public String display() {
			return colorName;
		}
		@QtPropertyWriter(enabled = false)
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
		@QtPropertyReader(enabled = false)
		public QColor decoration()
	    {
	        return QColor.fromString(colorName);
	    }
		@QtPropertyReader(enabled = false)
		public String toolTip()
	    {
	        return QColor.fromString(colorName).name();
	    }
		@QtPropertyReader(enabled = false)
		public String display() {
			return colorName;
		}
		@QtPropertyWriter(enabled = false)
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
		@QtPropertyReader(enabled = false)
		public QColor decoration()
	    {
	        return QColor.fromString(colorName);
	    }
		@QtPropertyReader(enabled = false)
		public String toolTip()
	    {
	        return QColor.fromString(colorName).name();
	    }
		@QtPropertyReader(enabled = false)
		public String display() {
			return colorName;
		}
		@QtPropertyWriter(enabled = false)
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
		@QtPropertyReader(enabled = false)
		public String display() {
			return colorName;
		}
		@QtPropertyReader(enabled = false)
		public String toolTip()
	    {
	        return QColor.fromString(colorName).name();
	    }
		@QtPropertyReader(enabled = false)
		public QColor decoration()
	    {
	        return QColor.fromString(colorName);
	    }
		@QtPropertyWriter(enabled = false)
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
		@QtPropertyReader(enabled = false)
		public QColor decoration()
	    {
	        return QColor.fromString(colorName);
	    }
		@QtPropertyReader(enabled = false)
		public String toolTip()
	    {
	        return QColor.fromString(colorName).name();
	    }
		@QtPropertyReader(enabled = false)
		public String display() {
			return colorName;
		}
		@QtPropertyWriter(enabled = false)
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
	
	static class AcceesRangeModel extends QRangeModel{
		public <T> AcceesRangeModel(@StrictNonNull QConstSpan<T> range, @Nullable QObject parent) {
			super(range, parent);
		}

		public <T> AcceesRangeModel(@StrictNonNull QConstSpan<T> range) {
			super(range);
		}

		public <T> AcceesRangeModel(@StrictNonNull QList<T> range, @Nullable QObject parent) {
			super(range, parent);
		}

		public <T> AcceesRangeModel(@StrictNonNull QList<T> range) {
			super(range);
		}

		public <T> AcceesRangeModel(@StrictNonNull QSpan<T> range, @Nullable QObject parent) {
			super(range, parent);
		}

		public <T> AcceesRangeModel(@StrictNonNull QSpan<T> range) {
			super(range);
		}

		@Override
		protected @Nullable ItemAccess<?> itemAccess(@NonNull Class<?> itemType) {
			if(itemType==ColorConstTreeEntryGadget.class) {
				ItemAccess<ColorConstTreeEntryGadget> access = (g,role)->{
					switch(role) {
					case Qt.ItemDataRole.DisplayRole: return g.display();
					case Qt.ItemDataRole.DecorationRole: return g.decoration();
					case Qt.ItemDataRole.ToolTipRole: return g.toolTip();
					}
					return null;
				};
				return access;
			}
			if(itemType==ColorConstTreeEntryObject.class) {
				ItemAccess<ColorConstTreeEntryObject> access = (g,role)->{
					switch(role) {
					case Qt.ItemDataRole.DisplayRole: return g.display();
					case Qt.ItemDataRole.DecorationRole: return g.decoration();
					case Qt.ItemDataRole.ToolTipRole: return g.toolTip();
					}
					return null;
				};
				return access;
			}
			if(itemType==ColorEntryObject.class) {
				return new ItemAccess<ColorEntryObject>() {
					@Override
					public Object readRole(ColorEntryObject item, int role) {
						switch(role) {
						case Qt.ItemDataRole.DisplayRole: return item.display();
						case Qt.ItemDataRole.DecorationRole: return item.decoration();
						case Qt.ItemDataRole.ToolTipRole: return item.toolTip();
						}
						return null;
					}

					@Override
					public boolean writeRole(ColorEntryObject item, int role, Object value) {
						switch(role) {
						case Qt.ItemDataRole.DisplayRole: 
							item.setDisplay((String)value);
							return true;
						}
						return false;
					}
				};
			}
			if(itemType==ColorEntryGadget.class) {
				return new ItemAccess<ColorEntryGadget>() {
					@Override
					public Object readRole(ColorEntryGadget item, int role) {
						switch(role) {
						case Qt.ItemDataRole.DisplayRole: return item.display();
						case Qt.ItemDataRole.DecorationRole: return item.decoration();
						case Qt.ItemDataRole.ToolTipRole: return item.toolTip();
						}
						return null;
					}

					@Override
					public boolean writeRole(ColorEntryGadget item, int role, Object value) {
						switch(role) {
						case Qt.ItemDataRole.DisplayRole: 
							item.setDisplay((String)value);
							return true;
						}
						return false;
					}
				};
			}
			if(itemType==ColorEntryValue.class) {
				return new ItemAccess<ColorEntryValue>() {
					@Override
					public Object readRole(ColorEntryValue item, int role) {
						switch(role) {
						case Qt.ItemDataRole.DisplayRole: return item.display();
						case Qt.ItemDataRole.DecorationRole: return item.decoration();
						case Qt.ItemDataRole.ToolTipRole: return item.toolTip();
						}
						return null;
					}

					@Override
					public boolean writeRole(ColorEntryValue item, int role, Object value) {
						switch(role) {
						case Qt.ItemDataRole.DisplayRole: 
							item.setDisplay((String)value);
							return true;
						}
						return false;
					}
				};
			}
			if(itemType==ColorTreeEntryGadget.class) {
				return new ItemAccess<ColorTreeEntryGadget>() {
					@Override
					public Object readRole(ColorTreeEntryGadget item, int role) {
						switch(role) {
						case Qt.ItemDataRole.DisplayRole: return item.display();
						case Qt.ItemDataRole.DecorationRole: return item.decoration();
						case Qt.ItemDataRole.ToolTipRole: return item.toolTip();
						}
						return null;
					}

					@Override
					public boolean writeRole(ColorTreeEntryGadget item, int role, Object value) {
						switch(role) {
						case Qt.ItemDataRole.DisplayRole: 
							item.setDisplay((String)value);
							return true;
						}
						return false;
					}
				};
			}
			if(itemType==ColorTreeEntryObject.class) {
				return new ItemAccess<ColorTreeEntryObject>() {
					@Override
					public Object readRole(ColorTreeEntryObject item, int role) {
						switch(role) {
						case Qt.ItemDataRole.DisplayRole: return item.display();
						case Qt.ItemDataRole.DecorationRole: return item.decoration();
						case Qt.ItemDataRole.ToolTipRole: return item.toolTip();
						}
						return null;
					}

					@Override
					public boolean writeRole(ColorTreeEntryObject item, int role, Object value) {
						switch(role) {
						case Qt.ItemDataRole.DisplayRole: 
							item.setDisplay((String)value);
							return true;
						}
						return false;
					}
				};
			}
			return super.itemAccess(itemType);
		}
	}
    
    @Test
    public void test1DimensionQObject() {
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
    	QRangeModel model = new AcceesRangeModel(range, parent);
    	assertEquals(defaultRoleNames, model.roleNames());
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
    	model = new AcceesRangeModel(QSpan.ofList(range));
    	assertEquals(defaultRoleNames, model.roleNames());
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
    	model = new AcceesRangeModel(QConstSpan.ofList(range));
    	assertEquals(defaultRoleNames, model.roleNames());
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
    	QRangeModel model = new AcceesRangeModel(range, parent);
    	assertEquals(defaultRoleNames, model.roleNames());
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
    	model.dispose();
    }
    
    @Test
    public void test1DimensionValue() {
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
    	QRangeModel model = new AcceesRangeModel(range, parent);
    	assertEquals(defaultRoleNames, model.roleNames());
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
    	QRangeModel model = new AcceesRangeModel(range, parent);
    	assertEquals(defaultRoleNames, model.roleNames());
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
    	model = new AcceesRangeModel(QSpan.ofList(range));
    	assertEquals(defaultRoleNames, model.roleNames());
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
    	model = new AcceesRangeModel(QConstSpan.ofList(range));
    	assertEquals(defaultRoleNames, model.roleNames());
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
    	QRangeModel model = new AcceesRangeModel(range, parent);
    	assertEquals(defaultRoleNames, model.roleNames());
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
    	model = new AcceesRangeModel(QSpan.ofList(range));
    	assertEquals(defaultRoleNames, model.roleNames());
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
    	model = new AcceesRangeModel(QConstSpan.ofList(range));
    	assertEquals(defaultRoleNames, model.roleNames());
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
    	QRangeModel model = new AcceesRangeModel(range, parent);
    	assertEquals(defaultRoleNames, model.roleNames());
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
    	model = new AcceesRangeModel(QSpan.ofList(range));
    	assertEquals(defaultRoleNames, model.roleNames());
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
    	model = new AcceesRangeModel(QConstSpan.ofList(range));
    	assertEquals(defaultRoleNames, model.roleNames());
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
    	QList<ColorConstTreeEntryObject> range = QList.of(new ColorConstTreeEntryObject("red"), new ColorConstTreeEntryObject("green"));
    	range.at(1).addChild("yellow");
    	ColorConstTreeEntryObject child = range.at(1).addChild("lightgreen");
    	child.addChild("darkgreen");
    	child.addChild("blue");
    	QRangeModel model = new AcceesRangeModel(range);
    	assertEquals(defaultRoleNames, model.roleNames());
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
    }
    
    @Test
    public void testTreeEntryObject() {
    	QList<ColorTreeEntryObject> range = QList.of(new ColorTreeEntryObject("red"), new ColorTreeEntryObject("green"));
    	range.at(1).addChild("yellow");
    	ColorTreeEntryObject child = range.at(1).addChild("lightgreen");
    	child.addChild("darkgreen");
    	child.addChild("blue");
    	QRangeModel model = new AcceesRangeModel(range);
    	assertEquals(defaultRoleNames, model.roleNames());
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
    }
    
    @Test
    public void testConstTreeEntryGadget() {
    	QList<ColorConstTreeEntryGadget> range = QList.of(new ColorConstTreeEntryGadget("red"), new ColorConstTreeEntryGadget("green"));
    	range.at(1).addChild("yellow");
    	ColorConstTreeEntryGadget child = range.at(1).addChild("lightgreen");
    	child.addChild("darkgreen");
    	child.addChild("blue");
    	QRangeModel model = new AcceesRangeModel(range){
			@Override
			protected @NonNull RowCategory rowCategory(@NonNull Class<?> itemType) {
				return QRangeModel.RowCategory.MultiRoleItem;
			}
    	};
    	assertEquals(defaultRoleNames, model.roleNames());
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
    }
    
    @Test
    public void testTreeEntryGadget() {
    	QList<ColorTreeEntryGadget> range = QList.of(new ColorTreeEntryGadget("red"), new ColorTreeEntryGadget("green"));
    	range.at(1).addChild("yellow");
    	ColorTreeEntryGadget child = range.at(1).addChild("lightgreen");
    	child.addChild("darkgreen");
    	child.addChild("blue");
    	QRangeModel model = new AcceesRangeModel(range);
    	assertEquals(defaultRoleNames, model.roleNames());
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
    }
}
