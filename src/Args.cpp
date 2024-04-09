#include "Args.hpp"
#include <SDL_types.h>

Args::Args(const int argc, const char *const *const argv)
	: ArgumentParser(argv[0])
{
	add_argument("audio_file")
		.help("audio file to visualize and play");

	add_argument("--encode")
		.help("encode to a video! arguments: <output_file> <fps> [vcodec] [acodec]")
		.nargs(2, 4)
		.validate();
	add_argument("--mono")
		.help("force a mono spectrum even if audio is stereo\nmust specify zero-indexed channel number to render\nnegative values disable this flag")
		.default_value(-1)
		.scan<'i', int>()
		.validate();

	add_argument("-n", "--sample-size")
		.help("number of samples to process at a time\n- higher amount increases accuracy\n- lower amount increases responsiveness")
		.default_value(3000u)
		.scan<'u', uint>()
		.validate();

	add_argument("-m", "--multiplier")
		.help("spectrum amplitude multiplier")
		.default_value(4.f)
		.scan<'f', float>()
		.validate();

	add_argument("-s", "--scale")
		.help("spectrum frequency scale: 'linear', 'log', 'nth-root'")
		.default_value("log");

	add_argument("--nth-root")
		.help("set the root to use with '--scale nth-root'")
		.default_value(2.f)
		.scan<'f', float>()
		.validate();

	add_argument("-a", "--accum-method")
		.help("frequency bin accumulation method\n- 'sum': greater treble detail, exaggerated amplitude\n- 'max': less treble detail, true-to-waveform amplitude")
		.default_value("max");

	add_argument("-w", "--window-func")
		.help("window function: 'none', 'hanning', 'hamming', 'blackman'\nwindow functions can reduce 'wiggling' in bass frequencies\nhowever they can reduce overall amplitude, so adjust '-m' accordingly")
		.default_value("blackman");

	add_argument("-i", "--interpolation")
		.help("spectrum interpolation type: 'none', 'linear', 'cspline', 'cspline_hermite'")
		.default_value("cspline");

	add_argument("--color")
		.help("enable a colorful spectrum!")
		.default_value("wheel");
	add_argument("--wheel-rate")
		.help("requires '--color wheel'\nmoves the colors on the spectrum with time!\nvalue must be between [0, 1] - 0.005 is a good start")
		.default_value(0.f)
		.scan<'f', float>()
		.validate();
	add_argument("--hsv")
		.help("requires '--color wheel'\nchoose a hue offset for the color wheel, saturation, and brightness\nvalues must be between [0, 1]")
		.nargs(3)
		.default_value(std::vector<float>{0.9, 0.7, 1})
		.scan<'f', float>()
		.validate();
	add_argument("--rgb")
		.help("requires '--color solid'\nrenders the spectrum with a solid color\nmust provide space-separated rgb integers")
		.nargs(3)
		.default_value(std::vector<Uint8>{255, 255, 255})
		.scan<'u', Uint8>()
		.validate();

	add_argument("--width")
		.help("window width in pixels")
		.default_value(800u)
		.scan<'u', uint>()
		.validate();
	add_argument("--height")
		.help("window height in pixels")
		.default_value(600u)
		.scan<'u', uint>()
		.validate();
	
	add_argument("-bw", "--bar-width")
		.help("bar width in pixels")
		.default_value(10u)
		.scan<'u', uint>()
		.validate();
	add_argument("-bs", "--bar-spacing")
		.help("bar spacing in pixels")
		.default_value(5u)
		.scan<'u', uint>()
		.validate();
	add_argument("-bt", "--bar-type")
		.help("spectrum bar style\n- 'bar': rectangular bar\n- 'pill': bar with rounded ends")
		.default_value("pill");

	try
	{
		parse_args(argc, argv);
	}
	catch (const std::exception &e)
	{
		// print error and help to stderr
		std::cerr << argv[0] << ": " << e.what() << '\n'
				  << *this;

		// just exit here since we don't want to print anything after the help
		_Exit(EXIT_FAILURE);
	}
}
