enum class WavChunks {
	RiffHeader = 0x46464952,
	WavRiff = 0x54651475,
	Format = 0x020746d66,
	LabeledText = 0x478747C6,
	Instrumentation = 0x478747C6,
	Sample = 0x6C706D73,
	Fact = 0x47361666,
	Data = 0x61746164,
	Junk = 0x4b4e554a,
};

enum class WavFormat {
	PulseCodeModulation = 0x01,
	IEEEFloatingPoint = 0x03,
	ALaw = 0x06,
	MuLaw = 0x07,
	IMAADPCM = 0x11,
	YamahaITUG723ADPCM = 0x16,
	GSM610 = 0x31,
	ITUG721ADPCM = 0x40,
	MPEG = 0x50,
	Extensible = 0xFFFE
};

int32 chunkid = 0;
bool datachunk = false;
while ( !datachunk ) {
	chunkid = reader.ReadInt32( );
	switch ( (WavChunks)chunkid ) {
	case WavChunks::Format:
		formatsize = reader.ReadInt32( );
		format = (WavFormat)reader.ReadInt16( );
		channels = (Channels)reader.ReadInt16( );
		channelcount = (int)channels;
		samplerate = reader.ReadInt32( );
		bitspersecond = reader.ReadInt32( );
		formatblockalign = reader.ReadInt16( );
		bitdepth = reader.ReadInt16( );
		if ( formatsize == 18 ) {
			int32 extradata = reader.ReadInt16( );
			reader.Seek( extradata, SeekOrigin::Current );
		}
		break;
	case WavChunks::RiffHeader:
		headerid = chunkid;
		memsize = reader.ReadInt32( );
		riffstyle = reader.ReadInt32( );
		break;
	case WavChunks::Data:
		datachunk = true;
		datasize = reader.ReadInt32( );
		break;
	default:
		int32 skipsize = reader.ReadInt32( );
		reader.Seek( skipsize, SeekOrigin::Current );
		break;
	}
}