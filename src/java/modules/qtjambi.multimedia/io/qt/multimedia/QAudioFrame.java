package io.qt.multimedia;

import io.qt.multimedia.QAudioFormat.ChannelConfig;
import io.qt.multimedia.QAudioFormat.SampleFormat;

public final class QAudioFrame<T> {
	
	public static QAudioFrame<Byte> byteAudioFrame(ChannelConfig config){
		return new QAudioFrame<>(config, SampleFormat.UInt8);
	}
	
	public static QAudioFrame<Short> shortAudioFrame(ChannelConfig config){
		return new QAudioFrame<>(config, SampleFormat.Int16);
	}
	
	public static QAudioFrame<Integer> intAudioFrame(ChannelConfig config){
		return new QAudioFrame<>(config, SampleFormat.Int32);
	}
	
	public static QAudioFrame<Float> floatAudioFrame(ChannelConfig config){
		return new QAudioFrame<>(config, SampleFormat.Float);
	}
	
	private QAudioFrame(ChannelConfig config, SampleFormat format) {
		super();
		this.config = config;
		this.format = format;
		switch(this.format) {
		case Float:
			this.channels = new float[constexprPopcount(config.value())];
			break;
		case Int16:
			this.channels = new short[constexprPopcount(config.value())];
			break;
		case Int32:
			this.channels = new int[constexprPopcount(config.value())];
			break;
		case UInt8:
			this.channels = new byte[constexprPopcount(config.value())];
			break;
		default:
			this.channels = null;
			break;
		}
	}
	
    // popcount in qalgorithms.h is unfortunately not on MSVC.
    // Use this here as a fallback
	private static int constexprPopcount(int i)
    {
        i = i - ((i >> 1) & 0x55555555);        // add pairs of bits
        i = (i & 0x33333333) + ((i >> 2) & 0x33333333);  // quads
        i = (i + (i >> 4)) & 0x0F0F0F0F;        // groups of 8
        return (i * 0x01010101) >> 24;          // horizontal sum of bytes
    }

    public int positionToIndex(QAudioFormat.AudioChannelPosition pos)
    {
        if ((config.value() & (1 << pos.value()))==0)
            return -1;

        int maskedChannels = config.value() & ((1 << pos.value()) - 1);
        return constexprPopcount(maskedChannels);
    }
    
    @SuppressWarnings("unchecked")
	public T value(QAudioFormat.AudioChannelPosition pos) {
    	return (T)_value(pos);
    }


    private Object _value(QAudioFormat.AudioChannelPosition pos) {
        int idx = positionToIndex(pos);
        switch(this.format) {
		case Float:
	        if (idx < 0)
	            return 0.f;
	        return ((float[])channels)[idx];
		case Int16:
	        if (idx < 0)
	            return (short)0;
	        return ((short[])channels)[idx];
		case Int32:
	        if (idx < 0)
	            return 0;
	        return ((int[])channels)[idx];
		case UInt8:
	        if (idx < 0)
	            return (byte)0;
	        return ((byte[])channels)[idx];
		default:
			break;
		}
        return null;
    }
    
    public void setValue(QAudioFormat.AudioChannelPosition pos, T val) {
        int idx = positionToIndex(pos);
        if (idx < 0)
            return;
        switch(this.format) {
		case Float:
	        ((float[])channels)[idx] = (float)val;
			break;
		case Int16:
	        ((short[])channels)[idx] = (short)val;
			break;
		case Int32:
	        ((int[])channels)[idx] = (int)val;
			break;
		case UInt8:
	        ((byte[])channels)[idx] = (byte)val;
			break;
		default:
			break;
		}
    }
    public void clear() {
    	switch(this.format) {
		case Float:
	        for (int i = 0; i < ((float[])channels).length; ++i)
	            ((float[])channels)[i] = 0.f;
			break;
		case Int16:
			for (int i = 0; i < ((short[])channels).length; ++i)
				((short[])channels)[i] = (short)0;
			break;
		case Int32:
			for (int i = 0; i < ((int[])channels).length; ++i)
				((int[])channels)[i] = 0;
			break;
		case UInt8:
			for (int i = 0; i < ((byte[])channels).length; ++i)
				((byte[])channels)[i] = (byte)0;
			break;
		default:
			break;
		}
    }
	
    private final Object channels;
	private final QAudioFormat.ChannelConfig config;
	private final QAudioFormat.SampleFormat format;
}

