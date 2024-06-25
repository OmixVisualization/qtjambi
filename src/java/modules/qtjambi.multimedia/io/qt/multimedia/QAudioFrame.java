package io.qt.multimedia;

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.ShortBuffer;

import io.qt.NonNull;
import io.qt.multimedia.QAudioFormat.ChannelConfig;

public final class QAudioFrame {
	
	public static QAudioFrame.@NonNull AsByte dataAsAudioFrame(java.nio.@NonNull ByteBuffer buffer, @NonNull ChannelConfig config){
		return new QAudioFrame(config).new AsByte(buffer);
	}
	
	public static QAudioFrame.@NonNull AsShort dataAsAudioFrame(java.nio.@NonNull ShortBuffer buffer, @NonNull ChannelConfig config){
		return new QAudioFrame(config).new AsShort(buffer);
	}
	
	public static QAudioFrame.@NonNull AsInt dataAsAudioFrame(java.nio.@NonNull IntBuffer buffer, @NonNull ChannelConfig config){
		return new QAudioFrame(config).new AsInt(buffer);
	}
	
	public static QAudioFrame.@NonNull AsFloat dataAsAudioFrame(java.nio.@NonNull FloatBuffer buffer, @NonNull ChannelConfig config){
		return new QAudioFrame(config).new AsFloat(buffer);
	}
	
	QAudioFrame(ChannelConfig config) {
		super();
		this.positionToIndexFunction = getPositionToIndex(config.value());
	}

    private static native int positionToIndex(int pos, long positionToIndexFunction);
    
    private static native long getPositionToIndex(int config);
	
	private final long positionToIndexFunction;
	
	public final class AsByte{
		private AsByte(ByteBuffer channels) {
			this.channels = channels;
		}
		
		public int positionToIndex(QAudioFormat.@NonNull AudioChannelPosition pos) {
			return QAudioFrame.positionToIndex(pos==null ? 0 : pos.value(), positionToIndexFunction);
		}
		
		public byte value(QAudioFormat.@NonNull AudioChannelPosition pos) {
	        int idx = positionToIndex(pos);
	        if (idx < 0)
	            return (byte)0x80;
	        return channels.get(idx);
	    }
	    
	    public void setValue(QAudioFormat.@NonNull AudioChannelPosition pos, byte val) {
	        int idx = positionToIndex(pos);
	        if (idx < 0)
	            return;
	        channels.put(idx, val);
	    }
	    public void clear() {
	    	channels.clear();
	        for (int i = 0; i < channels.limit(); ++i)
	            channels.put(i, (byte)0x80);
	    }

		private final java.nio.ByteBuffer channels;
	}
	
	public final class AsShort{
		private AsShort(ShortBuffer channels) {
			this.channels = channels;
		}
		
		public int positionToIndex(QAudioFormat.@NonNull AudioChannelPosition pos) {
			return QAudioFrame.positionToIndex(pos==null ? 0 : pos.value(), positionToIndexFunction);
		}
		
		public short value(QAudioFormat.@NonNull AudioChannelPosition pos) {
	        int idx = positionToIndex(pos);
	        if (idx < 0)
	            return 0;
	        return channels.get(idx);
	    }
	    
	    public void setValue(QAudioFormat.@NonNull AudioChannelPosition pos, short val) {
	        int idx = positionToIndex(pos);
	        if (idx < 0)
	            return;
	        channels.put(idx, val);
	    }
	    public void clear() {
	    	channels.clear();
	        for (int i = 0; i < channels.limit(); ++i)
	            channels.put(i, (short)0);
	    }

		private final java.nio.ShortBuffer channels;
	}
	
	public final class AsInt{
		private AsInt(IntBuffer channels) {
			this.channels = channels;
		}
		
		public int positionToIndex(QAudioFormat.@NonNull AudioChannelPosition pos) {
			return QAudioFrame.positionToIndex(pos==null ? 0 : pos.value(), positionToIndexFunction);
		}
		
		public int value(QAudioFormat.@NonNull AudioChannelPosition pos) {
	        int idx = positionToIndex(pos);
	        if (idx < 0)
	            return 0;
	        return channels.get(idx);
	    }
	    
	    public void setValue(QAudioFormat.@NonNull AudioChannelPosition pos, int val) {
	        int idx = positionToIndex(pos);
	        if (idx < 0)
	            return;
	        channels.put(idx, val);
	    }
	    public void clear() {
	    	channels.clear();
	        for (int i = 0; i < channels.limit(); ++i)
	            channels.put(i, 0);
	    }

		private final java.nio.IntBuffer channels;
	}
	
	public final class AsFloat{
		private AsFloat(FloatBuffer channels) {
			this.channels = channels;
		}
		
		public int positionToIndex(QAudioFormat.@NonNull AudioChannelPosition pos) {
			return QAudioFrame.positionToIndex(pos==null ? 0 : pos.value(), positionToIndexFunction);
		}
		
		public float value(QAudioFormat.@NonNull AudioChannelPosition pos) {
	        int idx = positionToIndex(pos);
	        if (idx < 0)
	            return 0.f;
	        return channels.get(idx);
	    }
	    
	    public void setValue(QAudioFormat.@NonNull AudioChannelPosition pos, float val) {
	        int idx = positionToIndex(pos);
	        if (idx < 0)
	            return;
	        channels.put(idx, val);
	    }
	    public void clear() {
	    	channels.clear();
	        for (int i = 0; i < channels.limit(); ++i)
	            channels.put(i, 0);
	    }

		private final java.nio.FloatBuffer channels;
	}
}

