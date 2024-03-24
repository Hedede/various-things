#include <aw/fileformat/wav/reader.h>
#include <aw/io/input_file_stream.h>

#include "AL/al.h"
#include "AL/alc.h"
#define AL_ALEXT_PROTOTYPES
#include "AL/alext.h"

int main(int, char** argv)
{
	using namespace aw;
	io::input_file_stream file( argv[1] );

	wav::wave_data data;
	wav::read(file, data);

	auto dev = alcOpenDevice( nullptr );
	if (!dev)
		return 1;
	auto ctx = alcCreateContext( dev, nullptr );
	if (!ctx)
		return 2;
	if (!alcMakeContextCurrent(ctx))
		return 3;

	ALuint buf;
	alGenBuffers(1, &buf);

	if (alGetError() != AL_NO_ERROR)
		return 4;

	alBufferi( buf, AL_UNPACK_BLOCK_ALIGNMENT_SOFT, data.samples_per_block() );

	if (alGetError() != AL_NO_ERROR)
		return 5;

	auto format = [&] () {
		if (data.channels == 1)
			return AL_FORMAT_MONO_IMA4;
		if (data.channels == 2)
			return AL_FORMAT_STEREO_IMA4;
	}();

	alBufferData(buf, format, data.ptr(), data.size(), data.sample_rate);
	/*alBufferSamplesSOFT(buf,
		data.sample_rate,
		AL_FORMAT_IMA_ADPCM_MONO16_EXT,
		data.samples_per_channel,
		AL_MONO_SOFT,
		AL_SHORT_SOFT,
		data.data.data());*/


	if (alGetError() != AL_NO_ERROR)
		return 6;

	ALuint src;
	alGenSources(1, &src);
	alSourcei(src, AL_BUFFER, buf);

	if (alGetError() != AL_NO_ERROR)
		return 7;

	alSourcePlay( src);

	if (alGetError() != AL_NO_ERROR)
		return 8;

	ALint state;
	do {
		alGetSourcei(src, AL_SOURCE_STATE, &state);
	} while(state == AL_PLAYING);

	if (alGetError() != AL_NO_ERROR)
		return 9;
}
