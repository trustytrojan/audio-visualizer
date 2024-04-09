#include "SpectrumRenderer.hpp"

void SpectrumRenderer::copy_channel_to_input(const float *audio, int num_channels, int channel, bool interleaved)
{
	fs.copy_channel_to_input(audio, num_channels, channel, interleaved);
}

void SpectrumRenderer::render_spectrum(const SDL2pp::Rect &rect, const bool backwards)
{
	// resize spectrum first! this is the old formula, except now it's relative to the passed in rect.
	// this makes things much much more flexible
	spectrum.resize((rect.w) / (bar.width + bar.spacing));

	// render spectrum
	fs.render(spectrum);

	for (int i = 0; i < (int)spectrum.size(); ++i)
	{
		const auto [r, g, b] = color.get((float)i / spectrum.size());
		const auto x = backwards ? (rect.GetBottomRight().x - bar.width - (i * (bar.width + bar.spacing)) + 1)
								 : (rect.x + (i * (bar.width + bar.spacing)));
		const auto h = std::min((float)rect.h, multiplier * std::max(0.f, spectrum[i]) * rect.h);
		switch (bar.type)
		{
		case BarType::RECTANGLE:
			drawBoxFromBottomLeft(x, rect.y + rect.h - 1, bar.width - 1, h, r, g, b, 255);
			break;
		case BarType::PILL:
			drawPillFromBottomLeft(x, rect.y + rect.h - 1, bar.width - 1, h, r, g, b, 255);
			break;
		default:
			throw std::logic_error("SpectrumRenderer::render_spectrum: switch(bar.type): default case hit");
		}
	}

	color.wheel.increment();
}
