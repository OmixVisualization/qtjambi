package io.qt.qtjambi.deployer;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Set;
import java.util.TreeMap;

import io.qt.QtUtilities;
import io.qt.core.QByteArray;
import io.qt.core.QCborMap;
import io.qt.core.QCborValue;
import io.qt.core.QCommandLineOption;
import io.qt.core.QCommandLineParser;
import io.qt.core.QDir;
import io.qt.core.QFile;
import io.qt.core.QIODevice;
import io.qt.core.QJsonArray;
import io.qt.core.QJsonObject;
import io.qt.core.QJsonValue;
import io.qt.core.QList;
import io.qt.core.QPair;
import io.qt.core.QStringList;
import io.qt.core.QTextStream;
import io.qt.internal.QtJambiInternal;

public class ContainerAccessGenerator {
	
	private enum ContainerType{
	    QList,
	    QVector,
	    QLinkedList,
	    QSet
	};
	
	private enum MapType{
	    QHash,
	    QMap,
	    QMultiHash,
	    QMultiMap,
	    QPair
	};
	
	private static class ContainerSpec{
		@Override
		public int hashCode() {
			return Objects.hash(alignment, containerType, size);
		}
		@Override
		public boolean equals(Object obj) {
			if (this == obj)
				return true;
			if (obj == null)
				return false;
			if (getClass() != obj.getClass())
				return false;
			ContainerSpec other = (ContainerSpec) obj;
			return alignment == other.alignment && containerType == other.containerType && size == other.size;
		}
		ContainerSpec(ContainerType containerType, long size, long alignment) {
			this.containerType = containerType;
			this.size = size;
			this.alignment = alignment;
		}
		final ContainerType containerType;
		final long size;
		final long alignment;
	}
	
	private static class MapSpec{
		@Override
		public int hashCode() {
			return Objects.hash(alignment1, alignment2, mapType, size1, size2);
		}
		@Override
		public boolean equals(Object obj) {
			if (this == obj)
				return true;
			if (obj == null)
				return false;
			if (getClass() != obj.getClass())
				return false;
			MapSpec other = (MapSpec) obj;
			return alignment1 == other.alignment1 && alignment2 == other.alignment2 && mapType == other.mapType
					&& size1 == other.size1 && size2 == other.size2;
		}
		MapSpec(MapType mapType, long size1, long alignment1, long size2, long alignment2) {
			this.mapType = mapType;
			this.size1 = size1;
			this.alignment1 = alignment1;
			this.size2 = size2;
			this.alignment2 = alignment2;
		}
		final MapType mapType;
		final long size1;
		final long alignment1;
		final long size2;
		final long alignment2;
	}
	
	private static final long[] alignments = {1, 2, 4, 8};
	private static final boolean isQt5 = QtUtilities.qtjambiVersion().majorVersion()==5;
	
	private static <A> void appendUnique(List<A> specs, A spec) {
		if(!specs.contains(spec))
			specs.add(spec);
	}
	
	static void generate(QCommandLineParser parser, String[] args, QCommandLineOption dirOption) throws InterruptedException, IOException {
		QCommandLineOption specificationsOption = new QCommandLineOption(QList.of("specifications"), "Target specifications (e.g. QList/1/1)", "specifications");
		parser.addOptions(Arrays.asList(
	    		dirOption,
	    		specificationsOption
			));

		if(args.length==1)
			parser.showHelp();
    	parser.process(new QStringList(args));

		QStringList unusedArguments = new QStringList(parser.positionalArguments());
		if(unusedArguments.size()==1)
			throw new Error("Qt plugin generation, illegal argument: "+unusedArguments.join(", "));
		if(unusedArguments.size()>1)
			throw new Error("Qt plugin generation, illegal argument: "+unusedArguments.join(", "));
		
		File dir;
		if(parser.isSet(dirOption))
			dir = new File(parser.value(dirOption));
		else
			throw new Error("Missing target directory. Please use --dir=...");
		List<String> specifications = new ArrayList<>();
		if(parser.isSet(specificationsOption))
			specifications.addAll(Arrays.asList(parser.value(specificationsOption).split("\\,")));
		else
			throw new Error("Missing specifications. Please use --specifications=...");
		
		List<ContainerSpec> containerSpecs = new ArrayList<>();
		List<MapSpec> mapSpecs = new ArrayList<>();
		if(specifications.remove("analyze")) {
			if(!isQt5) {
				QDir io_qt_dir = new QDir(":/io/qt");
				for(String subdir : io_qt_dir.entryList(QDir.Filter.Dirs, QDir.Filter.NoDotAndDotDot)) {
					try {
						QtUtilities.initializePackage("io.qt."+subdir);
					} catch (Throwable e) {}
				}
				Map<String, QPair<Long, Long>> map = new TreeMap<>();
				try {
					map = QtJambiInternal.getRegisteredTypeSizesAndAlignments();
				} catch (Throwable e) {
					e.printStackTrace();
					throw e;
				}
				QPair<Long, Long> stringPair= null;
				for(String key : new ArrayList<>(map.keySet())) {
					if(key.equals("QStringMatcher")
							|| key.equals("QByteArrayMatcher")
							|| key.startsWith("QStyleOption")
							|| key.startsWith("QXmlStream")
							|| key.startsWith("QDBusReply<")) {
						map.remove(key);
					}
					if(key.equals("QString")) {
						stringPair = map.get(key);
					}
				}
				Set<QPair<Long, Long>> sizesAndAlignments = new HashSet<>(map.values());
				/*
				for(ContainerType t : new ContainerType[]{ContainerType.QSet}) {
					for (QPair<Long, Long> pair : sizesAndAlignments)
						containerSpecs.add(new ContainerSpec(t, pair.first, pair.second));
				}
				appendUnique(containerSpecs, new ContainerSpec(ContainerType.QSet, 8, 1));
				*/
				Set<QPair<Long, Long>> keySizesAndAlignments = new HashSet<>();
				if(stringPair!=null)
					keySizesAndAlignments.add(stringPair);
				keySizesAndAlignments.add(new QPair<>(1l, 1l));
				keySizesAndAlignments.add(new QPair<>(2l, 2l));
				keySizesAndAlignments.add(new QPair<>(4l, 4l));
				keySizesAndAlignments.add(new QPair<>(8l, 8l));
//				for (QPair<Long, Long> pair2 : sizesAndAlignments) {
//					System.out.println("sizeAligns.insert({"+pair2.first+","+pair2.second+"});");
//				}
//				for (QPair<Long, Long> pair2 : keySizesAndAlignments) {
//					System.out.println("sizeAligns.insert({"+pair2.first+","+pair2.second+"});");
//				}
//				if(!sizesAndAlignments.isEmpty())
//					return;
				for(MapType t : new MapType[]{MapType.QMap, MapType.QMultiMap}) {
					for (QPair<Long, Long> pair2 : sizesAndAlignments) {
						for (QPair<Long, Long> pair : keySizesAndAlignments) {
							mapSpecs.add(new MapSpec(t, pair.first, pair.second, pair2.first, pair2.second));
						}
					}
				}
//				appendUnique(mapSpecs, new MapSpec(MapType.QMultiHash, 32, 8, 24, 8));
//				appendUnique(mapSpecs, new MapSpec(MapType.QMultiHash, 16, 4, 24, 8));
				appendUnique(mapSpecs, new MapSpec(MapType.QMultiMap, 16, 4, 2, 2));
//				appendUnique(mapSpecs, new MapSpec(MapType.QHash, 16, 4, 24, 8));
//				appendUnique(mapSpecs, new MapSpec(MapType.QMultiHash, 32, 8, 8, 8));
//				appendUnique(mapSpecs, new MapSpec(MapType.QMultiHash, 16, 4, 8, 8));
//				appendUnique(mapSpecs, new MapSpec(MapType.QHash, 8, 8, 8, 1));
				appendUnique(mapSpecs, new MapSpec(MapType.QMultiMap, 8, 8, 8, 1));
//				appendUnique(mapSpecs, new MapSpec(MapType.QMultiHash, 8, 8, 8, 1));
				appendUnique(mapSpecs, new MapSpec(MapType.QMap, 8, 8, 8, 1));
//				appendUnique(mapSpecs, new MapSpec(MapType.QHash, 16, 4, 8, 8));
			}
		}
		for(String specification : specifications) {
			String[] parts = specification.split("/");
			ContainerType containerType = null;
			MapType mapType = null;
			switch(parts[0]) {
			case "QList": containerType = ContainerType.QList; break;
			case "QLinkedList": containerType = ContainerType.QLinkedList; break;
			case "QVector": containerType = ContainerType.QVector; break;
			case "QSet": containerType = ContainerType.QSet; break;
			default:
				mapType = MapType.valueOf(parts[0]); break;
			}
			if(containerType!=null) {
				if(parts.length>=2) {
					long size = Long.parseLong(parts[1]);
					if(parts.length==2) {
						appendUnique(containerSpecs, new ContainerSpec(containerType, size, -1));
					}else {
						long alignment = Long.parseLong(parts[2]);
						if(alignment<=size)
							appendUnique(containerSpecs, new ContainerSpec(containerType, size, alignment));
					}
				}
			}else if(mapType!=null) {
				if(parts.length>=3) {
					long size1 = Long.parseLong(parts[1]);
					if(parts.length==3) {
						long size2 = Long.parseLong(parts[2]);
						appendUnique(mapSpecs, new MapSpec(mapType, size1, -1, size2, -1));
					}else if(parts.length>=5) {
						long alignment1 = Long.parseLong(parts[2]);
						long size2 = Long.parseLong(parts[3]);
						long alignment2 = Long.parseLong(parts[4]);
						if(alignment1<=size1 && alignment2<=size2)
							appendUnique(mapSpecs, new MapSpec(mapType, size1, alignment1, size2, alignment2));
					}
				}
			}
		}
		
		if(!containerSpecs.isEmpty() || !mapSpecs.isEmpty()) {
			final QStringList plugin_cpp = new QStringList();
			{
				QFile f = new QFile(":/io/qt/qtjambi/deployer/containeraccess.cpp");
		        if(f.open(QIODevice.OpenModeFlag.ReadOnly)){
		            QTextStream s = new QTextStream(f);
		            while(!s.atEnd()){
		            	plugin_cpp.add(s.readLine());
		            }
		            f.close();
		        }
			}
		    final QStringList plugin_pro = new QStringList();
		    {
		    	QFile f = new QFile(":/io/qt/qtjambi/deployer/containeraccess.pro");
		        if(f.open(QIODevice.OpenModeFlag.ReadOnly)){
		            QTextStream s = new QTextStream(f);
		            while(!s.atEnd()){
		            	plugin_pro.add(s.readLine());
		            }
		            f.close();
		        }
		    }
			final QCborMap cborValue = new QCborMap();
			cborValue.setValue(/*QtPluginMetaDataKeys::IID*/ 2, new QCborValue("io.qt.qtjambi.ContainerAccess"));
			cborValue.setValue(/*QtPluginMetaDataKeys::ClassName*/ 3, new QCborValue(""));
		    
			dir = new File(dir, "containeraccess");
			dir.mkdirs();
			List<String> subdirs = new ArrayList<>();
//			if(subdirs.isEmpty())return;
			for (ContainerSpec containerSpec : containerSpecs) {
				subdirs.add(generateSource(dir, containerSpec, plugin_cpp, plugin_pro, cborValue.clone()));
			}
			for (MapSpec mapSpec : mapSpecs) {
				subdirs.add(generateSource(dir, mapSpec, plugin_cpp, plugin_pro, cborValue.clone()));
			}
			QFile c_pro = new QFile(new QDir(dir.getAbsolutePath()).absoluteFilePath("containeraccess.pro"));
			if(c_pro.open(QIODevice.OpenModeFlag.WriteOnly, QIODevice.OpenModeFlag.Text)) {
				QTextStream s = new QTextStream(c_pro);
				s.append("TEMPLATE = subdirs").endl()
				 .append("CONFIG += ordered").endl();
				for (String subdir : subdirs) {
					if(subdir!=null && !subdir.isEmpty())
						s.append("SUBDIRS += ").append(subdir).endl();
				}
				c_pro.close();
			}
		}
	}
	
	private static String generateSource(File dir, ContainerSpec containerSpec, Iterable<String> plugin_cpp, Iterable<String> plugin_pro, QCborMap cborValue) {
		String target;
		if(containerSpec.alignment==-1) {
			target = String.format("%1$sAccess_S%2$s", containerSpec.containerType.name(), containerSpec.size);
		}else {
			target = String.format("%1$sAccess_S%2$s_A%3$s", containerSpec.containerType.name(), containerSpec.size, containerSpec.alignment);
		}
		dir = new File(dir, target);
		dir.mkdirs();
		
		String flatTypeName = containerSpec.containerType.name().toLowerCase().substring(1);
        StringBuilder implData = new StringBuilder();
        QTextStream stream2 = new QTextStream(implData);
		
        List<String> targets = new ArrayList<>();
		if(containerSpec.alignment==-1) {
			for(long algn : alignments){
				if(algn<=containerSpec.size) {
					targets.add(String.format("%1$sAccess_S%2$s_A%3$s", containerSpec.containerType.name(), containerSpec.size, algn));
					stream2.append("    ContainerAccessFactoryHelper<").append(containerSpec.containerType.name()).append(", ").append(algn).append(", ").append(containerSpec.size).append(", false>::registerContainerAccessFactory();").endl();
					if(containerSpec.containerType==ContainerType.QList && isQt5){
	                    stream2.append("    ContainerAccessFactoryHelper<").append(containerSpec.containerType.name()).append(", ").append(algn).append(", ").append(containerSpec.size).append(", true>::registerContainerAccessFactory();").endl();
					}
				}
	        }
		}else {
			long algn = containerSpec.alignment;
			if(algn<=containerSpec.size) {
				targets.add(target);
				stream2.append("    ContainerAccessFactoryHelper<").append(containerSpec.containerType.name()).append(", ").append(algn).append(", ").append(containerSpec.size).append(", false>::registerContainerAccessFactory();").endl();
				if(containerSpec.containerType==ContainerType.QList && isQt5){
                    stream2.append("    ContainerAccessFactoryHelper<").append(containerSpec.containerType.name()).append(", ").append(algn).append(", ").append(containerSpec.size).append(", true>::registerContainerAccessFactory();").endl();
				}
			}
		}
		stream2.dispose();
		generateFiles(dir, flatTypeName, implData, plugin_cpp, plugin_pro, cborValue, target, targets);
		return target;
	}
	
	private static String generateSource(File dir, MapSpec mapSpec, Iterable<String> plugin_cpp, Iterable<String> plugin_pro, QCborMap cborValue) {
		String target;
		if(mapSpec.alignment1==-1) {
			if(mapSpec.alignment2==-1) {
				target = String.format("%1$sAccess_S%2$s_S%3$s", mapSpec.mapType.name(), mapSpec.size1, mapSpec.size2);
			}else {
				target = String.format("%1$sAccess_S%2$s_A%3$s_S%4$s", mapSpec.mapType.name(), mapSpec.size1, mapSpec.alignment1, mapSpec.size2);
			}
		}else {
			if(mapSpec.alignment2==-1) {
				target = String.format("%1$sAccess_S%2$s_A%3$s_S%4$s", mapSpec.mapType.name(), mapSpec.size1, mapSpec.alignment1, mapSpec.size2);
			}else {
				target = String.format("%1$sAccess_S%2$s_A%3$s_S%4$s_A%5$s", mapSpec.mapType.name(), mapSpec.size1, mapSpec.alignment1, mapSpec.size2, mapSpec.alignment2);
			}
		}
		dir = new File(dir, target);
		dir.mkdirs();
		String flatTypeName = mapSpec.mapType.name().toLowerCase().substring(1);
        StringBuilder implData = new StringBuilder();
        QTextStream stream2 = new QTextStream(implData);
		
        List<String> targets = new ArrayList<>();
		if(mapSpec.alignment1==-1) {
			for(long algn1 : alignments){
				if(algn1<=mapSpec.size1) {
					if(mapSpec.alignment2==-1) {
						for(long algn2 : alignments){
							if(algn2<=mapSpec.size2) {
								targets.add(String.format("%1$sAccess_S%2$s_A%3$s_S%4$s_A%5$s", mapSpec.mapType.name(), mapSpec.size1, algn1, mapSpec.size2, algn2));
								stream2.append("    BiContainerAccessFactoryHelper<").append(mapSpec.mapType.name()).append(", ").append(algn1).append(", ").append(mapSpec.size1).append(", ").append(algn2).append(", ").append(mapSpec.size2).append(">::registerContainerAccessFactory();").endl();
							}
						}
					}else {
						long algn2 = mapSpec.alignment2;
						targets.add(String.format("%1$sAccess_S%2$s_A%3$s_S%4$s_A%5$s", mapSpec.mapType.name(), mapSpec.size1, algn1, mapSpec.size2, algn2));
						stream2.append("    BiContainerAccessFactoryHelper<").append(mapSpec.mapType.name()).append(", ").append(algn1).append(", ").append(mapSpec.size1).append(", ").append(algn2).append(", ").append(mapSpec.size2).append(">::registerContainerAccessFactory();").endl();
					}
				}
	        }
		}else {
			long algn1 = mapSpec.alignment1;
			if(algn1<=mapSpec.size1) {
				if(mapSpec.alignment2==-1) {
					for(long algn2 : alignments){
						if(algn2<=mapSpec.size2) {
							targets.add(String.format("%1$sAccess_S%2$s_A%3$s_S%4$s_A%5$s", mapSpec.mapType.name(), mapSpec.size1, algn1, mapSpec.size2, algn2));
							stream2.append("    BiContainerAccessFactoryHelper<").append(mapSpec.mapType.name()).append(", ").append(algn1).append(", ").append(mapSpec.size1).append(", ").append(algn2).append(", ").append(mapSpec.size2).append(">::registerContainerAccessFactory();").endl();
						}
					}
				}else {
					long algn2 = mapSpec.alignment2;
					targets.add(target);
					stream2.append("    BiContainerAccessFactoryHelper<").append(mapSpec.mapType.name()).append(", ").append(algn1).append(", ").append(mapSpec.size1).append(", ").append(algn2).append(", ").append(mapSpec.size2).append(">::registerContainerAccessFactory();").endl();
				}
			}
		}
		stream2.dispose();
		generateFiles(dir, flatTypeName, implData, plugin_cpp, plugin_pro, cborValue, target, targets);
		return target;
	}
	
	private static void generateFiles(File dir, String flatTypeName, StringBuilder implData, Iterable<String> plugin_cpp, Iterable<String> plugin_pro, QCborMap cborValue, String libName, List<String> targets) {
		QByteArray c_cpp_data = new QByteArray();
		{
			QTextStream s = new QTextStream(c_cpp_data);
	        for(String line : plugin_cpp){
	            if("#define CBOR_DATA".equals(line)){
	                s.append("#include <qtjambi/qtjambi_containeraccess_").append(flatTypeName).append(".h>").endl().endl()
		       		 .append("#define CBOR_DATA \\").endl().append("    ");
		       		QJsonObject metaData = new QJsonObject();
		       		QJsonArray targetArray = new QJsonArray();
		       		for(String target : targets) {
		       			targetArray.append(new QJsonValue(target));
		       		}
		       		metaData.setValue("Keys", new QJsonValue(targetArray));
		       		cborValue.setValue(/*QtPluginMetaDataKeys::MetaData*/ 4, new QCborValue(QCborMap.fromJsonObject(metaData)));
		       		QByteArray cborData = cborValue.toCborValue().toCbor();
		       		byte[] data = cborData.toByteArray();
		       		for (int i = 0, j = 0; i < data.length; ++i, ++j) {
		       			byte b = data[i];
		       			if(Character.isAlphabetic(b)) {
		       				s.append('\'').append((char)b).append("', ");
		       			}else {
		       				String hex = Integer.toHexString(Byte.toUnsignedInt(b));
		       				while(hex.length()>2 && hex.charAt(0)=='0') {
		       					hex = hex.substring(1);
		       				}
		       				s.append("0x").append(hex).append(", ");
		       			}
		       			if(j==7) {
		       				s.append("\\").endl().append("    ");
		       				j = 0;
		       			}
		       		}
		       		s.endl().endl();
	            }else if("//qt_plugin_instance_impl".equals(line)){
	            	s.append(implData.toString()).endl();
	            }else{
	            	s.append(line).endl();
	            }
	        }
	        s.dispose();
		}
        
		QFile c_cpp = new QFile(new QDir(dir.getAbsolutePath()).absoluteFilePath("containeraccess.cpp"));
		if(c_cpp.open(QIODevice.OpenModeFlag.ReadOnly)) {
			QByteArray all = c_cpp.readAll();
	        c_cpp.close();
	        if(!all.equals(c_cpp_data)) {
	        	if(c_cpp.open(QIODevice.OpenModeFlag.WriteOnly)) {
	        		c_cpp.write(c_cpp_data);
	    	        c_cpp.close();
	    		}
	        }
		}else {
			if(c_cpp.open(QIODevice.OpenModeFlag.WriteOnly)) {
        		c_cpp.write(c_cpp_data);
    	        c_cpp.close();
    		}
		}
		
		QByteArray c_pro_data = new QByteArray();
		{
			QTextStream s = new QTextStream(c_pro_data);
			s.append("TARGET = ").append(libName).endl();
            for(String line : plugin_pro){
                s.append(line).endl();
            }
            s.dispose();
		}
		QFile c_pro = new QFile(new QDir(dir.getAbsolutePath()).absoluteFilePath(libName+".pro"));
		if(c_pro.open(QIODevice.OpenModeFlag.ReadOnly)) {
			QByteArray all = c_pro.readAll();
	        c_pro.close();
	        if(!all.equals(c_pro_data)) {
	        	if(c_pro.open(QIODevice.OpenModeFlag.WriteOnly)) {
	        		c_pro.write(c_pro_data);
	    	        c_pro.close();
	    		}
	        }
		}else {
			if(c_pro.open(QIODevice.OpenModeFlag.WriteOnly)) {
        		c_pro.write(c_pro_data);
    	        c_pro.close();
    		}
		}
	}
}
